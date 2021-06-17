#ifndef StTrackingHFT_hh
#define StTrackingHFT_hh

#include "TString.h"
#include "TH2F.h"
#include "TCutG.h"
#include "TCanvas.h"

#include "TTree.h"
#include "StMessMgr.h"
#include "TChain.h"
#include "TNtuple.h"
#include <TROOT.h>
#include "TFile.h"
#include "TPolyMarker3D.h"
#include "TPolyLine3D.h"
#include "TMarker3DBox.h"
#include "TRandom.h"
#include <TFile.h>
#include <TObject.h>


#include "TH3D.h"
#include "TCanvas.h"
#include "TPolyMarker.h"
#include "TVirtualPad.h"
#include "TPolyLine.h"
#include "TVector3.h"
#include "TPolyMarker3D.h"
#include "TPolyLine3D.h"
#include "Math/MinimizerOptions.h"
#include "TGLViewer.h"
#include "TGLSAViewer.h"
#include "TGLCamera.h"
#include "TGLPerspectiveCamera.h"
#include "TGFrame.h"
#include "TGLUtil.h"
#include "TGLLightSet.h"
#include "TGLCameraOverlay.h"
#include "TLorentzVector.h"
#include "TGeoManager.h"
#include "TGeoMaterial.h"
#include "TGeoMedium.h"
#include "TGeoVolume.h"
#include "TGeoMatrix.h"
#include "TRandom.h"
#include "TRandom3.h"
#include "TGraph.h"
#include <vector>
#include "TF1.h"
#include "TColor.h"

#include "StarClassLibrary/SystemOfUnits.h"

#include "TVector3.h"
#include "StarClassLibrary/StThreeVectorF.hh"
#include "StarClassLibrary/StThreeVectorD.hh"
#include "TLorentzVector.h"
#include "StPhysicalHelixD.hh"
#include "THelix.h"


class StTrackingHFT {
public:
    StTrackingHFT();
    virtual ~StTrackingHFT();

    void  Init();
    void  Make();
    void  Finish();
    void  SetOutFileName(TString outfileName) {outFileName = outfileName; cout << "Output filename = " << outFileName << endl;}
    void  SetInputDir(TString inDir) {inputDir = inDir; cout << "Input directory = " << inDir << endl;}
    void  SetOutputDir(TString ouDir) {outputDir = ouDir; cout << "Output directory = " << ouDir << endl;}
    void  SetInputFile(TString infile) {inFile = infile; cout << "Input file = " << inFile << endl;}
    void  SetInDirList(TString indir_asciifile) {inDir_asciifile = indir_asciifile; cout << "inDir_asciifile = " << inDir_asciifile << endl;}
    void  SetInList(TString asciiparfile) {asciiParFile = asciiparfile; cout << "asciiParFile = " << asciiParFile << endl;}
    void  SetNEvents(Int_t nevents) {nEvents = nevents; cout << "Number of analyzed events set to " << nEvents << endl;}
    void  SetStartEvent(Int_t nfirst_event) {first_event = nfirst_event; cout << "First event = " << first_event << endl;}
    void  SetPlotMode(Int_t nplotmode) {plotmode = nplotmode; cout << "plotmode set to " << nplotmode << endl;}
    void  SetAnaMode(Int_t nanamode) {anamode = nanamode; cout << "anamode set to " << nanamode << endl;}
    void  SetFileNumber(Int_t nfilenumber) {filenumber = nfilenumber;};
    void  SetStartStopEvent(Long64_t nstartevent, Long64_t nstopevent) {startevent = nstartevent; stopevent = nstopevent;};

private:
    TString   outFileName,inputDir,outputDir,inFile,inDir_asciifile,asciiParFile;
    Int_t nEvents, plotmode, anamode, filenumber;
    Long64_t startevent, stopevent;
    Int_t first_event;
    Int_t eSE_ME_Flag;
    Int_t eAnalysisNum;
    Int_t eBeamTimeNum;
    TChain* input;
    TChain* input_B;
    ClassDef(StTrackingHFT,1)

};

#endif
