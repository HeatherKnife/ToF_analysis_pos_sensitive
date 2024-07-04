#include "TSpectrum.h"
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


Double_t line(Double_t *x, Double_t *par){
    Double_t  p_0 = par[0];
    Double_t  p_1 = par[1];

    return  p_0 + p_1*x[0];
}

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
std::vector<double> Mass = {4, 4, 4, 4, 4, 4, 4}; // amu
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

double calculateMass(double Energy, double TOF, double Flight_Path) {
    if (Energy == 0) {
        std::cerr << "Error: Division by zero in calculateComponent" << std::endl;
        return 0; // Handle division by zero gracefully
    }

    double A = pow(TOF/(pow(10, 9)*(Flight_Path*pow(10, -3)/light_vel)), 2)*((2*Energy));

    return A;
}

static Int_t gNPeaks; // Global variable to store the number of peaks

Double_t fpeaks(Double_t *x, Double_t *par) {
    Double_t result = 0;
    for (Int_t p = 0; p < gNPeaks; p++) {
        Double_t norm = par[3*p]; // amplitude
        Double_t mean = par[3*p+1];
        Double_t sigma = par[3*p+2];
        result += norm * TMath::Gaus(x[0], mean, sigma, kTRUE);
    }
    return result;
}

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

FitResult FitAndRetrieveParameters(TH1F* histogram) {
    TSpectrum spectrum(5); // Max peaks to search
    Int_t nfound = spectrum.Search(histogram, 1, "nobackground", 0.0001); // adjust sigma and threshold
    gNPeaks = nfound; // Set the global variable
    Double_t *xpeaks = spectrum.GetPositionX();

    TF1 *fitFunc = new TF1("fitFunc", fpeaks, 0, 23000, 3*nfound);
    fitFunc->SetNpx(1000);

    for (Int_t p = 0; p < nfound; p++) {
        fitFunc->SetParameter(3*p, histogram->GetBinContent(histogram->FindBin(xpeaks[p]))); // height
        fitFunc->SetParameter(3*p+1, xpeaks[p]); // mean
        fitFunc->SetParameter(3*p+2, 200); // sigma estimate
    }

    histogram->Fit(fitFunc, "RQ");

    FitResult result;
    for (Int_t p = 0; p < nfound; p++) {
        result.heights.push_back(fitFunc->GetParameter(3*p));
        result.centroids.push_back(fitFunc->GetParameter(3*p+1));
        result.sigmas.push_back(fitFunc->GetParameter(3*p+2));
        result.heightsErrors.push_back(fitFunc->GetParError(3*p));
        result.centroidsErrors.push_back(fitFunc->GetParError(3*p+1));
        result.sigmasErrors.push_back(fitFunc->GetParError(3*p+2));
    }

    delete fitFunc;
    return result;
}


//######################################################################################################################################################

