#include "TFile.h"
#include "TH1F.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TSpectrum.h"
#include <TGraphErrors.h>
#include <vector>
#include <iostream>
#include <TH2F.h>
#include <TEllipse.h>
#include <TLine.h>
#include <sstream>  // Include this header for std::stringstream

#include "TF1.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLatex.h"

void ToF_analysis_pos_sensitive_vectorized(const TString& inputFilename) {
    // Open the file containing the tree.
    TFile *myFile = TFile::Open(inputFilename);

    // Check if file was successfully opened
    if (!myFile || myFile->IsZombie()) {
        cout << "Error: Could not open input file " << inputFilename << endl;
        // continue; // Skip this myFile
    }

    const Double_t ns_to_mm = 1/(2*0.75); // 1 mm / 0.75 ns

    int Detectors = 20;

    TH1F* hTimeDiffX[Detectors];
    TH1F* hTimeDiffY[Detectors];
    TH1F* hTOF[Detectors];
    TH1F* hPH[Detectors];
    TH2F* ToF_ToF_XY[Detectors];  
    TH2F* ph_vs_TOF[Detectors];  

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    TCanvas* TFACFD_plots = new TCanvas("TFACFD_plots", "TFACFD_plots", 800, 600);
    TFACFD_plots->Divide(7,3);

    TCanvas* CRRC4_plots = new TCanvas("CRRC4_plots", "CRRC4_plots", 800, 600);
    CRRC4_plots->Divide(7,3);

    TCanvas *Mosaic_pos_plots = new TCanvas("Mosaic_pos_plots", "Mosaic_pos_plots", 1550, 700, 600, 550);
    Mosaic_pos_plots->Divide(7,3);

    TCanvas *ph_vs_TOF_plots = new TCanvas("ph_vs_TOF_plots", "ph_vs_TOF_plots", 1550, 700, 600, 550);
    ph_vs_TOF_plots->Divide(7,3);

    // Create a legend
    TLegend* legend = new TLegend(0.7, 0.7, 0.9, 0.9);

    TEllipse *e = new TEllipse(0,0, 20,20);
    e->SetLineColor(kCyan+3);
    e->SetLineWidth(3);
    e->SetFillStyle(0);
    
    TLine *vertLine = new TLine(0, -20, 0, 20);
    vertLine->SetLineColor(kRed);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    double maxY_TOF = 0;
    double maxY_PH = 0;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    // Analysis loop
    for (int i = 0; i < Detectors; i++) {

        // if (i == 15) continue;

        // Create a TTreeReader for the tree, replace the number for the Si detector you want to look at
        std::string treeName = "results_Si" + std::to_string(i); // Modify naming as necessary
        TTreeReader reader(treeName.c_str(), myFile);    

       // Create TTreeReaderValue objects for each branch dynamically
        TTreeReaderValue<Double_t> time_difference_X(reader, "time_difference_X");
        TTreeReaderValue<Double_t> time_difference_Y(reader, "time_difference_Y");
        TTreeReaderValue<Double_t> tof(reader, "tof");
        TTreeReaderValue<Double_t> ph(reader, "ph_Si");
        
        // Create histograms for each variable
        hTimeDiffX[i] = new TH1F("hTimeDiffX", "Time Difference X;Time(ns);Entries", 100, -40, 40);
        hTimeDiffY[i] = new TH1F("hTimeDiffY", "Time Difference Y;Time(ns);Entries", 100, -40, 40);
        hTOF[i] = new TH1F("hTOF", Form("TOF Si %d;TOF;Entries", i), 1600, -1, -1);
        hPH[i] = new TH1F("hPH", Form("Pulse Height Si %d;PH;Entries", i), 600, -1, -1);
        ToF_ToF_XY[i] = new TH2F("ToF_ToF_XY_ch", Form("ToF X1 and X2 vs ToF Y1 and Y2 Si %d", i), 40, -20, 20, 40, -20, 20);
        ph_vs_TOF[i] = new TH2F("ph_vs_TOF", Form("E vs TOF Si %d; TOF; E [MeV]", i), 1600, -1, -1, 1600, -1, -1);

        // Add histograms to the legend
        // legend->AddEntry(hTimeDiffX[i], filenames, "l");


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
            ph_vs_TOF[i]->Fill(*tof, *ph);

            // cout << "ph " << *ph << endl;

            // Calculate positions from time differences
            const Double_t pos_x = (*time_difference_X) * ns_to_mm;
            const Double_t pos_y = (*time_difference_Y) * ns_to_mm;

            // Fill the 2D histogram
            ToF_ToF_XY[i]->Fill(pos_x, pos_y);

        }

        // Set different line colors
        hTimeDiffX[i]->SetLineColor(kBlack);
        hTimeDiffY[i]->SetLineColor(kBlack);
        hTOF[i]->SetLineColor(kBlack);
        hPH[i]->SetLineColor(kBlack);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        // Normalize the histograms and find the maximum Y value for hTOF

        if (hTOF[i]->Integral() != 0) {
            hTOF[i]->Scale(1.0 / hTOF[i]->Integral());
        }
        maxY_TOF = std::max(maxY_TOF, hTOF[i]->GetMaximum());
    
        // hTOF[i]->SetMaximum(maxY_TOF * 1.0);
        
        TFACFD_plots->cd(i+1);
        // TFACFD_plots->cd(i+1)->SetLogy();
        hTOF[i]->Draw("HIST SAME");

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        // Normalize the histograms and find the maximum Y value for hPH

        if (hPH[i]->Integral() != 0) {
            hPH[i]->Scale(1.0 / hPH[i]->Integral());
        }

        maxY_PH = std::max(maxY_PH, hPH[i]->GetMaximum());
        hPH[i]->SetMaximum(maxY_PH * 1.0); 

        CRRC4_plots->cd(i+1);
        CRRC4_plots->cd(i+1)->SetLogy();  
        hPH[i]->Draw("HIST SAME");

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        Mosaic_pos_plots->cd(i+1);
        ToF_ToF_XY[i]->Draw("colz");
        e->Draw("same");
        vertLine->Draw("same");
        TLine *horizLine = new TLine(ToF_ToF_XY[i]->GetXaxis()->GetXmin(), 0, ToF_ToF_XY[i]->GetXaxis()->GetXmax(), 0);
        horizLine->SetLineColor(kBlue);
        horizLine->Draw("same");

        ph_vs_TOF_plots->cd(i+1);
        // ph_vs_TOF_plots->cd();
        ph_vs_TOF[i]->Draw("colz");

    }

}
