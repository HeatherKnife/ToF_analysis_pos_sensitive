#include "TFile.h"
#include "TH1F.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include <TGraphErrors.h>
#include <vector>
#include <iostream>
#include <TH2F.h>
#include <TEllipse.h>
#include <TLine.h>

#include "TH1F.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLatex.h"

//######################################################################################################################################################

class FitResult {
public:
    std::vector<double> heights;
    std::vector<double> heightsErrors;
    std::vector<double> centroids;
    std::vector<double> centroidsErrors;
    std::vector<double> sigmas;
    std::vector<double> sigmasErrors;
};

std::vector<std::vector<double>> Fit_centroids;
std::vector<std::vector<double>> Fit_centroids_error;

std::vector<std::vector<double>> Fit_centroids_TOF;
std::vector<std::vector<double>> Fit_centroids_TOF_error;

std::vector<TGraphErrors*> calibration_graphs;

// Example initialization assuming you know the number of peaks per histogram
                                
                                // Energies Pu239  // Energies Am241        // Energies Cm244  
// std::vector<double> energies = { 5.03489, 5.07841, 5.3426, 5.3857 };
// std::vector<double> energies = { 5.03489, 5.07841, 5.3426, 5.3857 };
std::vector<double> energies = { 4.7758, 4.93097, 5.03489, 5.07841 };
std::vector<double> energies_error = { 0.00001, 0.00001, 0.00001, 0.00001};
std::vector<double> mass_component = {4, 4, 4, 4}; // amu
std::vector<double> TOF_calculated; // ns
std::vector<double> TOF_calibration_ct;
double Flight_Path = 500; // mm

constexpr double light_vel = 299792458; // speed of light in m/s
constexpr double uma_to_MeVc2 = 931.494; // conversion factor from u to MeV/c^2

//######################################################################################################################################################

double calculateComponent(double E, double A, double Flight_Path) {
    if (E == 0) {
        std::cerr << "Error: Division by zero in calculateComponent" << std::endl;
        return 0; // Handle division by zero gracefully
    }

    double TOF_component = pow(10, 9)*(Flight_Path*pow(10, -3)/light_vel)*sqrt((A * uma_to_MeVc2)/(2*E));

    cout << "TOF_component " << TOF_component << endl;
    cout << "A " << A << endl;
    cout << "E " << E << endl;

    return TOF_component;
}

//######################################################################################################################################################

// Function Description:
// FitAndRetrieveParameters - This function fits a histogram with multiple Gaussian functions,
// each applied to a specific range defined for potential peaks or sub-peaks within the histogram.
// Initially, each peak is fitted individually to determine parameters for the specific range.
// Subsequently, a combined Gaussian fit is performed over a broader, specified total range.
// This is particularly useful for histograms that feature multiple overlapping peaks or complex
// structures like shoulders or embedded peaks.

// Parameters:
// - histogram: Pointer to the TH1F object representing the histogram to be fitted.
// - fitRanges: A vector of pairs, where each pair represents the start and end points of the fit range for each Gaussian component.
// - totalFitRangeStart: The starting point of the range for the combined fit across all Gaussian components.
// - totalFitRangeEnd: The ending point of the range for the combined fit across all Gaussian components.

// Returns:
// - FitResult: An object that contains vectors of fit parameters and their corresponding uncertainties from the combined fit.
//   This includes parameters such as amplitude, mean, and sigma for each Gaussian component used in the fit.

// Example Usage:
// FitResult results = FitAndRetrieveParameters(histogram, {{100, 200}, {250, 350}}, 90, 360);
// for (int i = 0; i < results.centroids.size(); i++) {
//     std::cout << "Peak " << i + 1 << ": Centroid = " << results.centroids[i] << " ± " << results.centroidsErrors[i]
//               << ", Amplitude = " << results.heights[i] << " ± " << results.heightsErrors[i]
//               << ", Sigma = " << results.sigmas[i] << " ± " << results.sigmasErrors[i] << std::endl;
// }