void calibration_for_mass_plot() {

    for (int i = 0; i < energies.size(); ++i)
        {
            TOF_calculated.push_back( calculateComponent(energies[i], Mass[i], Flight_Path) );
        }

    const Double_t ns_to_mm = 1/(2*0.75); // 1 mm / 0.75 ns

    // // List of ROOT files to process
    // const char* filenames[] = {
    //     "Measurements_240311/0.root",
    //     "Measurements_240311/1.root",
    //     "Measurements_240311/2.root",
    //     "Measurements_240311/3.root"
    //     // Add more filenames as needed
    // };

    std::vector<std::string> filenames = {"Measurements_240311/correct_smp_conv_first_test_ns.root"};

    std::vector<std::string> branchNames = {"time_difference_X", "time_difference_Y", "tof", "ph"};

    const int numFiles = filenames.size();
    const int numDetectors = 20;

    // Define vectors of vectors for histograms
    std::vector<std::vector<TH1F*>> hTimeDiffX(numDetectors, std::vector<TH1F*>(numFiles));
    std::vector<std::vector<TH1F*>> hTimeDiffY(numDetectors, std::vector<TH1F*>(numFiles));
    std::vector<std::vector<TH1F*>> hTOF(numDetectors, std::vector<TH1F*>(numFiles));
    std::vector<std::vector<TH1F*>> hPH(numDetectors, std::vector<TH1F*>(numFiles));
    std::vector<std::vector<TH2F*>> hToFToFXY(numDetectors, std::vector<TH2F*>(numFiles));

    // Create a legend
    TLegend* legend = new TLegend(0.7, 0.7, 0.9, 0.9);

    TCanvas* TFACFD_plots = new TCanvas("TFACFD_plots", "TFACFD_plots", 800, 600);
    TFACFD_plots->Divide(2,2);
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

    std::vector<FitResult> allFitResults_TOF;

    double maxY_TOF = 0;

    std::vector<TCanvas*> CRRC4_plots;

    TCanvas* Energy_plots = new TCanvas("Energy_plots", "Energy_plots", 800, 600);
    Energy_plots->Divide(2,2);

    TCanvas* TOF_plots = new TCanvas("TOF_plots", "TOF_plots", 800, 600);
    TOF_plots->Divide(2,2);

    TCanvas* energy_line_calibration_graphs = new TCanvas("energy_line_calibration_graphs", "energy_line_calibration_graphs", 800, 600);
    energy_line_calibration_graphs->Divide(2,2);

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

    std::vector<double> resolutions;
    std::vector<double> resolutionErrors;

    std::vector<double> centroids;
    std::vector<double> centroids_error;

    std::vector<FitResult> allFitResults;

    // Define the new histogram with a specific range and number of bins
    int numBins = 1000;  // Example bin number
    double newXmin = 0;  // New minimum x-value
    double newXmax = 10;  // New maximum x-value
    
    TF1* fitFunc = new TF1("fitFunc", "pol1", 12000, 14500);
    
    double maxY_PH = 0;
    double maxY_E = 0;

    std::vector<TF1*> fitFunctions;

    const char* calibrated_output_for_mass = "output_for_mass.root";        
    TFile *calibrated_output = new TFile(calibrated_output_for_mass, "RECREATE");

    std::vector<TTree*> trees;
    TTree *tree = new TTree("CalibratedData", "Calibrated Data");

    std::vector<Double_t> TOF_branch(numFiles);
    std::vector<Double_t> Energy_branch(numFiles);
    std::vector<Double_t> Mass_branch(numFiles);


    for (int Detector = 0; Detector < numDetectors; ++Detector) {
        CRRC4_plots.push_back(new TCanvas(Form("CRRC4_plots_%d", Detector), Form("CRRC4_plots_%d", Detector), 800, 600));
        CRRC4_plots.at(Detector)->Divide(2,2);
        for (int fileIndex = 0; fileIndex < numFiles; ++fileIndex) {
            hTimeDiffX[Detector][fileIndex] = new TH1F(("hTimeDiffX_" + std::to_string(Detector) + "_" + std::to_string(fileIndex)).c_str(), "Time Difference X", 100, -40, 40);
            hTimeDiffY[Detector][fileIndex] = new TH1F(("hTimeDiffY_" + std::to_string(Detector) + "_" + std::to_string(fileIndex)).c_str(), "Time Difference Y", 100, -40, 40);
            hTOF[Detector][fileIndex] = new TH1F(("hTOF_" + std::to_string(Detector) + "_" + std::to_string(fileIndex)).c_str(), "Time of Flight", 300, -1, -1);
            hPH[Detector][fileIndex] = new TH1F(("hPH_" + std::to_string(Detector) + "_" + std::to_string(fileIndex)).c_str(), "Pulse Height", 2000, -1, -1);
            hToFToFXY[Detector][fileIndex] = new TH2F(("hToFToFXY_" + std::to_string(Detector) + "_" + std::to_string(fileIndex)).c_str(), "ToF vs X vs Y", 100, -20, 20, 100, -20, 20);
        }
    }

    // Loop over files
    for (int Detector = 0; Detector < numDetectors; ++Detector) {
        
        for (int i = 0; i < numFiles; i++) 
            {
                // Open the ROOT file containing the results tree
                TFile file(filenames[i].c_str(), "READ");
                
                std::string treeName = "results_Si" + std::to_string(Detector); // Modify naming as necessary
                TTreeReader reader(treeName.c_str(), &file);               
                
                // Create TTreeReaderValue objects for each branch dynamically
                std::vector<std::unique_ptr<TTreeReaderValue<Double_t>>> readerValues;
                for (const auto& branchName : branchNames) {
                    std::string fullBranchName = branchName + "_Si" + std::to_string(Detector); // Modify naming as necessary
                    readerValues.emplace_back(std::make_unique<TTreeReaderValue<Double_t>>(reader, fullBranchName.c_str()));
                }
        
                // Create TTreeReaderValue objects for each branch
                TTreeReaderValue<Double_t> time_difference_X(reader, (branchNames[0] + "_Si" + std::to_string(Detector)).c_str());
                TTreeReaderValue<Double_t> time_difference_Y(reader, (branchNames[1] + "_Si" + std::to_string(Detector)).c_str());
                TTreeReaderValue<Double_t> tof(reader, (branchNames[2] + "_Si" + std::to_string(Detector)).c_str());
                TTreeReaderValue<Double_t> ph(reader, (branchNames[3] + "_Si" + std::to_string(Detector)).c_str());
        
                                
                // Loop over all entries in the tree and fill the histograms
                while (reader.Next()) {

                    hTimeDiffX[Detector][i]->Fill(*time_difference_X);
                    hTimeDiffY[Detector][i]->Fill(*time_difference_Y);
                    hTOF[Detector][i]->Fill(*tof);
                    hPH[Detector][i]->Fill(*ph);
        
                    // Calculate positions from time differences
                    const Double_t pos_x = (*time_difference_X) * ns_to_mm;
                    const Double_t pos_y = (*time_difference_Y) * ns_to_mm;
        
                    // Fill the 2D histogram
                    hToFToFXY[Detector][i]->Fill(pos_x, pos_y);
        
                }
            
                // Set different line colors
                hTimeDiffX[Detector][i]->SetLineColor(i+1);
                hTimeDiffY[Detector][i]->SetLineColor(i+1);
                hTOF[Detector][i]->SetLineColor(i+1);
                hPH[Detector][i]->SetLineColor(i+1);     

                // Normalize the histograms and find the maximum Y value 
                if (hTOF[Detector][i]->Integral() != 0) {
                    hTOF[Detector][i]->Scale(1.0 / hTOF[Detector][i]->Integral());
                }
                maxY_TOF = std::max(maxY_TOF, hTOF[Detector][i]->GetMaximum());
                hTOF[Detector][i]->SetMaximum(maxY_TOF);            
                

                if (hPH[Detector][i]->Integral() != 0) {
                    hPH[Detector][i]->Scale(1.0 / hPH[Detector][i]->Integral());
                }
                maxY_PH = std::max(maxY_PH, hPH[Detector][i]->GetMaximum());
                hPH[Detector][i]->SetMaximum(maxY_PH);
                    
                CRRC4_plots.at(Detector)->cd(i+1);

                CRRC4_plots.at(Detector)->Update();


                // TFACFD_plots->cd(i+1);

            
                // std::vector<double> temp_Fit_centroids;
                // std::vector<double> temp_Fit_centroids_error;
            
                // std::vector<double> temp_Fit_centroids_TOF;
                // std::vector<double> temp_Fit_centroids_TOF_error;
            
            
                double first_range, last_range;  

                const auto& firstPair = fitRangesPH_fullspectra[i].front();
                const auto& lastPair  = fitRangesPH_fullspectra[i].back();  

                first_range = firstPair.first;
                last_range = lastPair.second;

                auto results = FitAndRetrieveParameters(hPH[Detector][i]);           
            
                hPH[Detector][i]->Draw("HIST SAME");

            // /*        // Optionally print out results for debugging
            //         std::cout << "Histogram " << i << " Fit Parameters and Uncertainties:" << std::endl;*/
            
            //         for (size_t j = 0; j < results_TOF.heights.size(); ++j) {
            
            //             std::cout << "Height: " << results_TOF.heights[j] << " ± " << results_TOF.heightsErrors[j] << ", "
            //                       << "Centroid: " << results_TOF.centroids[j] << " ± " << results_TOF.centroidsErrors[j] << ", "
            //                       << "Sigma: " << results_TOF.sigmas[j] << " ± " << results_TOF.sigmasErrors[j] << std::endl;
            
            //             temp_Fit_centroids_TOF.push_back(results_TOF.centroids[j]);
            //             temp_Fit_centroids_TOF_error.push_back(results_TOF.centroidsErrors[j]);
            //         }
            
            //         Fit_centroids_TOF.push_back(temp_Fit_centroids_TOF);
            //         Fit_centroids_TOF_error.push_back(temp_Fit_centroids_TOF_error);
            
            //         TOF_calibration_ct.push_back(temp_Fit_centroids_TOF.at(2) - TOF_calculated[0]);
            //         cout << "TOF_calibration_ct " << TOF_calibration_ct.at(i) << endl;
            
            //         double cal_ct_TOF = TOF_calibration_ct.at(i);
            
            //         TOF_plots->cd(i+1);
            //         TOF[i]->SetLineColor(i+1);
            //         results_Si16->Draw(Form("tof_Si16-%6f>>TOF%d", cal_ct_TOF, i), "", "");
            
            //         double first_range, last_range;
            
            //         const auto& firstPair = fitRangesPH_fullspectra[i].front();
            //         const auto& lastPair  = fitRangesPH_fullspectra[i].back();
            
            //         first_range = firstPair.first;
            //         last_range = lastPair.second;
            
            //         auto results = FitAndRetrieveParameters(hPH[i], fitRangesPH_fullspectra.at(i), first_range, last_range);
            
            //         // Optionally print out results for debugging
            //         std::cout << "Histogram " << i << " Fit Parameters and Uncertainties:" << std::endl;
            
            //         hPH[i]->Draw("HIST SAME");
                    
            
            //         for (size_t j = 0; j < results.centroids.size(); j++) {
            
            //             if (j == 1) j++;
            
            //             std::cout << "Peak " << j + 1 << ": Centroid = " << results.centroids[j] << " ± " << results.centroidsErrors[j]
            //                       << ", Amplitude = " << results.heights[j] << " ± " << results.heightsErrors[j]
            //                       << ", Sigma = " << results.sigmas[j] << " ± " << results.sigmasErrors[j] << std::endl;
            
            //             temp_Fit_centroids.push_back(results.centroids[j]);
            //             temp_Fit_centroids_error.push_back(results.centroidsErrors[j]);
            
            //         }
            
            
            //         Fit_centroids.push_back(temp_Fit_centroids);
            //         Fit_centroids_error.push_back(temp_Fit_centroids_error);
            
            //         /////////////////////////// Calibration linear fit /////////////////////////////
            //         calibration_graphs.push_back( new TGraphErrors( temp_Fit_centroids.size(), temp_Fit_centroids.data(), energies.data(), temp_Fit_centroids_error.data(), energies_error.data() ));
            //         calibration_graphs.at(i)->Fit(fitFunc, "R");
            //         energy_line_calibration_graphs->cd(i+1);
            //         calibration_graphs.at(i)->Draw("AL");
            //         // Extract parameters
            //         double slope = fitFunc->GetParameter(1);
            //         double intercept = fitFunc->GetParameter(0);
            //         double slope_error = fitFunc->GetParError(1);
            //         double intercept_error = fitFunc->GetParError(0);
            
            //         // Print parameters
            //         std::cout << i << " Slope: " << slope << " ± " << slope_error
            //                   << ", Intercept: " << intercept << " ± " << intercept_error << std::endl;
            
            //         Energy_plots->cd(i+1);
            //         Energy[i]->SetLineColor(i+1);
            //         results_Si16->Draw(Form("ph_Si16*%.6f+%6f>>Energy%d", slope, intercept, i), "", "");
            
            //         reader.Restart();
            
            //         // calibrated_output->cd();
            
            //         while (reader.Next()) {
            
            //             Energy_branch[i] = (*ph)*slope+intercept;
            //             TOF_branch[i] = (*tof)-cal_ct_TOF;
            
            //             Mass_branch[i] = pow( (light_vel*((*tof)-cal_ct_TOF)/pow(10, 9)) / (Flight_Path*pow(10, -3)), 2 )*2*((*ph)*slope+intercept)/uma_to_MeVc2;
            //             // std::cout << "Energy_branch " << Energy_branch << " TOF_branch " << TOF_branch << " Mass_branch " << Mass_branch << std::endl;
            
            
            //             trees[i]->Fill();
            //         }
            }    
    }

    //     // Write and close the file
    // calibrated_output->cd();
    // for (auto& tree : trees) {
    //     tree->Write();
    // }
    // calibrated_output->Close();
    // delete calibrated_output; // Automatically writes to disk before deletion
}
    // Initialize histograms
