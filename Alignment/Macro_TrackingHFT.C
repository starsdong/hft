

#include <TSystem>

class StMaker;
class StChain;

void Macro_TrackingHFT()
{
    cout << "Macro_TrackingHFT started" << endl;
    gSystem ->Load("./.sl64_gcc447/lib/StTrackingHFT.so");
    gSystem ->AddIncludePath("/u/aschmah/local/include/");
    cout << "Shared libraries loaded" << endl;

    StTrackingHFT *HFT_Ana = new StTrackingHFT();

    HFT_Ana->SetPlotMode(0);
    HFT_Ana->SetAnaMode(0);
    HFT_Ana->SetFileNumber(0);
    HFT_Ana->SetStartStopEvent(0,50);
    HFT_Ana->Init();
    HFT_Ana->Make();
    HFT_Ana->Finish();
}