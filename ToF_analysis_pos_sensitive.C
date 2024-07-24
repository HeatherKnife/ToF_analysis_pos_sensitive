#include "TFile.h"
#include "TH1F.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
 
void ToF_analysis_pos_sensitive() {
    // Open the file containing the tree.

    // TFile *myFile = TFile::Open("Measurements_240321/2024-03-21T18:13:31_Run_0_Cu-248_experiment_Target_2450_Mirror_2550_MCP_2500_coincident_events.root"); // all channels
    // TFile *myFile = TFile::Open("Measurements_240503/2024-05-03T14-20-24_Cu-248_MCP-new-waan_events.root"); // all channels
    // TFile *myFile = TFile::Open("Measurements_240508/2024-05-08T173028_Cu-248_experiment_Target_2500_Mirror_2600_MCP_2525_UpdatedFW_coincident_events.root"); // all channels
    TFile *myFile = TFile::Open("Measurements_240515/2024-05-15T160706_Cu-248_experiment_Target_2500_Mirror_2600_MCP_2525_additionalFastCard_coincident_events.root"); // all channels
    // TFile *myFile = TFile::Open("Measurements_240619/2024-06-19T124000_Cu-248_experiment_Target_2500_Mirror_2600_MCP_2525_10cards_coincident_events.root"); // all channels
    // TFile *myFile = TFile::Open("2024_06_26T100251_Cu_248_experiment_Target_2500_Mirror_2600_MCP.root"); // all channels

    TTree *ABCD_events = (TTree*)myFile->Get("ABCD_events");
  
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

    const UChar_t channel_MCP_1 = 28;
    const UChar_t channel_X1 = 27;
    const UChar_t channel_X2 = 26;    
    const UChar_t channel_Y1 = 25;
    const UChar_t channel_Y2 = 24;
    const UChar_t channel_SiDetector_ch0 = 22;
    const UChar_t channel_SiDetector_ch1 = 1;
    const UChar_t channel_SiDetector_ch2 = 2;
    const UChar_t channel_SiDetector_ch3 = 3;
    const UChar_t channel_SiDetector_ch4 = 4;
    const UChar_t channel_SiDetector_ch5 = 5;
    const UChar_t channel_SiDetector_ch6 = 6;
    const UChar_t channel_SiDetector_ch7 = 7;
    const UChar_t channel_SiDetector_ch8 = 8;
    const UChar_t channel_SiDetector_ch9 = 9;
    const UChar_t channel_SiDetector_ch10 = 10;
    const UChar_t channel_SiDetector_ch11 = 11;
    const UChar_t channel_SiDetector_ch12 = 12;
    const UChar_t channel_SiDetector_ch13 = 13;
    const UChar_t channel_SiDetector_ch14 = 14;
    const UChar_t channel_SiDetector_ch15 = 23;
    const UChar_t channel_SiDetector_ch16 = 16;
    const UChar_t channel_SiDetector_ch17 = 17;
    const UChar_t channel_SiDetector_ch18 = 18;
    const UChar_t channel_SiDetector_ch19 = 19;
    const UChar_t channel_SiDetector_ch20 = 20;
    const UChar_t channel_SiDetector_ch21 = 21;
    const Double_t ch_to_ns = (25./64.)*(1e-3);
    // const Double_t ch_to_ns = 1.0e9 / 5e9 / (256);
    const Double_t ns_to_mm = 1/(2*0.75); // 1 mm / 0.75 ns
    // const Double_t ns_to_mm = 1/1.24; // 1 mm / 1.24 ns

    // Create a histogram for the values we read.
    TH1F *Si_spectrum_ch0 = new TH1F("Si_spectrum_ch0", "Si0 PH", 300, -1, -1);
    TH1F *ToF_X1X2_ch0 = new TH1F("ToF_X1X2_ch0", "ToF X1 and X2 in ns Si0", 300, -1, -1);
    TH1F *ToF_Y1Y2_ch0 = new TH1F("ToF_Y1Y2_ch0", "ToF Y1 and Y2 in ns Si0", 300, -1, -1);
    TH2F *ToF_ToF_XY_ch0 = new TH2F("ToF_ToF_XY_ch0", "ToF X1 and X2 vs ToF Y1 and Y2 Si0", 40, -20, 20, 40, -20, 20);
    TH2F *PH_vs_TOF_ch0 = new TH2F("PH_vs_TOF_ch0", "PH vs ToF Si0", 200, -1., -1., 200, -1., -1.);
    TH1F *ToF_MCPSi_ch0 = new TH1F("ToF_MCPSi_ch0", "ToF MCP and Si detector", 500, -1, -1);
    TH1F *ToF_X1X2_MCP_ch0 = new TH1F("ToF_X1X2_MCP_ch0", "ToF X1 + X2 _ch0", 300, -1, -1);
    TH1F *ToF_Y1Y2_MCP_ch0 = new TH1F("ToF_Y1Y2_MCP_ch0", "ToF Y1 + Y2 _ch0", 300, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_sum_ch0 = new TH2F("ToF_ToF_X1X2_Y1Y2_sum_ch0", "ToF X1 plus X2 vs ToF Y1 plus Y2 _ch0", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_subs_ch0 = new TH2F("ToF_ToF_X1X2_Y1Y2_subs_ch0", "ToF X1 minus X2 vs ToF Y1 minus Y2 _ch0", 40, -2, 2, 40, -2, 2);
    TH2F *ToF_ToF_X1minusX2_X1plusX2_ch0 = new TH2F("ToF_ToF_X1minusX2_X1plusX2_ch0", "ToF X1 minus X2 vs ToF X1 plus X2 _ch0", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_Y1minusY2_Y1plusY2_ch0 = new TH2F("ToF_ToF_Y1minusY2_Y1plusY2_ch0", "ToF Y1 minus Y2 vs ToF Y1 plus Y2 _ch0", 120, -1, -1, 120, -1, -1);

    
    TH1F *Si_spectrum_ch1 = new TH1F("Si_spectrum_ch1", "Si1 PH", 300, -1, -1);
    TH1F *ToF_X1X2_ch1 = new TH1F("ToF_X1X2_ch1", "ToF X1 and X2 in ns Si1", 300, -100, 100);
    TH1F *ToF_Y1Y2_ch1 = new TH1F("ToF_Y1Y2_ch1", "ToF Y1 and Y2 in ns Si1", 300, -100, 100);
    TH2F *ToF_ToF_XY_ch1 = new TH2F("ToF_ToF_XY_ch1", "ToF X1 and X2 vs ToF Y1 and Y2 Si1", 40, -20, 20, 40, -20, 20);
    TH2F *PH_vs_TOF_ch1 = new TH2F("PH_vs_TOF_ch1", "PH vs ToF Si1", 200, -1., -1., 200, -1., -1.);
    TH1F *ToF_MCPSi_ch1 = new TH1F("ToF_MCPSi_ch1", "ToF MCP and Si detector", 500, -1, -1);
    TH1F *ToF_X1X2_MCP_ch1 = new TH1F("ToF_X1X2_MCP_ch1", "ToF X1 + X2 _ch1", 300, -1, -1);
    TH1F *ToF_Y1Y2_MCP_ch1 = new TH1F("ToF_Y1Y2_MCP_ch1", "ToF Y1 + Y2 _ch1", 300, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_sum_ch1 = new TH2F("ToF_ToF_X1X2_Y1Y2_sum_ch1", "ToF X1 plus X2 vs ToF Y1 plus Y2 _ch1", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_subs_ch1 = new TH2F("ToF_ToF_X1X2_Y1Y2_subs_ch1", "ToF X1 minus X2 vs ToF Y1 minus Y2 _ch1", 40, -2, 2, 40, -2, 2);
    TH2F *ToF_ToF_X1minusX2_X1plusX2_ch1 = new TH2F("ToF_ToF_X1minusX2_X1plusX2_ch1", "ToF X1 minus X2 vs ToF X1 plus X2 _ch1", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_Y1minusY2_Y1plusY2_ch1 = new TH2F("ToF_ToF_Y1minusY2_Y1plusY2_ch1", "ToF Y1 minus Y2 vs ToF Y1 plus Y2 _ch1", 120, -1, -1, 120, -1, -1);


    TH1F *Si_spectrum_ch2 = new TH1F("Si_spectrum_ch2", "Si2 PH", 300, -1, -1);
    TH1F *ToF_X1X2_ch2 = new TH1F("ToF_X1X2_ch2", "ToF X1 and X2 in ns Si2", 300, -100, 100);
    TH1F *ToF_Y1Y2_ch2 = new TH1F("ToF_Y1Y2_ch2", "ToF Y1 and Y2 in ns Si2", 300, -100, 100);
    TH2F *ToF_ToF_XY_ch2 = new TH2F("ToF_ToF_XY_ch2", "ToF X1 and X2 vs ToF Y1 and Y2 Si2", 40, -20, 20, 40, -20, 20);
    TH2F *PH_vs_TOF_ch2 = new TH2F("PH_vs_TOF_ch2", "PH vs ToF Si2", 200, -1., -1., 200, -1., -1.);
    TH1F *ToF_MCPSi_ch2 = new TH1F("ToF_MCPSi_ch2", "ToF MCP and Si detector", 500, -1, -1);
    TH1F *ToF_X1X2_MCP_ch2 = new TH1F("ToF_X1X2_MCP_ch2", "ToF X1 + X2 _ch2", 300, -1, -1);
    TH1F *ToF_Y1Y2_MCP_ch2 = new TH1F("ToF_Y1Y2_MCP_ch2", "ToF Y1 + Y2 _ch2", 300, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_sum_ch2 = new TH2F("ToF_ToF_X1X2_Y1Y2_sum_ch2", "ToF X1 plus X2 vs ToF Y1 plus Y2 _ch2", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_subs_ch2 = new TH2F("ToF_ToF_X1X2_Y1Y2_subs_ch2", "ToF X1 minus X2 vs ToF Y1 minus Y2 _ch2", 40, -2, 2, 40, -2, 2);
    TH2F *ToF_ToF_X1minusX2_X1plusX2_ch2 = new TH2F("ToF_ToF_X1minusX2_X1plusX2_ch2", "ToF X1 minus X2 vs ToF X1 plus X2 _ch2", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_Y1minusY2_Y1plusY2_ch2 = new TH2F("ToF_ToF_Y1minusY2_Y1plusY2_ch2", "ToF Y1 minus Y2 vs ToF Y1 plus Y2 _ch2", 120, -1, -1, 120, -1, -1);


    TH1F *Si_spectrum_ch3 = new TH1F("Si_spectrum_ch3", "Si3 PH", 300, -1, -1);
    TH1F *ToF_X1X2_ch3 = new TH1F("ToF_X1X2_ch3", "ToF X1 and X2 in ns Si3", 300, -100, 100);
    TH1F *ToF_Y1Y2_ch3 = new TH1F("ToF_Y1Y2_ch3", "ToF Y1 and Y2 in ns Si3", 300, -100, 100);
    TH2F *ToF_ToF_XY_ch3 = new TH2F("ToF_ToF_XY_ch3", "ToF X1 and X2 vs ToF Y1 and Y2 Si3", 40, -20, 20, 40, -20, 20);
    TH2F *PH_vs_TOF_ch3 = new TH2F("PH_vs_TOF_ch3", "PH vs ToF Si3", 200, -1., -1., 200, -1., -1.);
    TH1F *ToF_MCPSi_ch3 = new TH1F("ToF_MCPSi_ch3", "ToF MCP and Si detector", 500, -1, -1);
    TH1F *ToF_X1X2_MCP_ch3 = new TH1F("ToF_X1X2_MCP_ch3", "ToF X1 + X2 _ch3", 300, -1, -1);
    TH1F *ToF_Y1Y2_MCP_ch3 = new TH1F("ToF_Y1Y2_MCP_ch3", "ToF Y1 + Y2 _ch3", 300, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_sum_ch3 = new TH2F("ToF_ToF_X1X2_Y1Y2_sum_ch3", "ToF X1 plus X2 vs ToF Y1 plus Y2 _ch3", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_subs_ch3 = new TH2F("ToF_ToF_X1X2_Y1Y2_subs_ch3", "ToF X1 minus X2 vs ToF Y1 minus Y2 _ch3", 40, -2, 2, 40, -2, 2);
    TH2F *ToF_ToF_X1minusX2_X1plusX2_ch3 = new TH2F("ToF_ToF_X1minusX2_X1plusX2_ch3", "ToF X1 minus X2 vs ToF X1 plus X2 _ch3", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_Y1minusY2_Y1plusY2_ch3 = new TH2F("ToF_ToF_Y1minusY2_Y1plusY2_ch3", "ToF Y1 minus Y2 vs ToF Y1 plus Y2 _ch3", 120, -1, -1, 120, -1, -1);


    TH1F *Si_spectrum_ch4 = new TH1F("Si_spectrum_ch4", "Si4 PH", 300, -1, -1);
    TH1F *ToF_X1X2_ch4 = new TH1F("ToF_X1X2_ch4", "ToF X1 and X2 in ns Si4", 300, -100, 100);
    TH1F *ToF_Y1Y2_ch4 = new TH1F("ToF_Y1Y2_ch4", "ToF Y1 and Y2 in ns Si4", 300, -100, 100);
    TH2F *ToF_ToF_XY_ch4 = new TH2F("ToF_ToF_XY_ch4", "ToF X1 and X2 vs ToF Y1 and Y2 Si4", 40, -20, 20, 40, -20, 20);
    TH2F *PH_vs_TOF_ch4 = new TH2F("PH_vs_TOF_ch4", "PH vs ToF Si4", 200, -1., -1., 200, -1., -1.);
    TH1F *ToF_MCPSi_ch4 = new TH1F("ToF_MCPSi_ch4", "ToF MCP and Si detector ch4", 500, -1, -1);
    TH1F *ToF_X1X2_MCP_ch4 = new TH1F("ToF_X1X2_MCP_ch4", "ToF X1 + X2 _ch4", 300, -1, -1);
    TH1F *ToF_Y1Y2_MCP_ch4 = new TH1F("ToF_Y1Y2_MCP_ch4", "ToF Y1 + Y2 _ch4", 300, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_sum_ch4 = new TH2F("ToF_ToF_X1X2_Y1Y2_sum_ch4", "ToF X1 plus X2 vs ToF Y1 plus Y2 _ch4", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_subs_ch4 = new TH2F("ToF_ToF_X1X2_Y1Y2_subs_ch4", "ToF X1 minus X2 vs ToF Y1 minus Y2 _ch4", 40, -2, 2, 40, -2, 2);
    TH2F *ToF_ToF_X1minusX2_X1plusX2_ch4 = new TH2F("ToF_ToF_X1minusX2_X1plusX2_ch4", "ToF X1 minus X2 vs ToF X1 plus X2 _ch4", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_Y1minusY2_Y1plusY2_ch4 = new TH2F("ToF_ToF_Y1minusY2_Y1plusY2_ch4", "ToF Y1 minus Y2 vs ToF Y1 plus Y2 _ch4", 120, -1, -1, 120, -1, -1);

    
    TH1F *Si_spectrum_ch5 = new TH1F("Si_spectrum_ch5", "Si5 PH", 300, -1, -1);
    TH1F *ToF_X1X2_ch5 = new TH1F("ToF_X1X2_ch5", "ToF X1 and X2 in ns Si5", 300, -100, 100);
    TH1F *ToF_Y1Y2_ch5 = new TH1F("ToF_Y1Y2_ch5", "ToF Y1 and Y2 in ns Si5", 300, -100, 100);
    TH2F *ToF_ToF_XY_ch5 = new TH2F("ToF_ToF_XY_ch5", "ToF X1 and X2 vs ToF Y1 and Y2 Si5", 40, -20, 20, 40, -20, 20);
    TH2F *PH_vs_TOF_ch5 = new TH2F("PH_vs_TOF_ch5", "PH vs ToF Si5", 200, -1., -1., 200, -1., -1.);
    TH1F *ToF_MCPSi_ch5 = new TH1F("ToF_MCPSi_ch5", "ToF MCP and Si detector ch5", 500, -1, -1);
    TH1F *ToF_X1X2_MCP_ch5 = new TH1F("ToF_X1X2_MCP_ch5", "ToF X1 + X2 _ch5", 300, -1, -1);
    TH1F *ToF_Y1Y2_MCP_ch5 = new TH1F("ToF_Y1Y2_MCP_ch5", "ToF Y1 + Y2 _ch5", 300, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_sum_ch5 = new TH2F("ToF_ToF_X1X2_Y1Y2_sum_ch5", "ToF X1 plus X2 vs ToF Y1 plus Y2 _ch5", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_subs_ch5 = new TH2F("ToF_ToF_X1X2_Y1Y2_subs_ch5", "ToF X1 minus X2 vs ToF Y1 minus Y2 _ch5", 40, -2, 2, 40, -2, 2);
    TH2F *ToF_ToF_X1minusX2_X1plusX2_ch5 = new TH2F("ToF_ToF_X1minusX2_X1plusX2_ch5", "ToF X1 minus X2 vs ToF X1 plus X2 _ch5", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_Y1minusY2_Y1plusY2_ch5 = new TH2F("ToF_ToF_Y1minusY2_Y1plusY2_ch5", "ToF Y1 minus Y2 vs ToF Y1 plus Y2 _ch5", 120, -1, -1, 120, -1, -1);


    TH1F *Si_spectrum_ch6 = new TH1F("Si_spectrum_ch6", "Si6 PH", 300, -1, -1);
    TH1F *ToF_X1X2_ch6 = new TH1F("ToF_X1X2_ch6", "ToF X1 and X2 in ns Si6", 300, -100, 100);
    TH1F *ToF_Y1Y2_ch6 = new TH1F("ToF_Y1Y2_ch6", "ToF Y1 and Y2 in ns Si6", 300, -100, 100);
    TH2F *ToF_ToF_XY_ch6 = new TH2F("ToF_ToF_XY_ch6", "ToF X1 and X2 vs ToF Y1 and Y2 Si6", 40, -20, 20, 40, -20, 20);
    TH2F *PH_vs_TOF_ch6 = new TH2F("PH_vs_TOF_ch6", "PH vs ToF Si6", 200, -1., -1., 200, -1., -1.);
    TH1F *ToF_MCPSi_ch6 = new TH1F("ToF_MCPSi_ch6", "ToF MCP and Si detector ch6", 500, -1, -1);
    TH1F *ToF_X1X2_MCP_ch6 = new TH1F("ToF_X1X2_MCP_ch6", "ToF X1 + X2 _ch6", 300, -1, -1);
    TH1F *ToF_Y1Y2_MCP_ch6 = new TH1F("ToF_Y1Y2_MCP_ch6", "ToF Y1 + Y2 _ch6", 300, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_sum_ch6 = new TH2F("ToF_ToF_X1X2_Y1Y2_sum_ch6", "ToF X1 plus X2 vs ToF Y1 plus Y2 _ch6", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_subs_ch6 = new TH2F("ToF_ToF_X1X2_Y1Y2_subs_ch6", "ToF X1 minus X2 vs ToF Y1 minus Y2 _ch6", 40, -2, 2, 40, -2, 2);
    TH2F *ToF_ToF_X1minusX2_X1plusX2_ch6 = new TH2F("ToF_ToF_X1minusX2_X1plusX2_ch6", "ToF X1 minus X2 vs ToF X1 plus X2 _ch6", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_Y1minusY2_Y1plusY2_ch6 = new TH2F("ToF_ToF_Y1minusY2_Y1plusY2_ch6", "ToF Y1 minus Y2 vs ToF Y1 plus Y2 _ch6", 120, -1, -1, 120, -1, -1);


    TH1F *Si_spectrum_ch7 = new TH1F("Si_spectrum_ch7", "Si7 PH", 300, -1, -1);
    TH1F *ToF_X1X2_ch7 = new TH1F("ToF_X1X2_ch7", "ToF X1 and X2 in ns Si7", 300, -100, 100);
    TH1F *ToF_Y1Y2_ch7 = new TH1F("ToF_Y1Y2_ch7", "ToF Y1 and Y2 in ns Si7", 300, -100, 100);
    TH2F *ToF_ToF_XY_ch7 = new TH2F("ToF_ToF_XY_ch7", "ToF X1 and X2 vs ToF Y1 and Y2 Si7", 40, -20, 20, 40, -20, 20);
    TH2F *PH_vs_TOF_ch7 = new TH2F("PH_vs_TOF_ch7", "PH vs ToF Si7", 200, -1., -1., 200, -1., -1.);
    TH1F *ToF_MCPSi_ch7 = new TH1F("ToF_MCPSi_ch7", "ToF MCP and Si detector ch7", 500, -1, -1);
    TH1F *ToF_X1X2_MCP_ch7 = new TH1F("ToF_X1X2_MCP_ch7", "ToF X1 + X2 _ch7", 300, -1, -1);
    TH1F *ToF_Y1Y2_MCP_ch7 = new TH1F("ToF_Y1Y2_MCP_ch7", "ToF Y1 + Y2 _ch7", 300, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_sum_ch7 = new TH2F("ToF_ToF_X1X2_Y1Y2_sum_ch7", "ToF X1 plus X2 vs ToF Y1 plus Y2 _ch7", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_subs_ch7 = new TH2F("ToF_ToF_X1X2_Y1Y2_subs_ch7", "ToF X1 minus X2 vs ToF Y1 minus Y2 _ch7", 40, -2, 2, 40, -2, 2);
    TH2F *ToF_ToF_X1minusX2_X1plusX2_ch7 = new TH2F("ToF_ToF_X1minusX2_X1plusX2_ch7", "ToF X1 minus X2 vs ToF X1 plus X2 _ch7", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_Y1minusY2_Y1plusY2_ch7 = new TH2F("ToF_ToF_Y1minusY2_Y1plusY2_ch7", "ToF Y1 minus Y2 vs ToF Y1 plus Y2 _ch7", 120, -1, -1, 120, -1, -1);


    TH1F *Si_spectrum_ch8 = new TH1F("Si_spectrum_ch8", "Si8 PH", 300, -1, -1);
    TH1F *ToF_X1X2_ch8 = new TH1F("ToF_X1X2_ch8", "ToF X1 and X2 in ns Si8", 300, -100, 100);
    TH1F *ToF_Y1Y2_ch8 = new TH1F("ToF_Y1Y2_ch8", "ToF Y1 and Y2 in ns Si8", 300, -100, 100);
    TH2F *ToF_ToF_XY_ch8 = new TH2F("ToF_ToF_XY_ch8", "ToF X1 and X2 vs ToF Y1 and Y2 Si8", 40, -20, 20, 40, -20, 20);
    TH2F *PH_vs_TOF_ch8 = new TH2F("PH_vs_TOF_ch8", "PH vs ToF Si8", 200, -1., -1., 200, -1., -1.);
    TH1F *ToF_MCPSi_ch8 = new TH1F("ToF_MCPSi_ch8", "ToF MCP and Si detector ch8", 500, -1, -1);
    TH1F *ToF_X1X2_MCP_ch8 = new TH1F("ToF_X1X2_MCP_ch8", "ToF X1 + X2 _ch8", 300, -1, -1);
    TH1F *ToF_Y1Y2_MCP_ch8 = new TH1F("ToF_Y1Y2_MCP_ch8", "ToF Y1 + Y2 _ch8", 300, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_sum_ch8 = new TH2F("ToF_ToF_X1X2_Y1Y2_sum_ch8", "ToF X1 plus X2 vs ToF Y1 plus Y2 _ch8", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_subs_ch8 = new TH2F("ToF_ToF_X1X2_Y1Y2_subs_ch8", "ToF X1 minus X2 vs ToF Y1 minus Y2 _ch8", 40, -2, 2, 40, -2, 2);
    TH2F *ToF_ToF_X1minusX2_X1plusX2_ch8 = new TH2F("ToF_ToF_X1minusX2_X1plusX2_ch8", "ToF X1 minus X2 vs ToF X1 plus X2 _ch8", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_Y1minusY2_Y1plusY2_ch8 = new TH2F("ToF_ToF_Y1minusY2_Y1plusY2_ch8", "ToF Y1 minus Y2 vs ToF Y1 plus Y2 _ch8", 120, -1, -1, 120, -1, -1);

    
    TH1F *Si_spectrum_ch9 = new TH1F("Si_spectrum_ch9", "Si9 PH", 300, -1, -1);
    TH1F *ToF_X1X2_ch9 = new TH1F("ToF_X1X2_ch9", "ToF X1 and X2 in ns Si9", 300, -100, 100);
    TH1F *ToF_Y1Y2_ch9 = new TH1F("ToF_Y1Y2_ch9", "ToF Y1 and Y2 in ns Si9", 300, -100, 100);
    TH2F *ToF_ToF_XY_ch9 = new TH2F("ToF_ToF_XY_ch9", "ToF X1 and X2 vs ToF Y1 and Y2 Si9", 40, -20, 20, 40, -20, 20);
    TH2F *PH_vs_TOF_ch9 = new TH2F("PH_vs_TOF_ch9", "PH vs ToF Si9", 200, -1., -1., 200, -1., -1.);
    TH1F *ToF_MCPSi_ch9 = new TH1F("ToF_MCPSi_ch9", "ToF MCP and Si detector ch9", 500, -1, -1);
    TH1F *ToF_X1X2_MCP_ch9 = new TH1F("ToF_X1X2_MCP_ch9", "ToF X1 + X2 _ch9", 300, -1, -1);
    TH1F *ToF_Y1Y2_MCP_ch9 = new TH1F("ToF_Y1Y2_MCP_ch9", "ToF Y1 + Y2 _ch9", 300, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_sum_ch9 = new TH2F("ToF_ToF_X1X2_Y1Y2_sum_ch9", "ToF X1 plus X2 vs ToF Y1 plus Y2 _ch9", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_subs_ch9 = new TH2F("ToF_ToF_X1X2_Y1Y2_subs_ch9", "ToF X1 minus X2 vs ToF Y1 minus Y2 _ch9", 40, -2, 2, 40, -2, 2);
    TH2F *ToF_ToF_X1minusX2_X1plusX2_ch9 = new TH2F("ToF_ToF_X1minusX2_X1plusX2_ch9", "ToF X1 minus X2 vs ToF X1 plus X2 _ch9", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_Y1minusY2_Y1plusY2_ch9 = new TH2F("ToF_ToF_Y1minusY2_Y1plusY2_ch9", "ToF Y1 minus Y2 vs ToF Y1 plus Y2 _ch9", 120, -1, -1, 120, -1, -1);


    TH1F *Si_spectrum_ch10 = new TH1F("Si_spectrum_ch10", "Si10 PH", 300, -1, -1);
    TH1F *ToF_X1X2_ch10 = new TH1F("ToF_X1X2_ch10", "ToF X1 and X2 in ns Si10", 300, -100, 100);
    TH1F *ToF_Y1Y2_ch10 = new TH1F("ToF_Y1Y2_ch10", "ToF Y1 and Y2 in ns Si10", 300, -100, 100);
    TH2F *ToF_ToF_XY_ch10 = new TH2F("ToF_ToF_XY_ch10", "ToF X1 and X2 vs ToF Y1 and Y2 Si10", 40, -20, 20, 40, -20, 20);
    TH2F *PH_vs_TOF_ch10 = new TH2F("PH_vs_TOF_ch10", "PH vs ToF Si10", 200, -1., -1., 200, -1., -1.);
    TH1F *ToF_MCPSi_ch10 = new TH1F("ToF_MCPSi_ch10", "ToF MCP and Si detector ch10", 500, -1, -1);
    TH1F *ToF_X1X2_MCP_ch10 = new TH1F("ToF_X1X2_MCP_ch10", "ToF X1 + X2 _ch10", 300, -1, -1);
    TH1F *ToF_Y1Y2_MCP_ch10 = new TH1F("ToF_Y1Y2_MCP_ch10", "ToF Y1 + Y2 _ch10", 300, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_sum_ch10 = new TH2F("ToF_ToF_X1X2_Y1Y2_sum_ch10", "ToF X1 plus X2 vs ToF Y1 plus Y2 _ch10", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_subs_ch10 = new TH2F("ToF_ToF_X1X2_Y1Y2_subs_ch10", "ToF X1 minus X2 vs ToF Y1 minus Y2 _ch10", 40, -2, 2, 40, -2, 2);
    TH2F *ToF_ToF_X1minusX2_X1plusX2_ch10 = new TH2F("ToF_ToF_X1minusX2_X1plusX2_ch10", "ToF X1 minus X2 vs ToF X1 plus X2 _ch10", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_Y1minusY2_Y1plusY2_ch10 = new TH2F("ToF_ToF_Y1minusY2_Y1plusY2_ch10", "ToF Y1 minus Y2 vs ToF Y1 plus Y2 _ch10", 120, -1, -1, 120, -1, -1);


    TH1F *Si_spectrum_ch11 = new TH1F("Si_spectrum_ch11", "Si11 PH", 300, -1, -1);
    TH1F *ToF_X1X2_ch11 = new TH1F("ToF_X1X2_ch11", "ToF X1 and X2 in ns Si11", 300, -100, 100);
    TH1F *ToF_Y1Y2_ch11 = new TH1F("ToF_Y1Y2_ch11", "ToF Y1 and Y2 in ns Si11", 300, -100, 100);
    TH2F *ToF_ToF_XY_ch11 = new TH2F("ToF_ToF_XY_ch11", "ToF X1 and X2 vs ToF Y1 and Y2 Si11", 40, -20, 20, 40, -20, 20);       
    TH2F *PH_vs_TOF_ch11 = new TH2F("PH_vs_TOF_ch11", "PH vs ToF Si11", 200, -1., -1., 200, -1., -1.);       
    TH1F *ToF_MCPSi_ch11 = new TH1F("ToF_MCPSi_ch11", "ToF MCP and Si detector ch11", 500, -1, -1);
    TH1F *ToF_X1X2_MCP_ch11 = new TH1F("ToF_X1X2_MCP_ch11", "ToF X1 + X2 _ch11", 300, -1, -1);
    TH1F *ToF_Y1Y2_MCP_ch11 = new TH1F("ToF_Y1Y2_MCP_ch11", "ToF Y1 + Y2 _ch11", 300, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_sum_ch11 = new TH2F("ToF_ToF_X1X2_Y1Y2_sum_ch11", "ToF X1 plus X2 vs ToF Y1 plus Y2 _ch11", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_subs_ch11 = new TH2F("ToF_ToF_X1X2_Y1Y2_subs_ch11", "ToF X1 minus X2 vs ToF Y1 minus Y2 _ch11", 40, -2, 2, 40, -2, 2);
    TH2F *ToF_ToF_X1minusX2_X1plusX2_ch11 = new TH2F("ToF_ToF_X1minusX2_X1plusX2_ch11", "ToF X1 minus X2 vs ToF X1 plus X2 _ch11", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_Y1minusY2_Y1plusY2_ch11 = new TH2F("ToF_ToF_Y1minusY2_Y1plusY2_ch11", "ToF Y1 minus Y2 vs ToF Y1 plus Y2 _ch11", 120, -1, -1, 120, -1, -1);


    TH1F *Si_spectrum_ch12 = new TH1F("Si_spectrum_ch12", "Si12 PH", 300, -1, -1);
    TH1F *ToF_X1X2_ch12 = new TH1F("ToF_X1X2_ch12", "ToF X1 and X2 in ns Si12", 300, -100, 100);
    TH1F *ToF_Y1Y2_ch12 = new TH1F("ToF_Y1Y2_ch12", "ToF Y1 and Y2 in ns Si12", 300, -100, 100);
    TH2F *ToF_ToF_XY_ch12 = new TH2F("ToF_ToF_XY_ch12", "ToF X1 and X2 vs ToF Y1 and Y2 Si12", 40, -20, 20, 40, -20, 20);       
    TH2F *PH_vs_TOF_ch12 = new TH2F("PH_vs_TOF_ch12", "PH vs ToF Si12", 200, -1., -1., 200, -1., -1.);       
    TH1F *ToF_MCPSi_ch12 = new TH1F("ToF_MCPSi_ch12", "ToF MCP and Si detector ch12", 500, -1, -1);
    TH1F *ToF_X1X2_MCP_ch12 = new TH1F("ToF_X1X2_MCP_ch12", "ToF X1 + X2 _ch12", 300, -1, -1);
    TH1F *ToF_Y1Y2_MCP_ch12 = new TH1F("ToF_Y1Y2_MCP_ch12", "ToF Y1 + Y2 _ch12", 300, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_sum_ch12 = new TH2F("ToF_ToF_X1X2_Y1Y2_sum_ch12", "ToF X1 plus X2 vs ToF Y1 plus Y2 _ch12", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_subs_ch12 = new TH2F("ToF_ToF_X1X2_Y1Y2_subs_ch12", "ToF X1 minus X2 vs ToF Y1 minus Y2 _ch12", 40, -2, 2, 40, -2, 2);
    TH2F *ToF_ToF_X1minusX2_X1plusX2_ch12 = new TH2F("ToF_ToF_X1minusX2_X1plusX2_ch12", "ToF X1 minus X2 vs ToF X1 plus X2 _ch12", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_Y1minusY2_Y1plusY2_ch12 = new TH2F("ToF_ToF_Y1minusY2_Y1plusY2_ch12", "ToF Y1 minus Y2 vs ToF Y1 plus Y2 _ch12", 120, -1, -1, 120, -1, -1);


    TH1F *Si_spectrum_ch13 = new TH1F("Si_spectrum_ch13", "Si13 PH", 300, -1, -1);
    TH1F *ToF_X1X2_ch13 = new TH1F("ToF_X1X2_ch13", "ToF X1 and X2 in ns Si13", 300, -100, 100);
    TH1F *ToF_Y1Y2_ch13 = new TH1F("ToF_Y1Y2_ch13", "ToF Y1 and Y2 in ns Si13", 300, -100, 100);
    TH2F *ToF_ToF_XY_ch13 = new TH2F("ToF_ToF_XY_ch13", "ToF X1 and X2 vs ToF Y1 and Y2 Si13", 40, -20, 20, 40, -20, 20);       
    TH2F *PH_vs_TOF_ch13 = new TH2F("PH_vs_TOF_ch13", "PH vs ToF Si13", 200, -1., -1., 200, -1., -1.);       
    TH1F *ToF_MCPSi_ch13 = new TH1F("ToF_MCPSi_ch13", "ToF MCP and Si detector ch13", 500, -1, -1);
    TH1F *ToF_X1X2_MCP_ch13 = new TH1F("ToF_X1X2_MCP_ch13", "ToF X1 + X2 _ch13", 300, -1, -1);
    TH1F *ToF_Y1Y2_MCP_ch13 = new TH1F("ToF_Y1Y2_MCP_ch13", "ToF Y1 + Y2 _ch13", 300, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_sum_ch13 = new TH2F("ToF_ToF_X1X2_Y1Y2_sum_ch13", "ToF X1 plus X2 vs ToF Y1 plus Y2 _ch13", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_subs_ch13 = new TH2F("ToF_ToF_X1X2_Y1Y2_subs_ch13", "ToF X1 minus X2 vs ToF Y1 minus Y2 _ch13", 40, -2, 2, 40, -2, 2);
    TH2F *ToF_ToF_X1minusX2_X1plusX2_ch13 = new TH2F("ToF_ToF_X1minusX2_X1plusX2_ch13", "ToF X1 minus X2 vs ToF X1 plus X2 _ch13", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_Y1minusY2_Y1plusY2_ch13 = new TH2F("ToF_ToF_Y1minusY2_Y1plusY2_ch13", "ToF Y1 minus Y2 vs ToF Y1 plus Y2 _ch13", 120, -1, -1, 120, -1, -1);


    TH1F *Si_spectrum_ch14 = new TH1F("Si_spectrum_ch14", "Si14 PH", 300, -1, -1);
    TH1F *ToF_X1X2_ch14 = new TH1F("ToF_X1X2_ch14", "ToF X1 and X2 in ns Si14", 300, -100, 100);
    TH1F *ToF_Y1Y2_ch14 = new TH1F("ToF_Y1Y2_ch14", "ToF Y1 and Y2 in ns Si14", 300, -100, 100);
    TH2F *ToF_ToF_XY_ch14 = new TH2F("ToF_ToF_XY_ch14", "ToF X1 and X2 vs ToF Y1 and Y2 Si14", 40, -20, 20, 40, -20, 20);       
    TH2F *PH_vs_TOF_ch14 = new TH2F("PH_vs_TOF_ch14", "PH vs ToF Si14", 200, -1., -1., 200, -1., -1.);       
    TH1F *ToF_MCPSi_ch14 = new TH1F("ToF_MCPSi_ch14", "ToF MCP and Si detector ch14", 500, -1, -1);
    TH1F *ToF_X1X2_MCP_ch14 = new TH1F("ToF_X1X2_MCP_ch14", "ToF X1 + X2 _ch14", 300, -1, -1);
    TH1F *ToF_Y1Y2_MCP_ch14 = new TH1F("ToF_Y1Y2_MCP_ch14", "ToF Y1 + Y2 _ch14", 300, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_sum_ch14 = new TH2F("ToF_ToF_X1X2_Y1Y2_sum_ch14", "ToF X1 plus X2 vs ToF Y1 plus Y2 _ch14", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_subs_ch14 = new TH2F("ToF_ToF_X1X2_Y1Y2_subs_ch14", "ToF X1 minus X2 vs ToF Y1 minus Y2 _ch14", 40, -2, 2, 40, -2, 2);
    TH2F *ToF_ToF_X1minusX2_X1plusX2_ch14 = new TH2F("ToF_ToF_X1minusX2_X1plusX2_ch14", "ToF X1 minus X2 vs ToF X1 plus X2 _ch14", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_Y1minusY2_Y1plusY2_ch14 = new TH2F("ToF_ToF_Y1minusY2_Y1plusY2_ch14", "ToF Y1 minus Y2 vs ToF Y1 plus Y2 _ch14", 120, -1, -1, 120, -1, -1);


    TH1F *Si_spectrum_ch15 = new TH1F("Si_spectrum_ch15", "Si15 PH", 300, -1, -1);
    TH1F *ToF_X1X2_ch15 = new TH1F("ToF_X1X2_ch15", "ToF X1 and X2 in ns Si15", 300, -100, 100);
    TH1F *ToF_Y1Y2_ch15 = new TH1F("ToF_Y1Y2_ch15", "ToF Y1 and Y2 in ns Si15", 300, -100, 100);
    TH2F *ToF_ToF_XY_ch15 = new TH2F("ToF_ToF_XY_ch15", "ToF X1 and X2 vs ToF Y1 and Y2 Si15", 40, -20, 20, 40, -20, 20);       
    TH2F *PH_vs_TOF_ch15 = new TH2F("PH_vs_TOF_ch15", "PH vs ToF Si15", 200, -1., -1., 200, -1., -1.);       
    TH1F *ToF_MCPSi_ch15 = new TH1F("ToF_MCPSi_ch15", "ToF MCP and Si detector ch15", 500, -1, -1);
    TH1F *ToF_X1X2_MCP_ch15 = new TH1F("ToF_X1X2_MCP_ch15", "ToF X1 + X2 _ch15", 300, -1, -1);
    TH1F *ToF_Y1Y2_MCP_ch15 = new TH1F("ToF_Y1Y2_MCP_ch15", "ToF Y1 + Y2 _ch15", 300, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_sum_ch15 = new TH2F("ToF_ToF_X1X2_Y1Y2_sum_ch15", "ToF X1 plus X2 vs ToF Y1 plus Y2 _ch15", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_subs_ch15 = new TH2F("ToF_ToF_X1X2_Y1Y2_subs_ch15", "ToF X1 minus X2 vs ToF Y1 minus Y2 _ch15", 40, -2, 2, 40, -2, 2);
    TH2F *ToF_ToF_X1minusX2_X1plusX2_ch15 = new TH2F("ToF_ToF_X1minusX2_X1plusX2_ch15", "ToF X1 minus X2 vs ToF X1 plus X2 _ch15", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_Y1minusY2_Y1plusY2_ch15 = new TH2F("ToF_ToF_Y1minusY2_Y1plusY2_ch15", "ToF Y1 minus Y2 vs ToF Y1 plus Y2 _ch15", 120, -1, -1, 120, -1, -1);


    TH1F *Si_spectrum_ch16 = new TH1F("Si_spectrum_ch16", "Si16 PH", 300, -1, -1);
    TH1F *ToF_X1X2_ch16 = new TH1F("ToF_X1X2_ch16", "ToF X1 and X2 in ns Si16", 300, -100, 100);
    TH1F *ToF_Y1Y2_ch16 = new TH1F("ToF_Y1Y2_ch16", "ToF Y1 and Y2 in ns Si16", 300, -100, 100);
    TH2F *ToF_ToF_XY_ch16 = new TH2F("ToF_ToF_XY_ch16", "ToF X1 and X2 vs ToF Y1 and Y2 Si16", 40, -20, 20, 40, -20, 20);       
    TH2F *PH_vs_TOF_ch16 = new TH2F("PH_vs_TOF_ch16", "PH vs ToF Si16", 200, -1., -1., 200, -1., -1.);       
    TH1F *ToF_MCPSi_ch16 = new TH1F("ToF_MCPSi_ch16", "ToF MCP and Si detector ch16", 500, -1, -1);
    TH1F *ToF_X1X2_MCP_ch16 = new TH1F("ToF_X1X2_MCP_ch16", "ToF X1 + X2 _ch16", 300, -1, -1);
    TH1F *ToF_Y1Y2_MCP_ch16 = new TH1F("ToF_Y1Y2_MCP_ch16", "ToF Y1 + Y2 _ch16", 300, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_sum_ch16 = new TH2F("ToF_ToF_X1X2_Y1Y2_sum_ch16", "ToF X1 plus X2 vs ToF Y1 plus Y2 _ch16", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_subs_ch16 = new TH2F("ToF_ToF_X1X2_Y1Y2_subs_ch16", "ToF X1 minus X2 vs ToF Y1 minus Y2 _ch16", 40, -2, 2, 40, -2, 2);
    TH2F *ToF_ToF_X1minusX2_X1plusX2_ch16 = new TH2F("ToF_ToF_X1minusX2_X1plusX2_ch16", "ToF X1 minus X2 vs ToF X1 plus X2 _ch16", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_Y1minusY2_Y1plusY2_ch16 = new TH2F("ToF_ToF_Y1minusY2_Y1plusY2_ch16", "ToF Y1 minus Y2 vs ToF Y1 plus Y2 _ch16", 120, -1, -1, 120, -1, -1);


    TH1F *Si_spectrum_ch17 = new TH1F("Si_spectrum_ch17", "Si17 PH", 300, -1, -1);
    TH1F *ToF_X1X2_ch17 = new TH1F("ToF_X1X2_ch17", "ToF X1 and X2 in ns Si17", 300, -100, 100);
    TH1F *ToF_Y1Y2_ch17 = new TH1F("ToF_Y1Y2_ch17", "ToF Y1 and Y2 in ns Si17", 300, -100, 100);
    TH2F *ToF_ToF_XY_ch17 = new TH2F("ToF_ToF_XY_ch17", "ToF X1 and X2 vs ToF Y1 and Y2 Si17", 40, -20, 20, 40, -20, 20);       
    TH2F *PH_vs_TOF_ch17 = new TH2F("PH_vs_TOF_ch17", "PH vs ToF Si17", 200, -1., -1., 200, -1., -1.);       
    TH1F *ToF_MCPSi_ch17 = new TH1F("ToF_MCPSi_ch17", "ToF MCP and Si detector ch17", 500, -1, -1);
    TH1F *ToF_X1X2_MCP_ch17 = new TH1F("ToF_X1X2_MCP_ch17", "ToF X1 + X2 _ch17", 300, -1, -1);
    TH1F *ToF_Y1Y2_MCP_ch17 = new TH1F("ToF_Y1Y2_MCP_ch17", "ToF Y1 + Y2 _ch17", 300, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_sum_ch17 = new TH2F("ToF_ToF_X1X2_Y1Y2_sum_ch17", "ToF X1 plus X2 vs ToF Y1 plus Y2 _ch17", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_subs_ch17 = new TH2F("ToF_ToF_X1X2_Y1Y2_subs_ch17", "ToF X1 minus X2 vs ToF Y1 minus Y2 _ch17", 40, -2, 2, 40, -2, 2);
    TH2F *ToF_ToF_X1minusX2_X1plusX2_ch17 = new TH2F("ToF_ToF_X1minusX2_X1plusX2_ch17", "ToF X1 minus X2 vs ToF X1 plus X2 _ch17", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_Y1minusY2_Y1plusY2_ch17 = new TH2F("ToF_ToF_Y1minusY2_Y1plusY2_ch17", "ToF Y1 minus Y2 vs ToF Y1 plus Y2 _ch17", 120, -1, -1, 120, -1, -1);


    TH1F *Si_spectrum_ch18 = new TH1F("Si_spectrum_ch18", "Si18 PH", 300, -1, -1);
    TH1F *ToF_X1X2_ch18 = new TH1F("ToF_X1X2_ch18", "ToF X1 and X2 in ns Si18", 300, -100, 100);
    TH1F *ToF_Y1Y2_ch18 = new TH1F("ToF_Y1Y2_ch18", "ToF Y1 and Y2 in ns Si18", 300, -100, 100);
    TH2F *ToF_ToF_XY_ch18 = new TH2F("ToF_ToF_XY_ch18", "ToF X1 and X2 vs ToF Y1 and Y2 Si18", 40, -20, 20, 40, -20, 20);       
    TH2F *PH_vs_TOF_ch18 = new TH2F("PH_vs_TOF_ch18", "PH vs ToF Si18", 200, -1., -1., 200, -1., -1.);       
    TH1F *ToF_MCPSi_ch18 = new TH1F("ToF_MCPSi_ch18", "ToF MCP and Si detector ch18", 500, -1, -1);
    TH1F *ToF_X1X2_MCP_ch18 = new TH1F("ToF_X1X2_MCP_ch18", "ToF X1 + X2 _ch18", 300, -1, -1);
    TH1F *ToF_Y1Y2_MCP_ch18 = new TH1F("ToF_Y1Y2_MCP_ch18", "ToF Y1 + Y2 _ch18", 300, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_sum_ch18 = new TH2F("ToF_ToF_X1X2_Y1Y2_sum_ch18", "ToF X1 plus X2 vs ToF Y1 plus Y2 _ch18", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_subs_ch18 = new TH2F("ToF_ToF_X1X2_Y1Y2_subs_ch18", "ToF X1 minus X2 vs ToF Y1 minus Y2 _ch18", 40, -2, 2, 40, -2, 2);
    TH2F *ToF_ToF_X1minusX2_X1plusX2_ch18 = new TH2F("ToF_ToF_X1minusX2_X1plusX2_ch18", "ToF X1 minus X2 vs ToF X1 plus X2 _ch18", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_Y1minusY2_Y1plusY2_ch18 = new TH2F("ToF_ToF_Y1minusY2_Y1plusY2_ch18", "ToF Y1 minus Y2 vs ToF Y1 plus Y2 _ch18", 120, -1, -1, 120, -1, -1);


    TH1F *Si_spectrum_ch19 = new TH1F("Si_spectrum_ch19", "Si19 PH", 300, -1, -1);
    TH1F *ToF_X1X2_ch19 = new TH1F("ToF_X1X2_ch19", "ToF X1 and X2 in ns Si19", 300, -100, 100);
    TH1F *ToF_Y1Y2_ch19 = new TH1F("ToF_Y1Y2_ch19", "ToF Y1 and Y2 in ns Si19", 300, -100, 100);
    TH2F *ToF_ToF_XY_ch19 = new TH2F("ToF_ToF_XY_ch19", "ToF X1 and X2 vs ToF Y1 and Y2 Si19", 40, -20, 20, 40, -20, 20);            
    TH2F *PH_vs_TOF_ch19 = new TH2F("PH_vs_TOF_ch19", "PH vs ToF Si19", 200, -1., -1., 200, -1., -1.);            
    TH1F *ToF_MCPSi_ch19 = new TH1F("ToF_MCPSi_ch19", "ToF MCP and Si detector ch19", 500, -1, -1);
    TH1F *ToF_X1X2_MCP_ch19 = new TH1F("ToF_X1X2_MCP_ch19", "ToF X1 + X2 _ch19", 300, -1, -1);
    TH1F *ToF_Y1Y2_MCP_ch19 = new TH1F("ToF_Y1Y2_MCP_ch19", "ToF Y1 + Y2 _ch19", 300, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_sum_ch19 = new TH2F("ToF_ToF_X1X2_Y1Y2_sum_ch19", "ToF X1 plus X2 vs ToF Y1 plus Y2 _ch19", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_subs_ch19 = new TH2F("ToF_ToF_X1X2_Y1Y2_subs_ch19", "ToF X1 minus X2 vs ToF Y1 minus Y2 _ch19", 40, -2, 2, 40, -2, 2);
    TH2F *ToF_ToF_X1minusX2_X1plusX2_ch19 = new TH2F("ToF_ToF_X1minusX2_X1plusX2_ch19", "ToF X1 minus X2 vs ToF X1 plus X2 _ch19", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_Y1minusY2_Y1plusY2_ch19 = new TH2F("ToF_ToF_Y1minusY2_Y1plusY2_ch19", "ToF Y1 minus Y2 vs ToF Y1 plus Y2 _ch19", 120, -1, -1, 120, -1, -1);


    TH1F *Si_spectrum_ch20 = new TH1F("Si_spectrum_ch20", "Si20 PH", 300, -1, -1);
    TH1F *ToF_X1X2_ch20 = new TH1F("ToF_X1X2_ch20", "ToF X1 and X2 in ns Si20", 300, -100, 100);
    TH1F *ToF_Y1Y2_ch20 = new TH1F("ToF_Y1Y2_ch20", "ToF Y1 and Y2 in ns Si20", 300, -100, 100);
    TH2F *ToF_ToF_XY_ch20 = new TH2F("ToF_ToF_XY_ch20", "ToF X1 and X2 vs ToF Y1 and Y2 Si20", 40, -20, 20, 40, -20, 20);            
    TH2F *PH_vs_TOF_ch20 = new TH2F("PH_vs_TOF_ch20", "PH vs ToF Si20", 1500, -1., -1., 1500, -1., -1.);            
    TH1F *ToF_MCPSi_ch20 = new TH1F("ToF_MCPSi_ch20", "ToF MCP and Si detector ch20", 500, -1, -1);
    TH1F *ToF_X1X2_MCP_ch20 = new TH1F("ToF_X1X2_MCP_ch20", "ToF X1 + X2 _ch20", 300, -1, -1);
    TH1F *ToF_Y1Y2_MCP_ch20 = new TH1F("ToF_Y1Y2_MCP_ch20", "ToF Y1 + Y2 _ch20", 300, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_sum_ch20 = new TH2F("ToF_ToF_X1X2_Y1Y2_sum_ch20", "ToF X1 plus X2 vs ToF Y1 plus Y2 _ch20", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_subs_ch20 = new TH2F("ToF_ToF_X1X2_Y1Y2_subs_ch20", "ToF X1 minus X2 vs ToF Y1 minus Y2 _ch20", 40, -2, 2, 40, -2, 2);
    TH2F *ToF_ToF_X1minusX2_X1plusX2_ch20 = new TH2F("ToF_ToF_X1minusX2_X1plusX2_ch20", "ToF X1 minus X2 vs ToF X1 plus X2 _ch20", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_Y1minusY2_Y1plusY2_ch20 = new TH2F("ToF_ToF_Y1minusY2_Y1plusY2_ch20", "ToF Y1 minus Y2 vs ToF Y1 plus Y2 _ch20", 120, -1, -1, 120, -1, -1);


    TH1F *Si_spectrum_ch21 = new TH1F("Si_spectrum_ch21", "Si21 PH", 300, -1, -1);
    TH1F *ToF_X1X2_ch21 = new TH1F("ToF_X1X2_ch21", "ToF X1 and X2 in ns Si21", 300, -100, 100);
    TH1F *ToF_Y1Y2_ch21 = new TH1F("ToF_Y1Y2_ch21", "ToF Y1 and Y2 in ns Si21", 300, -100, 100);
    TH2F *ToF_ToF_XY_ch21 = new TH2F("ToF_ToF_XY_ch21", "ToF X1 and X2 vs ToF Y1 and Y2 Si21", 40, -20, 20, 40, -20, 20);            
    TH2F *ToF_ToF_XY_ch21_5mm = new TH2F("ToF_ToF_XY_ch21_5mm", "ToF X1 and X2 vs ToF Y1 and Y2 Si21", 40, -20, 20, 40, -20, 20);            
    TH2F *ToF_ToF_XY_ch21_1cm_ring = new TH2F("ToF_ToF_XY_ch21_1cm_ring", "ToF X1 and X2 vs ToF Y1 and Y2 Si21", 40, -20, 20, 40, -20, 20);            
    TH2F *PH_vs_TOF_ch21 = new TH2F("PH_vs_TOF_ch21", "PH vs ToF Si21", 1500, -1., -1., 1500, -1., -1.);            
    TH1F *ToF_MCPSi_ch21 = new TH1F("ToF_MCPSi_ch21", "ToF MCP and Si detector ch21", 500, -1, -1);
    TH1F *ToF_MCPSi_ch21_5_mm_r = new TH1F("ToF_MCPSi_ch21_5_mm_r", "ToF MCP and Si detector ch21 5 mm MCP", 500, -1, -1);
    TH1F *ToF_MCPSi_ch21_1_cm_ring = new TH1F("ToF_MCPSi_ch21_1_cm_ring", "ToF MCP and Si detector ch21 1 cm MCP ring", 500, -1, -1);
    TH1F *ToF_X1X2_MCP_ch21 = new TH1F("ToF_X1X2_MCP_ch21", "ToF X1 + X2 _ch21", 300, -1, -1);
    TH1F *ToF_Y1Y2_MCP_ch21 = new TH1F("ToF_Y1Y2_MCP_ch21", "ToF Y1 + Y2 _ch21", 300, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_sum_ch21 = new TH2F("ToF_ToF_X1X2_Y1Y2_sum_ch21", "ToF X1 plus X2 vs ToF Y1 plus Y2 _ch21", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_subs_ch21 = new TH2F("ToF_ToF_X1X2_Y1Y2_subs_ch21", "ToF X1 minus X2 vs ToF Y1 minus Y2 _ch21", 40, -2, 2, 40, -2, 2);
    TH2F *ToF_ToF_X1minusX2_X1plusX2_ch21 = new TH2F("ToF_ToF_X1minusX2_X1plusX2_ch21", "ToF X1 minus X2 vs ToF X1 plus X2 _ch21", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_Y1minusY2_Y1plusY2_ch21 = new TH2F("ToF_ToF_Y1minusY2_Y1plusY2_ch21", "ToF Y1 minus Y2 vs ToF Y1 plus Y2 _ch21", 120, -1, -1, 120, -1, -1);

    
    TH1F *ToF_X1X2_MCP = new TH1F("ToF_X1X2_MCP", "ToF X1 + X2", 300, -1, -1);
    TH1F *ToF_Y1Y2_MCP = new TH1F("ToF_Y1Y2_MCP", "ToF Y1 + Y2", 300, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_sum = new TH2F("ToF_ToF_X1X2_Y1Y2_sum", "ToF X1 plus X2 vs ToF Y1 plus Y2", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_X1X2_Y1Y2_subs = new TH2F("ToF_ToF_X1X2_Y1Y2_subs", "ToF X1 minus X2 vs ToF Y1 minus Y2", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_X1minusX2_X1plusX2 = new TH2F("ToF_ToF_X1minusX2_X1plusX2", "ToF X1 minus X2 vs ToF X1 plus X2", 120, -1, -1, 120, -1, -1);
    TH2F *ToF_ToF_Y1minusY2_Y1plusY2 = new TH2F("ToF_ToF_Y1minusY2_Y1plusY2", "ToF Y1 minus Y2 vs ToF Y1 plus Y2", 120, -1, -1, 120, -1, -1);

    
    TH1F *ToF_MCPX1 = new TH1F("ToF_MCPX1", "ToF MCP and X1", 100, -1, -1);
    TH1F *ToF_X1Si = new TH1F("ToF_X1PSi", "ToF X1 and Si detector", 200, -1, -1);
    
    // Loop over all entries of the TTree
    for (size_t i = 0; i < ABCD_events->GetEntries(); i++) {
        ABCD_events->GetEntry(i);

        // cout << "Entry: " << i << endl;

        Bool_t found_MCP_1 = false;
        Bool_t found_X1 = false;
        Bool_t found_X2 = false;        
        Bool_t found_Y1 = false;
        Bool_t found_Y2 = false;
        Bool_t found_SiDetector_ch0 = false;
        Bool_t found_SiDetector_ch1 = false;
        Bool_t found_SiDetector_ch2 = false;
        Bool_t found_SiDetector_ch3 = false;
        Bool_t found_SiDetector_ch4 = false;
        Bool_t found_SiDetector_ch5 = false;
        Bool_t found_SiDetector_ch6 = false;
        Bool_t found_SiDetector_ch7 = false;
        Bool_t found_SiDetector_ch8 = false;
        Bool_t found_SiDetector_ch9 = false;
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
        Bool_t found_SiDetector_ch20 = false;
        Bool_t found_SiDetector_ch21 = false;


        Double_t PH_MCP_1 = 0;
        Double_t PH_X1 = 0;
        Double_t PH_X2 = 0;        
        Double_t PH_Y1 = 0;
        Double_t PH_Y2 = 0;
        Double_t PH_SiDetector_ch0 = 0;
        Double_t PH_SiDetector_ch1 = 0;
        Double_t PH_SiDetector_ch2 = 0;
        Double_t PH_SiDetector_ch3 = 0;
        Double_t PH_SiDetector_ch4 = 0;
        Double_t PH_SiDetector_ch5 = 0;
        Double_t PH_SiDetector_ch6 = 0;
        Double_t PH_SiDetector_ch7 = 0;
        Double_t PH_SiDetector_ch8 = 0;
        Double_t PH_SiDetector_ch9 = 0;
        Double_t PH_SiDetector_ch10 = 0;
        Double_t PH_SiDetector_ch11 = 0;
        Double_t PH_SiDetector_ch12 = 0;
        Double_t PH_SiDetector_ch13 = 0;
        Double_t PH_SiDetector_ch14 = 0;
        Double_t PH_SiDetector_ch15 = 0;
        Double_t PH_SiDetector_ch16 = 0;
        Double_t PH_SiDetector_ch17 = 0;
        Double_t PH_SiDetector_ch18 = 0;
        Double_t PH_SiDetector_ch19 = 0;
        Double_t PH_SiDetector_ch20 = 0;
        Double_t PH_SiDetector_ch21 = 0;


        Double_t timestamp_MCP_1 = 0;
        Double_t timestamp_X1 = 0;
        Double_t timestamp_X2 = 0;        
        Double_t timestamp_Y1 = 0;
        Double_t timestamp_Y2 = 0;
        Double_t timestamp_SiDetector_ch0 = 0;
        Double_t timestamp_SiDetector_ch1 = 0;
        Double_t timestamp_SiDetector_ch2 = 0;
        Double_t timestamp_SiDetector_ch3 = 0;
        Double_t timestamp_SiDetector_ch4 = 0;
        Double_t timestamp_SiDetector_ch5 = 0;
        Double_t timestamp_SiDetector_ch6 = 0;
        Double_t timestamp_SiDetector_ch7 = 0;
        Double_t timestamp_SiDetector_ch8 = 0;
        Double_t timestamp_SiDetector_ch9 = 0;
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
        Double_t timestamp_SiDetector_ch20 = 0;
        Double_t timestamp_SiDetector_ch21 = 0;


        for (size_t inner = 0; inner < coincidence_group_counter; inner++) {
            //cout << "inner: " << inner << " Timestamp: " << (unsigned long long)timestamps[inner] << " ch" << endl;

            if (channels[inner] == channel_MCP_1) {
                found_MCP_1 = true;
                timestamp_MCP_1 = timestamps[inner];
                PH_MCP_1 = qlongs[inner];
                //cout << "inner: " << inner << endl;
                //cout << "Found MCP! Timestamp: " << (unsigned long long)timestamp_MCP_1 << " ch" << endl;
            } else if (channels[inner] == channel_X1) {
                found_X1 = true;
                timestamp_X1 = timestamps[inner];
                PH_X1 = qlongs[inner];
                //cout << "inner: " << inner << endl;
                //cout << "Found X1! Timestamp: " << (unsigned long long)timestamp_X1 << " ch" << endl;
            } else if (channels[inner] == channel_X2) {
                found_X2 = true;
                timestamp_X2 = timestamps[inner];
                PH_X2 = qlongs[inner];
                //cout << "inner: " << inner << endl;
                //cout << "Found X2! Timestamp: " << (unsigned long long)timestamp_X2 << " ch" << endl;
            } else if (channels[inner] == channel_Y1) {
                found_Y1 = true;
                timestamp_Y1 = timestamps[inner];
                PH_Y1 = qlongs[inner];
                //cout << "inner: " << inner << endl;
                //cout << "Found Y1! Timestamp: " << (unsigned long long)timestamp_Y1 << " ch" << endl;
            } else if (channels[inner] == channel_Y2) {
                found_Y2 = true;
                timestamp_Y2 = timestamps[inner];
                PH_Y2 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found Y2! Timestamp: " << (unsigned long long)timestamp_Y2 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch0) {
                found_SiDetector_ch0 = true;
                timestamp_SiDetector_ch0 = timestamps[inner];
                PH_SiDetector_ch0 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch0 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch1) {
                found_SiDetector_ch1 = true;
                timestamp_SiDetector_ch1 = timestamps[inner];
                PH_SiDetector_ch1 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch1 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch2) {
                found_SiDetector_ch2 = true;
                timestamp_SiDetector_ch2 = timestamps[inner];
                PH_SiDetector_ch2 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch2 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch3) {
                found_SiDetector_ch3 = true;
                timestamp_SiDetector_ch3 = timestamps[inner];
                PH_SiDetector_ch3 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch3 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch4) {
                found_SiDetector_ch4 = true;
                timestamp_SiDetector_ch4 = timestamps[inner];
                PH_SiDetector_ch4 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch4 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch5) {
                found_SiDetector_ch5 = true;
                timestamp_SiDetector_ch5 = timestamps[inner];
                PH_SiDetector_ch5 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch5 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch6) {
                found_SiDetector_ch6 = true;
                timestamp_SiDetector_ch6 = timestamps[inner];
                PH_SiDetector_ch6 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch6 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch7) {
                found_SiDetector_ch7 = true;
                timestamp_SiDetector_ch7 = timestamps[inner];
                PH_SiDetector_ch7 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch7 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch8) {
                found_SiDetector_ch8 = true;
                timestamp_SiDetector_ch8 = timestamps[inner];
                PH_SiDetector_ch8 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch8 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch9) {
                found_SiDetector_ch9 = true;
                timestamp_SiDetector_ch9 = timestamps[inner];
                PH_SiDetector_ch9 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch9 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch10) {
                found_SiDetector_ch10 = true;
                timestamp_SiDetector_ch10 = timestamps[inner];
                PH_SiDetector_ch10 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch10 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch11) {
                found_SiDetector_ch11 = true;
                timestamp_SiDetector_ch11 = timestamps[inner];
                PH_SiDetector_ch11 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch11 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch12) {
                found_SiDetector_ch12 = true;
                timestamp_SiDetector_ch12 = timestamps[inner];
                PH_SiDetector_ch12 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch12 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch13) {
                found_SiDetector_ch13 = true;
                timestamp_SiDetector_ch13 = timestamps[inner];
                PH_SiDetector_ch13 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch13 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch14) {
                found_SiDetector_ch14 = true;
                timestamp_SiDetector_ch14 = timestamps[inner];
                PH_SiDetector_ch14 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch14 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch15) {
                found_SiDetector_ch15 = true;
                timestamp_SiDetector_ch15 = timestamps[inner];
                PH_SiDetector_ch15 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch15 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch16) {
                found_SiDetector_ch16 = true;
                timestamp_SiDetector_ch16 = timestamps[inner];
                PH_SiDetector_ch16 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch16 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch17) {
                found_SiDetector_ch17 = true;
                timestamp_SiDetector_ch17 = timestamps[inner];
                PH_SiDetector_ch17 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch17 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch18) {
                found_SiDetector_ch18 = true;
                timestamp_SiDetector_ch18 = timestamps[inner];
                PH_SiDetector_ch18 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch18 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch19) {
                found_SiDetector_ch19 = true;
                timestamp_SiDetector_ch19 = timestamps[inner];
                PH_SiDetector_ch19 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch19 << " ch" << endl;
            } else if (channels[inner] == channel_SiDetector_ch20) {
                found_SiDetector_ch20 = true;
                timestamp_SiDetector_ch20 = timestamps[inner];
                PH_SiDetector_ch20 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch20 << " ch" << endl;
            }else if (channels[inner] == channel_SiDetector_ch21) {
                found_SiDetector_ch21 = true;
                timestamp_SiDetector_ch21 = timestamps[inner];
                PH_SiDetector_ch21 = qlongs[inner];
                // cout << "inner: " << inner << endl;
                // cout << "Found SiDetector! Timestamp: " << (unsigned long long)timestamp_SiDetector_ch21 << " ch" << endl;
            }

        }

        if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch0 && found_MCP_1 ) ) {
            
            const Double_t offset_X = 41.5170;
            const Double_t offset_Y = 41.5170;

            // Since the x2 and y2 signals were mounred on one card and x1 y1 on the inmmediate next card on the chain there is a
            // constant offset of 41.51 ns betwween the two cards, this offset needs to be substracted from x2 and y2

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            // const Long64_t time_difference_int_X = (timestamp_X1 - (timestamp_X2 - 41.51));
            // const Double_t time_difference_X = time_difference_int_X * ns_to_mm;
            const Double_t time_difference_X = (time_difference_int_X) * ch_to_ns * ns_to_mm;

            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            const Long64_t time_difference_int_SiDetector_ch0 = (timestamp_SiDetector_ch0 - timestamp_MCP_1);
            const Double_t time_difference_SiDetector_ch0 = (time_difference_int_SiDetector_ch0) * ch_to_ns;

            // const Long64_t time_difference_int_Y = (timestamp_Y1 - (timestamp_Y2 - 41.51));
            // const Double_t time_difference_Y = time_difference_int_Y * ns_to_mm;
            const Double_t time_difference_Y = (time_difference_int_Y) * ch_to_ns * ns_to_mm;

            //cout << "Found both! Time difference: " << (long long)time_difference_int << " ch; " << time_difference << " ns" << endl;

            ToF_X1X2_ch0->Fill(time_difference_X);
            ToF_Y1Y2_ch0->Fill(time_difference_Y);
            ToF_MCPSi_ch0->Fill( time_difference_SiDetector_ch0 );

            const Double_t pos_x = (time_difference_X);
            const Double_t pos_y = (time_difference_Y);

            ToF_ToF_XY_ch0->Fill(  pos_x,  pos_y );
            Si_spectrum_ch0->Fill(  PH_SiDetector_ch0 );
            PH_vs_TOF_ch0->Fill( time_difference_SiDetector_ch0, PH_SiDetector_ch0 );

            const Long64_t time_sum_int_X = ( ( timestamp_X1 - timestamp_MCP_1) + ( timestamp_X2 - timestamp_MCP_1));
            const Long64_t time_diff_int_X = ( ( timestamp_X1 ) - ( timestamp_X2 ));
            
            const Double_t time_sum_X = time_sum_int_X*ch_to_ns;
            const Double_t time_diff_X = time_diff_int_X*ch_to_ns;

            const Long64_t time_sum_int_Y = ( ( timestamp_Y1 - timestamp_MCP_1) + ( timestamp_Y2 - timestamp_MCP_1));
            const Long64_t time_diff_int_Y = ( ( timestamp_Y1 ) - ( timestamp_Y2 ));

            const Double_t time_sum_Y = time_sum_int_Y*ch_to_ns;
            const Double_t time_diff_Y = time_diff_int_Y*ch_to_ns;
            
            const Double_t pos_diff_x = (time_diff_X)*ns_to_mm;
            const Double_t pos_diff_y = (time_diff_Y)*ns_to_mm;

            if ((pos_diff_x * pos_diff_x + pos_diff_y * pos_diff_y) < (1.128 * 1.128))
            {
                ToF_X1X2_MCP_ch0->Fill(time_sum_X);
                ToF_Y1Y2_MCP_ch0->Fill(time_sum_Y);

                const Double_t pos_sum_x = (time_sum_X)*ns_to_mm;
                const Double_t pos_sum_y = (time_sum_Y)*ns_to_mm;

                ToF_ToF_X1X2_Y1Y2_sum_ch0->Fill(  pos_sum_x,  pos_sum_y );
                ToF_ToF_X1X2_Y1Y2_subs_ch0->Fill(  pos_diff_x,  pos_diff_y );
                ToF_ToF_X1minusX2_X1plusX2_ch0->Fill(  pos_diff_x, pos_sum_x );
                ToF_ToF_Y1minusY2_Y1plusY2_ch0->Fill(  pos_diff_y, pos_sum_y );
            }

        }

        if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch1 && found_MCP_1 ) ) {
            
            const Double_t offset_X = 41.5170;
            const Double_t offset_Y = 41.5170;

            // Since the x1 and y1 signals were mounred on one card and x2 y2 on the inmmediate next card on the chain there is a
            // constant offset of 41.51 ns betwween the two cards, this offset needs to be substracted from x2 and y2

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            // const Long64_t time_difference_int_X = (timestamp_X1 - (timestamp_X2 - 41.51));
            // const Double_t time_difference_X = time_difference_int_X * ns_to_mm;
            const Double_t time_difference_X = (time_difference_int_X) * ch_to_ns * ns_to_mm;

            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            const Long64_t time_difference_int_SiDetector_ch1 = (timestamp_SiDetector_ch1 - timestamp_MCP_1);
            const Double_t time_difference_SiDetector_ch1 = (time_difference_int_SiDetector_ch1) * ch_to_ns;

            // const Long64_t time_difference_int_Y = (timestamp_Y1 - (timestamp_Y2 - 41.51));
            // const Double_t time_difference_Y = time_difference_int_Y * ns_to_mm;
            const Double_t time_difference_Y = (time_difference_int_Y) * ch_to_ns * ns_to_mm;

            //cout << "Found both! Time difference: " << (long long)time_difference_int << " ch; " << time_difference << " ns" << endl;

            ToF_X1X2_ch1->Fill(time_difference_X);
            ToF_Y1Y2_ch1->Fill(time_difference_Y);
            ToF_MCPSi_ch1->Fill( time_difference_SiDetector_ch1 );

            const Double_t pos_x = (time_difference_X);
            const Double_t pos_y = (time_difference_Y);

            ToF_ToF_XY_ch1->Fill(  pos_x,  pos_y );
            Si_spectrum_ch1->Fill(  PH_SiDetector_ch1 );
            PH_vs_TOF_ch1->Fill( time_difference_SiDetector_ch1, PH_SiDetector_ch1 );

            const Long64_t time_sum_int_X = ( ( timestamp_X1 - timestamp_MCP_1) + ( timestamp_X2 - timestamp_MCP_1));
            const Long64_t time_diff_int_X = ( ( timestamp_X1 ) - ( timestamp_X2 ));
            
            const Double_t time_sum_X = time_sum_int_X*ch_to_ns;
            const Double_t time_diff_X = time_diff_int_X*ch_to_ns;

            const Long64_t time_sum_int_Y = ( ( timestamp_Y1 - timestamp_MCP_1) + ( timestamp_Y2 - timestamp_MCP_1));
            const Long64_t time_diff_int_Y = ( ( timestamp_Y1 ) - ( timestamp_Y2 ));

            const Double_t time_sum_Y = time_sum_int_Y*ch_to_ns;
            const Double_t time_diff_Y = time_diff_int_Y*ch_to_ns;
            
            const Double_t pos_diff_x = (time_diff_X)*ns_to_mm;
            const Double_t pos_diff_y = (time_diff_Y)*ns_to_mm;

            if ((pos_diff_x * pos_diff_x + pos_diff_y * pos_diff_y) < (1.128 * 1.128))
            {
                ToF_X1X2_MCP_ch1->Fill(time_sum_X);
                ToF_Y1Y2_MCP_ch1->Fill(time_sum_Y);

                const Double_t pos_sum_x = (time_sum_X)*ns_to_mm;
                const Double_t pos_sum_y = (time_sum_Y)*ns_to_mm;

                ToF_ToF_X1X2_Y1Y2_sum_ch1->Fill(  pos_sum_x,  pos_sum_y );
                ToF_ToF_X1X2_Y1Y2_subs_ch1->Fill(  pos_diff_x,  pos_diff_y );
                ToF_ToF_X1minusX2_X1plusX2_ch1->Fill(  pos_diff_x, pos_sum_x );
                ToF_ToF_Y1minusY2_Y1plusY2_ch1->Fill(  pos_diff_y, pos_sum_y );
            }

        }
        

        if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch2 && found_MCP_1 ) ) {
            
            const Double_t offset_X = 41.5170;
            const Double_t offset_Y = 41.5170;

            // Since the x1 and y1 signals were mounred on one card and x2 y2 on the inmmediate next card on the chain there is a
            // constant offset of 41.51 ns betwween the two cards, this offset needs to be substracted from x2 and y2

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            // const Long64_t time_difference_int_X = (timestamp_X1 - (timestamp_X2 - 41.51));
            // const Double_t time_difference_X = time_difference_int_X * ns_to_mm;
            const Double_t time_difference_X = (time_difference_int_X) * ch_to_ns * ns_to_mm;

            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            const Long64_t time_difference_int_SiDetector_ch2 = (timestamp_SiDetector_ch2 - timestamp_MCP_1);
            const Double_t time_difference_SiDetector_ch2 = (time_difference_int_SiDetector_ch2) * ch_to_ns;

            // const Long64_t time_difference_int_Y = (timestamp_Y1 - (timestamp_Y2 - 41.51));
            // const Double_t time_difference_Y = time_difference_int_Y * ns_to_mm;
            const Double_t time_difference_Y = (time_difference_int_Y) * ch_to_ns * ns_to_mm;

            //cout << "Found both! Time difference: " << (long long)time_difference_int << " ch; " << time_difference << " ns" << endl;

            ToF_X1X2_ch2->Fill(time_difference_X);
            ToF_Y1Y2_ch2->Fill(time_difference_Y);
            ToF_MCPSi_ch2->Fill( time_difference_SiDetector_ch2 );

            const Double_t pos_x = (time_difference_X);
            const Double_t pos_y = (time_difference_Y);

            ToF_ToF_XY_ch2->Fill(  pos_x,  pos_y );
            Si_spectrum_ch2->Fill(  PH_SiDetector_ch2 );
            PH_vs_TOF_ch2->Fill( time_difference_SiDetector_ch2, PH_SiDetector_ch2 );

            const Long64_t time_sum_int_X = ( ( timestamp_X1 - timestamp_MCP_1) + ( timestamp_X2 - timestamp_MCP_1));
            const Long64_t time_diff_int_X = ( ( timestamp_X1 ) - ( timestamp_X2 ));
            
            const Double_t time_sum_X = time_sum_int_X*ch_to_ns;
            const Double_t time_diff_X = time_diff_int_X*ch_to_ns;

            const Long64_t time_sum_int_Y = ( ( timestamp_Y1 - timestamp_MCP_1) + ( timestamp_Y2 - timestamp_MCP_1));
            const Long64_t time_diff_int_Y = ( ( timestamp_Y1 ) - ( timestamp_Y2 ));

            const Double_t time_sum_Y = time_sum_int_Y*ch_to_ns;
            const Double_t time_diff_Y = time_diff_int_Y*ch_to_ns;
            
            const Double_t pos_diff_x = (time_diff_X)*ns_to_mm;
            const Double_t pos_diff_y = (time_diff_Y)*ns_to_mm;

            if ((pos_diff_x * pos_diff_x + pos_diff_y * pos_diff_y) < (1.128 * 1.128))
            {
                ToF_X1X2_MCP_ch2->Fill(time_sum_X);
                ToF_Y1Y2_MCP_ch2->Fill(time_sum_Y);

                const Double_t pos_sum_x = (time_sum_X)*ns_to_mm;
                const Double_t pos_sum_y = (time_sum_Y)*ns_to_mm;

                ToF_ToF_X1X2_Y1Y2_sum_ch2->Fill(  pos_sum_x,  pos_sum_y );
                ToF_ToF_X1X2_Y1Y2_subs_ch2->Fill(  pos_diff_x,  pos_diff_y );
                ToF_ToF_X1minusX2_X1plusX2_ch2->Fill(  pos_diff_x, pos_sum_x );
                ToF_ToF_Y1minusY2_Y1plusY2_ch2->Fill(  pos_diff_y, pos_sum_y );
            }

        }

        if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch3 && found_MCP_1 ) ) {
            
            const Double_t offset_X = 41.5170;
            const Double_t offset_Y = 41.5170;

            // Since the x1 and y1 signals were mounred on one card and x2 y2 on the inmmediate next card on the chain there is a
            // constant offset of 41.51 ns betwween the two cards, this offset needs to be substracted from x2 and y2

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            // const Long64_t time_difference_int_X = (timestamp_X1 - (timestamp_X2 - 41.51));
            // const Double_t time_difference_X = time_difference_int_X * ns_to_mm;
            const Double_t time_difference_X = (time_difference_int_X) * ch_to_ns * ns_to_mm;

            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            const Long64_t time_difference_int_SiDetector_ch3 = (timestamp_SiDetector_ch3 - timestamp_MCP_1);
            const Double_t time_difference_SiDetector_ch3 = (time_difference_int_SiDetector_ch3) * ch_to_ns;

            // const Long64_t time_difference_int_Y = (timestamp_Y1 - (timestamp_Y2 - 41.51));
            // const Double_t time_difference_Y = time_difference_int_Y * ns_to_mm;
            const Double_t time_difference_Y = (time_difference_int_Y) * ch_to_ns * ns_to_mm;

            //cout << "Found both! Time difference: " << (long long)time_difference_int << " ch; " << time_difference << " ns" << endl;

            ToF_X1X2_ch3->Fill(time_difference_X);
            ToF_Y1Y2_ch3->Fill(time_difference_Y);
            ToF_MCPSi_ch3->Fill( time_difference_SiDetector_ch3 );

            const Double_t pos_x = (time_difference_X);
            const Double_t pos_y = (time_difference_Y);

            ToF_ToF_XY_ch3->Fill(  pos_x,  pos_y );
            Si_spectrum_ch3->Fill(  PH_SiDetector_ch3 );
            PH_vs_TOF_ch3->Fill( time_difference_SiDetector_ch3, PH_SiDetector_ch3 );

            const Long64_t time_sum_int_X = ( ( timestamp_X1 - timestamp_MCP_1) + ( timestamp_X2 - timestamp_MCP_1));
            const Long64_t time_diff_int_X = ( ( timestamp_X1 ) - ( timestamp_X2 ));
            
            const Double_t time_sum_X = time_sum_int_X*ch_to_ns;
            const Double_t time_diff_X = time_diff_int_X*ch_to_ns;

            const Long64_t time_sum_int_Y = ( ( timestamp_Y1 - timestamp_MCP_1) + ( timestamp_Y2 - timestamp_MCP_1));
            const Long64_t time_diff_int_Y = ( ( timestamp_Y1 ) - ( timestamp_Y2 ));

            const Double_t time_sum_Y = time_sum_int_Y*ch_to_ns;
            const Double_t time_diff_Y = time_diff_int_Y*ch_to_ns;
            
            const Double_t pos_diff_x = (time_diff_X)*ns_to_mm;
            const Double_t pos_diff_y = (time_diff_Y)*ns_to_mm;

            if ((pos_diff_x * pos_diff_x + pos_diff_y * pos_diff_y) < (1.128 * 1.128))
            {
                ToF_X1X2_MCP_ch3->Fill(time_sum_X);
                ToF_Y1Y2_MCP_ch3->Fill(time_sum_Y);

                const Double_t pos_sum_x = (time_sum_X)*ns_to_mm;
                const Double_t pos_sum_y = (time_sum_Y)*ns_to_mm;

                ToF_ToF_X1X2_Y1Y2_sum_ch3->Fill(  pos_sum_x,  pos_sum_y );
                ToF_ToF_X1X2_Y1Y2_subs_ch3->Fill(  pos_diff_x,  pos_diff_y );
                ToF_ToF_X1minusX2_X1plusX2_ch3->Fill(  pos_diff_x, pos_sum_x );
                ToF_ToF_Y1minusY2_Y1plusY2_ch3->Fill(  pos_diff_y, pos_sum_y );
            }

        }

        if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch4 && found_MCP_1 ) ) {
            
            const Double_t offset_X = 41.5170;
            const Double_t offset_Y = 41.5170;

            // Since the x1 and y1 signals were mounred on one card and x2 y2 on the inmmediate next card on the chain there is a
            // constant offset of 41.51 ns betwween the two cards, this offset needs to be substracted from x2 and y2

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            // const Long64_t time_difference_int_X = (timestamp_X1 - (timestamp_X2 - 41.51));
            // const Double_t time_difference_X = time_difference_int_X * ns_to_mm;
            const Double_t time_difference_X = (time_difference_int_X) * ch_to_ns * ns_to_mm;

            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            const Long64_t time_difference_int_SiDetector_ch4 = (timestamp_SiDetector_ch4 - timestamp_MCP_1);
            const Double_t time_difference_SiDetector_ch4 = (time_difference_int_SiDetector_ch4) * ch_to_ns;

            // const Long64_t time_difference_int_Y = (timestamp_Y1 - (timestamp_Y2 - 41.51));
            // const Double_t time_difference_Y = time_difference_int_Y * ns_to_mm;
            const Double_t time_difference_Y = (time_difference_int_Y) * ch_to_ns * ns_to_mm;

            //cout << "Found both! Time difference: " << (long long)time_difference_int << " ch; " << time_difference << " ns" << endl;

            ToF_X1X2_ch4->Fill(time_difference_X);
            ToF_Y1Y2_ch4->Fill(time_difference_Y);
            ToF_MCPSi_ch4->Fill( time_difference_SiDetector_ch4 );

            const Double_t pos_x = (time_difference_X);
            const Double_t pos_y = (time_difference_Y);

            ToF_ToF_XY_ch4->Fill(  pos_x,  pos_y );
            Si_spectrum_ch4->Fill(  PH_SiDetector_ch4 );
            PH_vs_TOF_ch4->Fill( time_difference_SiDetector_ch4, PH_SiDetector_ch4 );

            const Long64_t time_sum_int_X = ( ( timestamp_X1 - timestamp_MCP_1) + ( timestamp_X2 - timestamp_MCP_1));
            const Long64_t time_diff_int_X = ( ( timestamp_X1 ) - ( timestamp_X2 ));
            
            const Double_t time_sum_X = time_sum_int_X*ch_to_ns;
            const Double_t time_diff_X = time_diff_int_X*ch_to_ns;

            const Long64_t time_sum_int_Y = ( ( timestamp_Y1 - timestamp_MCP_1) + ( timestamp_Y2 - timestamp_MCP_1));
            const Long64_t time_diff_int_Y = ( ( timestamp_Y1 ) - ( timestamp_Y2 ));

            const Double_t time_sum_Y = time_sum_int_Y*ch_to_ns;
            const Double_t time_diff_Y = time_diff_int_Y*ch_to_ns;
            
            const Double_t pos_diff_x = (time_diff_X)*ns_to_mm;
            const Double_t pos_diff_y = (time_diff_Y)*ns_to_mm;

            if ((pos_diff_x * pos_diff_x + pos_diff_y * pos_diff_y) < (1.128 * 1.128))
            {
                ToF_X1X2_MCP_ch4->Fill(time_sum_X);
                ToF_Y1Y2_MCP_ch4->Fill(time_sum_Y);

                const Double_t pos_sum_x = (time_sum_X)*ns_to_mm;
                const Double_t pos_sum_y = (time_sum_Y)*ns_to_mm;

                ToF_ToF_X1X2_Y1Y2_sum_ch4->Fill(  pos_sum_x,  pos_sum_y );
                ToF_ToF_X1X2_Y1Y2_subs_ch4->Fill(  pos_diff_x,  pos_diff_y );
                ToF_ToF_X1minusX2_X1plusX2_ch4->Fill(  pos_diff_x, pos_sum_x );
                ToF_ToF_Y1minusY2_Y1plusY2_ch4->Fill(  pos_diff_y, pos_sum_y );
            }

        }

        if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch5 && found_MCP_1 ) ) {
            
            const Double_t offset_X = 41.5170;
            const Double_t offset_Y = 41.5170;

            // Since the x1 and y1 signals were mounred on one card and x2 y2 on the inmmediate next card on the chain there is a
            // constant offset of 41.51 ns betwween the two cards, this offset needs to be substracted from x2 and y2

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            // const Long64_t time_difference_int_X = (timestamp_X1 - (timestamp_X2 - 41.51));
            // const Double_t time_difference_X = time_difference_int_X * ns_to_mm;
            const Double_t time_difference_X = (time_difference_int_X) * ch_to_ns * ns_to_mm;

            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            const Long64_t time_difference_int_SiDetector_ch5 = (timestamp_SiDetector_ch5 - timestamp_MCP_1);
            const Double_t time_difference_SiDetector_ch5 = (time_difference_int_SiDetector_ch5) * ch_to_ns;

            // const Long64_t time_difference_int_Y = (timestamp_Y1 - (timestamp_Y2 - 41.51));
            // const Double_t time_difference_Y = time_difference_int_Y * ns_to_mm;
            const Double_t time_difference_Y = (time_difference_int_Y) * ch_to_ns * ns_to_mm;

            //cout << "Found both! Time difference: " << (long long)time_difference_int << " ch; " << time_difference << " ns" << endl;

            ToF_X1X2_ch5->Fill(time_difference_X);
            ToF_Y1Y2_ch5->Fill(time_difference_Y);
            ToF_MCPSi_ch5->Fill( time_difference_SiDetector_ch5 );

            const Double_t pos_x = (time_difference_X);
            const Double_t pos_y = (time_difference_Y);

            ToF_ToF_XY_ch5->Fill(  pos_x,  pos_y );
            Si_spectrum_ch5->Fill(  PH_SiDetector_ch5 );
            PH_vs_TOF_ch5->Fill( time_difference_SiDetector_ch5, PH_SiDetector_ch5 );

            const Long64_t time_sum_int_X = ( ( timestamp_X1 - timestamp_MCP_1) + ( timestamp_X2 - timestamp_MCP_1));
            const Long64_t time_diff_int_X = ( ( timestamp_X1 ) - ( timestamp_X2 ));
            
            const Double_t time_sum_X = time_sum_int_X*ch_to_ns;
            const Double_t time_diff_X = time_diff_int_X*ch_to_ns;

            const Long64_t time_sum_int_Y = ( ( timestamp_Y1 - timestamp_MCP_1) + ( timestamp_Y2 - timestamp_MCP_1));
            const Long64_t time_diff_int_Y = ( ( timestamp_Y1 ) - ( timestamp_Y2 ));

            const Double_t time_sum_Y = time_sum_int_Y*ch_to_ns;
            const Double_t time_diff_Y = time_diff_int_Y*ch_to_ns;
            
            const Double_t pos_diff_x = (time_diff_X)*ns_to_mm;
            const Double_t pos_diff_y = (time_diff_Y)*ns_to_mm;

            if ((pos_diff_x * pos_diff_x + pos_diff_y * pos_diff_y) < (1.128 * 1.128))
            {
                ToF_X1X2_MCP_ch5->Fill(time_sum_X);
                ToF_Y1Y2_MCP_ch5->Fill(time_sum_Y);

                const Double_t pos_sum_x = (time_sum_X)*ns_to_mm;
                const Double_t pos_sum_y = (time_sum_Y)*ns_to_mm;

                ToF_ToF_X1X2_Y1Y2_sum_ch5->Fill(  pos_sum_x,  pos_sum_y );
                ToF_ToF_X1X2_Y1Y2_subs_ch5->Fill(  pos_diff_x,  pos_diff_y );
                ToF_ToF_X1minusX2_X1plusX2_ch5->Fill(  pos_diff_x, pos_sum_x );
                ToF_ToF_Y1minusY2_Y1plusY2_ch5->Fill(  pos_diff_y, pos_sum_y );
            }

        }


        if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch6 && found_MCP_1 ) ) {
            
            const Double_t offset_X = 41.5170;
            const Double_t offset_Y = 41.5170;

            // Since the x1 and y1 signals were mounred on one card and x2 y2 on the inmmediate next card on the chain there is a
            // constant offset of 41.51 ns betwween the two cards, this offset needs to be substracted from x2 and y2

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            // const Long64_t time_difference_int_X = (timestamp_X1 - (timestamp_X2 - 41.51));
            // const Double_t time_difference_X = time_difference_int_X * ns_to_mm;
            const Double_t time_difference_X = (time_difference_int_X) * ch_to_ns * ns_to_mm;

            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            const Long64_t time_difference_int_SiDetector_ch6 = (timestamp_SiDetector_ch6 - timestamp_MCP_1);
            const Double_t time_difference_SiDetector_ch6 = (time_difference_int_SiDetector_ch6) * ch_to_ns;

            // const Long64_t time_difference_int_Y = (timestamp_Y1 - (timestamp_Y2 - 41.51));
            // const Double_t time_difference_Y = time_difference_int_Y * ns_to_mm;
            const Double_t time_difference_Y = (time_difference_int_Y) * ch_to_ns * ns_to_mm;

            //cout << "Found both! Time difference: " << (long long)time_difference_int << " ch; " << time_difference << " ns" << endl;

            ToF_X1X2_ch6->Fill(time_difference_X);
            ToF_Y1Y2_ch6->Fill(time_difference_Y);
            ToF_MCPSi_ch6->Fill( time_difference_SiDetector_ch6 );

            const Double_t pos_x = (time_difference_X);
            const Double_t pos_y = (time_difference_Y);

            ToF_ToF_XY_ch6->Fill(  pos_x,  pos_y );
            Si_spectrum_ch6->Fill(  PH_SiDetector_ch6 );
            PH_vs_TOF_ch6->Fill( time_difference_SiDetector_ch6, PH_SiDetector_ch6 );

            const Long64_t time_sum_int_X = ( ( timestamp_X1 - timestamp_MCP_1) + ( timestamp_X2 - timestamp_MCP_1));
            const Long64_t time_diff_int_X = ( ( timestamp_X1 ) - ( timestamp_X2 ));
            
            const Double_t time_sum_X = time_sum_int_X*ch_to_ns;
            const Double_t time_diff_X = time_diff_int_X*ch_to_ns;

            const Long64_t time_sum_int_Y = ( ( timestamp_Y1 - timestamp_MCP_1) + ( timestamp_Y2 - timestamp_MCP_1));
            const Long64_t time_diff_int_Y = ( ( timestamp_Y1 ) - ( timestamp_Y2 ));

            const Double_t time_sum_Y = time_sum_int_Y*ch_to_ns;
            const Double_t time_diff_Y = time_diff_int_Y*ch_to_ns;
            
            const Double_t pos_diff_x = (time_diff_X)*ns_to_mm;
            const Double_t pos_diff_y = (time_diff_Y)*ns_to_mm;

            if ((pos_diff_x * pos_diff_x + pos_diff_y * pos_diff_y) < (1.128 * 1.128))
            {
                ToF_X1X2_MCP_ch6->Fill(time_sum_X);
                ToF_Y1Y2_MCP_ch6->Fill(time_sum_Y);

                const Double_t pos_sum_x = (time_sum_X)*ns_to_mm;
                const Double_t pos_sum_y = (time_sum_Y)*ns_to_mm;

                ToF_ToF_X1X2_Y1Y2_sum_ch6->Fill(  pos_sum_x,  pos_sum_y );
                ToF_ToF_X1X2_Y1Y2_subs_ch6->Fill(  pos_diff_x,  pos_diff_y );
                ToF_ToF_X1minusX2_X1plusX2_ch6->Fill(  pos_diff_x, pos_sum_x );
                ToF_ToF_Y1minusY2_Y1plusY2_ch6->Fill(  pos_diff_y, pos_sum_y );
            }

        }


        if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch7 && found_MCP_1 ) ) {
            
            const Double_t offset_X = 41.5170;
            const Double_t offset_Y = 41.5170;

            // Since the x1 and y1 signals were mounred on one card and x2 y2 on the inmmediate next card on the chain there is a
            // constant offset of 41.51 ns betwween the two cards, this offset needs to be substracted from x2 and y2

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            // const Long64_t time_difference_int_X = (timestamp_X1 - (timestamp_X2 - 41.51));
            // const Double_t time_difference_X = time_difference_int_X * ns_to_mm;
            const Double_t time_difference_X = (time_difference_int_X) * ch_to_ns * ns_to_mm;

            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            const Long64_t time_difference_int_SiDetector_ch7 = (timestamp_SiDetector_ch7 - timestamp_MCP_1);
            const Double_t time_difference_SiDetector_ch7 = (time_difference_int_SiDetector_ch7) * ch_to_ns;

            // const Long64_t time_difference_int_Y = (timestamp_Y1 - (timestamp_Y2 - 41.51));
            // const Double_t time_difference_Y = time_difference_int_Y * ns_to_mm;
            const Double_t time_difference_Y = (time_difference_int_Y) * ch_to_ns * ns_to_mm;

            //cout << "Found both! Time difference: " << (long long)time_difference_int << " ch; " << time_difference << " ns" << endl;

            ToF_X1X2_ch7->Fill(time_difference_X);
            ToF_Y1Y2_ch7->Fill(time_difference_Y);
            ToF_MCPSi_ch7->Fill( time_difference_SiDetector_ch7 );

            const Double_t pos_x = (time_difference_X);
            const Double_t pos_y = (time_difference_Y);

            ToF_ToF_XY_ch7->Fill(  pos_x,  pos_y );
            Si_spectrum_ch7->Fill(  PH_SiDetector_ch7 );
            PH_vs_TOF_ch7->Fill( time_difference_SiDetector_ch7, PH_SiDetector_ch7 );

            const Long64_t time_sum_int_X = ( ( timestamp_X1 - timestamp_MCP_1) + ( timestamp_X2 - timestamp_MCP_1));
            const Long64_t time_diff_int_X = ( ( timestamp_X1 ) - ( timestamp_X2 ));
            
            const Double_t time_sum_X = time_sum_int_X*ch_to_ns;
            const Double_t time_diff_X = time_diff_int_X*ch_to_ns;

            const Long64_t time_sum_int_Y = ( ( timestamp_Y1 - timestamp_MCP_1) + ( timestamp_Y2 - timestamp_MCP_1));
            const Long64_t time_diff_int_Y = ( ( timestamp_Y1 ) - ( timestamp_Y2 ));

            const Double_t time_sum_Y = time_sum_int_Y*ch_to_ns;
            const Double_t time_diff_Y = time_diff_int_Y*ch_to_ns;
            
            const Double_t pos_diff_x = (time_diff_X)*ns_to_mm;
            const Double_t pos_diff_y = (time_diff_Y)*ns_to_mm;

            if ((pos_diff_x * pos_diff_x + pos_diff_y * pos_diff_y) < (1.128 * 1.128))
            {
                ToF_X1X2_MCP_ch7->Fill(time_sum_X);
                ToF_Y1Y2_MCP_ch7->Fill(time_sum_Y);

                const Double_t pos_sum_x = (time_sum_X)*ns_to_mm;
                const Double_t pos_sum_y = (time_sum_Y)*ns_to_mm;

                ToF_ToF_X1X2_Y1Y2_sum_ch7->Fill(  pos_sum_x,  pos_sum_y );
                ToF_ToF_X1X2_Y1Y2_subs_ch7->Fill(  pos_diff_x,  pos_diff_y );
                ToF_ToF_X1minusX2_X1plusX2_ch7->Fill(  pos_diff_x, pos_sum_x );
                ToF_ToF_Y1minusY2_Y1plusY2_ch7->Fill(  pos_diff_y, pos_sum_y );
            }

        }


        if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch8 && found_MCP_1 ) ) {
            
            const Double_t offset_X = 41.5170;
            const Double_t offset_Y = 41.5170;

            // Since the x1 and y1 signals were mounred on one card and x2 y2 on the inmmediate next card on the chain there is a
            // constant offset of 41.51 ns betwween the two cards, this offset needs to be substracted from x2 and y2

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            // const Long64_t time_difference_int_X = (timestamp_X1 - (timestamp_X2 - 41.51));
            // const Double_t time_difference_X = time_difference_int_X * ns_to_mm;
            const Double_t time_difference_X = (time_difference_int_X) * ch_to_ns * ns_to_mm;

            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            const Long64_t time_difference_int_SiDetector_ch8 = (timestamp_SiDetector_ch8 - timestamp_MCP_1);
            const Double_t time_difference_SiDetector_ch8 = (time_difference_int_SiDetector_ch8) * ch_to_ns;

            // const Long64_t time_difference_int_Y = (timestamp_Y1 - (timestamp_Y2 - 41.51));
            // const Double_t time_difference_Y = time_difference_int_Y * ns_to_mm;
            const Double_t time_difference_Y = (time_difference_int_Y) * ch_to_ns * ns_to_mm;

            //cout << "Found both! Time difference: " << (long long)time_difference_int << " ch; " << time_difference << " ns" << endl;

            ToF_X1X2_ch8->Fill(time_difference_X);
            ToF_Y1Y2_ch8->Fill(time_difference_Y);
            ToF_MCPSi_ch8->Fill( time_difference_SiDetector_ch8 );

            const Double_t pos_x = (time_difference_X);
            const Double_t pos_y = (time_difference_Y);

            ToF_ToF_XY_ch8->Fill(  pos_x,  pos_y );
            Si_spectrum_ch8->Fill(  PH_SiDetector_ch8 );
            PH_vs_TOF_ch8->Fill( time_difference_SiDetector_ch8, PH_SiDetector_ch8 );

            const Long64_t time_sum_int_X = ( ( timestamp_X1 - timestamp_MCP_1) + ( timestamp_X2 - timestamp_MCP_1));
            const Long64_t time_diff_int_X = ( ( timestamp_X1 ) - ( timestamp_X2 ));
            
            const Double_t time_sum_X = time_sum_int_X*ch_to_ns;
            const Double_t time_diff_X = time_diff_int_X*ch_to_ns;

            const Long64_t time_sum_int_Y = ( ( timestamp_Y1 - timestamp_MCP_1) + ( timestamp_Y2 - timestamp_MCP_1));
            const Long64_t time_diff_int_Y = ( ( timestamp_Y1 ) - ( timestamp_Y2 ));

            const Double_t time_sum_Y = time_sum_int_Y*ch_to_ns;
            const Double_t time_diff_Y = time_diff_int_Y*ch_to_ns;
            
            const Double_t pos_diff_x = (time_diff_X)*ns_to_mm;
            const Double_t pos_diff_y = (time_diff_Y)*ns_to_mm;

            if ((pos_diff_x * pos_diff_x + pos_diff_y * pos_diff_y) < (1.128 * 1.128))
            {
                ToF_X1X2_MCP_ch8->Fill(time_sum_X);
                ToF_Y1Y2_MCP_ch8->Fill(time_sum_Y);

                const Double_t pos_sum_x = (time_sum_X)*ns_to_mm;
                const Double_t pos_sum_y = (time_sum_Y)*ns_to_mm;

                ToF_ToF_X1X2_Y1Y2_sum_ch8->Fill(  pos_sum_x,  pos_sum_y );
                ToF_ToF_X1X2_Y1Y2_subs_ch8->Fill(  pos_diff_x,  pos_diff_y );
                ToF_ToF_X1minusX2_X1plusX2_ch8->Fill(  pos_diff_x, pos_sum_x );
                ToF_ToF_Y1minusY2_Y1plusY2_ch8->Fill(  pos_diff_y, pos_sum_y );
            }

        }


        if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch9 && found_MCP_1 ) ) {
            
            const Double_t offset_X = 41.5170;
            const Double_t offset_Y = 41.5170;

            // Since the x1 and y1 signals were mounred on one card and x2 y2 on the inmmediate next card on the chain there is a
            // constant offset of 41.51 ns betwween the two cards, this offset needs to be substracted from x2 and y2

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            // const Long64_t time_difference_int_X = (timestamp_X1 - (timestamp_X2 - 41.51));
            // const Double_t time_difference_X = time_difference_int_X * ns_to_mm;
            const Double_t time_difference_X = (time_difference_int_X) * ch_to_ns * ns_to_mm;

            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            const Long64_t time_difference_int_SiDetector_ch9 = (timestamp_SiDetector_ch9 - timestamp_MCP_1);
            const Double_t time_difference_SiDetector_ch9 = (time_difference_int_SiDetector_ch9) * ch_to_ns;

            // const Long64_t time_difference_int_Y = (timestamp_Y1 - (timestamp_Y2 - 41.51));
            // const Double_t time_difference_Y = time_difference_int_Y * ns_to_mm;
            const Double_t time_difference_Y = (time_difference_int_Y) * ch_to_ns * ns_to_mm;

            //cout << "Found both! Time difference: " << (long long)time_difference_int << " ch; " << time_difference << " ns" << endl;

            ToF_X1X2_ch9->Fill(time_difference_X);
            ToF_Y1Y2_ch9->Fill(time_difference_Y);
            ToF_MCPSi_ch9->Fill( time_difference_SiDetector_ch9 );

            const Double_t pos_x = (time_difference_X);
            const Double_t pos_y = (time_difference_Y);

            ToF_ToF_XY_ch9->Fill(  pos_x,  pos_y );
            Si_spectrum_ch9->Fill(  PH_SiDetector_ch9 );
            PH_vs_TOF_ch9->Fill( time_difference_SiDetector_ch9, PH_SiDetector_ch9 );

            const Long64_t time_sum_int_X = ( ( timestamp_X1 - timestamp_MCP_1) + ( timestamp_X2 - timestamp_MCP_1));
            const Long64_t time_diff_int_X = ( ( timestamp_X1 ) - ( timestamp_X2 ));
            
            const Double_t time_sum_X = time_sum_int_X*ch_to_ns;
            const Double_t time_diff_X = time_diff_int_X*ch_to_ns;

            const Long64_t time_sum_int_Y = ( ( timestamp_Y1 - timestamp_MCP_1) + ( timestamp_Y2 - timestamp_MCP_1));
            const Long64_t time_diff_int_Y = ( ( timestamp_Y1 ) - ( timestamp_Y2 ));

            const Double_t time_sum_Y = time_sum_int_Y*ch_to_ns;
            const Double_t time_diff_Y = time_diff_int_Y*ch_to_ns;
            
            const Double_t pos_diff_x = (time_diff_X)*ns_to_mm;
            const Double_t pos_diff_y = (time_diff_Y)*ns_to_mm;

            if ((pos_diff_x * pos_diff_x + pos_diff_y * pos_diff_y) < (1.128 * 1.128))
            {
                ToF_X1X2_MCP_ch9->Fill(time_sum_X);
                ToF_Y1Y2_MCP_ch9->Fill(time_sum_Y);

                const Double_t pos_sum_x = (time_sum_X)*ns_to_mm;
                const Double_t pos_sum_y = (time_sum_Y)*ns_to_mm;

                ToF_ToF_X1X2_Y1Y2_sum_ch9->Fill(  pos_sum_x,  pos_sum_y );
                ToF_ToF_X1X2_Y1Y2_subs_ch9->Fill(  pos_diff_x,  pos_diff_y );
                ToF_ToF_X1minusX2_X1plusX2_ch9->Fill(  pos_diff_x, pos_sum_x );
                ToF_ToF_Y1minusY2_Y1plusY2_ch9->Fill(  pos_diff_y, pos_sum_y );
            }

        }


        if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch10 && found_MCP_1 ) ) {
            
            const Double_t offset_X = 41.5170;
            const Double_t offset_Y = 41.5170;

            // Since the x1 and y1 signals were mounred on one card and x2 y2 on the inmmediate next card on the chain there is a
            // constant offset of 41.51 ns betwween the two cards, this offset needs to be substracted from x2 and y2

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            // const Long64_t time_difference_int_X = (timestamp_X1 - (timestamp_X2 - 41.51));
            // const Double_t time_difference_X = time_difference_int_X * ns_to_mm;
            const Double_t time_difference_X = (time_difference_int_X) * ch_to_ns * ns_to_mm;

            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            const Long64_t time_difference_int_SiDetector_ch10 = (timestamp_SiDetector_ch10- timestamp_MCP_1);
            const Double_t time_difference_SiDetector_ch10 = (time_difference_int_SiDetector_ch10) * ch_to_ns;

            // const Long64_t time_difference_int_Y = (timestamp_Y1 - (timestamp_Y2 - 41.51));
            // const Double_t time_difference_Y = time_difference_int_Y * ns_to_mm;
            const Double_t time_difference_Y = (time_difference_int_Y) * ch_to_ns * ns_to_mm;

            //cout << "Found both! Time difference: " << (long long)time_difference_int << " ch; " << time_difference << " ns" << endl;

            ToF_X1X2_ch10->Fill(time_difference_X);
            ToF_Y1Y2_ch10->Fill(time_difference_Y);
            ToF_MCPSi_ch10->Fill( time_difference_SiDetector_ch10 );

            const Double_t pos_x = (time_difference_X);
            const Double_t pos_y = (time_difference_Y);

            ToF_ToF_XY_ch10->Fill(  pos_x,  pos_y );
            Si_spectrum_ch10->Fill(  PH_SiDetector_ch10 );
            PH_vs_TOF_ch10->Fill( time_difference_SiDetector_ch10, PH_SiDetector_ch10 );

            const Long64_t time_sum_int_X = ( ( timestamp_X1 - timestamp_MCP_1) + ( timestamp_X2 - timestamp_MCP_1));
            const Long64_t time_diff_int_X = ( ( timestamp_X1 ) - ( timestamp_X2 ));
            
            const Double_t time_sum_X = time_sum_int_X*ch_to_ns;
            const Double_t time_diff_X = time_diff_int_X*ch_to_ns;

            const Long64_t time_sum_int_Y = ( ( timestamp_Y1 - timestamp_MCP_1) + ( timestamp_Y2 - timestamp_MCP_1));
            const Long64_t time_diff_int_Y = ( ( timestamp_Y1 ) - ( timestamp_Y2 ));

            const Double_t time_sum_Y = time_sum_int_Y*ch_to_ns;
            const Double_t time_diff_Y = time_diff_int_Y*ch_to_ns;
            
            const Double_t pos_diff_x = (time_diff_X)*ns_to_mm;
            const Double_t pos_diff_y = (time_diff_Y)*ns_to_mm;

            if ((pos_diff_x * pos_diff_x + pos_diff_y * pos_diff_y) < (1.128 * 1.128))
            {
                ToF_X1X2_MCP_ch10->Fill(time_sum_X);
                ToF_Y1Y2_MCP_ch10->Fill(time_sum_Y);

                const Double_t pos_sum_x = (time_sum_X)*ns_to_mm;
                const Double_t pos_sum_y = (time_sum_Y)*ns_to_mm;

                ToF_ToF_X1X2_Y1Y2_sum_ch10->Fill(  pos_sum_x,  pos_sum_y );
                ToF_ToF_X1X2_Y1Y2_subs_ch10->Fill(  pos_diff_x,  pos_diff_y );
                ToF_ToF_X1minusX2_X1plusX2_ch10->Fill(  pos_diff_x, pos_sum_x );
                ToF_ToF_Y1minusY2_Y1plusY2_ch10->Fill(  pos_diff_y, pos_sum_y );
            }

        }


        if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch11 && found_MCP_1 ) ) {
            
            const Double_t offset_X = 41.5170;
            const Double_t offset_Y = 41.5170;

            // Since the x1 and y1 signals were mounred on one card and x2 y2 on the inmmediate next card on the chain there is a
            // constant offset of 41.51 ns betwween the two cards, this offset needs to be substracted from x2 and y2

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            // const Long64_t time_difference_int_X = (timestamp_X1 - (timestamp_X2 - 41.51));
            // const Double_t time_difference_X = time_difference_int_X * ns_to_mm;
            const Double_t time_difference_X = (time_difference_int_X) * ch_to_ns * ns_to_mm;

            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            const Long64_t time_difference_int_SiDetector_ch11 = (timestamp_SiDetector_ch11- timestamp_MCP_1);
            const Double_t time_difference_SiDetector_ch11 = (time_difference_int_SiDetector_ch11) * ch_to_ns;

            // const Long64_t time_difference_int_Y = (timestamp_Y1 - (timestamp_Y2 - 41.51));
            // const Double_t time_difference_Y = time_difference_int_Y * ns_to_mm;
            const Double_t time_difference_Y = (time_difference_int_Y) * ch_to_ns * ns_to_mm;

            //cout << "Found both! Time difference: " << (long long)time_difference_int << " ch; " << time_difference << " ns" << endl;

            ToF_X1X2_ch11->Fill(time_difference_X);
            ToF_Y1Y2_ch11->Fill(time_difference_Y);
            ToF_MCPSi_ch11->Fill( time_difference_SiDetector_ch11 );

            const Double_t pos_x = (time_difference_X);
            const Double_t pos_y = (time_difference_Y);

            ToF_ToF_XY_ch11->Fill(  pos_x,  pos_y );
            Si_spectrum_ch11->Fill(  PH_SiDetector_ch11 );
            PH_vs_TOF_ch11->Fill( time_difference_SiDetector_ch11, PH_SiDetector_ch11 );

            const Long64_t time_sum_int_X = ( ( timestamp_X1 - timestamp_MCP_1) + ( timestamp_X2 - timestamp_MCP_1));
            const Long64_t time_diff_int_X = ( ( timestamp_X1 ) - ( timestamp_X2 ));
            
            const Double_t time_sum_X = time_sum_int_X*ch_to_ns;
            const Double_t time_diff_X = time_diff_int_X*ch_to_ns;

            const Long64_t time_sum_int_Y = ( ( timestamp_Y1 - timestamp_MCP_1) + ( timestamp_Y2 - timestamp_MCP_1));
            const Long64_t time_diff_int_Y = ( ( timestamp_Y1 ) - ( timestamp_Y2 ));

            const Double_t time_sum_Y = time_sum_int_Y*ch_to_ns;
            const Double_t time_diff_Y = time_diff_int_Y*ch_to_ns;
            
            const Double_t pos_diff_x = (time_diff_X)*ns_to_mm;
            const Double_t pos_diff_y = (time_diff_Y)*ns_to_mm;

            if ((pos_diff_x * pos_diff_x + pos_diff_y * pos_diff_y) < (1.128 * 1.128))
            {
                ToF_X1X2_MCP_ch11->Fill(time_sum_X);
                ToF_Y1Y2_MCP_ch11->Fill(time_sum_Y);

                const Double_t pos_sum_x = (time_sum_X)*ns_to_mm;
                const Double_t pos_sum_y = (time_sum_Y)*ns_to_mm;

                ToF_ToF_X1X2_Y1Y2_sum_ch11->Fill(  pos_sum_x,  pos_sum_y );
                ToF_ToF_X1X2_Y1Y2_subs_ch11->Fill(  pos_diff_x,  pos_diff_y );
                ToF_ToF_X1minusX2_X1plusX2_ch11->Fill(  pos_diff_x, pos_sum_x );
                ToF_ToF_Y1minusY2_Y1plusY2_ch11->Fill(  pos_diff_y, pos_sum_y );
            }

        }

        if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch12 && found_MCP_1 ) ) {
            
            const Double_t offset_X = 41.5170;
            const Double_t offset_Y = 41.5170;

            // Since the x1 and y1 signals were mounred on one card and x2 y2 on the inmmediate next card on the chain there is a
            // constant offset of 41.51 ns betwween the two cards, this offset needs to be substracted from x2 and y2

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            // const Long64_t time_difference_int_X = (timestamp_X1 - (timestamp_X2 - 41.51));
            // const Double_t time_difference_X = time_difference_int_X * ns_to_mm;
            const Double_t time_difference_X = (time_difference_int_X) * ch_to_ns * ns_to_mm;

            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            const Long64_t time_difference_int_SiDetector_ch12 = (timestamp_SiDetector_ch12- timestamp_MCP_1);
            const Double_t time_difference_SiDetector_ch12 = (time_difference_int_SiDetector_ch12) * ch_to_ns;

            // const Long64_t time_difference_int_Y = (timestamp_Y1 - (timestamp_Y2 - 41.51));
            // const Double_t time_difference_Y = time_difference_int_Y * ns_to_mm;
            const Double_t time_difference_Y = (time_difference_int_Y) * ch_to_ns * ns_to_mm;

            //cout << "Found both! Time difference: " << (long long)time_difference_int << " ch; " << time_difference << " ns" << endl;

            ToF_X1X2_ch12->Fill(time_difference_X);
            ToF_Y1Y2_ch12->Fill(time_difference_Y);
            ToF_MCPSi_ch12->Fill( time_difference_SiDetector_ch12 );

            const Double_t pos_x = (time_difference_X);
            const Double_t pos_y = (time_difference_Y);

            ToF_ToF_XY_ch12->Fill(  pos_x,  pos_y );
            Si_spectrum_ch12->Fill(  PH_SiDetector_ch12 );
            PH_vs_TOF_ch12->Fill( time_difference_SiDetector_ch12, PH_SiDetector_ch12 );

            const Long64_t time_sum_int_X = ( ( timestamp_X1 - timestamp_MCP_1) + ( timestamp_X2 - timestamp_MCP_1));
            const Long64_t time_diff_int_X = ( ( timestamp_X1 ) - ( timestamp_X2 ));
            
            const Double_t time_sum_X = time_sum_int_X*ch_to_ns;
            const Double_t time_diff_X = time_diff_int_X*ch_to_ns;

            const Long64_t time_sum_int_Y = ( ( timestamp_Y1 - timestamp_MCP_1) + ( timestamp_Y2 - timestamp_MCP_1));
            const Long64_t time_diff_int_Y = ( ( timestamp_Y1 ) - ( timestamp_Y2 ));

            const Double_t time_sum_Y = time_sum_int_Y*ch_to_ns;
            const Double_t time_diff_Y = time_diff_int_Y*ch_to_ns;
            
            const Double_t pos_diff_x = (time_diff_X)*ns_to_mm;
            const Double_t pos_diff_y = (time_diff_Y)*ns_to_mm;

            if ((pos_diff_x * pos_diff_x + pos_diff_y * pos_diff_y) < (1.128 * 1.128))
            {
                ToF_X1X2_MCP_ch12->Fill(time_sum_X);
                ToF_Y1Y2_MCP_ch12->Fill(time_sum_Y);

                const Double_t pos_sum_x = (time_sum_X)*ns_to_mm;
                const Double_t pos_sum_y = (time_sum_Y)*ns_to_mm;

                ToF_ToF_X1X2_Y1Y2_sum_ch12->Fill(  pos_sum_x,  pos_sum_y );
                ToF_ToF_X1X2_Y1Y2_subs_ch12->Fill(  pos_diff_x,  pos_diff_y );
                ToF_ToF_X1minusX2_X1plusX2_ch12->Fill(  pos_diff_x, pos_sum_x );
                ToF_ToF_Y1minusY2_Y1plusY2_ch12->Fill(  pos_diff_y, pos_sum_y );
            }

        }


        if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch13 && found_MCP_1 ) ) {
            
            const Double_t offset_X = 41.5170;
            const Double_t offset_Y = 41.5170;

            // Since the x1 and y1 signals were mounred on one card and x2 y2 on the inmmediate next card on the chain there is a
            // constant offset of 41.51 ns betwween the two cards, this offset needs to be substracted from x2 and y2

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            // const Long64_t time_difference_int_X = (timestamp_X1 - (timestamp_X2 - 41.51));
            // const Double_t time_difference_X = time_difference_int_X * ns_to_mm;
            const Double_t time_difference_X = (time_difference_int_X) * ch_to_ns * ns_to_mm;

            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            const Long64_t time_difference_int_SiDetector_ch13 = (timestamp_SiDetector_ch13- timestamp_MCP_1);
            const Double_t time_difference_SiDetector_ch13 = (time_difference_int_SiDetector_ch13) * ch_to_ns;

            // const Long64_t time_difference_int_Y = (timestamp_Y1 - (timestamp_Y2 - 41.51));
            // const Double_t time_difference_Y = time_difference_int_Y * ns_to_mm;
            const Double_t time_difference_Y = (time_difference_int_Y) * ch_to_ns * ns_to_mm;

            //cout << "Found both! Time difference: " << (long long)time_difference_int << " ch; " << time_difference << " ns" << endl;

            ToF_X1X2_ch13->Fill(time_difference_X);
            ToF_Y1Y2_ch13->Fill(time_difference_Y);
            ToF_MCPSi_ch13->Fill( time_difference_SiDetector_ch13 );

            const Double_t pos_x = (time_difference_X);
            const Double_t pos_y = (time_difference_Y);

            ToF_ToF_XY_ch13->Fill(  pos_x,  pos_y );
            Si_spectrum_ch13->Fill(  PH_SiDetector_ch13 );
            PH_vs_TOF_ch13->Fill( time_difference_SiDetector_ch13, PH_SiDetector_ch13 );

            const Long64_t time_sum_int_X = ( ( timestamp_X1 - timestamp_MCP_1) + ( timestamp_X2 - timestamp_MCP_1));
            const Long64_t time_diff_int_X = ( ( timestamp_X1 ) - ( timestamp_X2 ));
            
            const Double_t time_sum_X = time_sum_int_X*ch_to_ns;
            const Double_t time_diff_X = time_diff_int_X*ch_to_ns;

            const Long64_t time_sum_int_Y = ( ( timestamp_Y1 - timestamp_MCP_1) + ( timestamp_Y2 - timestamp_MCP_1));
            const Long64_t time_diff_int_Y = ( ( timestamp_Y1 ) - ( timestamp_Y2 ));

            const Double_t time_sum_Y = time_sum_int_Y*ch_to_ns;
            const Double_t time_diff_Y = time_diff_int_Y*ch_to_ns;
            
            const Double_t pos_diff_x = (time_diff_X)*ns_to_mm;
            const Double_t pos_diff_y = (time_diff_Y)*ns_to_mm;

            if ((pos_diff_x * pos_diff_x + pos_diff_y * pos_diff_y) < (1.128 * 1.128))
            {
                ToF_X1X2_MCP_ch13->Fill(time_sum_X);
                ToF_Y1Y2_MCP_ch13->Fill(time_sum_Y);

                const Double_t pos_sum_x = (time_sum_X)*ns_to_mm;
                const Double_t pos_sum_y = (time_sum_Y)*ns_to_mm;

                ToF_ToF_X1X2_Y1Y2_sum_ch13->Fill(  pos_sum_x,  pos_sum_y );
                ToF_ToF_X1X2_Y1Y2_subs_ch13->Fill(  pos_diff_x,  pos_diff_y );
                ToF_ToF_X1minusX2_X1plusX2_ch13->Fill(  pos_diff_x, pos_sum_x );
                ToF_ToF_Y1minusY2_Y1plusY2_ch13->Fill(  pos_diff_y, pos_sum_y );
            }

        }        

        if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch14 && found_MCP_1 ) ) {
            
            const Double_t offset_X = 41.5170;
            const Double_t offset_Y = 41.5170;

            // Since the x1 and y1 signals were mounred on one card and x2 y2 on the inmmediate next card on the chain there is a
            // constant offset of 41.51 ns betwween the two cards, this offset needs to be substracted from x2 and y2

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            // const Long64_t time_difference_int_X = (timestamp_X1 - (timestamp_X2 - 41.51));
            // const Double_t time_difference_X = time_difference_int_X * ns_to_mm;
            const Double_t time_difference_X = (time_difference_int_X) * ch_to_ns * ns_to_mm;

            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            const Long64_t time_difference_int_SiDetector_ch14 = (timestamp_SiDetector_ch14- timestamp_MCP_1);
            const Double_t time_difference_SiDetector_ch14 = (time_difference_int_SiDetector_ch14) * ch_to_ns;

            // const Long64_t time_difference_int_Y = (timestamp_Y1 - (timestamp_Y2 - 41.51));
            // const Double_t time_difference_Y = time_difference_int_Y * ns_to_mm;
            const Double_t time_difference_Y = (time_difference_int_Y) * ch_to_ns * ns_to_mm;

            //cout << "Found both! Time difference: " << (long long)time_difference_int << " ch; " << time_difference << " ns" << endl;

            ToF_X1X2_ch14->Fill(time_difference_X);
            ToF_Y1Y2_ch14->Fill(time_difference_Y);
            ToF_MCPSi_ch14->Fill( time_difference_SiDetector_ch14 );

            const Double_t pos_x = (time_difference_X);
            const Double_t pos_y = (time_difference_Y);

            ToF_ToF_XY_ch14->Fill(  pos_x,  pos_y );
            Si_spectrum_ch14->Fill(  PH_SiDetector_ch14 );
            PH_vs_TOF_ch14->Fill( time_difference_SiDetector_ch14, PH_SiDetector_ch14 );

            const Long64_t time_sum_int_X = ( ( timestamp_X1 - timestamp_MCP_1) + ( timestamp_X2 - timestamp_MCP_1));
            const Long64_t time_diff_int_X = ( ( timestamp_X1 ) - ( timestamp_X2 ));
            
            const Double_t time_sum_X = time_sum_int_X*ch_to_ns;
            const Double_t time_diff_X = time_diff_int_X*ch_to_ns;

            const Long64_t time_sum_int_Y = ( ( timestamp_Y1 - timestamp_MCP_1) + ( timestamp_Y2 - timestamp_MCP_1));
            const Long64_t time_diff_int_Y = ( ( timestamp_Y1 ) - ( timestamp_Y2 ));

            const Double_t time_sum_Y = time_sum_int_Y*ch_to_ns;
            const Double_t time_diff_Y = time_diff_int_Y*ch_to_ns;
            
            const Double_t pos_diff_x = (time_diff_X)*ns_to_mm;
            const Double_t pos_diff_y = (time_diff_Y)*ns_to_mm;

            if ((pos_diff_x * pos_diff_x + pos_diff_y * pos_diff_y) < (1.128 * 1.128))
            {
                ToF_X1X2_MCP_ch14->Fill(time_sum_X);
                ToF_Y1Y2_MCP_ch14->Fill(time_sum_Y);

                const Double_t pos_sum_x = (time_sum_X)*ns_to_mm;
                const Double_t pos_sum_y = (time_sum_Y)*ns_to_mm;

                ToF_ToF_X1X2_Y1Y2_sum_ch14->Fill(  pos_sum_x,  pos_sum_y );
                ToF_ToF_X1X2_Y1Y2_subs_ch14->Fill(  pos_diff_x,  pos_diff_y );
                ToF_ToF_X1minusX2_X1plusX2_ch14->Fill(  pos_diff_x, pos_sum_x );
                ToF_ToF_Y1minusY2_Y1plusY2_ch14->Fill(  pos_diff_y, pos_sum_y );
            }

        }      


        if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch15 && found_MCP_1 ) ) {
            
            const Double_t offset_X = 41.5170;
            const Double_t offset_Y = 41.5170;

            // Since the x1 and y1 signals were mounred on one card and x2 y2 on the inmmediate next card on the chain there is a
            // constant offset of 41.51 ns betwween the two cards, this offset needs to be substracted from x2 and y2

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            // const Long64_t time_difference_int_X = (timestamp_X1 - (timestamp_X2 - 41.51));
            // const Double_t time_difference_X = time_difference_int_X * ns_to_mm;
            const Double_t time_difference_X = (time_difference_int_X) * ch_to_ns * ns_to_mm;

            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            const Long64_t time_difference_int_SiDetector_ch15 = (timestamp_SiDetector_ch15- timestamp_MCP_1);
            const Double_t time_difference_SiDetector_ch15 = (time_difference_int_SiDetector_ch15) * ch_to_ns;

            // const Long64_t time_difference_int_Y = (timestamp_Y1 - (timestamp_Y2 - 41.51));
            // const Double_t time_difference_Y = time_difference_int_Y * ns_to_mm;
            const Double_t time_difference_Y = (time_difference_int_Y) * ch_to_ns * ns_to_mm;

            //cout << "Found both! Time difference: " << (long long)time_difference_int << " ch; " << time_difference << " ns" << endl;

            ToF_X1X2_ch15->Fill(time_difference_X);
            ToF_Y1Y2_ch15->Fill(time_difference_Y);
            ToF_MCPSi_ch15->Fill( time_difference_SiDetector_ch15 );

            const Double_t pos_x = (time_difference_X);
            const Double_t pos_y = (time_difference_Y);

            ToF_ToF_XY_ch15->Fill(  pos_x,  pos_y );
            Si_spectrum_ch15->Fill(  PH_SiDetector_ch15 );
            PH_vs_TOF_ch15->Fill( time_difference_SiDetector_ch15, PH_SiDetector_ch15 );

            const Long64_t time_sum_int_X = ( ( timestamp_X1 - timestamp_MCP_1) + ( timestamp_X2 - timestamp_MCP_1));
            const Long64_t time_diff_int_X = ( ( timestamp_X1 ) - ( timestamp_X2 ));
            
            const Double_t time_sum_X = time_sum_int_X*ch_to_ns;
            const Double_t time_diff_X = time_diff_int_X*ch_to_ns;

            const Long64_t time_sum_int_Y = ( ( timestamp_Y1 - timestamp_MCP_1) + ( timestamp_Y2 - timestamp_MCP_1));
            const Long64_t time_diff_int_Y = ( ( timestamp_Y1 ) - ( timestamp_Y2 ));

            const Double_t time_sum_Y = time_sum_int_Y*ch_to_ns;
            const Double_t time_diff_Y = time_diff_int_Y*ch_to_ns;
            
            const Double_t pos_diff_x = (time_diff_X)*ns_to_mm;
            const Double_t pos_diff_y = (time_diff_Y)*ns_to_mm;

            if ((pos_diff_x * pos_diff_x + pos_diff_y * pos_diff_y) < (1.128 * 1.128))
            {
                ToF_X1X2_MCP_ch15->Fill(time_sum_X);
                ToF_Y1Y2_MCP_ch15->Fill(time_sum_Y);

                const Double_t pos_sum_x = (time_sum_X)*ns_to_mm;
                const Double_t pos_sum_y = (time_sum_Y)*ns_to_mm;

                ToF_ToF_X1X2_Y1Y2_sum_ch15->Fill(  pos_sum_x,  pos_sum_y );
                ToF_ToF_X1X2_Y1Y2_subs_ch15->Fill(  pos_diff_x,  pos_diff_y );
                ToF_ToF_X1minusX2_X1plusX2_ch15->Fill(  pos_diff_x, pos_sum_x );
                ToF_ToF_Y1minusY2_Y1plusY2_ch15->Fill(  pos_diff_y, pos_sum_y );
            }

        }      


        if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch16 && found_MCP_1 ) ) {
            
            const Double_t offset_X = 41.5170;
            const Double_t offset_Y = 41.5170;

            // Since the x1 and y1 signals were mounred on one card and x2 y2 on the inmmediate next card on the chain there is a
            // constant offset of 41.51 ns betwween the two cards, this offset needs to be substracted from x2 and y2

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            // const Long64_t time_difference_int_X = (timestamp_X1 - (timestamp_X2 - 41.51));
            // const Double_t time_difference_X = time_difference_int_X * ns_to_mm;
            const Double_t time_difference_X = (time_difference_int_X) * ch_to_ns * ns_to_mm;

            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            const Long64_t time_difference_int_SiDetector_ch16 = (timestamp_SiDetector_ch16- timestamp_MCP_1);
            const Double_t time_difference_SiDetector_ch16 = (time_difference_int_SiDetector_ch16) * ch_to_ns;

            // const Long64_t time_difference_int_Y = (timestamp_Y1 - (timestamp_Y2 - 41.51));
            // const Double_t time_difference_Y = time_difference_int_Y * ns_to_mm;
            const Double_t time_difference_Y = (time_difference_int_Y) * ch_to_ns * ns_to_mm;

            //cout << "Found both! Time difference: " << (long long)time_difference_int << " ch; " << time_difference << " ns" << endl;

            ToF_X1X2_ch16->Fill(time_difference_X);
            ToF_Y1Y2_ch16->Fill(time_difference_Y);
            ToF_MCPSi_ch16->Fill( time_difference_SiDetector_ch16 );

            const Double_t pos_x = (time_difference_X);
            const Double_t pos_y = (time_difference_Y);

            ToF_ToF_XY_ch16->Fill(  pos_x,  pos_y );
            Si_spectrum_ch16->Fill(  PH_SiDetector_ch16 );
            PH_vs_TOF_ch16->Fill( time_difference_SiDetector_ch16, PH_SiDetector_ch16 );

            const Long64_t time_sum_int_X = ( ( timestamp_X1 - timestamp_MCP_1) + ( timestamp_X2 - timestamp_MCP_1));
            const Long64_t time_diff_int_X = ( ( timestamp_X1 ) - ( timestamp_X2 ));
            
            const Double_t time_sum_X = time_sum_int_X*ch_to_ns;
            const Double_t time_diff_X = time_diff_int_X*ch_to_ns;

            const Long64_t time_sum_int_Y = ( ( timestamp_Y1 - timestamp_MCP_1) + ( timestamp_Y2 - timestamp_MCP_1));
            const Long64_t time_diff_int_Y = ( ( timestamp_Y1 ) - ( timestamp_Y2 ));

            const Double_t time_sum_Y = time_sum_int_Y*ch_to_ns;
            const Double_t time_diff_Y = time_diff_int_Y*ch_to_ns;
            
            const Double_t pos_diff_x = (time_diff_X)*ns_to_mm;
            const Double_t pos_diff_y = (time_diff_Y)*ns_to_mm;

            if ((pos_diff_x * pos_diff_x + pos_diff_y * pos_diff_y) < (1.128 * 1.128))
            {
                ToF_X1X2_MCP_ch16->Fill(time_sum_X);
                ToF_Y1Y2_MCP_ch16->Fill(time_sum_Y);

                const Double_t pos_sum_x = (time_sum_X)*ns_to_mm;
                const Double_t pos_sum_y = (time_sum_Y)*ns_to_mm;

                ToF_ToF_X1X2_Y1Y2_sum_ch16->Fill(  pos_sum_x,  pos_sum_y );
                ToF_ToF_X1X2_Y1Y2_subs_ch16->Fill(  pos_diff_x,  pos_diff_y );
                ToF_ToF_X1minusX2_X1plusX2_ch16->Fill(  pos_diff_x, pos_sum_x );
                ToF_ToF_Y1minusY2_Y1plusY2_ch16->Fill(  pos_diff_y, pos_sum_y );
            }

        }      

        if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch17 && found_MCP_1 ) ) {
            
            const Double_t offset_X = 41.5170;
            const Double_t offset_Y = 41.5170;

            // Since the x1 and y1 signals were mounred on one card and x2 y2 on the inmmediate next card on the chain there is a
            // constant offset of 41.51 ns betwween the two cards, this offset needs to be substracted from x2 and y2

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            // const Long64_t time_difference_int_X = (timestamp_X1 - (timestamp_X2 - 41.51));
            // const Double_t time_difference_X = time_difference_int_X * ns_to_mm;
            const Double_t time_difference_X = (time_difference_int_X) * ch_to_ns * ns_to_mm;

            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            const Long64_t time_difference_int_SiDetector_ch17 = (timestamp_SiDetector_ch17- timestamp_MCP_1);
            const Double_t time_difference_SiDetector_ch17 = (time_difference_int_SiDetector_ch17) * ch_to_ns;

            // const Long64_t time_difference_int_Y = (timestamp_Y1 - (timestamp_Y2 - 41.51));
            // const Double_t time_difference_Y = time_difference_int_Y * ns_to_mm;
            const Double_t time_difference_Y = (time_difference_int_Y) * ch_to_ns * ns_to_mm;

            //cout << "Found both! Time difference: " << (long long)time_difference_int << " ch; " << time_difference << " ns" << endl;

            ToF_X1X2_ch17->Fill(time_difference_X);
            ToF_Y1Y2_ch17->Fill(time_difference_Y);
            ToF_MCPSi_ch17->Fill( time_difference_SiDetector_ch17 );

            const Double_t pos_x = (time_difference_X);
            const Double_t pos_y = (time_difference_Y);

            ToF_ToF_XY_ch17->Fill(  pos_x,  pos_y );
            Si_spectrum_ch17->Fill(  PH_SiDetector_ch17 );
            PH_vs_TOF_ch17->Fill( time_difference_SiDetector_ch17, PH_SiDetector_ch17 );

            const Long64_t time_sum_int_X = ( ( timestamp_X1 - timestamp_MCP_1) + ( timestamp_X2 - timestamp_MCP_1));
            const Long64_t time_diff_int_X = ( ( timestamp_X1 ) - ( timestamp_X2 ));
            
            const Double_t time_sum_X = time_sum_int_X*ch_to_ns;
            const Double_t time_diff_X = time_diff_int_X*ch_to_ns;

            const Long64_t time_sum_int_Y = ( ( timestamp_Y1 - timestamp_MCP_1) + ( timestamp_Y2 - timestamp_MCP_1));
            const Long64_t time_diff_int_Y = ( ( timestamp_Y1 ) - ( timestamp_Y2 ));

            const Double_t time_sum_Y = time_sum_int_Y*ch_to_ns;
            const Double_t time_diff_Y = time_diff_int_Y*ch_to_ns;
            
            const Double_t pos_diff_x = (time_diff_X)*ns_to_mm;
            const Double_t pos_diff_y = (time_diff_Y)*ns_to_mm;

            if ((pos_diff_x * pos_diff_x + pos_diff_y * pos_diff_y) < (1.128 * 1.128))
            {
                ToF_X1X2_MCP_ch17->Fill(time_sum_X);
                ToF_Y1Y2_MCP_ch17->Fill(time_sum_Y);

                const Double_t pos_sum_x = (time_sum_X)*ns_to_mm;
                const Double_t pos_sum_y = (time_sum_Y)*ns_to_mm;

                ToF_ToF_X1X2_Y1Y2_sum_ch17->Fill(  pos_sum_x,  pos_sum_y );
                ToF_ToF_X1X2_Y1Y2_subs_ch17->Fill(  pos_diff_x,  pos_diff_y );
                ToF_ToF_X1minusX2_X1plusX2_ch17->Fill(  pos_diff_x, pos_sum_x );
                ToF_ToF_Y1minusY2_Y1plusY2_ch17->Fill(  pos_diff_y, pos_sum_y );
            }

        }      

        if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch18 && found_MCP_1 ) ) {
            
            const Double_t offset_X = 41.5170;
            const Double_t offset_Y = 41.5170;

            // Since the x1 and y1 signals were mounred on one card and x2 y2 on the inmmediate next card on the chain there is a
            // constant offset of 41.51 ns betwween the two cards, this offset needs to be substracted from x2 and y2

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            // const Long64_t time_difference_int_X = (timestamp_X1 - (timestamp_X2 - 41.51));
            // const Double_t time_difference_X = time_difference_int_X * ns_to_mm;
            const Double_t time_difference_X = (time_difference_int_X) * ch_to_ns * ns_to_mm;

            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            const Long64_t time_difference_int_SiDetector_ch18 = (timestamp_SiDetector_ch18- timestamp_MCP_1);
            const Double_t time_difference_SiDetector_ch18 = (time_difference_int_SiDetector_ch18) * ch_to_ns;

            // const Long64_t time_difference_int_Y = (timestamp_Y1 - (timestamp_Y2 - 41.51));
            // const Double_t time_difference_Y = time_difference_int_Y * ns_to_mm;
            const Double_t time_difference_Y = (time_difference_int_Y) * ch_to_ns * ns_to_mm;

            //cout << "Found both! Time difference: " << (long long)time_difference_int << " ch; " << time_difference << " ns" << endl;

            ToF_X1X2_ch18->Fill(time_difference_X);
            ToF_Y1Y2_ch18->Fill(time_difference_Y);
            ToF_MCPSi_ch18->Fill( time_difference_SiDetector_ch18 );

            const Double_t pos_x = (time_difference_X);
            const Double_t pos_y = (time_difference_Y);

            ToF_ToF_XY_ch18->Fill(  pos_x,  pos_y );
            Si_spectrum_ch18->Fill(  PH_SiDetector_ch18 );
            PH_vs_TOF_ch18->Fill( time_difference_SiDetector_ch18, PH_SiDetector_ch18 );

            const Long64_t time_sum_int_X = ( ( timestamp_X1 - timestamp_MCP_1) + ( timestamp_X2 - timestamp_MCP_1));
            const Long64_t time_diff_int_X = ( ( timestamp_X1 ) - ( timestamp_X2 ));
            
            const Double_t time_sum_X = time_sum_int_X*ch_to_ns;
            const Double_t time_diff_X = time_diff_int_X*ch_to_ns;

            const Long64_t time_sum_int_Y = ( ( timestamp_Y1 - timestamp_MCP_1) + ( timestamp_Y2 - timestamp_MCP_1));
            const Long64_t time_diff_int_Y = ( ( timestamp_Y1 ) - ( timestamp_Y2 ));

            const Double_t time_sum_Y = time_sum_int_Y*ch_to_ns;
            const Double_t time_diff_Y = time_diff_int_Y*ch_to_ns;
            
            const Double_t pos_diff_x = (time_diff_X)*ns_to_mm;
            const Double_t pos_diff_y = (time_diff_Y)*ns_to_mm;

            if ((pos_diff_x * pos_diff_x + pos_diff_y * pos_diff_y) < (1.128 * 1.128))
            {
                ToF_X1X2_MCP_ch18->Fill(time_sum_X);
                ToF_Y1Y2_MCP_ch18->Fill(time_sum_Y);

                const Double_t pos_sum_x = (time_sum_X)*ns_to_mm;
                const Double_t pos_sum_y = (time_sum_Y)*ns_to_mm;

                ToF_ToF_X1X2_Y1Y2_sum_ch18->Fill(  pos_sum_x,  pos_sum_y );
                ToF_ToF_X1X2_Y1Y2_subs_ch18->Fill(  pos_diff_x,  pos_diff_y );
                ToF_ToF_X1minusX2_X1plusX2_ch18->Fill(  pos_diff_x, pos_sum_x );
                ToF_ToF_Y1minusY2_Y1plusY2_ch18->Fill(  pos_diff_y, pos_sum_y );
            }

        }      

        if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch19 && found_MCP_1 ) ) {
            
            const Double_t offset_X = 41.5170;
            const Double_t offset_Y = 41.5170;

            // Since the x1 and y1 signals were mounred on one card and x2 y2 on the inmmediate next card on the chain there is a
            // constant offset of 41.51 ns betwween the two cards, this offset needs to be substracted from x2 and y2

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            // const Long64_t time_difference_int_X = (timestamp_X1 - (timestamp_X2 - 41.51));
            // const Double_t time_difference_X = time_difference_int_X * ns_to_mm;
            const Double_t time_difference_X = (time_difference_int_X) * ch_to_ns * ns_to_mm;

            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            const Long64_t time_difference_int_SiDetector_ch19 = (timestamp_SiDetector_ch19- timestamp_MCP_1);
            const Double_t time_difference_SiDetector_ch19 = (time_difference_int_SiDetector_ch19) * ch_to_ns;

            // const Long64_t time_difference_int_Y = (timestamp_Y1 - (timestamp_Y2 - 41.51));
            // const Double_t time_difference_Y = time_difference_int_Y * ns_to_mm;
            const Double_t time_difference_Y = (time_difference_int_Y) * ch_to_ns * ns_to_mm;

            //cout << "Found both! Time difference: " << (long long)time_difference_int << " ch; " << time_difference << " ns" << endl;

            ToF_X1X2_ch19->Fill(time_difference_X);
            ToF_Y1Y2_ch19->Fill(time_difference_Y);
            ToF_MCPSi_ch19->Fill( time_difference_SiDetector_ch19 );

            const Double_t pos_x = (time_difference_X);
            const Double_t pos_y = (time_difference_Y);

            ToF_ToF_XY_ch19->Fill(  pos_x,  pos_y );
            Si_spectrum_ch19->Fill(  PH_SiDetector_ch19 );
            PH_vs_TOF_ch19->Fill( time_difference_SiDetector_ch19, PH_SiDetector_ch19 );

            const Long64_t time_sum_int_X = ( ( timestamp_X1 - timestamp_MCP_1) + ( timestamp_X2 - timestamp_MCP_1));
            const Long64_t time_diff_int_X = ( ( timestamp_X1 ) - ( timestamp_X2 ));
            
            const Double_t time_sum_X = time_sum_int_X*ch_to_ns;
            const Double_t time_diff_X = time_diff_int_X*ch_to_ns;

            const Long64_t time_sum_int_Y = ( ( timestamp_Y1 - timestamp_MCP_1) + ( timestamp_Y2 - timestamp_MCP_1));
            const Long64_t time_diff_int_Y = ( ( timestamp_Y1 ) - ( timestamp_Y2 ));

            const Double_t time_sum_Y = time_sum_int_Y*ch_to_ns;
            const Double_t time_diff_Y = time_diff_int_Y*ch_to_ns;
            
            const Double_t pos_diff_x = (time_diff_X)*ns_to_mm;
            const Double_t pos_diff_y = (time_diff_Y)*ns_to_mm;

            if ((pos_diff_x * pos_diff_x + pos_diff_y * pos_diff_y) < (1.128 * 1.128))
            {
                ToF_X1X2_MCP_ch19->Fill(time_sum_X);
                ToF_Y1Y2_MCP_ch19->Fill(time_sum_Y);

                const Double_t pos_sum_x = (time_sum_X)*ns_to_mm;
                const Double_t pos_sum_y = (time_sum_Y)*ns_to_mm;

                ToF_ToF_X1X2_Y1Y2_sum_ch19->Fill(  pos_sum_x,  pos_sum_y );
                ToF_ToF_X1X2_Y1Y2_subs_ch19->Fill(  pos_diff_x,  pos_diff_y );
                ToF_ToF_X1minusX2_X1plusX2_ch19->Fill(  pos_diff_x, pos_sum_x );
                ToF_ToF_Y1minusY2_Y1plusY2_ch19->Fill(  pos_diff_y, pos_sum_y );
            }

        }


        if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch20 && found_MCP_1 ) ) {
            
            const Double_t offset_X = 41.5170;
            const Double_t offset_Y = 41.5170;

            // Since the x1 and y1 signals were mounred on one card and x2 y2 on the inmmediate next card on the chain there is a
            // constant offset of 41.51 ns betwween the two cards, this offset needs to be substracted from x2 and y2

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            // const Long64_t time_difference_int_X = (timestamp_X1 - (timestamp_X2 - 41.51));
            // const Double_t time_difference_X = time_difference_int_X * ns_to_mm;
            const Double_t time_difference_X = (time_difference_int_X) * ch_to_ns * ns_to_mm;

            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            const Long64_t time_difference_int_SiDetector_ch20 = (timestamp_SiDetector_ch20- timestamp_MCP_1);
            const Double_t time_difference_SiDetector_ch20 = (time_difference_int_SiDetector_ch20) * ch_to_ns;

            // const Long64_t time_difference_int_Y = (timestamp_Y1 - (timestamp_Y2 - 41.51));
            // const Double_t time_difference_Y = time_difference_int_Y * ns_to_mm;
            const Double_t time_difference_Y = (time_difference_int_Y) * ch_to_ns * ns_to_mm;

            //cout << "Found both! Time difference: " << (long long)time_difference_int << " ch; " << time_difference << " ns" << endl;

            ToF_X1X2_ch20->Fill(time_difference_X);
            ToF_Y1Y2_ch20->Fill(time_difference_Y);
            ToF_MCPSi_ch20->Fill( time_difference_SiDetector_ch20 );

            const Double_t pos_x = (time_difference_X);
            const Double_t pos_y = (time_difference_Y);

            ToF_ToF_XY_ch20->Fill(  pos_x,  pos_y );
            Si_spectrum_ch20->Fill(  PH_SiDetector_ch20 );
            PH_vs_TOF_ch20->Fill( time_difference_SiDetector_ch20, PH_SiDetector_ch20 );

            const Long64_t time_sum_int_X = ( ( timestamp_X1 - timestamp_MCP_1) + ( timestamp_X2 - timestamp_MCP_1));
            const Long64_t time_diff_int_X = ( ( timestamp_X1 ) - ( timestamp_X2 ));
            
            const Double_t time_sum_X = time_sum_int_X*ch_to_ns;
            const Double_t time_diff_X = time_diff_int_X*ch_to_ns;

            const Long64_t time_sum_int_Y = ( ( timestamp_Y1 - timestamp_MCP_1) + ( timestamp_Y2 - timestamp_MCP_1));
            const Long64_t time_diff_int_Y = ( ( timestamp_Y1 ) - ( timestamp_Y2 ));

            const Double_t time_sum_Y = time_sum_int_Y*ch_to_ns;
            const Double_t time_diff_Y = time_diff_int_Y*ch_to_ns;
            
            const Double_t pos_diff_x = (time_diff_X)*ns_to_mm;
            const Double_t pos_diff_y = (time_diff_Y)*ns_to_mm;

            if ((pos_diff_x * pos_diff_x + pos_diff_y * pos_diff_y) < (1.128 * 1.128))
            {
                ToF_X1X2_MCP_ch20->Fill(time_sum_X);
                ToF_Y1Y2_MCP_ch20->Fill(time_sum_Y);

                const Double_t pos_sum_x = (time_sum_X)*ns_to_mm;
                const Double_t pos_sum_y = (time_sum_Y)*ns_to_mm;

                ToF_ToF_X1X2_Y1Y2_sum_ch20->Fill(  pos_sum_x,  pos_sum_y );
                ToF_ToF_X1X2_Y1Y2_subs_ch20->Fill(  pos_diff_x,  pos_diff_y );
                ToF_ToF_X1minusX2_X1plusX2_ch20->Fill(  pos_diff_x, pos_sum_x );
                ToF_ToF_Y1minusY2_Y1plusY2_ch20->Fill(  pos_diff_y, pos_sum_y );
            }

        }

        if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_SiDetector_ch21 && found_MCP_1 ) ) {
            
            const Double_t offset_X = 41.5170;
            const Double_t offset_Y = 41.5170;

            // Since the x1 and y1 signals were mounred on one card and x2 y2 on the inmmediate next card on the chain there is a
            // constant offset of 41.51 ns betwween the two cards, this offset needs to be substracted from x2 and y2

            const Long64_t time_difference_int_X = (timestamp_X1 - timestamp_X2);
            // const Long64_t time_difference_int_X = (timestamp_X1 - (timestamp_X2 - 41.51));
            // const Double_t time_difference_X = time_difference_int_X * ns_to_mm;
            const Double_t time_difference_X = ((time_difference_int_X) * ch_to_ns * ns_to_mm);

            const Long64_t time_difference_int_Y = (timestamp_Y1 - timestamp_Y2);
            const Long64_t time_difference_int_SiDetector_ch21 = (timestamp_SiDetector_ch21- timestamp_MCP_1);
            const Double_t time_difference_SiDetector_ch21 = (time_difference_int_SiDetector_ch21) * ch_to_ns;

            // const Long64_t time_difference_int_Y = (timestamp_Y1 - (timestamp_Y2 - 41.51));
            // const Double_t time_difference_Y = time_difference_int_Y * ns_to_mm;
            const Double_t time_difference_Y = (time_difference_int_Y) * ch_to_ns * ns_to_mm;

            //cout << "Found both! Time difference: " << (long long)time_difference_int << " ch; " << time_difference << " ns" << endl;

            ToF_X1X2_ch21->Fill(time_difference_X);
            ToF_Y1Y2_ch21->Fill(time_difference_Y);
            ToF_MCPSi_ch21->Fill( time_difference_SiDetector_ch21 );

            const Double_t pos_x = (time_difference_X);
            const Double_t pos_y = (time_difference_Y);

            ToF_ToF_XY_ch21->Fill(  pos_x,  pos_y );
            Si_spectrum_ch21->Fill(  PH_SiDetector_ch21 );
            PH_vs_TOF_ch21->Fill( time_difference_SiDetector_ch21, PH_SiDetector_ch21 );

            const Long64_t time_sum_int_X = ( ( timestamp_X1 - timestamp_MCP_1) + ( timestamp_X2 - timestamp_MCP_1));
            const Long64_t time_diff_int_X = ( ( timestamp_X1 ) - ( timestamp_X2 ));
            
            const Double_t time_sum_X = time_sum_int_X*ch_to_ns;
            const Double_t time_diff_X = time_diff_int_X*ch_to_ns;

            const Long64_t time_sum_int_Y = ( ( timestamp_Y1 - timestamp_MCP_1) + ( timestamp_Y2 - timestamp_MCP_1));
            const Long64_t time_diff_int_Y = ( ( timestamp_Y1 ) - ( timestamp_Y2 ));

            const Double_t time_sum_Y = time_sum_int_Y*ch_to_ns;
            const Double_t time_diff_Y = time_diff_int_Y*ch_to_ns;
            
            const Double_t pos_diff_x = (time_diff_X)*ns_to_mm;
            const Double_t pos_diff_y = (time_diff_Y)*ns_to_mm;


            // const Long64_t time_sum_int_X = ( ( timestamp_X1 - timestamp_MCP_1) + ( timestamp_X2 - timestamp_MCP_1));
            // const Long64_t time_diff_int_X = ( ( timestamp_X1 ) - ( timestamp_X2 ));
            
            // const Double_t time_sum_X = time_sum_int_X*ch_to_ns;
            // const Double_t time_diff_X = time_diff_int_X*ch_to_ns;

            // const Long64_t time_sum_int_Y = ( ( timestamp_Y1 - timestamp_MCP_1) + ( timestamp_Y2 - timestamp_MCP_1));
            // const Long64_t time_diff_int_Y = ( ( timestamp_Y1 ) - ( timestamp_Y2 ));

            // const Double_t time_sum_Y = time_sum_int_Y*ch_to_ns;
            // const Double_t time_diff_Y = time_diff_int_Y*ch_to_ns;
            
            // const Double_t pos_diff_x = (time_diff_X)*ns_to_mm;
            // const Double_t pos_diff_y = (time_diff_Y)*ns_to_mm;

            if ((pos_diff_x * pos_diff_x + pos_diff_y * pos_diff_y) < (1.128 * 1.128))
            {
                ToF_X1X2_MCP->Fill(time_sum_X);
                ToF_Y1Y2_MCP->Fill(time_sum_Y);

                const Double_t pos_sum_x = (time_sum_X)*ns_to_mm;
                const Double_t pos_sum_y = (time_sum_Y)*ns_to_mm;

                ToF_ToF_X1X2_Y1Y2_sum->Fill(  pos_sum_x,  pos_sum_y );
                ToF_ToF_X1X2_Y1Y2_subs->Fill(  pos_diff_x,  pos_diff_y );
                ToF_ToF_X1minusX2_X1plusX2->Fill(  pos_diff_x, pos_sum_x );
                ToF_ToF_Y1minusY2_Y1plusY2->Fill(  pos_diff_y, pos_sum_y );
            }

            if ((pos_diff_x * pos_diff_x + pos_diff_y * pos_diff_y) < (1.128 * 1.128))
            {
                ToF_X1X2_MCP_ch21->Fill(time_sum_X);
                ToF_Y1Y2_MCP_ch21->Fill(time_sum_Y);

                const Double_t pos_sum_x = (time_sum_X)*ns_to_mm;
                const Double_t pos_sum_y = (time_sum_Y)*ns_to_mm;

                ToF_ToF_X1X2_Y1Y2_sum_ch21->Fill(  pos_sum_x,  pos_sum_y );
                ToF_ToF_X1X2_Y1Y2_subs_ch21->Fill(  pos_diff_x,  pos_diff_y );
                ToF_ToF_X1minusX2_X1plusX2_ch21->Fill(  pos_diff_x, pos_sum_x );
                ToF_ToF_Y1minusY2_Y1plusY2_ch21->Fill(  pos_diff_y, pos_sum_y );
            }

            double distance_squared = pos_diff_x * pos_diff_x + pos_diff_y * pos_diff_y;

            if ((pos_diff_x * pos_diff_x + pos_diff_y * pos_diff_y) < (5. * 5.))
            {
                ToF_MCPSi_ch21_5_mm_r->Fill( time_difference_SiDetector_ch21 );
                ToF_ToF_XY_ch21_5mm->Fill(  pos_x,  pos_y );

            }

            if (distance_squared < (22. * 22.) && distance_squared > (12. * 12.))
            {
                ToF_MCPSi_ch21_1_cm_ring->Fill( time_difference_SiDetector_ch21 );
                ToF_ToF_XY_ch21_1cm_ring->Fill(  pos_x,  pos_y );
            }

        }

        if ( ( found_X1 && found_X2 && found_Y1 && found_Y2 ) && ( found_MCP_1 ) ) {

            const Long64_t time_sum_int_X = ( ( timestamp_X1 - timestamp_MCP_1) + ( timestamp_X2 - timestamp_MCP_1));
            const Long64_t time_diff_int_X = ( ( timestamp_X1 ) - ( timestamp_X2 ));
            
            const Double_t time_sum_X = time_sum_int_X*ch_to_ns;
            const Double_t time_diff_X = time_diff_int_X*ch_to_ns;

            const Long64_t time_sum_int_Y = ( ( timestamp_Y1 - timestamp_MCP_1) + ( timestamp_Y2 - timestamp_MCP_1));
            const Long64_t time_diff_int_Y = ( ( timestamp_Y1 ) - ( timestamp_Y2 ));

            const Double_t time_sum_Y = time_sum_int_Y*ch_to_ns;
            const Double_t time_diff_Y = time_diff_int_Y*ch_to_ns;
            
            const Double_t pos_diff_x = (time_diff_X)*ns_to_mm;
            const Double_t pos_diff_y = (time_diff_Y)*ns_to_mm;

            if ((pos_diff_x * pos_diff_x + pos_diff_y * pos_diff_y) < (1.128 * 1.128))
            {
                ToF_X1X2_MCP->Fill(time_sum_X);
                ToF_Y1Y2_MCP->Fill(time_sum_Y);

                const Double_t pos_sum_x = (time_sum_X)*ns_to_mm;
                const Double_t pos_sum_y = (time_sum_Y)*ns_to_mm;

                ToF_ToF_X1X2_Y1Y2_sum->Fill(  pos_sum_x,  pos_sum_y );
                ToF_ToF_X1X2_Y1Y2_subs->Fill(  pos_diff_x,  pos_diff_y );
                ToF_ToF_X1minusX2_X1plusX2->Fill(  pos_diff_x, pos_sum_x );
                ToF_ToF_Y1minusY2_Y1plusY2->Fill(  pos_diff_y, pos_sum_y );
            }

        }      

    }


    TCanvas *ToF_positionsX = new TCanvas("ToF_positionsX", "ToF positions X1 and X2 in ns", 1550, 700, 600, 550);
    ToF_X1X2_MCP->Draw();

    TCanvas *ToF_positionsY = new TCanvas("ToF_positionsY", "ToF positions Y1 and Y2 in ns", 1550, 700, 600, 550);
    ToF_Y1Y2_MCP->Draw();

    TEllipse *e = new TEllipse(0,0, 20,20);
    e->SetLineColor(kCyan+3);
    e->SetLineWidth(3);
    e->SetFillStyle(0);
    e->Draw("same");

    TCanvas *ToF_positions_comparison = new TCanvas("ToF_positions_comparison", "ToF positions pos sen cuts comparison", 1550, 700, 600, 550);
    ToF_positions_comparison->DivideSquare(6);
    ToF_positions_comparison->cd(1);
    ToF_MCPSi_ch21->Draw();
    ToF_positions_comparison->cd(2);
    ToF_MCPSi_ch21_5_mm_r->Draw();
    ToF_positions_comparison->cd(3);
    ToF_MCPSi_ch21_1_cm_ring->Draw();

    ToF_positions_comparison->cd(4);
    ToF_ToF_XY_ch21->Draw("colz");
    e->Draw("same");
    ToF_positions_comparison->cd(5);
    ToF_ToF_XY_ch21_5mm->Draw("colz");
    e->Draw("same");
    ToF_positions_comparison->cd(6);
    ToF_ToF_XY_ch21_1cm_ring->Draw("colz");
    e->Draw("same");

    TCanvas *Plots_Ottmar = new TCanvas("Plots_Ottmar", "ToF X1 + X2 vs ToF Y1 + Y2", 1550, 700, 600, 550);
    Plots_Ottmar->DivideSquare(4);
    Plots_Ottmar->cd(1);
    ToF_ToF_X1X2_Y1Y2_sum->GetXaxis()->SetTitle("Position X [mm]");
    ToF_ToF_X1X2_Y1Y2_sum->GetYaxis()->SetTitle("Position Y [mm]");
    gStyle->SetPalette(109);
    // Set the range for the X and Y axes
    ToF_ToF_X1X2_Y1Y2_sum->GetXaxis()->SetRangeUser(-20, 20); // Set X-axis range from 2 to 8
    ToF_ToF_X1X2_Y1Y2_sum->GetYaxis()->SetRangeUser(-20, 20); // Set Y-axis range from 3 to 9
    ToF_ToF_X1X2_Y1Y2_sum->Draw("colz");
    e->SetLineColor(kCyan+3);
    e->SetLineWidth(3);
    e->SetFillStyle(0);
    e->Draw("same");

    // TCanvas *ToF_ToF_posX_sum_and_diff = new TCanvas("ToF_ToF_posX_sum_and_diff", "ToF X1 - X2 vs ToF X1 + X2", 1550, 700, 600, 550);
    Plots_Ottmar->cd(2);
    ToF_ToF_X1minusX2_X1plusX2->GetXaxis()->SetTitle("Position X1-X2 [mm]");
    ToF_ToF_X1minusX2_X1plusX2->GetYaxis()->SetTitle("Position X1+X2 [mm]");
    gStyle->SetPalette(109);
    // Set the range for the X and Y axes
    ToF_ToF_X1minusX2_X1plusX2->GetXaxis()->SetRangeUser(-20, 20); // Set X-axis range from 2 to 8
    ToF_ToF_X1minusX2_X1plusX2->GetYaxis()->SetRangeUser(-20, 20); // Set Y-axis range from 3 to 9
    ToF_ToF_X1minusX2_X1plusX2->Draw("colz");
    e->SetLineColor(kCyan+3);
    e->SetLineWidth(3);
    e->SetFillStyle(0);
    e->Draw("same");

    // TCanvas *ToF_ToF_posY_sum_and_diff = new TCanvas("ToF_ToF_posY_sum_and_diff", "ToF Y1 - Y2 vs ToF Y1 + Y2", 1550, 700, 600, 550);
    Plots_Ottmar->cd(3);
    ToF_ToF_Y1minusY2_Y1plusY2->GetXaxis()->SetTitle("Position Y1-Y2 [mm]");
    ToF_ToF_Y1minusY2_Y1plusY2->GetYaxis()->SetTitle("Position Y1+Y2 [mm]");
    gStyle->SetPalette(109);
    // Set the range for the Y and Y axes
    ToF_ToF_Y1minusY2_Y1plusY2->GetYaxis()->SetRangeUser(-20, 20); // Set Y-axis range from 2 to 8
    ToF_ToF_Y1minusY2_Y1plusY2->GetYaxis()->SetRangeUser(-20, 20); // Set Y-axis range from 3 to 9
    ToF_ToF_Y1minusY2_Y1plusY2->Draw("colz");
    e->SetLineColor(kCyan+3);
    e->SetLineWidth(3);
    e->SetFillStyle(0);
    e->Draw("same");

    Plots_Ottmar->cd(4);
    ToF_ToF_X1X2_Y1Y2_subs->GetXaxis()->SetTitle("Position X [mm]");
    ToF_ToF_X1X2_Y1Y2_subs->GetYaxis()->SetTitle("Position Y [mm]");
    gStyle->SetPalette(109);
    // Set the range for the X and Y axes
    ToF_ToF_X1X2_Y1Y2_subs->GetXaxis()->SetRangeUser(-20, 20); // Set X-axis range from 2 to 8
    ToF_ToF_X1X2_Y1Y2_subs->GetYaxis()->SetRangeUser(-20, 20); // Set Y-axis range from 3 to 9
    ToF_ToF_X1X2_Y1Y2_subs->Draw("colz");
    e->SetLineColor(kCyan+3);
    e->SetLineWidth(3);
    e->SetFillStyle(0);
    e->Draw("same");   

    TCanvas *Mosaic_pos_plots = new TCanvas("Mosaic_pos_plots", "Mosaic_pos_plots", 1550, 700, 600, 550);
    Mosaic_pos_plots->DivideSquare(22);
    for (int i = 1; i <= 22; ++i) {
        Mosaic_pos_plots->cd(i);
        TH2F *hist = nullptr;
        if (i == 1) hist = ToF_ToF_XY_ch0;
        else if (i == 2) hist = ToF_ToF_XY_ch1;
        else if (i == 3) hist = ToF_ToF_XY_ch2;
        else if (i == 4) hist = ToF_ToF_XY_ch3;
        else if (i == 5) hist = ToF_ToF_XY_ch4;
        else if (i == 6) hist = ToF_ToF_XY_ch5;
        else if (i == 7) hist = ToF_ToF_XY_ch6;
        else if (i == 8) hist = ToF_ToF_XY_ch7;
        else if (i == 9) hist = ToF_ToF_XY_ch8;
        else if (i == 10) hist = ToF_ToF_XY_ch9;
        else if (i == 11) hist = ToF_ToF_XY_ch10;
        else if (i == 12) hist = ToF_ToF_XY_ch11;
        else if (i == 13) hist = ToF_ToF_XY_ch12;
        else if (i == 14) hist = ToF_ToF_XY_ch13;
        else if (i == 15) hist = ToF_ToF_XY_ch14;
        else if (i == 16) hist = ToF_ToF_XY_ch15;
        else if (i == 17) hist = ToF_ToF_XY_ch16;
        else if (i == 18) hist = ToF_ToF_XY_ch17;
        else if (i == 19) hist = ToF_ToF_XY_ch18;
        else if (i == 20) hist = ToF_ToF_XY_ch19;
        else if (i == 21) hist = ToF_ToF_XY_ch20;
        else if (i == 22) hist = ToF_ToF_XY_ch21;
        hist->Draw("colz");
        hist->GetXaxis()->SetTitle("Position X [mm]");
        hist->GetYaxis()->SetTitle("Position Y [mm]");
        e->Draw("same");

        // Add vertical line along the y-axis
        TLine *vertLine = new TLine(0, -20, 0, 20);
        vertLine->SetLineColor(kRed);
        vertLine->Draw("same");

        // Add horizontal line along the x-axis
        TLine *horizLine = new TLine(hist->GetXaxis()->GetXmin(), 0, hist->GetXaxis()->GetXmax(), 0);
        horizLine->SetLineColor(kBlue);
        horizLine->Draw("same");
    }

    TCanvas *Mosaic_PH_vs_TOF_plots = new TCanvas("Mosaic_PH_vs_TOF_plots", "Mosaic_PH_vs_TOF_plots", 1550, 700, 600, 550);
    Mosaic_PH_vs_TOF_plots->DivideSquare(22);
    for (int i = 1; i <= 22; ++i) {
        Mosaic_PH_vs_TOF_plots->cd(i);
        TH2F *hist = nullptr;
        if (i == 1) hist = PH_vs_TOF_ch0;
        else if (i == 2) hist = PH_vs_TOF_ch1;
        else if (i == 3) hist = PH_vs_TOF_ch2;
        else if (i == 4) hist = PH_vs_TOF_ch3;
        else if (i == 5) hist = PH_vs_TOF_ch4;
        else if (i == 6) hist = PH_vs_TOF_ch5;
        else if (i == 7) hist = PH_vs_TOF_ch6;
        else if (i == 8) hist = PH_vs_TOF_ch7;
        else if (i == 9) hist = PH_vs_TOF_ch8;
        else if (i == 10) hist = PH_vs_TOF_ch9;
        else if (i == 11) hist = PH_vs_TOF_ch10;
        else if (i == 12) hist = PH_vs_TOF_ch11;
        else if (i == 13) hist = PH_vs_TOF_ch12;
        else if (i == 14) hist = PH_vs_TOF_ch13;
        else if (i == 15) hist = PH_vs_TOF_ch14;
        else if (i == 16) hist = PH_vs_TOF_ch15;
        else if (i == 17) hist = PH_vs_TOF_ch16;
        else if (i == 18) hist = PH_vs_TOF_ch17;
        else if (i == 19) hist = PH_vs_TOF_ch18;
        else if (i == 20) hist = PH_vs_TOF_ch19;
        else if (i == 21) hist = PH_vs_TOF_ch20;
        else if (i == 22) hist = PH_vs_TOF_ch21;
        hist->GetXaxis()->SetTitle("TOF [ns]");
        hist->GetYaxis()->SetTitle("PH [ch]");
        hist->Draw("colz");
    }    

    TCanvas *Mosaic_PH_vs_TOF_plots_individual = new TCanvas("Mosaic_PH_vs_TOF_plots_individual", "Mosaic_PH_vs_TOF_plots_individual", 1550, 700, 600, 550);
    PH_vs_TOF_ch21->Draw("colz");

    TCanvas *Si_spectrum_plots = new TCanvas("Si_spectrum_plots", "Si_spectrum_plots", 1550, 700, 600, 550);
    Si_spectrum_plots->DivideSquare(22);
    for (int i = 1; i <= 22; ++i) {
        Si_spectrum_plots->cd(i);
        TH1F *hist = nullptr;
        if (i == 1) hist = Si_spectrum_ch0;
        else if (i == 2) hist = Si_spectrum_ch1;
        else if (i == 3) hist = Si_spectrum_ch2;
        else if (i == 4) hist = Si_spectrum_ch3;
        else if (i == 5) hist = Si_spectrum_ch4;
        else if (i == 6) hist = Si_spectrum_ch5;
        else if (i == 7) hist = Si_spectrum_ch6;
        else if (i == 8) hist = Si_spectrum_ch7;
        else if (i == 9) hist = Si_spectrum_ch8;
        else if (i == 10) hist = Si_spectrum_ch9;
        else if (i == 11) hist = Si_spectrum_ch10;
        else if (i == 12) hist = Si_spectrum_ch11;
        else if (i == 13) hist = Si_spectrum_ch12;
        else if (i == 14) hist = Si_spectrum_ch13;
        else if (i == 15) hist = Si_spectrum_ch14;
        else if (i == 16) hist = Si_spectrum_ch15;
        else if (i == 17) hist = Si_spectrum_ch16;
        else if (i == 18) hist = Si_spectrum_ch17;
        else if (i == 19) hist = Si_spectrum_ch18;
        else if (i == 20) hist = Si_spectrum_ch19;
        else if (i == 21) hist = Si_spectrum_ch20;
        else if (i == 22) hist = Si_spectrum_ch21;
        hist->GetXaxis()->SetTitle("PH [ch]");
        hist->Draw("colz");
        gPad->SetLogy();
    }

    TCanvas *TOF_plots = new TCanvas("TOF_plots", "TOF_plots", 1550, 700, 600, 550);
    TOF_plots->DivideSquare(22);
    for (int i = 1; i <= 22; ++i) {
        TOF_plots->cd(i);
        gPad->SetLogy(); 
        TH1F *hist = nullptr;
        if (i == 1) hist = ToF_MCPSi_ch0;
        else if (i == 2) hist = ToF_MCPSi_ch1;
        else if (i == 3) hist = ToF_MCPSi_ch2;
        else if (i == 4) hist = ToF_MCPSi_ch3;
        else if (i == 5) hist = ToF_MCPSi_ch4;
        else if (i == 6) hist = ToF_MCPSi_ch5;
        else if (i == 7) hist = ToF_MCPSi_ch6;
        else if (i == 8) hist = ToF_MCPSi_ch7;
        else if (i == 9) hist = ToF_MCPSi_ch8;
        else if (i == 10) hist = ToF_MCPSi_ch9;
        else if (i == 11) hist = ToF_MCPSi_ch10;
        else if (i == 12) hist = ToF_MCPSi_ch11;
        else if (i == 13) hist = ToF_MCPSi_ch12;
        else if (i == 14) hist = ToF_MCPSi_ch13;
        else if (i == 15) hist = ToF_MCPSi_ch14;
        else if (i == 16) hist = ToF_MCPSi_ch15;
        else if (i == 17) hist = ToF_MCPSi_ch16;
        else if (i == 18) hist = ToF_MCPSi_ch17;
        else if (i == 19) hist = ToF_MCPSi_ch18;
        else if (i == 20) hist = ToF_MCPSi_ch19;
        else if (i == 21) hist = ToF_MCPSi_ch20;
        else if (i == 22) hist = ToF_MCPSi_ch21;
        hist->GetXaxis()->SetTitle("TOF [ns]");
        hist->Draw("colz");
    }

    TCanvas *ToF_X1X2_MCP_plots = new TCanvas("ToF_X1X2_MCP_plots", "ToF_X1X2_MCP_plots", 1550, 700, 600, 550);
    ToF_X1X2_MCP_plots->DivideSquare(22);
    for (int i = 1; i <= 22; ++i) {
        ToF_X1X2_MCP_plots->cd(i);
        TH1F *hist = nullptr;
        if (i == 1) hist = ToF_X1X2_MCP_ch0;
        else if (i == 2) hist = ToF_X1X2_MCP_ch1;
        else if (i == 3) hist = ToF_X1X2_MCP_ch2;
        else if (i == 4) hist = ToF_X1X2_MCP_ch3;
        else if (i == 5) hist = ToF_X1X2_MCP_ch4;
        else if (i == 6) hist = ToF_X1X2_MCP_ch5;
        else if (i == 7) hist = ToF_X1X2_MCP_ch6;
        else if (i == 8) hist = ToF_X1X2_MCP_ch7;
        else if (i == 9) hist = ToF_X1X2_MCP_ch8;
        else if (i == 10) hist = ToF_X1X2_MCP_ch9;
        else if (i == 11) hist = ToF_X1X2_MCP_ch10;
        else if (i == 12) hist = ToF_X1X2_MCP_ch11;
        else if (i == 13) hist = ToF_X1X2_MCP_ch12;
        else if (i == 14) hist = ToF_X1X2_MCP_ch13;
        else if (i == 15) hist = ToF_X1X2_MCP_ch14;
        else if (i == 16) hist = ToF_X1X2_MCP_ch15;
        else if (i == 17) hist = ToF_X1X2_MCP_ch16;
        else if (i == 18) hist = ToF_X1X2_MCP_ch17;
        else if (i == 19) hist = ToF_X1X2_MCP_ch18;
        else if (i == 20) hist = ToF_X1X2_MCP_ch19;
        else if (i == 21) hist = ToF_X1X2_MCP_ch20;
        else if (i == 22) hist = ToF_X1X2_MCP_ch21;
        hist->GetXaxis()->SetTitle("Position X [mm]");
        hist->Draw("colz");
    }

    TCanvas *ToF_Y1Y2_MCP_plots = new TCanvas("ToF_Y1Y2_MCP_plots", "ToF_Y1Y2_MCP_plots", 1550, 700, 600, 550);
    ToF_Y1Y2_MCP_plots->DivideSquare(22);
    for (int i = 1; i <= 22; ++i) {
        ToF_Y1Y2_MCP_plots->cd(i);
        TH1F *hist = nullptr;
        if (i == 1) hist = ToF_Y1Y2_MCP_ch0;
        else if (i == 2) hist = ToF_Y1Y2_MCP_ch1;
        else if (i == 3) hist = ToF_Y1Y2_MCP_ch2;
        else if (i == 4) hist = ToF_Y1Y2_MCP_ch3;
        else if (i == 5) hist = ToF_Y1Y2_MCP_ch4;
        else if (i == 6) hist = ToF_Y1Y2_MCP_ch5;
        else if (i == 7) hist = ToF_Y1Y2_MCP_ch6;
        else if (i == 8) hist = ToF_Y1Y2_MCP_ch7;
        else if (i == 9) hist = ToF_Y1Y2_MCP_ch8;
        else if (i == 10) hist = ToF_Y1Y2_MCP_ch9;
        else if (i == 11) hist = ToF_Y1Y2_MCP_ch10;
        else if (i == 12) hist = ToF_Y1Y2_MCP_ch11;
        else if (i == 13) hist = ToF_Y1Y2_MCP_ch12;
        else if (i == 14) hist = ToF_Y1Y2_MCP_ch13;
        else if (i == 15) hist = ToF_Y1Y2_MCP_ch14;
        else if (i == 16) hist = ToF_Y1Y2_MCP_ch15;
        else if (i == 17) hist = ToF_Y1Y2_MCP_ch16;
        else if (i == 18) hist = ToF_Y1Y2_MCP_ch17;
        else if (i == 19) hist = ToF_Y1Y2_MCP_ch18;
        else if (i == 20) hist = ToF_Y1Y2_MCP_ch19;
        else if (i == 21) hist = ToF_Y1Y2_MCP_ch20;
        else if (i == 22) hist = ToF_Y1Y2_MCP_ch21;
        hist->GetXaxis()->SetTitle("Position X [mm]");
        hist->Draw("colz");
    }

    TCanvas *ToF_X1X2_plots = new TCanvas("ToF_X1X2_plots", "ToF_X1X2_plots", 1550, 700, 600, 550);
    ToF_X1X2_plots->DivideSquare(22);
    for (int i = 1; i <= 22; ++i) {
        ToF_X1X2_plots->cd(i);
        TH1F *hist = nullptr;
        if (i == 1) hist = ToF_X1X2_ch0;
        else if (i == 2) hist = ToF_X1X2_ch1;
        else if (i == 3) hist = ToF_X1X2_ch2;
        else if (i == 4) hist = ToF_X1X2_ch3;
        else if (i == 5) hist = ToF_X1X2_ch4;
        else if (i == 6) hist = ToF_X1X2_ch5;
        else if (i == 7) hist = ToF_X1X2_ch6;
        else if (i == 8) hist = ToF_X1X2_ch7;
        else if (i == 9) hist = ToF_X1X2_ch8;
        else if (i == 10) hist = ToF_X1X2_ch9;
        else if (i == 11) hist = ToF_X1X2_ch10;
        else if (i == 12) hist = ToF_X1X2_ch11;
        else if (i == 13) hist = ToF_X1X2_ch12;
        else if (i == 14) hist = ToF_X1X2_ch13;
        else if (i == 15) hist = ToF_X1X2_ch14;
        else if (i == 16) hist = ToF_X1X2_ch15;
        else if (i == 17) hist = ToF_X1X2_ch16;
        else if (i == 18) hist = ToF_X1X2_ch17;
        else if (i == 19) hist = ToF_X1X2_ch18;
        else if (i == 20) hist = ToF_X1X2_ch19;
        else if (i == 21) hist = ToF_X1X2_ch20;
        else if (i == 22) hist = ToF_X1X2_ch21;
        hist->GetXaxis()->SetTitle("Position X [mm]");
        hist->Draw("colz");
    }

    TCanvas *ToF_Y1Y2_plots = new TCanvas("ToF_Y1Y2_plots", "ToF_Y1Y2_plots", 1550, 700, 600, 550);
    ToF_Y1Y2_plots->DivideSquare(22);
    for (int i = 1; i <= 22; ++i) {
        ToF_Y1Y2_plots->cd(i);
        TH1F *hist = nullptr;
        if (i == 1) hist = ToF_Y1Y2_ch0;
        else if (i == 2) hist = ToF_Y1Y2_ch1;
        else if (i == 3) hist = ToF_Y1Y2_ch2;
        else if (i == 4) hist = ToF_Y1Y2_ch3;
        else if (i == 5) hist = ToF_Y1Y2_ch4;
        else if (i == 6) hist = ToF_Y1Y2_ch5;
        else if (i == 7) hist = ToF_Y1Y2_ch6;
        else if (i == 8) hist = ToF_Y1Y2_ch7;
        else if (i == 9) hist = ToF_Y1Y2_ch8;
        else if (i == 10) hist = ToF_Y1Y2_ch9;
        else if (i == 11) hist = ToF_Y1Y2_ch10;
        else if (i == 12) hist = ToF_Y1Y2_ch11;
        else if (i == 13) hist = ToF_Y1Y2_ch12;
        else if (i == 14) hist = ToF_Y1Y2_ch13;
        else if (i == 15) hist = ToF_Y1Y2_ch14;
        else if (i == 16) hist = ToF_Y1Y2_ch15;
        else if (i == 17) hist = ToF_Y1Y2_ch16;
        else if (i == 18) hist = ToF_Y1Y2_ch17;
        else if (i == 19) hist = ToF_Y1Y2_ch18;
        else if (i == 20) hist = ToF_Y1Y2_ch19;
        else if (i == 21) hist = ToF_Y1Y2_ch20;
        else if (i == 22) hist = ToF_Y1Y2_ch21;
        hist->GetXaxis()->SetTitle("Position X [mm]");
        hist->Draw("colz");
    }

    TCanvas *ToF_ToF_X1X2_Y1Y2_sum_plots = new TCanvas("ToF_ToF_X1X2_Y1Y2_sum_plots", "ToF_ToF_X1X2_Y1Y2_sum_plots", 1550, 700, 600, 550);
    ToF_ToF_X1X2_Y1Y2_sum_plots->DivideSquare(22);
    for (int i = 1; i <= 22; ++i) {
        ToF_ToF_X1X2_Y1Y2_sum_plots->cd(i);
        TH2F *hist = nullptr;
        if (i == 1) hist = ToF_ToF_X1X2_Y1Y2_sum_ch0;
        else if (i == 2) hist = ToF_ToF_X1X2_Y1Y2_sum_ch1;
        else if (i == 3) hist = ToF_ToF_X1X2_Y1Y2_sum_ch2;
        else if (i == 4) hist = ToF_ToF_X1X2_Y1Y2_sum_ch3;
        else if (i == 5) hist = ToF_ToF_X1X2_Y1Y2_sum_ch4;
        else if (i == 6) hist = ToF_ToF_X1X2_Y1Y2_sum_ch5;
        else if (i == 7) hist = ToF_ToF_X1X2_Y1Y2_sum_ch6;
        else if (i == 8) hist = ToF_ToF_X1X2_Y1Y2_sum_ch7;
        else if (i == 9) hist = ToF_ToF_X1X2_Y1Y2_sum_ch8;
        else if (i == 10) hist = ToF_ToF_X1X2_Y1Y2_sum_ch9;
        else if (i == 11) hist = ToF_ToF_X1X2_Y1Y2_sum_ch10;
        else if (i == 12) hist = ToF_ToF_X1X2_Y1Y2_sum_ch11;
        else if (i == 13) hist = ToF_ToF_X1X2_Y1Y2_sum_ch12;
        else if (i == 14) hist = ToF_ToF_X1X2_Y1Y2_sum_ch13;
        else if (i == 15) hist = ToF_ToF_X1X2_Y1Y2_sum_ch14;
        else if (i == 16) hist = ToF_ToF_X1X2_Y1Y2_sum_ch15;
        else if (i == 17) hist = ToF_ToF_X1X2_Y1Y2_sum_ch16;
        else if (i == 18) hist = ToF_ToF_X1X2_Y1Y2_sum_ch17;
        else if (i == 19) hist = ToF_ToF_X1X2_Y1Y2_sum_ch18;
        else if (i == 20) hist = ToF_ToF_X1X2_Y1Y2_sum_ch19;
        else if (i == 21) hist = ToF_ToF_X1X2_Y1Y2_sum_ch20;
        else if (i == 22) hist = ToF_ToF_X1X2_Y1Y2_sum_ch21;
        hist->GetXaxis()->SetTitle("Position X [mm]");
        hist->GetYaxis()->SetTitle("Position Y [mm]");
        hist->Draw("colz");
    }


    TCanvas *ToF_ToF_X1X2_Y1Y2_subs_plots = new TCanvas("ToF_ToF_X1X2_Y1Y2_subs_plots", "ToF_ToF_X1X2_Y1Y2_subs_plots", 1550, 700, 600, 550);
    ToF_ToF_X1X2_Y1Y2_subs_plots->DivideSquare(22);
    for (int i = 1; i <= 22; ++i) {
        ToF_ToF_X1X2_Y1Y2_subs_plots->cd(i);
        TH2F *hist = nullptr;
        if (i == 1) hist = ToF_ToF_X1X2_Y1Y2_subs_ch0;
        else if (i == 2) hist = ToF_ToF_X1X2_Y1Y2_subs_ch1;
        else if (i == 3) hist = ToF_ToF_X1X2_Y1Y2_subs_ch2;
        else if (i == 4) hist = ToF_ToF_X1X2_Y1Y2_subs_ch3;
        else if (i == 5) hist = ToF_ToF_X1X2_Y1Y2_subs_ch4;
        else if (i == 6) hist = ToF_ToF_X1X2_Y1Y2_subs_ch5;
        else if (i == 7) hist = ToF_ToF_X1X2_Y1Y2_subs_ch6;
        else if (i == 8) hist = ToF_ToF_X1X2_Y1Y2_subs_ch7;
        else if (i == 9) hist = ToF_ToF_X1X2_Y1Y2_subs_ch8;
        else if (i == 10) hist = ToF_ToF_X1X2_Y1Y2_subs_ch9;
        else if (i == 11) hist = ToF_ToF_X1X2_Y1Y2_subs_ch10;
        else if (i == 12) hist = ToF_ToF_X1X2_Y1Y2_subs_ch11;
        else if (i == 13) hist = ToF_ToF_X1X2_Y1Y2_subs_ch12;
        else if (i == 14) hist = ToF_ToF_X1X2_Y1Y2_subs_ch13;
        else if (i == 15) hist = ToF_ToF_X1X2_Y1Y2_subs_ch14;
        else if (i == 16) hist = ToF_ToF_X1X2_Y1Y2_subs_ch15;
        else if (i == 17) hist = ToF_ToF_X1X2_Y1Y2_subs_ch16;
        else if (i == 18) hist = ToF_ToF_X1X2_Y1Y2_subs_ch17;
        else if (i == 19) hist = ToF_ToF_X1X2_Y1Y2_subs_ch18;
        else if (i == 20) hist = ToF_ToF_X1X2_Y1Y2_subs_ch19;
        else if (i == 21) hist = ToF_ToF_X1X2_Y1Y2_subs_ch20;
        else if (i == 22) hist = ToF_ToF_X1X2_Y1Y2_subs_ch21;
        hist->GetXaxis()->SetTitle("Position X [mm]");
        hist->GetYaxis()->SetTitle("Position Y [mm]");
        hist->Draw("colz");
    }

    TCanvas *ToF_ToF_X1minusX2_X1plusX2_plots = new TCanvas("ToF_ToF_X1minusX2_X1plusX2_plots", "ToF_ToF_X1minusX2_X1plusX2_plots", 1550, 700, 600, 550);
    ToF_ToF_X1minusX2_X1plusX2_plots->DivideSquare(22);
    for (int i = 1; i <= 22; ++i) {
        ToF_ToF_X1minusX2_X1plusX2_plots->cd(i);
        TH2F *hist = nullptr;
        if (i == 1) hist = ToF_ToF_X1minusX2_X1plusX2_ch0;
        else if (i == 2) hist = ToF_ToF_X1minusX2_X1plusX2_ch1;
        else if (i == 3) hist = ToF_ToF_X1minusX2_X1plusX2_ch2;
        else if (i == 4) hist = ToF_ToF_X1minusX2_X1plusX2_ch3;
        else if (i == 5) hist = ToF_ToF_X1minusX2_X1plusX2_ch4;
        else if (i == 6) hist = ToF_ToF_X1minusX2_X1plusX2_ch5;
        else if (i == 7) hist = ToF_ToF_X1minusX2_X1plusX2_ch6;
        else if (i == 8) hist = ToF_ToF_X1minusX2_X1plusX2_ch7;
        else if (i == 9) hist = ToF_ToF_X1minusX2_X1plusX2_ch8;
        else if (i == 10) hist = ToF_ToF_X1minusX2_X1plusX2_ch9;
        else if (i == 11) hist = ToF_ToF_X1minusX2_X1plusX2_ch10;
        else if (i == 12) hist = ToF_ToF_X1minusX2_X1plusX2_ch11;
        else if (i == 13) hist = ToF_ToF_X1minusX2_X1plusX2_ch12;
        else if (i == 14) hist = ToF_ToF_X1minusX2_X1plusX2_ch13;
        else if (i == 15) hist = ToF_ToF_X1minusX2_X1plusX2_ch14;
        else if (i == 16) hist = ToF_ToF_X1minusX2_X1plusX2_ch15;
        else if (i == 17) hist = ToF_ToF_X1minusX2_X1plusX2_ch16;
        else if (i == 18) hist = ToF_ToF_X1minusX2_X1plusX2_ch17;
        else if (i == 19) hist = ToF_ToF_X1minusX2_X1plusX2_ch18;
        else if (i == 20) hist = ToF_ToF_X1minusX2_X1plusX2_ch19;
        else if (i == 21) hist = ToF_ToF_X1minusX2_X1plusX2_ch20;
        else if (i == 22) hist = ToF_ToF_X1minusX2_X1plusX2_ch21;
        hist->GetXaxis()->SetTitle("Position X [mm]");
        hist->GetYaxis()->SetTitle("Position Y [mm]");
        hist->Draw("colz");
    }

    TCanvas *ToF_ToF_Y1minusY2_Y1plusY2_plots = new TCanvas("ToF_ToF_Y1minusY2_Y1plusY2_plots", "ToF_ToF_Y1minusY2_Y1plusY2_plots", 1550, 700, 600, 550);
    ToF_ToF_Y1minusY2_Y1plusY2_plots->DivideSquare(22);
    for (int i = 1; i <= 22; ++i) {
        ToF_ToF_Y1minusY2_Y1plusY2_plots->cd(i);
        TH2F *hist = nullptr;
        if (i == 1) hist = ToF_ToF_Y1minusY2_Y1plusY2_ch0;
        else if (i == 2) hist = ToF_ToF_Y1minusY2_Y1plusY2_ch1;
        else if (i == 3) hist = ToF_ToF_Y1minusY2_Y1plusY2_ch2;
        else if (i == 4) hist = ToF_ToF_Y1minusY2_Y1plusY2_ch3;
        else if (i == 5) hist = ToF_ToF_Y1minusY2_Y1plusY2_ch4;
        else if (i == 6) hist = ToF_ToF_Y1minusY2_Y1plusY2_ch5;
        else if (i == 7) hist = ToF_ToF_Y1minusY2_Y1plusY2_ch6;
        else if (i == 8) hist = ToF_ToF_Y1minusY2_Y1plusY2_ch7;
        else if (i == 9) hist = ToF_ToF_Y1minusY2_Y1plusY2_ch8;
        else if (i == 10) hist = ToF_ToF_Y1minusY2_Y1plusY2_ch9;
        else if (i == 11) hist = ToF_ToF_Y1minusY2_Y1plusY2_ch10;
        else if (i == 12) hist = ToF_ToF_Y1minusY2_Y1plusY2_ch11;
        else if (i == 13) hist = ToF_ToF_Y1minusY2_Y1plusY2_ch12;
        else if (i == 14) hist = ToF_ToF_Y1minusY2_Y1plusY2_ch13;
        else if (i == 15) hist = ToF_ToF_Y1minusY2_Y1plusY2_ch14;
        else if (i == 16) hist = ToF_ToF_Y1minusY2_Y1plusY2_ch15;
        else if (i == 17) hist = ToF_ToF_Y1minusY2_Y1plusY2_ch16;
        else if (i == 18) hist = ToF_ToF_Y1minusY2_Y1plusY2_ch17;
        else if (i == 19) hist = ToF_ToF_Y1minusY2_Y1plusY2_ch18;
        else if (i == 20) hist = ToF_ToF_Y1minusY2_Y1plusY2_ch19;
        else if (i == 21) hist = ToF_ToF_Y1minusY2_Y1plusY2_ch20;
        else if (i == 22) hist = ToF_ToF_Y1minusY2_Y1plusY2_ch21;
        hist->GetXaxis()->SetTitle("Position X [mm]");
        hist->GetYaxis()->SetTitle("Position Y [mm]");
        hist->Draw("colz");
    }


}
