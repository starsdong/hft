

#include <TSystem>

class StMaker;
class StChain;

void Macro_StReadHFTtrees()
{
    cout << "Macro_StReadHFTtrees started" << endl;
    gSystem ->Load("./.sl64_gcc447/lib/StReadHFTtrees.so");
    gSystem ->AddIncludePath("/u/aschmah/local/include/");
    cout << "Shared libraries loaded" << endl;

    StReadHFTtrees *HFT_Ana = new StReadHFTtrees();

    HFT_Ana->SetPlotMode(0);
    HFT_Ana->SetAnaMode(0); // use 1 for filling the correction histograms, needs two loops, 2 for writing ASCII tables for Howard
    HFT_Ana->SetStartStopEvent(0,1000000);

    HFT_Ana->SetInputDir("/star/institutions/lbl/aschmah/HFT/Cosmics/Corrections/Full_Align/MagnetOn/");
    HFT_Ana->SetInputFile("Event_15088054_10212014_TPC_hits");
    HFT_Ana->SetOutputDir("/star/institutions/lbl/aschmah/HFT/Cosmics/Corrections/");
    HFT_Ana->SetMagnetOnOff(1); // 0 = off, 1 = on
    HFT_Ana->SetSaveTree(1); // 0 = all trees/histograms, 1 = only HFT/IST/TPC tree

    HFT_Ana->Init();
    HFT_Ana->Make();
    HFT_Ana->Finish();
}