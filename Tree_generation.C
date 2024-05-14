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
    
    TTree *resultsTree0 = new TTree("results_Si0", "Analysis Results for Si 0");
    TTree *resultsTree1 = new TTree("results_Si1", "Analysis Results for Si 1");
    TTree *resultsTree2 = new TTree("results_Si2", "Analysis Results for Si 2");
    TTree *resultsTree3 = new TTree("results_Si3", "Analysis Results for Si 3");
    TTree *resultsTree4 = new TTree("results_Si4", "Analysis Results for Si 4");
    TTree *resultsTree5 = new TTree("results_Si5", "Analysis Results for Si 5");
    TTree *resultsTree6 = new TTree("results_Si6", "Analysis Results for Si 6");
    TTree *resultsTree7 = new TTree("results_Si7", "Analysis Results for Si 7");
    TTree *resultsTree8 = new TTree("results_Si8", "Analysis Results for Si 8");
    TTree *resultsTree9 = new TTree("results_Si9", "Analysis Results for Si 9");
    TTree *resultsTree10 = new TTree("results_Si10", "Analysis Results for Si 10");
    TTree *resultsTree11 = new TTree("results_Si11", "Analysis Results for Si 11");
    TTree *resultsTree12 = new TTree("results_Si12", "Analysis Results for Si 12");
    TTree *resultsTree13 = new TTree("results_Si13", "Analysis Results for Si 13");
    TTree *resultsTree14 = new TTree("results_Si14", "Analysis Results for Si 14");
    TTree *resultsTree15 = new TTree("results_Si15", "Analysis Results for Si 15");
    TTree *resultsTree16 = new TTree("results_Si16", "Analysis Results for Si 16");
    TTree *resultsTree17 = new TTree("results_Si17", "Analysis Results for Si 17");
    TTree *resultsTree18 = new TTree("results_Si18", "Analysis Results for Si 18");
    TTree *resultsTree19 = new TTree("results_Si19", "Analysis Results for Si 19");
    
    // Define variables to hold the data for each branch
    // Double_t time_difference_X, time_difference_Y, tof, pos_x, pos_y, ph_Si0;

    // Create branches in the results tree
    
    Double_t ph_MCP;

    Double_t ph_X1;
    Double_t ph_X2;
    Double_t ph_Y1;
    Double_t ph_Y2;

    Double_t pos_x_Si0;
    Double_t pos_y_Si0;

    Double_t time_difference_X_Si0;
    Double_t time_difference_X_Si1;
    Double_t time_difference_X_Si2;
    Double_t time_difference_X_Si3;
    Double_t time_difference_X_Si4;
    Double_t time_difference_X_Si5;
    Double_t time_difference_X_Si6;
    Double_t time_difference_X_Si7;
    Double_t time_difference_X_Si8;
    Double_t time_difference_X_Si9;
    Double_t time_difference_X_Si10;
    Double_t time_difference_X_Si11;
    Double_t time_difference_X_Si12;
    Double_t time_difference_X_Si13;
    Double_t time_difference_X_Si14;
    Double_t time_difference_X_Si15;
    Double_t time_difference_X_Si16;
    Double_t time_difference_X_Si17;
    Double_t time_difference_X_Si18;
    Double_t time_difference_X_Si19;

    Double_t time_difference_Y_Si0;
    Double_t time_difference_Y_Si1;
    Double_t time_difference_Y_Si2;
    Double_t time_difference_Y_Si3;
    Double_t time_difference_Y_Si4;
    Double_t time_difference_Y_Si5;
    Double_t time_difference_Y_Si6;
    Double_t time_difference_Y_Si7;
    Double_t time_difference_Y_Si8;
    Double_t time_difference_Y_Si9;
    Double_t time_difference_Y_Si10;
    Double_t time_difference_Y_Si11;
    Double_t time_difference_Y_Si12;
    Double_t time_difference_Y_Si13;
    Double_t time_difference_Y_Si14;
    Double_t time_difference_Y_Si15;
    Double_t time_difference_Y_Si16;
    Double_t time_difference_Y_Si17;
    Double_t time_difference_Y_Si18;
    Double_t time_difference_Y_Si19;

    Double_t tof_Si0;
    Double_t tof_Si1;
    Double_t tof_Si2;
    Double_t tof_Si3;
    Double_t tof_Si4;
    Double_t tof_Si5;
    Double_t tof_Si6;
    Double_t tof_Si7;
    Double_t tof_Si8;
    Double_t tof_Si9;
    Double_t tof_Si10;
    Double_t tof_Si11;
    Double_t tof_Si12;
    Double_t tof_Si13;
    Double_t tof_Si14;
    Double_t tof_Si15;
    Double_t tof_Si16;
    Double_t tof_Si17;
    Double_t tof_Si18;
    Double_t tof_Si19;
    
    Double_t ph_Si0;
    Double_t ph_Si1;
    Double_t ph_Si2;
    Double_t ph_Si3;
    Double_t ph_Si4;
    Double_t ph_Si5;
    Double_t ph_Si6;
    Double_t ph_Si7;
    Double_t ph_Si8;
    Double_t ph_Si9;
    Double_t ph_Si10;
    Double_t ph_Si11;
    Double_t ph_Si12;
    Double_t ph_Si13;
    Double_t ph_Si14;
    Double_t ph_Si15;
    Double_t ph_Si16;
    Double_t ph_Si17;
    Double_t ph_Si18;
    Double_t ph_Si19;

    resultsTree0->Branch("ph_MCP", &ph_MCP);
    resultsTree0->Branch("ph_X1", &ph_X1);
    resultsTree0->Branch("ph_X2", &ph_X2);
    resultsTree0->Branch("ph_Y1", &ph_Y1);
    resultsTree0->Branch("ph_Y2", &ph_Y2);

    resultsTree0->Branch("pos_x_Si0", &pos_x_Si0);
    resultsTree0->Branch("pos_y_Si0", &pos_y_Si0);

    resultsTree0->Branch("time_difference_X_Si0", &time_difference_X_Si0);
    resultsTree1->Branch("time_difference_X_Si1", &time_difference_X_Si1);
    resultsTree2->Branch("time_difference_X_Si2", &time_difference_X_Si2);
    resultsTree3->Branch("time_difference_X_Si3", &time_difference_X_Si3);
    resultsTree4->Branch("time_difference_X_Si4", &time_difference_X_Si4);
    resultsTree5->Branch("time_difference_X_Si5", &time_difference_X_Si5);
    resultsTree6->Branch("time_difference_X_Si6", &time_difference_X_Si6);
    resultsTree7->Branch("time_difference_X_Si7", &time_difference_X_Si7);
    resultsTree8->Branch("time_difference_X_Si8", &time_difference_X_Si8);
    resultsTree9->Branch("time_difference_X_Si9", &time_difference_X_Si9);
    resultsTree10->Branch("time_difference_X_Si10", &time_difference_X_Si10);
    resultsTree11->Branch("time_difference_X_Si11", &time_difference_X_Si11);
    resultsTree12->Branch("time_difference_X_Si12", &time_difference_X_Si12);
    resultsTree13->Branch("time_difference_X_Si13", &time_difference_X_Si13);
    resultsTree14->Branch("time_difference_X_Si14", &time_difference_X_Si14);
    resultsTree15->Branch("time_difference_X_Si15", &time_difference_X_Si15);
    resultsTree16->Branch("time_difference_X_Si16", &time_difference_X_Si16);
    resultsTree17->Branch("time_difference_X_Si17", &time_difference_X_Si17);
    resultsTree18->Branch("time_difference_X_Si18", &time_difference_X_Si18);
    resultsTree19->Branch("time_difference_X_Si19", &time_difference_X_Si19);

    resultsTree0->Branch("time_difference_Y_Si0",  &time_difference_Y_Si0);
    resultsTree1->Branch("time_difference_Y_Si1",  &time_difference_Y_Si1);
    resultsTree2->Branch("time_difference_Y_Si2",  &time_difference_Y_Si2);
    resultsTree3->Branch("time_difference_Y_Si3",  &time_difference_Y_Si3);
    resultsTree4->Branch("time_difference_Y_Si4",  &time_difference_Y_Si4);
    resultsTree5->Branch("time_difference_Y_Si5",  &time_difference_Y_Si5);
    resultsTree6->Branch("time_difference_Y_Si6",  &time_difference_Y_Si6);
    resultsTree7->Branch("time_difference_Y_Si7",  &time_difference_Y_Si7);
    resultsTree8->Branch("time_difference_Y_Si8",  &time_difference_Y_Si8);
    resultsTree9->Branch("time_difference_Y_Si9",  &time_difference_Y_Si9);
    resultsTree10->Branch("time_difference_Y_Si10", &time_difference_Y_Si10);
    resultsTree11->Branch("time_difference_Y_Si11", &time_difference_Y_Si11);
    resultsTree12->Branch("time_difference_Y_Si12", &time_difference_Y_Si12);
    resultsTree13->Branch("time_difference_Y_Si13", &time_difference_Y_Si13);
    resultsTree14->Branch("time_difference_Y_Si14", &time_difference_Y_Si14);
    resultsTree15->Branch("time_difference_Y_Si15", &time_difference_Y_Si15);
    resultsTree16->Branch("time_difference_Y_Si16", &time_difference_Y_Si16);
    resultsTree17->Branch("time_difference_Y_Si17", &time_difference_Y_Si17);
    resultsTree18->Branch("time_difference_Y_Si18", &time_difference_Y_Si18);
    resultsTree19->Branch("time_difference_Y_Si19", &time_difference_Y_Si19);

    resultsTree0->Branch("tof_Si0", &tof_Si0);
    resultsTree1->Branch("tof_Si1", &tof_Si1);
    resultsTree2->Branch("tof_Si2", &tof_Si2);
    resultsTree3->Branch("tof_Si3", &tof_Si3);
    resultsTree4->Branch("tof_Si4", &tof_Si4);
    resultsTree5->Branch("tof_Si5", &tof_Si5);
    resultsTree6->Branch("tof_Si6", &tof_Si6);
    resultsTree7->Branch("tof_Si7", &tof_Si7);
    resultsTree8->Branch("tof_Si8", &tof_Si8);
    resultsTree9->Branch("tof_Si9", &tof_Si9);
    resultsTree10->Branch("tof_Si10", &tof_Si10);
    resultsTree11->Branch("tof_Si11", &tof_Si11);
    resultsTree12->Branch("tof_Si12", &tof_Si12);
    resultsTree13->Branch("tof_Si13", &tof_Si13);
    resultsTree14->Branch("tof_Si14", &tof_Si14);
    resultsTree15->Branch("tof_Si15", &tof_Si15);
    resultsTree16->Branch("tof_Si16", &tof_Si16);
    resultsTree17->Branch("tof_Si17", &tof_Si17);
    resultsTree18->Branch("tof_Si18", &tof_Si18);
    resultsTree19->Branch("tof_Si19", &tof_Si19);
    
    resultsTree0->Branch("ph_Si0", &ph_Si0);
    resultsTree1->Branch("ph_Si1", &ph_Si1);
    resultsTree2->Branch("ph_Si2", &ph_Si2);
    resultsTree3->Branch("ph_Si3", &ph_Si3);
    resultsTree4->Branch("ph_Si4", &ph_Si4);
    resultsTree5->Branch("ph_Si5", &ph_Si5);
    resultsTree6->Branch("ph_Si6", &ph_Si6);
    resultsTree7->Branch("ph_Si7", &ph_Si7);
    resultsTree8->Branch("ph_Si8", &ph_Si8);
    resultsTree9->Branch("ph_Si9", &ph_Si9);
    resultsTree10->Branch("ph_Si10", &ph_Si10);
    resultsTree11->Branch("ph_Si11", &ph_Si11);
    resultsTree12->Branch("ph_Si12", &ph_Si12);
    resultsTree13->Branch("ph_Si13", &ph_Si13);
    resultsTree14->Branch("ph_Si14", &ph_Si14);
    resultsTree15->Branch("ph_Si15", &ph_Si15);
    resultsTree16->Branch("ph_Si16", &ph_Si16);
    resultsTree17->Branch("ph_Si17", &ph_Si17);
    resultsTree18->Branch("ph_Si18", &ph_Si18);
    resultsTree19->Branch("ph_Si19", &ph_Si19);

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

    const UChar_t channel_MCP_1 = 24;
    const UChar_t channel_X1 = 20;
    const UChar_t channel_X2 = 21;    
    const UChar_t channel_Y1 = 22;
    const UChar_t channel_Y2 = 23;
    const UChar_t channel_SiDetector_ch0  = 0;
    const UChar_t channel_SiDetector_ch1  = 1;
    const UChar_t channel_SiDetector_ch2  = 2;
    const UChar_t channel_SiDetector_ch3  = 3;
    const UChar_t channel_SiDetector_ch4  = 4;
    const UChar_t channel_SiDetector_ch5  = 5;
    const UChar_t channel_SiDetector_ch6  = 6;
    const UChar_t channel_SiDetector_ch7  = 7;
    const UChar_t channel_SiDetector_ch8  = 8;
    const UChar_t channel_SiDetector_ch9  = 9;
    const UChar_t channel_SiDetector_ch10 = 10;
    const UChar_t channel_SiDetector_ch11 = 11;
    const UChar_t channel_SiDetector_ch12 = 12;
    const UChar_t channel_SiDetector_ch13 = 13;
    const UChar_t channel_SiDetector_ch14 = 14;
    const UChar_t channel_SiDetector_ch15 = 15;
    const UChar_t channel_SiDetector_ch16 = 16;
    const UChar_t channel_SiDetector_ch17 = 17;
    const UChar_t channel_SiDetector_ch18 = 18;
    const UChar_t channel_SiDetector_ch19 = 19;
    const Double_t ch_to_ns = 1.0e9 / 5e9 / 256;
    // const Double_t ch_to_ns_2 = 1.0e9 / 5e9 / 256;
    const Double_t ch_to_ns_2 = 1.0e9 / 2.5e9 / 256;
    const Double_t ns_to_mm = 1/(2*0.75); // 1 mm / 0.75 ns
    // const Double_t ns_to_mm = 1/1.24; // 1 mm / 1.24 ns
    
    // Loop over all entries of the TTree
    for (size_t i = 0; i < ABCD_events->GetEntries(); i++) {
        ABCD_events->GetEntry(i);

        // cout << "Entry: " << i << endl;

        Double_t timestamp_MCP_1 = 0;
        Double_t timestamp_X1 = 0;
        Double_t timestamp_X2 = 0;        
        Double_t timestamp_Y1 = 0;
        Double_t timestamp_Y2 = 0;
        Double_t timestamp_SiDetector_ch0  = 0;
        Double_t timestamp_SiDetector_ch1  = 0;
        Double_t timestamp_SiDetector_ch2  = 0;
        Double_t timestamp_SiDetector_ch3  = 0;
        Double_t timestamp_SiDetector_ch4  = 0;
        Double_t timestamp_SiDetector_ch5  = 0;
        Double_t timestamp_SiDetector_ch6  = 0;
        Double_t timestamp_SiDetector_ch7  = 0;
        Double_t timestamp_SiDetector_ch8  = 0;
        Double_t timestamp_SiDetector_ch9  = 0;
        Double_t timestamp_SiDetector_ch10 = 0;
        Double_t timestamp_SiDetector_ch11 = 0;
        Double_t timestamp_SiDetector_ch12 = 0;
        Double_t timestamp_SiDetector_ch13 = 0;
        Double_t timestamp_SiDetector_ch14 = 0;
        Double_t timestamp_SiDetector_ch15 = 0;
        Double_t timestamp_SiDetector_ch16 = 0;
        Double_t timestamp_SiDetector_ch17 = 0;
        Double_t timestamp_SiDetector_ch18 = 0;
        Double_t timestamp_SiDetector_ch19 = 0;

        Bool_t found_MCP_1 = false;
        Bool_t found_X1 = false;
        Bool_t found_X2 = false;        
        Bool_t found_Y1 = false;
        Bool_t found_Y2 = false;

        Bool_t found_SiDetector_ch0  = false;
        Bool_t found_SiDetector_ch1  = false;
        Bool_t found_SiDetector_ch2  = false;
        Bool_t found_SiDetector_ch3  = false;
        Bool_t found_SiDetector_ch4  = false;
        Bool_t found_SiDetector_ch5  = false;
        Bool_t found_SiDetector_ch6  = false;
        Bool_t found_SiDetector_ch7  = false;
        Bool_t found_SiDetector_ch8  = false;
        Bool_t found_SiDetector_ch9  = false;
        Bool_t found_SiDetector_ch10 = false;
        Bool_t found_SiDetector_ch11 = false;
        Bool_t found_SiDetector_ch12 = false;
        Bool_t found_SiDetector_ch13 = false;
        Bool_t found_SiDetector_ch14 = false;
        Bool_t found_SiDetector_ch15 = false;
        Bool_t found_SiDetector_ch16 = false;
        Bool_t found_SiDetector_ch17 = false;
        Bool_t found_SiDetector_ch18 = false;
        Bool_t found_SiDetector_ch19 = false;

        Double_t qlong_MCP = 0;
        Double_t qlong_X1 = 0;
        Double_t qlong_X2 = 0;
        Double_t qlong_Y1 = 0;
        Double_t qlong_Y2 = 0;

        Double_t qlong_SiDetector_ch0  = 0;
        Double_t qlong_SiDetector_ch1  = 0;
        Double_t qlong_SiDetector_ch2  = 0;
        Double_t qlong_SiDetector_ch3  = 0;
        Double_t qlong_SiDetector_ch4  = 0;
        Double_t qlong_SiDetector_ch5  = 0;
        Double_t qlong_SiDetector_ch6  = 0;
        Double_t qlong_SiDetector_ch7  = 0;
        Double_t qlong_SiDetector_ch8  = 0;
        Double_t qlong_SiDetector_ch9  = 0;
        Double_t qlong_SiDetector_ch10 = 0;
        Double_t qlong_SiDetector_ch11 = 0;
        Double_t qlong_SiDetector_ch12 = 0;
        Double_t qlong_SiDetector_ch13 = 0;
        Double_t qlong_SiDetector_ch14 = 0;
        Double_t qlong_SiDetector_ch15 = 0;
        Double_t qlong_SiDetector_ch16 = 0;
        Double_t qlong_SiDetector_ch17 = 0;
        Double_t qlong_SiDetector_ch18 = 0;
        Double_t qlong_SiDetector_ch19 = 0;

        for (size_t inner = 0; inner < coincidence_group_counter; inner++) {
            //cout << "inner: " << inner << " Timestamp: " << (unsigned long long)timestamps[inner] << " ch" << endl;

            if (channels[inner] == channel_MCP_1) {
                found_MCP_1 = true;
                timestamp_MCP_1 = timestamps[inner];
                //cout << "inner: " << inner << endl;
                //cout << "Found MCP! Timestamp: " << (unsigned long long)timestamp_MCP_1 << " ch" << endl;
            } else if (channels[inner] == channel_X1) {
                found_X1 = true;
                timestamp_X1 = timestamps[inner];
                //cout << "inner: " << inner << endl;
                //cout << "Found X1! Timestamp: " << (unsigned long long)timestamp_X1 << " ch" << endl;
            } else if (channels[inner] == channel_X2) {
                found_X2 = true;
                timestamp_X2 = timestamps[inner];
                //cout << "inner: " << inner << endl;
                //cout << "Found X2! Timestamp: " << (unsigned long long)timestamp_X2 << " ch" << endl;
            } else if (channels[inner] == channel_Y1) {
                found_Y1 = true;
                timestamp_Y1 = timestamps[inner];
                //cout << "inner: " << inner << endl;
                //cout << "Found Y1! Timestamp: " << (unsigned long long)timestamp_Y1 << " ch" << endl;
            } else if (channels[inner] == channel_Y2) {
                found_Y2 = true;
                timestamp_Y2 = timestamps[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found Y2! Timestamp: " << (unsigned long long)timestamp_Y2 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch0) {
                found_SiDetector_ch0 = true;
                timestamp_SiDetector_ch0 = timestamps[inner];
                qlong_SiDetector_ch0 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch0 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch1) {
                found_SiDetector_ch1 = true;
                timestamp_SiDetector_ch1 = timestamps[inner];
                qlong_SiDetector_ch1 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch1 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch2) {
                found_SiDetector_ch2 = true;
                timestamp_SiDetector_ch2 = timestamps[inner];
                qlong_SiDetector_ch2 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch2 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch3) {
                found_SiDetector_ch3 = true;
                timestamp_SiDetector_ch3 = timestamps[inner];
                qlong_SiDetector_ch3 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch3 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch4) {
                found_SiDetector_ch4 = true;
                timestamp_SiDetector_ch4 = timestamps[inner];
                qlong_SiDetector_ch4 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch4 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch5) {
                found_SiDetector_ch5 = true;
                timestamp_SiDetector_ch5 = timestamps[inner];
                qlong_SiDetector_ch5 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch5 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch6) {
                found_SiDetector_ch6 = true;
                timestamp_SiDetector_ch6 = timestamps[inner];
                qlong_SiDetector_ch6 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch6 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch7) {
                found_SiDetector_ch7 = true;
                timestamp_SiDetector_ch7 = timestamps[inner];
                qlong_SiDetector_ch7 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch7 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch8) {
                found_SiDetector_ch8 = true;
                timestamp_SiDetector_ch8 = timestamps[inner];
                qlong_SiDetector_ch8 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch8 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch9) {
                found_SiDetector_ch9 = true;
                timestamp_SiDetector_ch9 = timestamps[inner];
                qlong_SiDetector_ch9 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch9 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch10) {
                found_SiDetector_ch10 = true;
                timestamp_SiDetector_ch10 = timestamps[inner];
                qlong_SiDetector_ch10 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch10 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch11) {
                found_SiDetector_ch11 = true;
                timestamp_SiDetector_ch11 = timestamps[inner];
                qlong_SiDetector_ch11 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch11 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch12) {
                found_SiDetector_ch12 = true;
                timestamp_SiDetector_ch12 = timestamps[inner];
                qlong_SiDetector_ch12 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch12 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch13) {
                found_SiDetector_ch13 = true;
                timestamp_SiDetector_ch13 = timestamps[inner];
                qlong_SiDetector_ch13 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch13 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch14) {
                found_SiDetector_ch14 = true;
                timestamp_SiDetector_ch14 = timestamps[inner];
                qlong_SiDetector_ch14 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch14 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch15) {
                found_SiDetector_ch15 = true;
                timestamp_SiDetector_ch15 = timestamps[inner];
                qlong_SiDetector_ch15 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch15 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch16) {
                found_SiDetector_ch16 = true;
                timestamp_SiDetector_ch16 = timestamps[inner];
                qlong_SiDetector_ch16 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch16 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch17) {
                found_SiDetector_ch17 = true;
                timestamp_SiDetector_ch17 = timestamps[inner];
                qlong_SiDetector_ch17 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch17 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch18) {
                found_SiDetector_ch18 = true;
                timestamp_SiDetector_ch18 = timestamps[inner];
                qlong_SiDetector_ch18 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch18 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch19) {
                found_SiDetector_ch19 = true;
                timestamp_SiDetector_ch19 = timestamps[inner];
                qlong_SiDetector_ch19 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch19 << " ch" << endl;
            }
        }


        if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch0 && found_MCP_1 ) ) {

            // cout << "Found MCP and Si! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;
            //cout << "Found both! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            // const Long64_t TOF_int = timestamp_SiDetector_ch0 - timestamp_MCP_1;
            
            time_difference_X_Si0 = (time_difference_int_X) * ch_to_ns;
            time_difference_Y_Si0 = (time_difference_int_Y) * ch_to_ns;
            tof_Si0 = timestamp_SiDetector_ch0*ch_to_ns_2 - timestamp_MCP_1* ch_to_ns_2;
            
            // pos_x = (time_difference_X_Si0)*ns_to_mm;
            // pos_y = (time_difference_Y_Si0)*ns_to_mm;

            ph_Si0 = qlong_SiDetector_ch0;
            
            resultsTree0->Fill();

        } else if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch1 && found_MCP_1 ) ) {

            // cout << "Found MCP and Si! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;
            //cout << "Found both! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            // const Long64_t TOF_int = timestamp_SiDetector_ch1 - timestamp_MCP_1;
            
            time_difference_X_Si1 = (time_difference_int_X) * ch_to_ns;
            time_difference_Y_Si1 = (time_difference_int_Y) * ch_to_ns;
            tof_Si1 = timestamp_SiDetector_ch1*ch_to_ns_2 - timestamp_MCP_1* ch_to_ns_2;
            
            // pos_x = (time_difference_X_Si1)*ns_to_mm;
            // pos_y = (time_difference_Y_Si1)*ns_to_mm;

            ph_Si1 = qlong_SiDetector_ch1;
            
            resultsTree1->Fill();
        } else if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch2 && found_MCP_1 ) ) {

            // cout << "Found MCP and Si! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;
            //cout << "Found both! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            // const Long64_t TOF_int = timestamp_SiDetector_ch2 - timestamp_MCP_1;
            
            time_difference_X_Si2 = (time_difference_int_X) * ch_to_ns;
            time_difference_Y_Si2 = (time_difference_int_Y) * ch_to_ns;
            tof_Si2 = timestamp_SiDetector_ch2*ch_to_ns_2 - timestamp_MCP_1* ch_to_ns_2;
            
            // pos_x = (time_difference_X_Si2)*ns_to_mm;
            // pos_y = (time_difference_Y_Si2)*ns_to_mm;

            ph_Si1 = qlong_SiDetector_ch2;
            
            resultsTree2->Fill();
        }else if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch3 && found_MCP_1 ) ) {

            // cout << "Found MCP and Si! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;
            //cout << "Found both! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            // const Long64_t TOF_int = timestamp_SiDetector_ch3 - timestamp_MCP_1;
            
            time_difference_X_Si3 = (time_difference_int_X) * ch_to_ns;
            time_difference_Y_Si3 = (time_difference_int_Y) * ch_to_ns;
            tof_Si3 = timestamp_SiDetector_ch3*ch_to_ns_2 - timestamp_MCP_1* ch_to_ns_2;
            
            // pos_x = (time_difference_X_Si3)*ns_to_mm;
            // pos_y = (time_difference_Y_Si3)*ns_to_mm;

            ph_Si2 = qlong_SiDetector_ch3;
            
            resultsTree3->Fill();
        }else if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch4 && found_MCP_1 ) ) {

            // cout << "Found MCP and Si! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;
            //cout << "Found both! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            // const Long64_t TOF_int = timestamp_SiDetector_ch4 - timestamp_MCP_1;
            
            time_difference_X_Si4 = (time_difference_int_X) * ch_to_ns;
            time_difference_Y_Si4 = (time_difference_int_Y) * ch_to_ns;
            tof_Si4 = timestamp_SiDetector_ch4*ch_to_ns_2 - timestamp_MCP_1* ch_to_ns_2;
            
            // pos_x = (time_difference_X_Si4)*ns_to_mm;
            // pos_y = (time_difference_Y_Si4)*ns_to_mm;

            ph_Si4 = qlong_SiDetector_ch4;
            
            resultsTree4->Fill();
        }else if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch5 && found_MCP_1 ) ) {

            // cout << "Found MCP and Si! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;
            //cout << "Found both! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            // const Long64_t TOF_int = timestamp_SiDetector_ch5 - timestamp_MCP_1;
            
            time_difference_X_Si5 = (time_difference_int_X) * ch_to_ns;
            time_difference_Y_Si5 = (time_difference_int_Y) * ch_to_ns;
            tof_Si5 = timestamp_SiDetector_ch5*ch_to_ns_2 - timestamp_MCP_1* ch_to_ns_2;
            
            // pos_x = (time_difference_X_Si5)*ns_to_mm;
            // pos_y = (time_difference_Y_Si5)*ns_to_mm;

            ph_Si5 = qlong_SiDetector_ch5;
            
            resultsTree5->Fill();
        }else if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch6 && found_MCP_1 ) ) {

            // cout << "Found MCP and Si! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;
            //cout << "Found both! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            // const Long64_t TOF_int = timestamp_SiDetector_ch6 - timestamp_MCP_1;
            
            time_difference_X_Si6 = (time_difference_int_X) * ch_to_ns;
            time_difference_Y_Si6 = (time_difference_int_Y) * ch_to_ns;
            tof_Si6 = timestamp_SiDetector_ch6*ch_to_ns_2 - timestamp_MCP_1* ch_to_ns_2;
            
            // pos_x = (time_difference_X_Si6)*ns_to_mm;
            // pos_y = (time_difference_Y_Si6)*ns_to_mm;

            ph_Si6 = qlong_SiDetector_ch6;
            
            resultsTree6->Fill();
        }else if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch7 && found_MCP_1 ) ) {

            // cout << "Found MCP and Si! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;
            //cout << "Found both! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            // const Long64_t TOF_int = timestamp_SiDetector_ch7 - timestamp_MCP_1;
            
            time_difference_X_Si7 = (time_difference_int_X) * ch_to_ns;
            time_difference_Y_Si7 = (time_difference_int_Y) * ch_to_ns;
            tof_Si7 = timestamp_SiDetector_ch7*ch_to_ns_2 - timestamp_MCP_1* ch_to_ns_2;
            
            // pos_x = (time_difference_X_Si7)*ns_to_mm;
            // pos_y = (time_difference_Y_Si7)*ns_to_mm;

            ph_Si7 = qlong_SiDetector_ch7;
            
            resultsTree7->Fill();
        }else if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch8 && found_MCP_1 ) ) {

            // cout << "Found MCP and Si! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;
            //cout << "Found both! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            // const Long64_t TOF_int = timestamp_SiDetector_ch8 - timestamp_MCP_1;
            
            time_difference_X_Si8 = (time_difference_int_X) * ch_to_ns;
            time_difference_Y_Si8 = (time_difference_int_Y) * ch_to_ns;
            tof_Si8 = timestamp_SiDetector_ch8*ch_to_ns_2 - timestamp_MCP_1* ch_to_ns_2;
            
            // pos_x = (time_difference_X_Si8)*ns_to_mm;
            // pos_y = (time_difference_Y_Si8)*ns_to_mm;

            ph_Si8 = qlong_SiDetector_ch8;
            
            resultsTree8->Fill();
        }else if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch9 && found_MCP_1 ) ) {

            // cout << "Found MCP and Si! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;
            //cout << "Found both! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            // const Long64_t TOF_int = timestamp_SiDetector_ch9 - timestamp_MCP_1;
            
            time_difference_X_Si9 = (time_difference_int_X) * ch_to_ns;
            time_difference_Y_Si9 = (time_difference_int_Y) * ch_to_ns;
            tof_Si9 = timestamp_SiDetector_ch9*ch_to_ns_2 - timestamp_MCP_1* ch_to_ns_2;

/*            cout << "not convertered " << timestamp_SiDetector_ch9*ch_to_ns_2 << endl;
            cout << "convertered " << timestamp_SiDetector_ch9 << endl;
            cout << "MCP not convertered " << timestamp_MCP_1*ch_to_ns << endl;
            cout << "MCP convertered " << timestamp_MCP_1 << endl;
            cout << "TOF " << timestamp_SiDetector_ch9 - timestamp_MCP_1 << endl;
            cout << "TOF convertered " << timestamp_SiDetector_ch9*ch_to_ns_2 - timestamp_MCP_1*ch_to_ns << endl;*/
            
            // pos_x = (time_difference_X_Si9)*ns_to_mm;
            // pos_y = (time_difference_Y_Si9)*ns_to_mm;

            ph_Si9 = qlong_SiDetector_ch9;
            
            resultsTree9->Fill();
        }

        if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch10 && found_MCP_1 ) ) {

            // cout << "Found MCP and Si! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;
            //cout << "Found both! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            // const Long64_t TOF_int = timestamp_SiDetector_ch10 - timestamp_MCP_1;
            
            time_difference_X_Si10 = (time_difference_int_X) * ch_to_ns;
            time_difference_Y_Si10 = (time_difference_int_Y) * ch_to_ns;
            tof_Si10 = timestamp_SiDetector_ch10*ch_to_ns_2 - timestamp_MCP_1* ch_to_ns_2;
            
            // pos_x = (time_difference_X_Si10)*ns_to_mm;
            // pos_y = (time_difference_Y_Si10)*ns_to_mm;

            ph_Si10 = qlong_SiDetector_ch10;
            
            resultsTree10->Fill();
        } 

        if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch11 && found_MCP_1 ) ) {

            // cout << "Found MCP and Si! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;
            //cout << "Found both! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            // const Long64_t TOF_int = timestamp_SiDetector_ch11 - timestamp_MCP_1;
            
            time_difference_X_Si11 = (time_difference_int_X) * ch_to_ns;
            time_difference_Y_Si11 = (time_difference_int_Y) * ch_to_ns;
            tof_Si11 = timestamp_SiDetector_ch11*ch_to_ns_2 - timestamp_MCP_1* ch_to_ns_2;
            
            // pos_x = (time_difference_X_Si11)*ns_to_mm;
            // pos_y = (time_difference_Y_Si11)*ns_to_mm;

            ph_Si11 = qlong_SiDetector_ch11;
            
            resultsTree11->Fill();
        }else if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch12 && found_MCP_1 ) ) {

            // cout << "Found MCP and Si! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;
            //cout << "Found both! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            // const Long64_t TOF_int = timestamp_SiDetector_ch12 - timestamp_MCP_1;
            
            time_difference_X_Si12 = (time_difference_int_X) * ch_to_ns;
            time_difference_Y_Si12 = (time_difference_int_Y) * ch_to_ns;
            tof_Si12 = timestamp_SiDetector_ch12*ch_to_ns_2 - timestamp_MCP_1* ch_to_ns_2;
            
            // pos_x = (time_difference_X_Si12)*ns_to_mm;
            // pos_y = (time_difference_Y_Si12)*ns_to_mm;

            ph_Si12 = qlong_SiDetector_ch12;
            
            resultsTree12->Fill();
        }else if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch13 && found_MCP_1 ) ) {

            // cout << "Found MCP and Si! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;
            //cout << "Found both! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            // const Long64_t TOF_int = timestamp_SiDetector_ch13 - timestamp_MCP_1;
            
            time_difference_X_Si13 = (time_difference_int_X) * ch_to_ns;
            time_difference_Y_Si13 = (time_difference_int_Y) * ch_to_ns;
            tof_Si13 = timestamp_SiDetector_ch13*ch_to_ns_2 - timestamp_MCP_1* ch_to_ns_2;
            
            // pos_x = (time_difference_X_Si13)*ns_to_mm;
            // pos_y = (time_difference_Y_Si13)*ns_to_mm;

            ph_Si13 = qlong_SiDetector_ch13;
            
            resultsTree13->Fill();
        }else if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch14 && found_MCP_1 ) ) {

            // cout << "Found MCP and Si! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;
            //cout << "Found both! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            // const Long64_t TOF_int = timestamp_SiDetector_ch14 - timestamp_MCP_1;
            
            time_difference_X_Si14 = (time_difference_int_X) * ch_to_ns;
            time_difference_Y_Si14 = (time_difference_int_Y) * ch_to_ns;
            tof_Si14 = timestamp_SiDetector_ch14*ch_to_ns_2 - timestamp_MCP_1* ch_to_ns_2;
            
            // pos_x = (time_difference_X_Si14)*ns_to_mm;
            // pos_y = (time_difference_Y_Si14)*ns_to_mm;

            ph_Si14 = qlong_SiDetector_ch14;
            
            resultsTree14->Fill();
        }else if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch15 && found_MCP_1 ) ) {

            // cout << "Found MCP and Si! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;
            //cout << "Found both! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            // const Long64_t TOF_int = timestamp_SiDetector_ch15 - timestamp_MCP_1;
            
            time_difference_X_Si15 = (time_difference_int_X) * ch_to_ns;
            time_difference_Y_Si15 = (time_difference_int_Y) * ch_to_ns;
            tof_Si15 = timestamp_SiDetector_ch15*ch_to_ns_2 - timestamp_MCP_1* ch_to_ns_2;
            
            // pos_x = (time_difference_X_Si15)*ns_to_mm;
            // pos_y = (time_difference_Y_Si15)*ns_to_mm;

            ph_Si15 = qlong_SiDetector_ch15;
            
            resultsTree15->Fill();
        }else if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch16 && found_MCP_1 ) ) {

            // cout << "Found MCP and Si! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;
            //cout << "Found both! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            // const Long64_t TOF_int = timestamp_SiDetector_ch16 - timestamp_MCP_1;
            
            time_difference_X_Si16 = (time_difference_int_X) * ch_to_ns;
            time_difference_Y_Si16 = (time_difference_int_Y) * ch_to_ns;
            tof_Si16 = timestamp_SiDetector_ch16*ch_to_ns_2 - timestamp_MCP_1* ch_to_ns_2;
            
            // pos_x = (time_difference_X_Si16)*ns_to_mm;
            // pos_y = (time_difference_Y_Si16)*ns_to_mm;

            ph_Si16 = qlong_SiDetector_ch16;
            
            resultsTree16->Fill();
        }else if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch17 && found_MCP_1 ) ) {

            // cout << "Found MCP and Si! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;
            //cout << "Found both! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            // const Long64_t TOF_int = timestamp_SiDetector_ch17 - timestamp_MCP_1;
            
            time_difference_X_Si17 = (time_difference_int_X) * ch_to_ns;
            time_difference_Y_Si17 = (time_difference_int_Y) * ch_to_ns;
            tof_Si17 = timestamp_SiDetector_ch17*ch_to_ns_2 - timestamp_MCP_1* ch_to_ns_2;
            
            // pos_x = (time_difference_X_Si17)*ns_to_mm;
            // pos_y = (time_difference_Y_Si17)*ns_to_mm;

            ph_Si17 = qlong_SiDetector_ch17;
            
            resultsTree17->Fill();
        }else if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch18 && found_MCP_1 ) ) {

            // cout << "Found MCP and Si! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;
            //cout << "Found both! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            // const Long64_t TOF_int = timestamp_SiDetector_ch18 - timestamp_MCP_1;
            
            time_difference_X_Si18 = (time_difference_int_X) * ch_to_ns;
            time_difference_Y_Si18 = (time_difference_int_Y) * ch_to_ns;
            tof_Si18 = timestamp_SiDetector_ch18*ch_to_ns_2 - timestamp_MCP_1* ch_to_ns_2;
            
            // pos_x = (time_difference_X_Si18)*ns_to_mm;
            // pos_y = (time_difference_Y_Si18)*ns_to_mm;

            ph_Si18 = qlong_SiDetector_ch18;
            
            resultsTree18->Fill();
        }else if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch19 && found_MCP_1 ) ) {

            // cout << "Found MCP and Si! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;
            //cout << "Found both! Time difference: " << (long long)TOF_int << " ch; " << tof << " ns" << endl;

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            // const Long64_t TOF_int = timestamp_SiDetector_ch19 - timestamp_MCP_1;
            
            time_difference_X_Si19 = (time_difference_int_X) * ch_to_ns;
            time_difference_Y_Si19 = (time_difference_int_Y) * ch_to_ns;
            tof_Si19 = timestamp_SiDetector_ch19*ch_to_ns_2 - timestamp_MCP_1* ch_to_ns_2;
            
            // pos_x = (time_difference_X_Si19)*ns_to_mm;
            // pos_y = (time_difference_Y_Si19)*ns_to_mm;

            ph_Si19 = qlong_SiDetector_ch19;
            
            resultsTree19->Fill();
        }


    }

    // Write the tree to file and clean up
    resultsTree0->Write();
    resultsTree1->Write();
    resultsTree2->Write();
    resultsTree3->Write();
    resultsTree4->Write();
    resultsTree5->Write();
    resultsTree6->Write();
    resultsTree7->Write();
    resultsTree8->Write();
    resultsTree9->Write();
    resultsTree10->Write();
    resultsTree11->Write();
    resultsTree12->Write();
    resultsTree13->Write();
    resultsTree14->Write();
    resultsTree15->Write();
    resultsTree16->Write();
    resultsTree17->Write();
    resultsTree18->Write();
    resultsTree19->Write();
    delete outputFile; // Automatically writes to disk before deletion

}