FitResult FitAndRetrieveParameters(TH1F* histogram,
                                   const std::vector<std::pair<double, double>>& fitRanges,
                                   double totalFitRangeStart,
                                   double totalFitRangeEnd) {
    std::vector<TF1*> gaussFits;
    int nParsPerGauss = 3; // Number of parameters per Gaussian fit
    int totalPars = nParsPerGauss * fitRanges.size();
    double* pars = new double[totalPars];
    double* parsError = new double[totalPars];

    // Use TSpectrum to find peaks for initial guesses
    TSpectrum spectrum;
    int nFound = spectrum.Search(histogram, 2, "", 0.005); // Parameters can be adjusted
    double* xPositions = spectrum.GetPositionX();
    double* widths = spectrum.GetPositionY(); // Not directly widths, can calculate based on your needs


    // Loop over each fit range and perform Gaussian fits
    for (int i = 0; i < fitRanges.size(); ++i) {
        TF1* gaussFit = new TF1(Form("Gauss_Fit_%d", i), "gaus", fitRanges[i].first, fitRanges[i].second);
        // gaussFit->FixParameter(1, 12950); // Fix mean to 150

        cout << "fitRanges[i].first " << fitRanges[i].first << endl;
        gaussFit->SetLineColor(kRed + i); // Adjust color for visibility
        histogram->Fit(gaussFit, "R"); // 'RQ' for range fit and quiet mode
        for (int j = 0; j < nParsPerGauss; ++j) {
            pars[i * nParsPerGauss + j] = gaussFit->GetParameter(j);
            parsError[i * nParsPerGauss + j] = gaussFit->GetParError(j);
        }
        gaussFits.push_back(gaussFit);
    }

    // Creating the total fit function dynamically based on the number of parameters
    std::string formula = "gaus(0)";
    for (int i = 1; i < fitRanges.size(); ++i) {
        formula += "+gaus(" + std::to_string(i * nParsPerGauss) + ")";
    }
    TF1* totalFit = new TF1("Total_Fit", formula.c_str(), totalFitRangeStart, totalFitRangeEnd);
    totalFit->SetParameters(pars);
    totalFit->SetLineColor(kGreen+2);
    histogram->Fit(totalFit, "R+"); // 'R+' for range fit and add to the existing fits

    FitResult result;
    int nGaussians = fitRanges.size();  // Correct loop count based on number of Gaussian fits
    for (int i = 0; i < nGaussians; ++i) {
        int baseIndex = i * nParsPerGauss;
        result.heights.push_back(totalFit->GetParameter(baseIndex));       // Amplitude
        result.centroids.push_back(totalFit->GetParameter(baseIndex + 1)); // Mean
        result.sigmas.push_back(totalFit->GetParameter(baseIndex + 2));    // Sigma

        result.heightsErrors.push_back(totalFit->GetParError(baseIndex));
        result.centroidsErrors.push_back(totalFit->GetParError(baseIndex + 1));
        result.sigmasErrors.push_back(totalFit->GetParError(baseIndex + 2));
    }

    // Cleanup dynamically allocated memory and objects
    delete[] pars;
    delete[] parsError;
    for (auto& fit : gaussFits) {
        delete fit;
    }
    delete totalFit;

    return result;
}

//######################################################################################################################################################

