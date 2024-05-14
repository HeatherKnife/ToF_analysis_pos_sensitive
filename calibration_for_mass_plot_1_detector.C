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
std::vector<double> energies = { 5.105, 5.155,      5.388, 5.443, 5.486,    5.763, 5.805};
std::vector<double> energies_error = { 0.00001, 0.00001, 0.00001, 0.00001, 0.00001, 0.00001, 0.00001};
std::vector<double> mass_component = {4, 4, 4, 4, 4, 4, 4}; // amu
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
        gaussFit->FixParameter(1, 12950); // Fix mean to 150

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

void calibration_for_mass_plot_1_detector() {

    for (int i = 0; i < energies.size(); ++i)
        {
            TOF_calculated.push_back( calculateComponent(energies[i], mass_component[i], Flight_Path) );
        }

    const Double_t ns_to_mm = 1/(2*0.75); // 1 mm / 0.75 ns

    // List of ROOT files to process
    const char* filenames[] = {
        "Measurements_240311/0.root",
        "Measurements_240311/1.root",
        "Measurements_240311/2.root",
        "Measurements_240311/3.root"
        // Add more filenames as needed
    };

    std::vector<std::string> fileNames = {"file1.root", "file2.root", "file3.root", "file4.root"};
    
    const int numFiles = sizeof(filenames) / sizeof(filenames[0]);

    // Arrays of histograms for each variable, one for each file
    TH1F* hTimeDiffX[numFiles];
    TH1F* hTimeDiffY[numFiles];
    TH1F* hTOF[numFiles];
    TH1F* hPH[numFiles];
    TH1F* Energy[numFiles];
    TH1F* TOF[numFiles];
    TH1F* Mass[numFiles];
    TH2F *ToF_ToF_XY_ch16[numFiles];  
    TH2F *E_vs_TOF[numFiles];  
    TH2F *A_vs_TOF[numFiles];  
    TH2F *A_vs_E[numFiles];  

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    TCanvas* TFACFD_plots = new TCanvas("TFACFD_plots", "TFACFD_plots", 800, 600);
    TFACFD_plots->Divide(2,2);

    TCanvas* CRRC4_plots = new TCanvas("CRRC4_plots", "CRRC4_plots", 800, 600);
    CRRC4_plots->Divide(2,2);

    TCanvas* Energy_plots = new TCanvas("Energy_plots", "Energy_plots", 800, 600);
    Energy_plots->Divide(2,2);

    TCanvas* TOF_plots = new TCanvas("TOF_plots", "TOF_plots", 800, 600);
    TOF_plots->Divide(2,2);

    TCanvas* Mass_plots = new TCanvas("Mass_plots", "Mass_plots", 800, 600);
    Mass_plots->Divide(2,2);

    TCanvas *Mosaic_pos_plots = new TCanvas("Mosaic_pos_plots", "Mosaic_pos_plots", 1550, 700, 600, 550);
    Mosaic_pos_plots->Divide(2,2);

    TCanvas *E_vs_TOF_plots = new TCanvas("E_vs_TOF_plots", "E_vs_TOF_plots", 1550, 700, 600, 550);
    E_vs_TOF_plots->Divide(2,2);

    TCanvas *A_vs_TOF_plots = new TCanvas("A_vs_TOF_plots", "A_vs_TOF_plots", 1550, 700, 600, 550);
    A_vs_TOF_plots->Divide(2,2);

    TCanvas *A_vs_E_plots = new TCanvas("A_vs_E_plots", "A_vs_E_plots", 1550, 700, 600, 550);
    A_vs_E_plots->Divide(2,2);

    TEllipse *e = new TEllipse(0,0, 20,20);
    e->SetLineColor(kCyan+3);
    e->SetLineWidth(3);
    e->SetFillStyle(0);
    
    TLine *vertLine = new TLine(0, -20, 0, 20);
    vertLine->SetLineColor(kRed);

    TCanvas* energy_line_calibration_graphs = new TCanvas("energy_line_calibration_graphs", "energy_line_calibration_graphs", 800, 600);
    energy_line_calibration_graphs->Divide(2,2);

    // Create a legend
    TLegend* legend = new TLegend(0.7, 0.7, 0.9, 0.9);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::vector<std::vector<std::pair<double, double>>> fitRangesTOF = {
       {{255  , 259.1},
        {259.1, 263.0},
        {263.0, 270.0}},

       {{255, 259},
        {259, 263},
        {263, 270}},
      
       {{259, 263},
        {263, 266},
        {266, 271}},

       {{262, 266},
        {260, 270},
        {270, 275}},
    };

    double totalFitRangeStartTOF = 255;
    double totalFitRangeEndTOF = 275;

    double maxY_TOF = 0;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::vector<std::vector<std::pair<double, double>>> fitRangesPH_fullspectra = {
       {{12230, 12290},
        {12290, 12340},
        {12340, 12500},
        {12853, 13033},
        {13033, 13127},
        {13127, 13342},
        {13800, 13900},
        {13900, 14050}},

       {{12150, 12290},
        {12290, 12360},
        {12360, 12500},
        {12860, 12980},
        {13040, 13127},
        {13127, 13342},
        {13700, 13890},
        {13890, 14100}},

       {{12250, 12304},
        {12304, 12400},
        {12400, 12500},
        {12960, 13000},
        {13030, 13140},
        {13140, 13336},
        {13850, 13990},
        {13990, 14100}},

       {{12250, 12338},
        {12338, 12395},
        {12395, 12500},
        {12853, 13033},
        {13033, 13160},
        {13160, 13342},
        {13839, 13972},
        {13972, 14118}}
        
        // Define the fit ranges specific for the hPH histograms
        // Example: {left_edge_peak1, right_edge_peak1}, {left_edge_peak2, right_edge_peak2}, ...
    };

    double totalFitRangeStartPH = 12853.1;
    double totalFitRangeEndPH = 14100.1;

    double maxY_PH = 0;
    double maxY_E = 0;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    TF1* fitFunc = new TF1("fitFunc", "pol1", 12000, 14500);

    const char* calibrated_output_for_mass = "output_for_mass.root";        
    TFile *calibrated_output = new TFile(calibrated_output_for_mass, "RECREATE");

    std::vector<TTree*> trees;
    TTree *tree = new TTree("CalibratedData", "Calibrated Data");

    std::vector<Double_t> TOF_branch(numFiles);
    std::vector<Double_t> Energy_branch(numFiles);
    std::vector<Double_t> Mass_branch(numFiles);

    // Create trees and branches in a loop
    for (int i = 0; i < numFiles; i++) {
        std::stringstream ssTreeName;
        ssTreeName << "CalibratedData_" << (i + 1);
        trees.push_back(new TTree(ssTreeName.str().c_str(), "Calibrated Data"));

        trees[i]->Branch("TOF_branch", &TOF_branch[i]);
        trees[i]->Branch("Energy_branch", &Energy_branch[i]);
        trees[i]->Branch("Mass_branch", &Mass_branch[i]);
    }

    // Analysis loop
    for (int i = 0; i < numFiles; i++) {

        // Open the ROOT file containing the results tree
        TFile* file = TFile::Open(filenames[i]);

        // std::cout << "filenames: " << filenames[i] << std::endl;

        // Check if file was successfully opened
        if (!file || file->IsZombie()) {
            std::cerr << "Error opening file: " << filenames[i] << std::endl;
            continue; // Skip this file
        }
        
        // Create a TTreeReader for the tree, replace the number for the Si detector you want to look at
        TTreeReader reader("results_Si16", file);

        // Create TTreeReaderValue objects for each branch
        TTreeReaderValue<Double_t> time_difference_X(reader, "time_difference_X_Si16");
        TTreeReaderValue<Double_t> time_difference_Y(reader, "time_difference_Y_Si16");
        TTreeReaderValue<Double_t> tof(reader, "tof_Si16");
        TTreeReaderValue<Double_t> ph(reader, "ph_Si16");
        
        // Create histograms for each variable
        hTimeDiffX[i] = new TH1F(Form("hTimeDiffX_%d", i), "Time Difference X;Time(ns);Entries", 100, -40, 40);
        hTimeDiffY[i] = new TH1F(Form("hTimeDiffY_%d", i), "Time Difference Y;Time(ns);Entries", 100, -40, 40);
        hTOF[i] = new TH1F(Form("hTOF_%d", i), "TOF;Time(ns);Entries", 300, -1, -1);
        hPH[i] = new TH1F(Form("hPH_%d", i), "Pulse Height;PH;Entries", 2000, 8000, 22000);

        Energy[i] = new TH1F(Form("Energy%d", i), "Energy;E;Entries", 1000, 4.5, 6.5);
        TOF[i] = new TH1F(Form("TOF%d", i), "TOF;TOF;Entries", 100, 20., 40.);
        Mass[i] = new TH1F(Form("Mass%d", i), "Mass;A;Entries", 100, 2., 5.);

        ToF_ToF_XY_ch16[i] = new TH2F(Form("ToF_ToF_XY_ch16_%d", i), "ToF X1 and X2 vs ToF Y1 and Y2 Si16", 40, -20, 20, 40, -20, 20);
        E_vs_TOF[i] = new TH2F(Form("E_vs_TOF_%d", i), "E vs TOF Si16", 1000, 4.5, 6.5, 100, 20., 40.);
        A_vs_TOF[i] = new TH2F(Form("A_vs_TOF%d", i), "A vs TOF Si16", 100, 2., 5., 100, 20., 40.);
        A_vs_E[i] = new TH2F(Form("A_vs_E%d", i), "A vs E Si16", 100, 2., 5., 1000, 4.5, 6.5);

        // Add histograms to the legend
        legend->AddEntry(hTimeDiffX[i], filenames[i], "l");
        
        // Loop over all entries in the tree and fill the histograms
        while (reader.Next()) {
            hTimeDiffX[i]->Fill(*time_difference_X);
            hTimeDiffY[i]->Fill(*time_difference_Y);
            hTOF[i]->Fill(*tof);
            hPH[i]->Fill(*ph);

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

            if (j == 1) j++;

            std::cout << "Peak " << j + 1 << ": Centroid = " << results.centroids[j] << " ± " << results.centroidsErrors[j]
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

            E_vs_TOF[i]->Fill(calibrated_energy, calibrated_TOF);
            A_vs_TOF[i]->Fill(A, calibrated_TOF);
            A_vs_E[i]->Fill(A, calibrated_energy);
            
            // std::cout << "Energy_branch " << Energy_branch << " TOF_branch " << TOF_branch << " Mass_branch " << Mass_branch << std::endl;

            trees[i]->Fill();
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        Energy_plots->cd(i+1);
        Energy[i]->SetLineColor(i+1);
        Energy[i]->Draw();

        TOF_plots->cd(i+1);
        TOF[i]->SetLineColor(i+1);
        TOF[i]->Draw();
        
        Mass_plots->cd(i+1);
        Mass[i]->SetLineColor(i+1);
        Mass[i]->Draw();

        Mosaic_pos_plots->cd(i+1);
        ToF_ToF_XY_ch16[i]->Draw("colz");
        e->Draw("same");
        vertLine->Draw("same");
        TLine *horizLine = new TLine(ToF_ToF_XY_ch16[i]->GetXaxis()->GetXmin(), 0, ToF_ToF_XY_ch16[i]->GetXaxis()->GetXmax(), 0);
        horizLine->SetLineColor(kBlue);
        horizLine->Draw("same");

        E_vs_TOF_plots->cd(i+1);
        E_vs_TOF[i]->Draw("colz");

        A_vs_TOF_plots->cd(i+1);
        A_vs_TOF[i]->Draw("colz");

        A_vs_E_plots->cd(i+1);
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
