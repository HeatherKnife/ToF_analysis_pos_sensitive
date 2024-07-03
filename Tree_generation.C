#include "TFile.h"
#include "TH1F.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include <vector>
#include <string>
#include <TF1.h>
#include <TLatex.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <iostream>
#include <map>

void Tree_generation(const TString& inputFilename, const TString& outputFilename) {

    // Open the file containing the tree.

    TFile *myFile = TFile::Open(inputFilename);
    if (!myFile || !myFile->IsOpen()) {
        cout << "Error: Could not open input file " << inputFilename << endl;
        return;
    }

    TTree *ABCD_events = (TTree*)myFile->Get("ABCD_events");
    if (!ABCD_events) {
        cout << "Error: Could not find tree ABCD_events in file " << inputFilename << endl;
        myFile->Close();
        return;
    }

    TFile *outputFile = new TFile(outputFilename, "RECREATE");
    if (!outputFile || !outputFile->IsOpen()) {
        cout << "Error: Could not create output file " << outputFilename << endl;
        myFile->Close();
        return;
    }

    int numDetectors = 22;

    // Create a vector of TTrees.
    std::vector<TTree*> resultsTrees;
    for (int i = 0; i < numDetectors; i++) {  
        resultsTrees.push_back(new TTree(Form("results_Si%d", i), Form("Analysis Results for Si %d", i)));
    }

    // Define variables for branches.
    
    Double_t ph_MCP;

    Double_t ph_X1;
    Double_t ph_X2;
    Double_t ph_Y1;
    Double_t ph_Y2;

    std::vector<Double_t> pos_x(numDetectors), pos_y(numDetectors);
    std::vector<Double_t> time_difference_X(numDetectors), time_difference_Y(numDetectors);
    std::vector<Double_t> tof(numDetectors);
    std::vector<Double_t> ph_Si(numDetectors);

    // Create branches for MCP and Si detectors.
    for (int i = 0; i < numDetectors; i++) {  
        resultsTrees[i]->Branch("ph_MCP", &ph_MCP);
        resultsTrees[i]->Branch("ph_X1", &ph_X1);
        resultsTrees[i]->Branch("ph_X2", &ph_X2);
        resultsTrees[i]->Branch("ph_Y1", &ph_Y1);
        resultsTrees[i]->Branch("ph_Y2", &ph_Y2);
        resultsTrees[i]->Branch("pos_x", &pos_x[i]);
        resultsTrees[i]->Branch("pos_y", &pos_y[i]);
        resultsTrees[i]->Branch("time_difference_X", &time_difference_X[i]);
        resultsTrees[i]->Branch("time_difference_Y", &time_difference_Y[i]);
        resultsTrees[i]->Branch("tof", &tof[i]);
        resultsTrees[i]->Branch("ph_Si", &ph_Si[i]);
    }

    // Define channels for other measurements
    const UChar_t channel_MCP_1 = 28;
    const UChar_t channel_X1 = 27;
    const UChar_t channel_X2 = 26;    
    const UChar_t channel_Y1 = 25;
    const UChar_t channel_Y2 = 24;

    UChar_t coincidence_group_counter;

    ULong64_t timestamps[1 + UINT8_MAX];
    UShort_t qshorts[1 + UINT8_MAX];
    UShort_t qlongs[1 + UINT8_MAX];
    UShort_t baselines[1 + UINT8_MAX];
    UChar_t channels[1 + UINT8_MAX];
    UChar_t group_counters[1 + UINT8_MAX];

    ABCD_events->SetBranchAddress("coincidence_group_counter", &coincidence_group_counter);
    ABCD_events->SetBranchAddress("timestamp", timestamps);
    ABCD_events->SetBranchAddress("qshort",    qshorts);
    ABCD_events->SetBranchAddress("qlong",     qlongs);
    ABCD_events->SetBranchAddress("baseline",  baselines);
    ABCD_events->SetBranchAddress("channel",   channels);
    ABCD_events->SetBranchAddress("group_counter", group_counters);

    const Double_t ch_to_ns = (25./64.)*(1e-3);

    // const Double_t ch_to_ns = 1.0e9 / 5e9 / 256;
    // const Double_t ch_to_ns_2 = 1.0e9 / 5e9 / 256;
    // const Double_t ch_to_ns_2 = 1.0e9 / 2.5e9 / 256;
    const Double_t ns_to_mm = 1/(2*0.75); // 1 mm / 0.75 ns
    // const Double_t ns_to_mm = 1/1.24; // 1 mm / 1.24 ns

    // Process each event.
    for (size_t i = 0; i < ABCD_events->GetEntries(); i++) {
        ABCD_events->GetEntry(i);

        bool found_X1 = false, found_X2 = false, found_Y1 = false, found_Y2 = false, found_MCP_1 = false;
        Double_t timestamp_X1 = 0, timestamp_X2 = 0, timestamp_Y1 = 0, timestamp_Y2 = 0, timestamp_MCP_1 = 0;
        std::vector<bool> found_Si(numDetectors, false);
        std::vector<Double_t> timestamp_Si(numDetectors, 0);

        // Identify channels and timestamps.
        for (size_t inner = 0; inner < coincidence_group_counter; inner++) {
            switch (channels[inner]) {
                case channel_MCP_1: found_MCP_1 = true; timestamp_MCP_1 = timestamps[inner]; break;
                case channel_X1: found_X1 = true; timestamp_X1 = timestamps[inner]; break;
                case channel_X2: found_X2 = true; timestamp_X2 = timestamps[inner]; break;
                case channel_Y1: found_Y1 = true; timestamp_Y1 = timestamps[inner]; break;
                case channel_Y2: found_Y2 = true; timestamp_Y2 = timestamps[inner]; break;
                default:
                    if (channels[inner] < numDetectors) {
                        found_Si[channels[inner]] = true;
                        timestamp_Si[channels[inner]] = timestamps[inner];
                        ph_Si[channels[inner]] = qlongs[inner];
                    }
                    break;
            }
        }

        // Compute values for detectors where all required signals were found.
        for (int j = 1; j < numDetectors+1; j++) {
            // printf("\n j: %i", j );
            if (found_X1 && found_X2 && found_Y1 && found_Y2 && found_Si[j] && found_MCP_1) {
                const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
                const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
                const Long64_t TOF_int = timestamp_Si[j] - timestamp_MCP_1;

                time_difference_X[j] = time_difference_int_X * ch_to_ns;
                time_difference_Y[j] = time_difference_int_Y * ch_to_ns;
                tof[j] = TOF_int * ch_to_ns;
                // if (j==1)
                // {
                //     printf("tof: %f", tof[j]);
                // }
                resultsTrees[j-1]->Fill();
            }
        }
    }

    // Save all trees and close the file.
    for (auto tree : resultsTrees) {
        tree->Write();
    }
    delete outputFile;
}