void calibration_for_mass_plot_multiple_detectors() {

    for (int i = 0; i < energies.size(); ++i)
        {
            TOF_calculated.push_back( calculateComponent(energies[i], mass_component[i], Flight_Path) );
        }

    const Double_t ns_to_mm = 1/(2*0.75); // 1 mm / 0.75 ns

    // List of ROOT files to process
    const char* filenames = "test_output_5GHz_Masses_correct_channels_debugging_2.root";
    std::vector<std::string> branchNames = {"time_difference_X", "time_difference_Y", "tof", "ph_Si"};

    // std::vector<std::string> fileNames = {"file1.root", "file2.root", "file3.root", "file4.root"};
    
    const int Detectors = 21;
    // const int Detectors = sizeof(filenames) / sizeof(filenames[0]);

    // Arrays of histograms for each variable, one for each file
    TH1F* hTimeDiffX[Detectors];
    TH1F* hTimeDiffY[Detectors];
    TH1F* hTOF[Detectors];
    TH1F* hPH[Detectors];
    TH1F* Energy[Detectors];
    TH1F* TOF[Detectors];
    TH1F* Mass[Detectors];
    TH2F *ToF_ToF_XY_ch16[Detectors];  
    TH2F *E_vs_TOF[Detectors];  
    TH2F *A_vs_TOF[Detectors];  
    TH2F *A_vs_E[Detectors];  

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    TCanvas* TFACFD_plots = new TCanvas("TFACFD_plots", "TFACFD_plots", 800, 600);
    TFACFD_plots->Divide(7,3);

    TCanvas* CRRC4_plots = new TCanvas("CRRC4_plots", "CRRC4_plots", 800, 600);
    CRRC4_plots->Divide(7,3);

    TCanvas* Energy_plots = new TCanvas("Energy_plots", "Energy_plots", 800, 600);
    Energy_plots->Divide(7,3);

    TCanvas* TOF_plots = new TCanvas("TOF_plots", "TOF_plots", 800, 600);
    TOF_plots->Divide(7,3);

    TCanvas* Mass_plots = new TCanvas("Mass_plots", "Mass_plots", 800, 600);
    Mass_plots->Divide(7,3);

    TCanvas *Mosaic_pos_plots = new TCanvas("Mosaic_pos_plots", "Mosaic_pos_plots", 1550, 700, 600, 550);
    Mosaic_pos_plots->Divide(7,3);

    TCanvas *E_vs_TOF_plots = new TCanvas("E_vs_TOF_plots", "E_vs_TOF_plots", 1550, 700, 600, 550);
    E_vs_TOF_plots->Divide(7,3);

    TCanvas *A_vs_TOF_plots = new TCanvas("A_vs_TOF_plots", "A_vs_TOF_plots", 1550, 700, 600, 550);
    A_vs_TOF_plots->Divide(7,3);

    TCanvas *A_vs_E_plots = new TCanvas("A_vs_E_plots", "A_vs_E_plots", 1550, 700, 600, 550);
    A_vs_E_plots->Divide(7,3);

    TEllipse *e = new TEllipse(0,0, 20,20);
    e->SetLineColor(kCyan+3);
    e->SetLineWidth(3);
    e->SetFillStyle(0);
    
    TLine *vertLine = new TLine(0, -20, 0, 20);
    vertLine->SetLineColor(kRed);

    TCanvas* energy_line_calibration_graphs = new TCanvas("energy_line_calibration_graphs", "energy_line_calibration_graphs", 800, 600);
    energy_line_calibration_graphs->Divide(7,3);

    // Create a legend
    TLegend* legend = new TLegend(0.7, 0.7, 0.9, 0.9);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::vector<std::vector<std::pair<double, double>>> fitRangesTOF = {
       {{144.0, 148.0},
        {150.0, 154.0},
        {154.0, 160.0},
        {160.0, 165.0}},

       {{144.0, 148.0},
        {150.0, 154.0},
        {154.0, 160.0},
        {160.0, 165.0}},
      
       {{144.0, 148.0},
        {150.0, 154.0},
        {154.0, 160.0},
        {160.0, 165.0}},

       {{140.0, 148.0},
        {150.0, 154.0},
        {154.0, 160.0},
        {160.0, 165.0}},

       {{90.0,  103.0},
        {103.0, 110.0},
        {110.0, 115.0},
        {115.0, 122.0}},  

       {{90.0,  103.0},
        {103.0, 110.0},
        {110.0, 115.0},
        {115.0, 122.0}}, 

       {{90.0,  103.0},
        {103.0, 110.0},
        {110.0, 115.0},
        {115.0, 122.0}}, 

       {{90.0,  103.0},
        {103.0, 110.0},
        {110.0, 115.0},
        {115.0, 122.0}}, 

       {{140.0, 148.0},
        {150.0, 154.0},
        {154.0, 160.0},
        {160.0, 165.0}},

       {{90.0,  103.0},
        {103.0, 110.0},
        {110.0, 115.0},
        {115.0, 122.0}},  

       {{90.0,  103.0},
        {103.0, 110.0},
        {110.0, 115.0},
        {115.0, 122.0}}, 

       {{90.0,  103.0},
        {103.0, 110.0},
        {110.0, 115.0},
        {115.0, 122.0}}, 

       {{140.0, 148.0},
        {150.0, 154.0},
        {154.0, 160.0},
        {160.0, 165.0}},

       {{90.0,  103.0},
        {103.0, 110.0},
        {110.0, 115.0},
        {115.0, 122.0}},  

       {{90.0,  103.0},
        {103.0, 110.0},
        {110.0, 115.0},
        {115.0, 122.0}}, 

       {{90.0,  103.0},
        {103.0, 110.0},
        {110.0, 115.0},
        {115.0, 122.0}},         

       {{150.0, 160.0},
        {150.0, 154.0},
        {154.0, 160.0},
        {160.0, 165.0}},

       {{90.0,  103.0},
        {103.0, 110.0},
        {110.0, 115.0},
        {115.0, 122.0}},  

       {{90.0,  103.0},
        {103.0, 110.0},
        {110.0, 115.0},
        {115.0, 122.0}},         

       {{90.0,  103.0},
        {103.0, 110.0},
        {110.0, 115.0},
        {115.0, 122.0}},         

       {{520.0, 539.0},
        {539.0, 550.0},
        {550.0, 564.0},
        {571.0, 591.0}}

    };

    double totalFitRangeStartTOF = 255;
    double totalFitRangeEndTOF = 275;

    double maxY_TOF = 0;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::vector<std::vector<std::pair<double, double>>> fitRangesPH_fullspectra = {
       {{1046, 1126},
        {1126, 1224},
        {1224, 1282},
        {1282, 1350}},

       {{1046, 1126},
        {1126, 1224},
        {1224, 1282},
        {1282, 1350}},

       {{1046, 1126},
        {1126, 1224},
        {1224, 1282},
        {1282, 1350}},

       {{1046, 1126},
        {1126, 1224},
        {1224, 1282},
        {1282, 1350}},

       {{1046, 1126},
        {1126, 1224},
        {1224, 1282},
        {1282, 1350}},

       {{1046, 1126},
        {1126, 1224},
        {1224, 1282},
        {1282, 1350}},

       {{1046, 1126},
        {1126, 1224},
        {1224, 1282},
        {1282, 1350}},

       {{1046, 1126},
        {1126, 1224},
        {1224, 1282},
        {1282, 1350}},    

       {{1046, 1126},
        {1126, 1224},
        {1224, 1282},
        {1282, 1350}},

       {{1046, 1126},
        {1126, 1224},
        {1224, 1282},
        {1282, 1350}},

       {{1046, 1126},
        {1126, 1224},
        {1224, 1282},
        {1282, 1350}},

       {{1046, 1126},
        {1126, 1224},
        {1224, 1282},
        {1282, 1350}},    

       {{1046, 1126},
        {1126, 1224},
        {1224, 1282},
        {1282, 1350}},        

       {{1046, 1126},
        {1126, 1224},
        {1224, 1282},
        {1282, 1350}},

       {{1046, 1126},
        {1126, 1224},
        {1224, 1282},
        {1282, 1350}},

       {{1046, 1126},
        {1126, 1224},
        {1224, 1282},
        {1282, 1350}},        

       {{1046, 1126},
        {1126, 1224},
        {1224, 1282},
        {1282, 1350}},    

       {{1046, 1126},
        {1126, 1224},
        {1224, 1282},
        {1282, 1350}},

       {{1046, 1126},
        {1126, 1224},
        {1224, 1282},
        {1282, 1350}},

       {{1046, 1126},
        {1126, 1224},
        {1224, 1282},
        {1282, 1350}},        

       {{ 980, 1043},
        {1043, 1098},
        {1152, 1186},
        {1186, 1237}}     
        // Define the fit ranges specific for the hPH histograms
        // Example: {left_edge_peak1, right_edge_peak1}, {left_edge_peak2, right_edge_peak2}, ...
    };

    double totalFitRangeStartPH = 1360.1;
    double totalFitRangeEndPH = 14100.1;

    double maxY_PH = 0;
    double maxY_E = 0;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    TF1* fitFunc = new TF1("fitFunc", "pol1", 1000, 1400);

    const char* calibrated_output_for_mass = "test_output_5GHz_Masses.root";        
    TFile *calibrated_output = new TFile(calibrated_output_for_mass, "RECREATE");

    std::vector<TTree*> trees;
    TTree *tree = new TTree("CalibratedData", "Calibrated Data");

    // // Vectors to store histograms and other analysis objects
    // std::vector<TTreeReader> readers;

    std::vector<Double_t> TOF_branch(Detectors);
    std::vector<Double_t> Energy_branch(Detectors);
    std::vector<Double_t> Mass_branch(Detectors);

    // Create trees and branches in a loop
    for (int i = 0; i < Detectors; i++) {
        std::stringstream ssTreeName;
        ssTreeName << "CalibratedData_" << (i + 1);
        trees.push_back(new TTree(ssTreeName.str().c_str(), "Calibrated Data"));

        trees[i]->Branch("TOF_branch", &TOF_branch[i]);
        trees[i]->Branch("Energy_branch", &Energy_branch[i]);
        trees[i]->Branch("Mass_branch", &Mass_branch[i]);
    }

    // Open the ROOT file containing the results tree
    TFile* file = TFile::Open(filenames);

    // std::cout << "filenames: " << filenames << std::endl;

    // Check if file was successfully opened
    if (!file || file->IsZombie()) {
        std::cerr << "Error opening file: " << filenames << std::endl;
        // continue; // Skip this file
    }

    // Analysis loop
    for (int i = 0; i < Detectors; i++) {

        // Create a TTreeReader for the tree, replace the number for the Si detector you want to look at

        std::string treeName = "results_Si" + std::to_string(i); // Modify naming as necessary
        TTreeReader reader(treeName.c_str(), file);               
                
        // Create TTreeReaderValue objects for each branch dynamically
        std::vector<std::unique_ptr<TTreeReaderValue<Double_t>>> readerValues;
        for (const auto& branchName : branchNames) {
            readerValues.emplace_back(std::make_unique<TTreeReaderValue<Double_t>>(reader, branchName.c_str()));
        }

        // std::string treeName = "results_Si" + std::to_string(i);
        // readers.emplace_back(treeName.c_str(), file);

        // Create TTreeReaderValue objects for each branch
        // Create TTreeReaderValue objects for each branch dynamically
        TTreeReaderValue<Double_t> time_difference_X(reader, "time_difference_X");
        TTreeReaderValue<Double_t> time_difference_Y(reader, "time_difference_Y");
        TTreeReaderValue<Double_t> tof(reader, "tof");
        TTreeReaderValue<Double_t> ph(reader, "ph_Si");
        
        // Create histograms for each variable
        hTimeDiffX[i] = new TH1F("hTimeDiffX", "Time Difference X;Time(ns);Entries", 100, -40, 40);
        hTimeDiffY[i] = new TH1F("hTimeDiffY", "Time Difference Y;Time(ns);Entries", 100, -40, 40);
        hTOF[i] = new TH1F("hTOF", Form("TOF Si %d;Time(ns);Entries", i), 300, -1, -1);
        hPH[i] = new TH1F("hPH", Form("Pulse Height Si %d;PH;Entries", i), 5500, -1, -1);

        Energy[i] = new TH1F("Energy", Form("Energy Si %d;E;Entries", i), 1000, -1., -1);
        TOF[i] = new TH1F("TOF", Form("TOF Si %d;TOF;Entries", i), 100, -1., -1.);
        Mass[i] = new TH1F("Mass", Form("Mass Si %d;A;Entries", i), 100, -1., -1.);

        // TTree::Print()
        
        ToF_ToF_XY_ch16[i] = new TH2F("ToF_ToF_XY_ch", Form("ToF X1 and X2 vs ToF Y1 and Y2 Si %d", i), 40, -20, 20, 40, -20, 20);
        E_vs_TOF[i] = new TH2F("E_vs_TOF", Form("E vs TOF Si %d", i), 200, -1, -1, 200, -1., -1.);
        A_vs_TOF[i] = new TH2F("A_vs_TOF", Form("A vs TOF Si %d", i), 200, -1., -1., 200, -1., -1.);
        A_vs_E[i] = new TH2F("A_vs_E", Form("A vs E Si %d", i), 200, -1., -1., 200, -1., -1.);

        // Add histograms to the legend
        legend->AddEntry(hTimeDiffX[i], filenames, "l");
        
        TTree* tree;
        file->GetObject(treeName.c_str(), tree); // Assuming 'treeName' is the name of your tree

        // Loop over all entries in the tree and fill the histograms
        while (reader.Next()) {

            // std::cout << "Reading entry: " << reader.GetCurrentEntry() << std::endl;
            
            // // Iterate over each TTreeReaderValue in the vector and print the value
            // for (auto& rv : readerValues) {
            //     std::cout << rv->GetBranchName() << ": " << **rv << " ";
            // }
            std::cout << std::endl; // End the line after printing all branch values for this entry

            hTimeDiffX[i]->Fill(*time_difference_X);
            hTimeDiffY[i]->Fill(*time_difference_Y);
            hTOF[i]->Fill(*tof);
            hPH[i]->Fill(*ph);

            // cout << "ph " << *ph << endl;

            // Calculate positions from time differences
            const Double_t pos_x = (*time_difference_X) * ns_to_mm;
            const Double_t pos_y = (*time_difference_Y) * ns_to_mm;

            // Fill the 2D histogram
            ToF_ToF_XY_ch16[i]->Fill(pos_x, pos_y);

        }

        // Set different line colors
        hTimeDiffX[i]->SetLineColor(i+1);
        hTimeDiffY[i]->SetLineColor(i+1);
        hTOF[i]->SetLineColor(i+1);
        hPH[i]->SetLineColor(i+1);
        

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        // Normalize the histograms and find the maximum Y value for hTOF

        if (hTOF[i]->Integral() != 0) {
            hTOF[i]->Scale(1.0 / hTOF[i]->Integral());
        }
        maxY_TOF = std::max(maxY_TOF, hTOF[i]->GetMaximum());
    
        hTOF[i]->SetMaximum(maxY_TOF * 1.0);
        
        TFACFD_plots->cd(i+1);
        // TFACFD_plots->cd();
        
        std::vector<double> temp_Fit_centroids_TOF;
        std::vector<double> temp_Fit_centroids_TOF_error;

        double first_range_TOF, last_range_TOF;

        const auto& firstPair_TOF = fitRangesTOF[i].front();
        const auto& lastPair_TOF  = fitRangesTOF[i].back();

        first_range_TOF = firstPair_TOF.first;
        last_range_TOF = lastPair_TOF.second;
        auto results_TOF = FitAndRetrieveParameters(hTOF[i], fitRangesTOF.at(i), first_range_TOF, last_range_TOF);           

        hTOF[i]->Draw("HIST SAME");

        for (size_t j = 0; j < results_TOF.heights.size(); ++j) {

            std::cout << "Height: " << results_TOF.heights[j] << " ± " << results_TOF.heightsErrors[j] << ", "
                      << "Centroid: " << results_TOF.centroids[j] << " ± " << results_TOF.centroidsErrors[j] << ", "
                      << "Sigma: " << results_TOF.sigmas[j] << " ± " << results_TOF.sigmasErrors[j] << std::endl;

            temp_Fit_centroids_TOF.push_back(results_TOF.centroids[j]);
            temp_Fit_centroids_TOF_error.push_back(results_TOF.centroidsErrors[j]);
        }

        Fit_centroids_TOF.push_back(temp_Fit_centroids_TOF);
        Fit_centroids_TOF_error.push_back(temp_Fit_centroids_TOF_error);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        // Normalize the histograms and find the maximum Y value for hPH

        if (hPH[i]->Integral() != 0) {
            hPH[i]->Scale(1.0 / hPH[i]->Integral());
        }
        maxY_PH = std::max(maxY_PH, hPH[i]->GetMaximum());
        hPH[i]->SetMaximum(maxY_PH * 1.0); 

        CRRC4_plots->cd(i+1);
        // CRRC4_plots->cd();

        std::vector<double> temp_Fit_centroids;
        std::vector<double> temp_Fit_centroids_error;

        double first_range, last_range;

        const auto& firstPair = fitRangesPH_fullspectra[i].front();
        const auto& lastPair  = fitRangesPH_fullspectra[i].back();

        first_range = firstPair.first;
        last_range = lastPair.second;

        auto results = FitAndRetrieveParameters(hPH[i], fitRangesPH_fullspectra.at(i), first_range, last_range);

        // Optionally print out results for debugging
        std::cout << "Histogram " << i << " Fit Parameters and Uncertainties:" << std::endl;

        hPH[i]->Draw("HIST SAME");
        

        for (size_t j = 0; j < results.centroids.size(); j++) {

            std::cout << "Peak " << j << ": Centroid = " << results.centroids[j] << " ± " << results.centroidsErrors[j]
                      << ", Amplitude = " << results.heights[j] << " ± " << results.heightsErrors[j]
                      << ", Sigma = " << results.sigmas[j] << " ± " << results.sigmasErrors[j] << std::endl;

            temp_Fit_centroids.push_back(results.centroids[j]);
            temp_Fit_centroids_error.push_back(results.centroidsErrors[j]);

        }


        Fit_centroids.push_back(temp_Fit_centroids);
        Fit_centroids_error.push_back(temp_Fit_centroids_error);


        /////////////////////////// Calibration TOF /////////////////////////////


        TOF_calibration_ct.push_back(temp_Fit_centroids_TOF.at(2) - TOF_calculated[0]);
        cout << "TOF_calibration_ct " << TOF_calibration_ct.at(i) << endl;

        double cal_ct_TOF = TOF_calibration_ct.at(i);


        /////////////////////////// Calibration Energy /////////////////////////////

        calibration_graphs.push_back( new TGraphErrors( temp_Fit_centroids.size(), temp_Fit_centroids.data(), energies.data(), temp_Fit_centroids_error.data(), energies_error.data() ));
        calibration_graphs.at(i)->Fit(fitFunc, "R");
        energy_line_calibration_graphs->cd(i+1);
        // energy_line_calibration_graphs->cd();
        calibration_graphs.at(i)->Draw("AL");
        // Extract parameters
        double slope = fitFunc->GetParameter(1);
        double intercept = fitFunc->GetParameter(0);
        double slope_error = fitFunc->GetParError(1);
        double intercept_error = fitFunc->GetParError(0);

        // Print parameters
        std::cout << i << " Slope: " << slope << " ± " << slope_error
                  << ", Intercept: " << intercept << " ± " << intercept_error << std::endl;


        reader.Restart();

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        while (reader.Next()) {

            const Double_t calibrated_energy = (*ph)*slope+intercept;
            const Double_t calibrated_TOF = (*tof)-cal_ct_TOF;
            const Double_t A = pow( (light_vel*((*tof)-cal_ct_TOF)/pow(10, 9)) / (Flight_Path*pow(10, -3)), 2 )*2*((*ph)*slope+intercept)/uma_to_MeVc2;

            Energy[i]->Fill(calibrated_energy);
            TOF[i]->Fill(calibrated_TOF);
            Mass[i]->Fill(A);

            Energy_branch[i] = calibrated_energy;
            TOF_branch[i] = calibrated_TOF;
            Mass_branch[i] = A;

            E_vs_TOF[i]->Fill(calibrated_TOF, calibrated_energy);
            A_vs_TOF[i]->Fill(calibrated_TOF, A);
            A_vs_E[i]->Fill(A, calibrated_energy);
            
            // std::cout << "Energy_branch " << Energy_branch << " TOF_branch " << TOF_branch << " Mass_branch " << Mass_branch << std::endl;

            trees[i]->Fill();
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        Energy_plots->cd(i+1);
        // Energy_plots->cd();
        Energy[i]->SetLineColor(i+1);
        Energy[i]->Draw();

        TOF_plots->cd(i+1);
        // TOF_plots->cd();
        TOF[i]->SetLineColor(i+1);
        TOF[i]->Draw();
        
        Mass_plots->cd(i+1);
        // Mass_plots->cd();
        Mass[i]->SetLineColor(i+1);
        Mass[i]->Draw();

        Mosaic_pos_plots->cd(i+1);
        // Mosaic_pos_plots->cd();
        ToF_ToF_XY_ch16[i]->Draw("colz");
        e->Draw("same");
        vertLine->Draw("same");
        TLine *horizLine = new TLine(ToF_ToF_XY_ch16[i]->GetXaxis()->GetXmin(), 0, ToF_ToF_XY_ch16[i]->GetXaxis()->GetXmax(), 0);
        horizLine->SetLineColor(kBlue);
        horizLine->Draw("same");

        E_vs_TOF_plots->cd(i+1);
        // E_vs_TOF_plots->cd();
        E_vs_TOF[i]->Draw("colz");

        A_vs_TOF_plots->cd(i+1);
        // A_vs_TOF_plots->cd();
        A_vs_TOF[i]->Draw("colz");

        A_vs_E_plots->cd(i+1);
        // A_vs_E_plots->cd();
        A_vs_E[i]->Draw("colz");
    
    }

    // Write and close the file
    calibrated_output->cd();
    for (auto& tree : trees) {
        tree->Write();
    }
    calibrated_output->Close();
    delete calibrated_output; // Automatically writes to disk before deletion
}
