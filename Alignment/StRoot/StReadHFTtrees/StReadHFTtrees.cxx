#include "StReadHFTtrees.h"

#include "StReadHFTtrees_Func.h"
//#include "/u/aschmah/STAR/Analysis/HFT/StRoot/StHFTtree/StHFTtree.h"

static const Double_t MAGFIELDFACTOR = kilogauss;

// Fixed size dimensions of array or collections stored in the TTree if any.
const Int_t kMaxfVertices = 1;
const Int_t kMaxfTracks = 500;
const Int_t kMaxfHits = 1500;
const Int_t kMaxfMatchHits = 200;
const Int_t N_max_sensors  = 1500;

const Double_t hit_threshold = 600;

static TString HistName;

static TH1D* h_diff_angle = new TH1D("h_diff_angle","h_diff_angle",100,-3.2,3.2);
static TH1D* h_phiA       = new TH1D("h_phiA","h_phiA",100,-3.2,3.2);
static TH1D* h_phiB       = new TH1D("h_phiB","h_phiB",100,-3.2,3.2);
static TH1D* h_thetaA     = new TH1D("h_thetaA","h_thetaA",100,-3.2,3.2);
static TH1D* h_thetaB     = new TH1D("h_thetaB","h_thetaB",100,-3.2,3.2);
static TH1D* h_x_dcaA     = new TH1D("h_x_dcaA","h_x_dcaA",400,-20,20);
static TH1D* h_y_dcaA     = new TH1D("h_y_dcaA","h_y_dcaA",400,-20,20);
static TH1D* h_z_dcaA     = new TH1D("h_z_dcaA","h_z_dcaA",400,-20,20);
static TH1D* h_x_dcaB     = new TH1D("h_x_dcaB","h_x_dcaB",400,-20,20);
static TH1D* h_y_dcaB     = new TH1D("h_y_dcaB","h_y_dcaB",400,-20,20);
static TH1D* h_z_dcaB     = new TH1D("h_z_dcaB","h_z_dcaB",400,-20,20);

static const Int_t N_pixel_dca_cuts = 10;
static TH1F* h_pixel_dca[N_pixel_dca_cuts][3];
static TH1F* h_pixel_line_dca[N_pixel_dca_cuts][3];
static TH2F* h_HFT_hits_local;

static const Int_t N_HFT_hits_global = 4;
static TH2F* h_HFT_hits_global[N_HFT_hits_global];

static const Int_t N_corr_loops = 4;
static TH1F* h_HFT_hits_local_X[N_max_sensors][N_corr_loops];
static TH1F* h_HFT_hits_local_Z[N_max_sensors][N_corr_loops];

static TH1F* h_cosmic_momentum;

static TPolyMarker3D PM_Pixel_hit;

static TFile* corr_in_file;

static TNtuple* NT_HFT_zero_field_cosmic;
static TNtuple* NT_PXL_IST_zero_field_cosmic;


static TTree*   Tree_PXL_IST_zero_field_cosmic;
// Define some simple structures
typedef struct {Float_t PXL_IA_x,PXL_IA_y,PXL_IA_z,PXL_IB_x,PXL_IB_y,PXL_IB_z,PXL_OA_x,PXL_OA_y,PXL_OA_z,PXL_OB_x,PXL_OB_y,PXL_OB_z,PXL_id_IA,PXL_id_IB,PXL_id_OA,PXL_id_OB,IST_x,IST_y,IST_z,IST_id,IST_counter,IST_hit,magfac,qA,pxA,pyA,pzA,oxA,oyA,ozA,qB,pxB,pyB,pzB,oxB,oyB,ozB;} HFT_TREE_MAIN;
static HFT_TREE_MAIN HFT_tree_main;

static const Int_t n_TPC_hits_c  = 45;
static Int_t n_TPC_hits = n_TPC_hits_c;

// track A
static Float_t TPC_hitA_X[n_TPC_hits_c];
static Float_t TPC_hitA_Y[n_TPC_hits_c];
static Float_t TPC_hitA_Z[n_TPC_hits_c];
static Float_t TPC_errA_X[n_TPC_hits_c];
static Float_t TPC_errA_Y[n_TPC_hits_c];
static Float_t TPC_errA_Z[n_TPC_hits_c];

static Float_t TPC_hitA_sector[n_TPC_hits_c];
static Float_t TPC_hitA_padrow[n_TPC_hits_c];
static Float_t TPC_hitA_pad[n_TPC_hits_c];

// track B
static Float_t TPC_hitB_X[n_TPC_hits_c];
static Float_t TPC_hitB_Y[n_TPC_hits_c];
static Float_t TPC_hitB_Z[n_TPC_hits_c];
static Float_t TPC_errB_X[n_TPC_hits_c];
static Float_t TPC_errB_Y[n_TPC_hits_c];
static Float_t TPC_errB_Z[n_TPC_hits_c];

static Float_t TPC_hitB_sector[n_TPC_hits_c];
static Float_t TPC_hitB_padrow[n_TPC_hits_c];
static Float_t TPC_hitB_pad[n_TPC_hits_c];

static Float_t NT_HFT_zero_field_cosmic_array[16];
static Float_t NT_PXL_IST_zero_field_cosmic_array[37];

static TFile* Outputfile;

TH3D* h_3D_dummy;
TCanvas* c_3D;

ClassImp(StReadHFTtrees)
    //_____________________________________________________________________________
    StReadHFTtrees::StReadHFTtrees() {

    }
//_____________________________________________________________________________
StReadHFTtrees::~StReadHFTtrees() {

}

void StReadHFTtrees::Init()
{
    cout << "Init started" << endl;

    TString outputfile_name = outputDir;
    outputfile_name += inFile;
    outputfile_name += "_out.root";
    Outputfile = new TFile(outputfile_name.Data(),"RECREATE");
    cout << "Outputfile created: " << outputfile_name.Data() << endl;
 
    NT_HFT_zero_field_cosmic     = new TNtuple("NT_HFT_zero_field_cosmic","NT_HFT_zero_field_cosmic Ntuple","x1:y1:z1:x2:y2:z2:x3:y3:z3:x4:y4:z4:id_IL:id_OL:id_IR:id_OR");
    NT_PXL_IST_zero_field_cosmic = new TNtuple("NT_PXL_IST_zero_field_cosmic","NT_PXL_IST_zero_field_cosmic Ntuple","PXL_IA_x:PXL_IA_y:PXL_IA_z:PXL_IB_x:PXL_IB_y:PXL_IB_z:PXL_OA_x:PXL_OA_y:PXL_OA_z:PXL_OB_x:PXL_OB_y:PXL_OB_z:PXL_id_IA:PXL_id_IB:PXL_id_OA:PXL_id_OB:IST_x:IST_y:IST_z:IST_id:IST_counter:IST_hit:magfac:qA:pxA:pyA:pzA:oxA:oyA:ozA:qB:pxB:pyB:pzB:oxB:oyB:ozB");

    Tree_PXL_IST_zero_field_cosmic = new TTree("Tree_PXL_IST_zero_field_cosmic","Tree_PXL_IST_zero_field_cosmic Tree");
    Tree_PXL_IST_zero_field_cosmic->Branch("HFT_tree_main",&HFT_tree_main,"PXL_IA_x:PXL_IA_y:PXL_IA_z:PXL_IB_x:PXL_IB_y:PXL_IB_z:PXL_OA_x:PXL_OA_y:PXL_OA_z:PXL_OB_x:PXL_OB_y:PXL_OB_z:PXL_id_IA:PXL_id_IB:PXL_id_OA:PXL_id_OB:IST_x:IST_y:IST_z:IST_id:IST_counter:IST_hit:magfac:qA:pxA:pyA:pzA:oxA:oyA:ozA:qB:pxB:pyB:pzB:oxB:oyB:ozB");

    Tree_PXL_IST_zero_field_cosmic->Branch("n_TPC_hits",&n_TPC_hits,"n_TPC_hits/I");

    Tree_PXL_IST_zero_field_cosmic->Branch("TPC_hitA_X",TPC_hitA_X,"TPC_hitA_X[n_TPC_hits]/F");
    Tree_PXL_IST_zero_field_cosmic->Branch("TPC_hitA_Y",TPC_hitA_Y,"TPC_hitA_Y[n_TPC_hits]/F");
    Tree_PXL_IST_zero_field_cosmic->Branch("TPC_hitA_Z",TPC_hitA_Z,"TPC_hitA_Z[n_TPC_hits]/F");
    Tree_PXL_IST_zero_field_cosmic->Branch("TPC_errA_X",TPC_errA_X,"TPC_errA_X[n_TPC_hits]/F");
    Tree_PXL_IST_zero_field_cosmic->Branch("TPC_errA_Y",TPC_errA_Y,"TPC_errA_Y[n_TPC_hits]/F");
    Tree_PXL_IST_zero_field_cosmic->Branch("TPC_errA_Z",TPC_errA_Z,"TPC_errA_Z[n_TPC_hits]/F");

    Tree_PXL_IST_zero_field_cosmic->Branch("TPC_hitA_sector",TPC_hitA_sector,"TPC_hitA_sector[n_TPC_hits]/F");
    Tree_PXL_IST_zero_field_cosmic->Branch("TPC_hitA_padrow",TPC_hitA_padrow,"TPC_hitA_padrow[n_TPC_hits]/F");
    Tree_PXL_IST_zero_field_cosmic->Branch("TPC_hitA_pad",TPC_hitA_pad,"TPC_hitA_pad[n_TPC_hits]/F");

    Tree_PXL_IST_zero_field_cosmic->Branch("TPC_hitB_X",TPC_hitB_X,"TPC_hitB_X[n_TPC_hits]/F");
    Tree_PXL_IST_zero_field_cosmic->Branch("TPC_hitB_Y",TPC_hitB_Y,"TPC_hitB_Y[n_TPC_hits]/F");
    Tree_PXL_IST_zero_field_cosmic->Branch("TPC_hitB_Z",TPC_hitB_Z,"TPC_hitB_Z[n_TPC_hits]/F");
    Tree_PXL_IST_zero_field_cosmic->Branch("TPC_errB_X",TPC_errB_X,"TPC_errB_X[n_TPC_hits]/F");
    Tree_PXL_IST_zero_field_cosmic->Branch("TPC_errB_Y",TPC_errB_Y,"TPC_errB_Y[n_TPC_hits]/F");
    Tree_PXL_IST_zero_field_cosmic->Branch("TPC_errB_Z",TPC_errB_Z,"TPC_errB_Z[n_TPC_hits]/F");

    Tree_PXL_IST_zero_field_cosmic->Branch("TPC_hitB_sector",TPC_hitB_sector,"TPC_hitB_sector[n_TPC_hits]/F");
    Tree_PXL_IST_zero_field_cosmic->Branch("TPC_hitB_padrow",TPC_hitB_padrow,"TPC_hitB_padrow[n_TPC_hits]/F");
    Tree_PXL_IST_zero_field_cosmic->Branch("TPC_hitB_pad",TPC_hitB_pad,"TPC_hitB_pad[n_TPC_hits]/F");

    h_cosmic_momentum = new TH1F("h_cosmic_momentum","h_cosmic_momentum",1000,0,500);

    for(Int_t i_glob = 0; i_glob < N_HFT_hits_global; i_glob++)
    {
        HistName = "h_HFT_hits_global_";
        HistName += i_glob;
        h_HFT_hits_global[i_glob] = new TH2F(HistName.Data(),HistName.Data(),500,-20.0,20.0,500,-20.0,20.0);
    }

    for(Int_t i_cut = 0; i_cut < N_pixel_dca_cuts; i_cut++)
    {
        for(Int_t i_xyz = 0; i_xyz < 3; i_xyz++)
        {
            HistName = "h_pixel_dca_";
            HistName += i_cut;
            HistName += "_xyz_";
            HistName += i_xyz;
            h_pixel_dca[i_cut][i_xyz] = new TH1F(HistName.Data(),HistName.Data(),4000,-25,25);

            HistName = "h_pixel_line_dca_";
            HistName += i_cut;
            HistName += "_xyz_";
            HistName += i_xyz;
            h_pixel_line_dca[i_cut][i_xyz] = new TH1F(HistName.Data(),HistName.Data(),4000,-25,25);
        }
    }

    HistName = "h_HFT_hits_local";
    h_HFT_hits_local = new TH2F(HistName.Data(),HistName.Data(),500,-2,2,500,-2,2);

    for(Int_t i_sensor = 0; i_sensor < N_max_sensors; i_sensor++)
    {
        for(Int_t i_corr_loop = 0; i_corr_loop < N_corr_loops; i_corr_loop++)
        {
            HistName = "h_HFT_hits_local_X_sens_";
            HistName += i_sensor;
            HistName += "_corr_";
            HistName += i_corr_loop;
            h_HFT_hits_local_X[i_sensor][i_corr_loop] = new TH1F(HistName.Data(),HistName.Data(),4000,-2.1,2.1);

            HistName = "h_HFT_hits_local_Z_sens_";
            HistName += i_sensor;
            HistName += "_corr_";
            HistName += i_corr_loop;
            h_HFT_hits_local_Z[i_sensor][i_corr_loop] = new TH1F(HistName.Data(),HistName.Data(),8000,-4.1,4.1);
        }
    }

}

void StReadHFTtrees::Make()
{
    cout << "Make started" << endl;

    TTree          *fChain;   //!pointer to the analyzed TTree or TChain
    Int_t           fCurrent; //!current Tree number in a TChain

    // Declaration of leaf types
    //EventT          *EventT;
    UInt_t          fUniqueID;
    UInt_t          fBits;
    UInt_t          fNPTracks;
    UInt_t          fNvertex;
    UInt_t          fNtrack;
    UInt_t          fNhit;
    UInt_t          fNmatchhit;
    UInt_t          fFlag;
    Float_t         fVzVpd;
    Int_t           fNVpdHits;
    Int_t           fEvtHdr_fEvtNum;
    Int_t           fEvtHdr_fRun;
    Int_t           fEvtHdr_fDate;
    Double32_t      fEvtHdr_fField;
    Double32_t      fVertex[3];
    Double32_t      fCovariantMatrix[6];
    UInt_t          fNPredHFT[4];
    Int_t           fVertices_;
    UInt_t          fVertices_fUniqueID[kMaxfVertices];   //[fVertices_]
    UInt_t          fVertices_fBits[kMaxfVertices];   //[fVertices_]
    Char_t          fVertices_beg[kMaxfVertices];   //[fVertices_]
    Double32_t      fVertices_fVx[kMaxfVertices];   //[fVertices_]
    Double32_t      fVertices_fVy[kMaxfVertices];   //[fVertices_]
    Double32_t      fVertices_fVz[kMaxfVertices];   //[fVertices_]
    Int_t           fVertices_fNtracks[kMaxfVertices];   //[fVertices_]
    Char_t          fVertices_end[kMaxfVertices];   //[fVertices_]
    Int_t           fTracks_;
    UInt_t          fTracks_fUniqueID[kMaxfTracks];   //[fTracks_]
    UInt_t          fTracks_fBits[kMaxfTracks];   //[fTracks_]
    Char_t          fTracks_beg[kMaxfTracks];   //[fTracks_]
    Double32_t      fTracks_fPxDca[kMaxfTracks];   //[fTracks_]
    Double32_t      fTracks_fPyDca[kMaxfTracks];   //[fTracks_]
    Double32_t      fTracks_fPzDca[kMaxfTracks];   //[fTracks_]
    Double32_t      fTracks_fPPx[kMaxfTracks];   //[fTracks_]
    Double32_t      fTracks_fPPy[kMaxfTracks];   //[fTracks_]
    Double32_t      fTracks_fPPz[kMaxfTracks];   //[fTracks_]
    Double32_t      fTracks_fOriginXDca[kMaxfTracks];   //[fTracks_]
    Double32_t      fTracks_fOriginYDca[kMaxfTracks];   //[fTracks_]
    Double32_t      fTracks_fOriginZDca[kMaxfTracks];   //[fTracks_]
    Double32_t      fTracks_fPx[kMaxfTracks];   //[fTracks_]
    Double32_t      fTracks_fPy[kMaxfTracks];   //[fTracks_]
    Double32_t      fTracks_fPz[kMaxfTracks];   //[fTracks_]
    Double32_t      fTracks_fOriginX[kMaxfTracks];   //[fTracks_]
    Double32_t      fTracks_fOriginY[kMaxfTracks];   //[fTracks_]
    Double32_t      fTracks_fOriginZ[kMaxfTracks];   //[fTracks_]
    Int_t           fTracks_fNpoint[kMaxfTracks];   //[fTracks_]
    Double32_t      fTracks_fTpcHitX[kMaxfTracks][45];   //[fTracks_]
    Double32_t      fTracks_fTpcHitY[kMaxfTracks][45];   //[fTracks_]
    Double32_t      fTracks_fTpcHitZ[kMaxfTracks][45];   //[fTracks_]
    Double32_t      fTracks_fTpcHitEX[kMaxfTracks][45];   //[fTracks_]
    Double32_t      fTracks_fTpcHitEY[kMaxfTracks][45];   //[fTracks_]
    Double32_t      fTracks_fTpcHitEZ[kMaxfTracks][45];   //[fTracks_]
    UShort_t        fTracks_fTpcHitSector[kMaxfTracks][45];   //[fTracks_]
    Short_t         fTracks_fTpcHitPadrow[kMaxfTracks][45];   //[fTracks_]
    Float_t         fTracks_fTpcHitPad[kMaxfTracks][45];   //[fTracks_]
    Double32_t      fTracks_fNsigmaPi[kMaxfTracks];   //[fTracks_]
    Double32_t      fTracks_fDca2D[kMaxfTracks];   //[fTracks_]
    Double32_t      fTracks_fDca3D[kMaxfTracks];   //[fTracks_]
    UInt_t          fTracks_fPxlHitPattern[kMaxfTracks];   //[fTracks_]
    UInt_t          fTracks_fIstHitPattern[kMaxfTracks];   //[fTracks_]
    UInt_t          fTracks_fSsdHitPattern[kMaxfTracks];   //[fTracks_]
    Double32_t      fTracks_fPxlHitX[kMaxfTracks][3];   //[fTracks_]
    Double32_t      fTracks_fPxlHitY[kMaxfTracks][3];   //[fTracks_]
    Double32_t      fTracks_fPxlHitZ[kMaxfTracks][3];   //[fTracks_]
    Double32_t      fTracks_fPxlHitXL[kMaxfTracks][3];   //[fTracks_]
    Double32_t      fTracks_fPxlHitYL[kMaxfTracks][3];   //[fTracks_]
    Double32_t      fTracks_fPxlHitZL[kMaxfTracks][3];   //[fTracks_]
    Double32_t      fTracks_fIstHitX[kMaxfTracks][2];   //[fTracks_]
    Double32_t      fTracks_fIstHitY[kMaxfTracks][2];   //[fTracks_]
    Double32_t      fTracks_fIstHitZ[kMaxfTracks][2];   //[fTracks_]
    Double32_t      fTracks_fIstHitXL[kMaxfTracks][2];   //[fTracks_]
    Double32_t      fTracks_fIstHitYL[kMaxfTracks][2];   //[fTracks_]
    Double32_t      fTracks_fIstHitZL[kMaxfTracks][2];   //[fTracks_]
    Double32_t      fTracks_fSsdHitX[kMaxfTracks][2];   //[fTracks_]
    Double32_t      fTracks_fSsdHitY[kMaxfTracks][2];   //[fTracks_]
    Double32_t      fTracks_fSsdHitZ[kMaxfTracks][2];   //[fTracks_]
    Double32_t      fTracks_fSsdHitXL[kMaxfTracks][2];   //[fTracks_]
    Double32_t      fTracks_fSsdHitYL[kMaxfTracks][2];   //[fTracks_]
    Double32_t      fTracks_fSsdHitZL[kMaxfTracks][2];   //[fTracks_]
    Char_t          fTracks_end[kMaxfTracks];   //[fTracks_]
    Int_t           fHits_;
    UInt_t          fHits_fUniqueID[kMaxfHits];   //[fHits_]
    UInt_t          fHits_fBits[kMaxfHits];   //[fHits_]
    Char_t          fHits_start[kMaxfHits];   //[fHits_]
    Int_t           fHits_Id[kMaxfHits];   //[fHits_]
    UInt_t          fHits_nRawHits[kMaxfHits];   //[fHits_]
    Double32_t      fHits_xG[kMaxfHits];   //[fHits_]
    Double32_t      fHits_yG[kMaxfHits];   //[fHits_]
    Double32_t      fHits_zG[kMaxfHits];   //[fHits_]
    Double32_t      fHits_xL[kMaxfHits];   //[fHits_]
    Double32_t      fHits_yL[kMaxfHits];   //[fHits_]
    Double32_t      fHits_zL[kMaxfHits];   //[fHits_]
    UInt_t          fHits_index2Track[kMaxfHits];   //[fHits_]
    Char_t          fHits_end[kMaxfHits];   //[fHits_]
    Int_t           fMatchHits_;
    UInt_t          fMatchHits_fUniqueID[kMaxfMatchHits];   //[fMatchHits_]
    UInt_t          fMatchHits_fBits[kMaxfMatchHits];   //[fMatchHits_]
    Char_t          fMatchHits_start[kMaxfMatchHits];   //[fMatchHits_]
    UInt_t          fMatchHits_index2Track[kMaxfMatchHits];   //[fMatchHits_]
    UInt_t          fMatchHits_index2Hit[kMaxfMatchHits];   //[fMatchHits_]
    UInt_t          fMatchHits_detId[kMaxfMatchHits];   //[fMatchHits_]
    UInt_t          fMatchHits_nRawHits[kMaxfMatchHits];   //[fMatchHits_]
    Double32_t      fMatchHits_xGP[kMaxfMatchHits];   //[fMatchHits_]
    Double32_t      fMatchHits_yGP[kMaxfMatchHits];   //[fMatchHits_]
    Double32_t      fMatchHits_zGP[kMaxfMatchHits];   //[fMatchHits_]
    Double32_t      fMatchHits_xLP[kMaxfMatchHits];   //[fMatchHits_]
    Double32_t      fMatchHits_yLP[kMaxfMatchHits];   //[fMatchHits_]
    Double32_t      fMatchHits_zLP[kMaxfMatchHits];   //[fMatchHits_]
    Double32_t      fMatchHits_tuP[kMaxfMatchHits];   //[fMatchHits_]
    Double32_t      fMatchHits_tvP[kMaxfMatchHits];   //[fMatchHits_]
    Double32_t      fMatchHits_xG[kMaxfMatchHits];   //[fMatchHits_]
    Double32_t      fMatchHits_yG[kMaxfMatchHits];   //[fMatchHits_]
    Double32_t      fMatchHits_zG[kMaxfMatchHits];   //[fMatchHits_]
    Double32_t      fMatchHits_xL[kMaxfMatchHits];   //[fMatchHits_]
    Double32_t      fMatchHits_yL[kMaxfMatchHits];   //[fMatchHits_]
    Double32_t      fMatchHits_zL[kMaxfMatchHits];   //[fMatchHits_]
    Double32_t      fMatchHits_pT[kMaxfMatchHits];   //[fMatchHits_]
    Double32_t      fMatchHits_eta[kMaxfMatchHits];   //[fMatchHits_]
    Double32_t      fMatchHits_phi[kMaxfMatchHits];   //[fMatchHits_]
    Double32_t      fMatchHits_ox[kMaxfMatchHits];   //[fMatchHits_]
    Double32_t      fMatchHits_oy[kMaxfMatchHits];   //[fMatchHits_]
    Double32_t      fMatchHits_oz[kMaxfMatchHits];   //[fMatchHits_]
    Int_t           fMatchHits_npoint[kMaxfMatchHits];   //[fMatchHits_]
    Double32_t      fMatchHits_firstPointR[kMaxfMatchHits];   //[fMatchHits_]
    Double32_t      fMatchHits_firstPointZ[kMaxfMatchHits];   //[fMatchHits_]
    Char_t          fMatchHits_end[kMaxfMatchHits];   //[fMatchHits_]
    Bool_t          fIsValid;

    // List of branches
    TBranch        *b_EventT_fUniqueID;   //!
    TBranch        *b_EventT_fBits;   //!
    TBranch        *b_EventT_fNPTracks;   //!
    TBranch        *b_EventT_fNvertex;   //!
    TBranch        *b_EventT_fNtrack;   //!
    TBranch        *b_EventT_fNhit;   //!
    TBranch        *b_EventT_fNmatchhit;   //!
    TBranch        *b_EventT_fFlag;   //!
    TBranch        *b_EventT_fVzVpd;   //!
    TBranch        *b_EventT_fNVpdHits;   //!
    TBranch        *b_EventT_fEvtHdr_fEvtNum;   //!
    TBranch        *b_EventT_fEvtHdr_fRun;   //!
    TBranch        *b_EventT_fEvtHdr_fDate;   //!
    TBranch        *b_EventT_fEvtHdr_fField;   //!
    TBranch        *b_EventT_fVertex;   //!
    TBranch        *b_EventT_fCovariantMatrix;   //!
    TBranch        *b_EventT_fNPredHFT;   //!
    TBranch        *b_EventT_fVertices_;   //!
    TBranch        *b_fVertices_fUniqueID;   //!
    TBranch        *b_fVertices_fBits;   //!
    TBranch        *b_fVertices_beg;   //!
    TBranch        *b_fVertices_fVx;   //!
    TBranch        *b_fVertices_fVy;   //!
    TBranch        *b_fVertices_fVz;   //!
    TBranch        *b_fVertices_fNtracks;   //!
    TBranch        *b_fVertices_end;   //!
    TBranch        *b_EventT_fTracks_;   //!
    TBranch        *b_fTracks_fUniqueID;   //!
    TBranch        *b_fTracks_fBits;   //!
    TBranch        *b_fTracks_beg;   //!
    TBranch        *b_fTracks_fPxDca;   //!
    TBranch        *b_fTracks_fPyDca;   //!
    TBranch        *b_fTracks_fPzDca;   //!
    TBranch        *b_fTracks_fPPx;   //!
    TBranch        *b_fTracks_fPPy;   //!
    TBranch        *b_fTracks_fPPz;   //!
    TBranch        *b_fTracks_fOriginXDca;   //!
    TBranch        *b_fTracks_fOriginYDca;   //!
    TBranch        *b_fTracks_fOriginZDca;   //!
    TBranch        *b_fTracks_fPx;   //!
    TBranch        *b_fTracks_fPy;   //!
    TBranch        *b_fTracks_fPz;   //!
    TBranch        *b_fTracks_fOriginX;   //!
    TBranch        *b_fTracks_fOriginY;   //!
    TBranch        *b_fTracks_fOriginZ;   //!
    TBranch        *b_fTracks_fNpoint;   //!
    TBranch        *b_fTracks_fTpcHitX;   //!
    TBranch        *b_fTracks_fTpcHitY;   //!
    TBranch        *b_fTracks_fTpcHitZ;   //!
    TBranch        *b_fTracks_fTpcHitEX;   //!
    TBranch        *b_fTracks_fTpcHitEY;   //!
    TBranch        *b_fTracks_fTpcHitEZ;   //!
    TBranch        *b_fTracks_fTpcHitSector;   //!
    TBranch        *b_fTracks_fTpcHitPadrow;   //!
    TBranch        *b_fTracks_fTpcHitPad;   //!
    TBranch        *b_fTracks_fNsigmaPi;   //!
    TBranch        *b_fTracks_fDca2D;   //!
    TBranch        *b_fTracks_fDca3D;   //!
    TBranch        *b_fTracks_fPxlHitPattern;   //!
    TBranch        *b_fTracks_fIstHitPattern;   //!
    TBranch        *b_fTracks_fSsdHitPattern;   //!
    TBranch        *b_fTracks_fPxlHitX;   //!
    TBranch        *b_fTracks_fPxlHitY;   //!
    TBranch        *b_fTracks_fPxlHitZ;   //!
    TBranch        *b_fTracks_fPxlHitXL;   //!
    TBranch        *b_fTracks_fPxlHitYL;   //!
    TBranch        *b_fTracks_fPxlHitZL;   //!
    TBranch        *b_fTracks_fIstHitX;   //!
    TBranch        *b_fTracks_fIstHitY;   //!
    TBranch        *b_fTracks_fIstHitZ;   //!
    TBranch        *b_fTracks_fIstHitXL;   //!
    TBranch        *b_fTracks_fIstHitYL;   //!
    TBranch        *b_fTracks_fIstHitZL;   //!
    TBranch        *b_fTracks_fSsdHitX;   //!
    TBranch        *b_fTracks_fSsdHitY;   //!
    TBranch        *b_fTracks_fSsdHitZ;   //!
    TBranch        *b_fTracks_fSsdHitXL;   //!
    TBranch        *b_fTracks_fSsdHitYL;   //!
    TBranch        *b_fTracks_fSsdHitZL;   //!
    TBranch        *b_fTracks_end;   //!
    TBranch        *b_EventT_fHits_;   //!
    TBranch        *b_fHits_fUniqueID;   //!
    TBranch        *b_fHits_fBits;   //!
    TBranch        *b_fHits_start;   //!
    TBranch        *b_fHits_Id;   //!
    TBranch        *b_fHits_nRawHits;   //!
    TBranch        *b_fHits_xG;   //!
    TBranch        *b_fHits_yG;   //!
    TBranch        *b_fHits_zG;   //!
    TBranch        *b_fHits_xL;   //!
    TBranch        *b_fHits_yL;   //!
    TBranch        *b_fHits_zL;   //!
    TBranch        *b_fHits_index2Track;   //!
    TBranch        *b_fHits_end;   //!
    TBranch        *b_EventT_fMatchHits_;   //!
    TBranch        *b_fMatchHits_fUniqueID;   //!
    TBranch        *b_fMatchHits_fBits;   //!
    TBranch        *b_fMatchHits_start;   //!
    TBranch        *b_fMatchHits_index2Track;   //!
    TBranch        *b_fMatchHits_index2Hit;   //!
    TBranch        *b_fMatchHits_detId;   //!
    TBranch        *b_fMatchHits_nRawHits;   //!
    TBranch        *b_fMatchHits_xGP;   //!
    TBranch        *b_fMatchHits_yGP;   //!
    TBranch        *b_fMatchHits_zGP;   //!
    TBranch        *b_fMatchHits_xLP;   //!
    TBranch        *b_fMatchHits_yLP;   //!
    TBranch        *b_fMatchHits_zLP;   //!
    TBranch        *b_fMatchHits_tuP;   //!
    TBranch        *b_fMatchHits_tvP;   //!
    TBranch        *b_fMatchHits_xG;   //!
    TBranch        *b_fMatchHits_yG;   //!
    TBranch        *b_fMatchHits_zG;   //!
    TBranch        *b_fMatchHits_xL;   //!
    TBranch        *b_fMatchHits_yL;   //!
    TBranch        *b_fMatchHits_zL;   //!
    TBranch        *b_fMatchHits_pT;   //!
    TBranch        *b_fMatchHits_eta;   //!
    TBranch        *b_fMatchHits_phi;   //!
    TBranch        *b_fMatchHits_ox;   //!
    TBranch        *b_fMatchHits_oy;   //!
    TBranch        *b_fMatchHits_oz;   //!
    TBranch        *b_fMatchHits_npoint;   //!
    TBranch        *b_fMatchHits_firstPointR;   //!
    TBranch        *b_fMatchHits_firstPointZ;   //!
    TBranch        *b_fMatchHits_end;   //!
    TBranch        *b_EventT_fIsValid;   //!

    // ~dongx/lbl/hft/Run14/Cosmic_Data_FullSurvey/output

    TTree *tree;
    TFile *f;


    TString infile_name = inputDir;
    infile_name += inFile;
    infile_name += ".root";
    f = TFile::Open(infile_name.Data());
    cout << "infile_name: " << infile_name.Data() << endl;

    if(!f || !f->IsOpen())
    {
        cout << "WARNING: File cannot be opened" << endl;
        f = new TFile(infile_name.Data());
    }
    f->GetObject("T",tree);


    cout << "Open HFT sensor hit histogram loop 1" << endl;
    corr_in_file = TFile::Open("/star/institutions/lbl/aschmah/HFT/Cosmics/Corrections/HFT_local_hit_histograms_loop_Survey_PXL_IST_V4.root");
    for(Int_t i_sensor = 0; i_sensor < N_max_sensors; i_sensor++)
    {
        HistName = "h_HFT_hits_local_X_sens_";
        HistName += i_sensor;
        HistName += "_corr_";
        HistName += 0;
        if(corr_in_file->Get(HistName.Data()))
        {
            h_HFT_hits_local_X[i_sensor][1] = (TH1F*)corr_in_file->Get(HistName.Data());
            HistName += "_loop1";
            h_HFT_hits_local_X[i_sensor][1] ->SetName(HistName.Data());
        }
        HistName = "h_HFT_hits_local_Z_sens_";
        HistName += i_sensor;
        HistName += "_corr_";
        HistName += 0;
        if(corr_in_file->Get(HistName.Data()))
        {
            h_HFT_hits_local_Z[i_sensor][1] = (TH1F*)corr_in_file->Get(HistName.Data());
            HistName += "_loop1";
            h_HFT_hits_local_Z[i_sensor][1] ->SetName(HistName.Data());
        }
        HistName = "h_HFT_hits_local_X_sens_";
        HistName += i_sensor;
        HistName += "_corr_";
        HistName += 2;
        if(corr_in_file->Get(HistName.Data()))
        {
            h_HFT_hits_local_X[i_sensor][3] = (TH1F*)corr_in_file->Get(HistName.Data());
            HistName += "_loop1";
            h_HFT_hits_local_X[i_sensor][3] ->SetName(HistName.Data());
        }
        HistName = "h_HFT_hits_local_Z_sens_";
        HistName += i_sensor;
        HistName += "_corr_";
        HistName += 2;
        if(corr_in_file->Get(HistName.Data()))
        {
            h_HFT_hits_local_Z[i_sensor][3] = (TH1F*)corr_in_file->Get(HistName.Data());
            HistName += "_loop1";
            h_HFT_hits_local_Z[i_sensor][3] ->SetName(HistName.Data());
        }
    }
    cout << "Sensor hit histograms opened" << endl;


    fChain = tree;
    fCurrent = -1;
    fChain->SetMakeClass(1);

    fChain->SetBranchAddress("fUniqueID", &fUniqueID, &b_EventT_fUniqueID);
    fChain->SetBranchAddress("fBits", &fBits, &b_EventT_fBits);
    fChain->SetBranchAddress("fNPTracks", &fNPTracks, &b_EventT_fNPTracks);
    fChain->SetBranchAddress("fNvertex", &fNvertex, &b_EventT_fNvertex);
    fChain->SetBranchAddress("fNtrack", &fNtrack, &b_EventT_fNtrack);
    fChain->SetBranchAddress("fNhit", &fNhit, &b_EventT_fNhit);
    fChain->SetBranchAddress("fNmatchhit", &fNmatchhit, &b_EventT_fNmatchhit);
    fChain->SetBranchAddress("fFlag", &fFlag, &b_EventT_fFlag);
    fChain->SetBranchAddress("fVzVpd", &fVzVpd, &b_EventT_fVzVpd);
    fChain->SetBranchAddress("fNVpdHits", &fNVpdHits, &b_EventT_fNVpdHits);
    fChain->SetBranchAddress("fEvtHdr.fEvtNum", &fEvtHdr_fEvtNum, &b_EventT_fEvtHdr_fEvtNum);
    fChain->SetBranchAddress("fEvtHdr.fRun", &fEvtHdr_fRun, &b_EventT_fEvtHdr_fRun);
    fChain->SetBranchAddress("fEvtHdr.fDate", &fEvtHdr_fDate, &b_EventT_fEvtHdr_fDate);
    fChain->SetBranchAddress("fEvtHdr.fField", &fEvtHdr_fField, &b_EventT_fEvtHdr_fField);
    fChain->SetBranchAddress("fVertex[3]", fVertex, &b_EventT_fVertex);
    fChain->SetBranchAddress("fCovariantMatrix[6]", fCovariantMatrix, &b_EventT_fCovariantMatrix);
    fChain->SetBranchAddress("fNPredHFT[4]", fNPredHFT, &b_EventT_fNPredHFT);
    fChain->SetBranchAddress("fVertices", &fVertices_, &b_EventT_fVertices_);
    fChain->SetBranchAddress("fVertices.fUniqueID", fVertices_fUniqueID, &b_fVertices_fUniqueID);
    fChain->SetBranchAddress("fVertices.fBits", fVertices_fBits, &b_fVertices_fBits);
    fChain->SetBranchAddress("fVertices.beg", fVertices_beg, &b_fVertices_beg);
    fChain->SetBranchAddress("fVertices.fVx", fVertices_fVx, &b_fVertices_fVx);
    fChain->SetBranchAddress("fVertices.fVy", fVertices_fVy, &b_fVertices_fVy);
    fChain->SetBranchAddress("fVertices.fVz", fVertices_fVz, &b_fVertices_fVz);
    fChain->SetBranchAddress("fVertices.fNtracks", fVertices_fNtracks, &b_fVertices_fNtracks);
    fChain->SetBranchAddress("fVertices.end", fVertices_end, &b_fVertices_end);
    fChain->SetBranchAddress("fTracks", &fTracks_, &b_EventT_fTracks_);
    fChain->SetBranchAddress("fTracks.fUniqueID", fTracks_fUniqueID, &b_fTracks_fUniqueID);
    fChain->SetBranchAddress("fTracks.fBits", fTracks_fBits, &b_fTracks_fBits);
    fChain->SetBranchAddress("fTracks.beg", fTracks_beg, &b_fTracks_beg);
    fChain->SetBranchAddress("fTracks.fPxDca", fTracks_fPxDca, &b_fTracks_fPxDca);
    fChain->SetBranchAddress("fTracks.fPyDca", fTracks_fPyDca, &b_fTracks_fPyDca);
    fChain->SetBranchAddress("fTracks.fPzDca", fTracks_fPzDca, &b_fTracks_fPzDca);
    fChain->SetBranchAddress("fTracks.fPPx", fTracks_fPPx, &b_fTracks_fPPx);
    fChain->SetBranchAddress("fTracks.fPPy", fTracks_fPPy, &b_fTracks_fPPy);
    fChain->SetBranchAddress("fTracks.fPPz", fTracks_fPPz, &b_fTracks_fPPz);
    fChain->SetBranchAddress("fTracks.fOriginXDca", fTracks_fOriginXDca, &b_fTracks_fOriginXDca);
    fChain->SetBranchAddress("fTracks.fOriginYDca", fTracks_fOriginYDca, &b_fTracks_fOriginYDca);
    fChain->SetBranchAddress("fTracks.fOriginZDca", fTracks_fOriginZDca, &b_fTracks_fOriginZDca);
    fChain->SetBranchAddress("fTracks.fPx", fTracks_fPx, &b_fTracks_fPx);
    fChain->SetBranchAddress("fTracks.fPy", fTracks_fPy, &b_fTracks_fPy);
    fChain->SetBranchAddress("fTracks.fPz", fTracks_fPz, &b_fTracks_fPz);
    fChain->SetBranchAddress("fTracks.fOriginX", fTracks_fOriginX, &b_fTracks_fOriginX);
    fChain->SetBranchAddress("fTracks.fOriginY", fTracks_fOriginY, &b_fTracks_fOriginY);
    fChain->SetBranchAddress("fTracks.fOriginZ", fTracks_fOriginZ, &b_fTracks_fOriginZ);
    fChain->SetBranchAddress("fTracks.fNpoint", fTracks_fNpoint, &b_fTracks_fNpoint);
    fChain->SetBranchAddress("fTracks.fTpcHitX[45]", fTracks_fTpcHitX, &b_fTracks_fTpcHitX);
    fChain->SetBranchAddress("fTracks.fTpcHitY[45]", fTracks_fTpcHitY, &b_fTracks_fTpcHitY);
    fChain->SetBranchAddress("fTracks.fTpcHitZ[45]", fTracks_fTpcHitZ, &b_fTracks_fTpcHitZ);
    fChain->SetBranchAddress("fTracks.fTpcHitEX[45]", fTracks_fTpcHitEX, &b_fTracks_fTpcHitEX);
    fChain->SetBranchAddress("fTracks.fTpcHitEY[45]", fTracks_fTpcHitEY, &b_fTracks_fTpcHitEY);
    fChain->SetBranchAddress("fTracks.fTpcHitEZ[45]", fTracks_fTpcHitEZ, &b_fTracks_fTpcHitEZ);
    fChain->SetBranchAddress("fTracks.fTpcHitSector[45]", fTracks_fTpcHitSector, &b_fTracks_fTpcHitSector);
    fChain->SetBranchAddress("fTracks.fTpcHitPadrow[45]", fTracks_fTpcHitPadrow, &b_fTracks_fTpcHitPadrow);
    fChain->SetBranchAddress("fTracks.fTpcHitPad[45]", fTracks_fTpcHitPad, &b_fTracks_fTpcHitPad);
    fChain->SetBranchAddress("fTracks.fNsigmaPi", fTracks_fNsigmaPi, &b_fTracks_fNsigmaPi);
    fChain->SetBranchAddress("fTracks.fDca2D", fTracks_fDca2D, &b_fTracks_fDca2D);
    fChain->SetBranchAddress("fTracks.fDca3D", fTracks_fDca3D, &b_fTracks_fDca3D);
    fChain->SetBranchAddress("fTracks.fPxlHitPattern", fTracks_fPxlHitPattern, &b_fTracks_fPxlHitPattern);
    fChain->SetBranchAddress("fTracks.fIstHitPattern", fTracks_fIstHitPattern, &b_fTracks_fIstHitPattern);
    fChain->SetBranchAddress("fTracks.fSsdHitPattern", fTracks_fSsdHitPattern, &b_fTracks_fSsdHitPattern);
    fChain->SetBranchAddress("fTracks.fPxlHitX[3]", fTracks_fPxlHitX, &b_fTracks_fPxlHitX);
    fChain->SetBranchAddress("fTracks.fPxlHitY[3]", fTracks_fPxlHitY, &b_fTracks_fPxlHitY);
    fChain->SetBranchAddress("fTracks.fPxlHitZ[3]", fTracks_fPxlHitZ, &b_fTracks_fPxlHitZ);
    fChain->SetBranchAddress("fTracks.fPxlHitXL[3]", fTracks_fPxlHitXL, &b_fTracks_fPxlHitXL);
    fChain->SetBranchAddress("fTracks.fPxlHitYL[3]", fTracks_fPxlHitYL, &b_fTracks_fPxlHitYL);
    fChain->SetBranchAddress("fTracks.fPxlHitZL[3]", fTracks_fPxlHitZL, &b_fTracks_fPxlHitZL);
    fChain->SetBranchAddress("fTracks.fIstHitX[2]", fTracks_fIstHitX, &b_fTracks_fIstHitX);
    fChain->SetBranchAddress("fTracks.fIstHitY[2]", fTracks_fIstHitY, &b_fTracks_fIstHitY);
    fChain->SetBranchAddress("fTracks.fIstHitZ[2]", fTracks_fIstHitZ, &b_fTracks_fIstHitZ);
    fChain->SetBranchAddress("fTracks.fIstHitXL[2]", fTracks_fIstHitXL, &b_fTracks_fIstHitXL);
    fChain->SetBranchAddress("fTracks.fIstHitYL[2]", fTracks_fIstHitYL, &b_fTracks_fIstHitYL);
    fChain->SetBranchAddress("fTracks.fIstHitZL[2]", fTracks_fIstHitZL, &b_fTracks_fIstHitZL);
    fChain->SetBranchAddress("fTracks.fSsdHitX[2]", fTracks_fSsdHitX, &b_fTracks_fSsdHitX);
    fChain->SetBranchAddress("fTracks.fSsdHitY[2]", fTracks_fSsdHitY, &b_fTracks_fSsdHitY);
    fChain->SetBranchAddress("fTracks.fSsdHitZ[2]", fTracks_fSsdHitZ, &b_fTracks_fSsdHitZ);
    fChain->SetBranchAddress("fTracks.fSsdHitXL[2]", fTracks_fSsdHitXL, &b_fTracks_fSsdHitXL);
    fChain->SetBranchAddress("fTracks.fSsdHitYL[2]", fTracks_fSsdHitYL, &b_fTracks_fSsdHitYL);
    fChain->SetBranchAddress("fTracks.fSsdHitZL[2]", fTracks_fSsdHitZL, &b_fTracks_fSsdHitZL);
    fChain->SetBranchAddress("fTracks.end", fTracks_end, &b_fTracks_end);
    fChain->SetBranchAddress("fHits", &fHits_, &b_EventT_fHits_);
    fChain->SetBranchAddress("fHits.fUniqueID", fHits_fUniqueID, &b_fHits_fUniqueID);
    fChain->SetBranchAddress("fHits.fBits", fHits_fBits, &b_fHits_fBits);
    fChain->SetBranchAddress("fHits.start", fHits_start, &b_fHits_start);
    fChain->SetBranchAddress("fHits.Id", fHits_Id, &b_fHits_Id);
    fChain->SetBranchAddress("fHits.nRawHits", fHits_nRawHits, &b_fHits_nRawHits);
    fChain->SetBranchAddress("fHits.xG", fHits_xG, &b_fHits_xG);
    fChain->SetBranchAddress("fHits.yG", fHits_yG, &b_fHits_yG);
    fChain->SetBranchAddress("fHits.zG", fHits_zG, &b_fHits_zG);
    fChain->SetBranchAddress("fHits.xL", fHits_xL, &b_fHits_xL);
    fChain->SetBranchAddress("fHits.yL", fHits_yL, &b_fHits_yL);
    fChain->SetBranchAddress("fHits.zL", fHits_zL, &b_fHits_zL);
    fChain->SetBranchAddress("fHits.index2Track", fHits_index2Track, &b_fHits_index2Track);
    fChain->SetBranchAddress("fHits.end", fHits_end, &b_fHits_end);
    fChain->SetBranchAddress("fMatchHits", &fMatchHits_, &b_EventT_fMatchHits_);
    fChain->SetBranchAddress("fMatchHits.fUniqueID", fMatchHits_fUniqueID, &b_fMatchHits_fUniqueID);
    fChain->SetBranchAddress("fMatchHits.fBits", fMatchHits_fBits, &b_fMatchHits_fBits);
    fChain->SetBranchAddress("fMatchHits.start", fMatchHits_start, &b_fMatchHits_start);
    fChain->SetBranchAddress("fMatchHits.index2Track", fMatchHits_index2Track, &b_fMatchHits_index2Track);
    fChain->SetBranchAddress("fMatchHits.index2Hit", fMatchHits_index2Hit, &b_fMatchHits_index2Hit);
    fChain->SetBranchAddress("fMatchHits.detId", fMatchHits_detId, &b_fMatchHits_detId);
    fChain->SetBranchAddress("fMatchHits.nRawHits", fMatchHits_nRawHits, &b_fMatchHits_nRawHits);
    fChain->SetBranchAddress("fMatchHits.xGP", fMatchHits_xGP, &b_fMatchHits_xGP);
    fChain->SetBranchAddress("fMatchHits.yGP", fMatchHits_yGP, &b_fMatchHits_yGP);
    fChain->SetBranchAddress("fMatchHits.zGP", fMatchHits_zGP, &b_fMatchHits_zGP);
    fChain->SetBranchAddress("fMatchHits.xLP", fMatchHits_xLP, &b_fMatchHits_xLP);
    fChain->SetBranchAddress("fMatchHits.yLP", fMatchHits_yLP, &b_fMatchHits_yLP);
    fChain->SetBranchAddress("fMatchHits.zLP", fMatchHits_zLP, &b_fMatchHits_zLP);
    fChain->SetBranchAddress("fMatchHits.tuP", fMatchHits_tuP, &b_fMatchHits_tuP);
    fChain->SetBranchAddress("fMatchHits.tvP", fMatchHits_tvP, &b_fMatchHits_tvP);
    fChain->SetBranchAddress("fMatchHits.xG", fMatchHits_xG, &b_fMatchHits_xG);
    fChain->SetBranchAddress("fMatchHits.yG", fMatchHits_yG, &b_fMatchHits_yG);
    fChain->SetBranchAddress("fMatchHits.zG", fMatchHits_zG, &b_fMatchHits_zG);
    fChain->SetBranchAddress("fMatchHits.xL", fMatchHits_xL, &b_fMatchHits_xL);
    fChain->SetBranchAddress("fMatchHits.yL", fMatchHits_yL, &b_fMatchHits_yL);
    fChain->SetBranchAddress("fMatchHits.zL", fMatchHits_zL, &b_fMatchHits_zL);
    fChain->SetBranchAddress("fMatchHits.pT", fMatchHits_pT, &b_fMatchHits_pT);
    fChain->SetBranchAddress("fMatchHits.eta", fMatchHits_eta, &b_fMatchHits_eta);
    fChain->SetBranchAddress("fMatchHits.phi", fMatchHits_phi, &b_fMatchHits_phi);
    fChain->SetBranchAddress("fMatchHits.ox", fMatchHits_ox, &b_fMatchHits_ox);
    fChain->SetBranchAddress("fMatchHits.oy", fMatchHits_oy, &b_fMatchHits_oy);
    fChain->SetBranchAddress("fMatchHits.oz", fMatchHits_oz, &b_fMatchHits_oz);
    fChain->SetBranchAddress("fMatchHits.npoint", fMatchHits_npoint, &b_fMatchHits_npoint);
    fChain->SetBranchAddress("fMatchHits.firstPointR", fMatchHits_firstPointR, &b_fMatchHits_firstPointR);
    fChain->SetBranchAddress("fMatchHits.firstPointZ", fMatchHits_firstPointZ, &b_fMatchHits_firstPointZ);
    fChain->SetBranchAddress("fMatchHits.end", fMatchHits_end, &b_fMatchHits_end);
    fChain->SetBranchAddress("fIsValid", &fIsValid, &b_EventT_fIsValid);

    Long64_t nentries = fChain->GetEntriesFast();
    cout << "nentries = " << nentries << endl;


    TString PXL_file_Name = "PXL_data.txt";
    FILE* fPXL_tables = fopen(PXL_file_Name.Data(),"w");


    //-------------------------------------------------
    if(plotmode == 1)
    {
        cout << "Draw 3D image" << endl;
        h_3D_dummy = new TH3D("h_3D_dummy","h_3D_dummy",200,-300,300,200,-300,300,200,-1000,1000);
        c_3D    = new TCanvas("c_3D","c_3D",10,10,800,800);
        Draw_STAR_3D();
    }
    //-------------------------------------------------


    Long64_t start_event_use = startevent;
    Long64_t stop_event_use  = stopevent;
    if(stop_event_use > nentries) stop_event_use = nentries;

    Int_t idx_HFT_det, idx_PXL_in_out_IST;

    Long64_t N_plot_counter = 0;


    for(Long64_t jentry = start_event_use; jentry < stop_event_use; jentry++)
    //for(Long64_t jentry = 0; jentry < 1; jentry++)
    {

        if (!fChain->GetEntry( jentry )) // take the event -> information is stored in event
            break;  // end of data chunk

        if (jentry != 0  &&  jentry % 10 == 0)
            cout << "." << flush;
        if (jentry != 0  &&  jentry % 100 == 0)
        {
            if((stop_event_use-start_event_use) > 0)
            {
                Double_t event_percent = 100.0*((Double_t)(jentry-start_event_use))/((Double_t)(stop_event_use-start_event_use));
                cout << " " << jentry << " (" << event_percent << "%) " << "\n" << "==> Processing data (HFT) " << flush;
            }
        }

        if(fEvtHdr_fField != 0)
        {
            //cout << "WARNING! Magnetic field is not 0, value = " << fEvtHdr_fField << endl;
            if(MagnetOnOff == 0) fEvtHdr_fField = 0.0;
        }

        //cout << "fMatchHits_ = " << fMatchHits_ << ", fEvtHdr_fField = " << fEvtHdr_fField
        //    << ", fNtrack = " << fNtrack << ", fNPTracks = " << fNPTracks << endl;


        Long64_t good_HFT_hit = 0;
        for(Int_t i_hit = 0; i_hit < ((Int_t)fNhit); i_hit++)
        {
            if(fHits_nRawHits[i_hit] > 1) good_HFT_hit++;
        }

        //cout << "Event number = " << jentry << ", number of tracks = " << fNtrack << ", number of HFT hits = " << fNhit << ", good_HFT_hit = " << good_HFT_hit << endl;

        StPhysicalHelixD helixA, helixAB[2];
        StThreeVectorF vec_gMom, vec_Origin;


        //-------------------------------------------------------------------------------------------
        // QA TPC histograms
        if( ((Int_t)fNtrack) == 2)
        {
            StThreeVectorF dir_vec[2], pos_vec[2], dca_vec[2];
            for(Int_t i_track = 0; i_track < ((Int_t)fNtrack); i_track++)
            {
                Float_t charge = ((Float_t)fTracks_fNpoint[i_track])/fabs((Float_t)fTracks_fNpoint[i_track]);
                vec_gMom.set(fTracks_fPxDca[i_track],fTracks_fPyDca[i_track],fTracks_fPzDca[i_track]);
                vec_Origin.set(fTracks_fOriginXDca[i_track],fTracks_fOriginYDca[i_track],fTracks_fOriginZDca[i_track]);
                helixAB[i_track] = StPhysicalHelixD(vec_gMom,vec_Origin,fEvtHdr_fField*MAGFIELDFACTOR,charge);

                StThreeVectorF center_vec;
                Double_t pathLength;
                if(i_track == 0) center_vec.set(0.0,0.0,0.0);
                if(i_track == 1) center_vec = pos_vec[0];
                pathLength = helixAB[i_track].pathLength(center_vec);
                dir_vec[i_track]    = helixAB[i_track].cat(pathLength);
                pos_vec[i_track]    = helixAB[i_track].at(pathLength);

                dca_vec[i_track]    = pos_vec[i_track];
                dca_vec[i_track]   -= center_vec;
            }


            Double_t diff_angle = dir_vec[0].angle(dir_vec[1]);
            Double_t phiA       = dir_vec[0].phi();
            Double_t phiB       = dir_vec[1].phi();
            Double_t thetaA     = dir_vec[0].theta();
            Double_t thetaB     = dir_vec[1].theta();

            h_diff_angle   ->Fill(diff_angle);
            h_phiA         ->Fill(phiA);
            h_phiB         ->Fill(phiB);
            h_thetaA       ->Fill(thetaA);
            h_thetaB       ->Fill(thetaB);
            h_x_dcaA       ->Fill(dca_vec[0].x());
            h_y_dcaA       ->Fill(dca_vec[0].y());
            h_z_dcaA       ->Fill(dca_vec[0].z());
            h_x_dcaB       ->Fill(dca_vec[1].x());
            h_y_dcaB       ->Fill(dca_vec[1].y());
            h_z_dcaB       ->Fill(dca_vec[1].z());
        }
        //-------------------------------------------------------------------------------------------


        // TPC track loop
        for(Int_t i_track = 0; i_track < ((Int_t)fNtrack); i_track++)
        {
            Double_t p_total = TMath::Sqrt(fTracks_fPx[i_track]*fTracks_fPx[i_track] + fTracks_fPy[i_track]*fTracks_fPy[i_track] + fTracks_fPz[i_track]*fTracks_fPz[i_track]);
            if(i_track == 0)
            {
                h_cosmic_momentum ->Fill(p_total);
            }

            //cout << "i_track = " << i_track << ", px = " << fTracks_fPx[i_track]  << ", py = " << fTracks_fPy[i_track]
            //    << ", pz = " << fTracks_fPz[i_track] << ", p_total = " << p_total << ", fNpoint = " << fTracks_fNpoint[i_track] << endl;

            Float_t charge = ((Float_t)fTracks_fNpoint[i_track])/fabs((Float_t)fTracks_fNpoint[i_track]);

            vec_gMom.set(fTracks_fPxDca[i_track],fTracks_fPyDca[i_track],fTracks_fPzDca[i_track]);
            vec_Origin.set(fTracks_fOriginXDca[i_track],fTracks_fOriginYDca[i_track],fTracks_fOriginZDca[i_track]);
            helixA = StPhysicalHelixD(vec_gMom,vec_Origin,fEvtHdr_fField*MAGFIELDFACTOR,charge);

            Double_t TH_vec_pos[3] = {vec_Origin.x(),vec_Origin.y(),vec_Origin.z()};
            Double_t TH_vec_dir[3] = {vec_gMom.x(),vec_gMom.y(),vec_gMom.z()};
            Double_t TH_ang_freq   = 1.0*fEvtHdr_fField*MAGFIELDFACTOR;
            THelix ThelixA(TH_vec_pos,TH_vec_dir,TH_ang_freq);

            //cout << "charge = " << charge << ", fEvtHdr_fField = " << fEvtHdr_fField << ", r = " << helixA.at(0).mag() << endl;
            //cout << "px = " << fTracks_fPx[i_track] << ", py = " << fTracks_fPy[i_track] << ", pz = " << fTracks_fPz[i_track] << endl;
            //cout << "ox = " << fTracks_fOriginX[i_track] << ", oy = " << fTracks_fOriginY[i_track] << ", oz = " << fTracks_fOriginZ[i_track] << endl;

            if(0)
            {
                Draw_Helix_3D(helixA,kBlue+1,1,1,0.0,200.0,100,4.0); // helix, color, style, width, min_r, max_r, helix points,  step_size (cm)
                Draw_Helix_3D(helixA,kBlue+1,2,2,0.0,200.0,100,-4.0); // helix, color, style, width, min_r, max_r, helix points,  step_size (cm)
            }

            StThreeVectorF pixel_hit, helix_point_at_dca, diff_vector;

            // First pixel hit loop
            Long64_t pixel_hit_match_counter = 0;
            for(Int_t i_hit = 0; i_hit < ((Int_t)fNhit); i_hit++)
            {
                if(fHits_Id[i_hit] >= 2000) continue;

                pixel_hit.set(fHits_xG[i_hit],fHits_yG[i_hit],fHits_zG[i_hit]);


                if(anamode == 2)
                {
                    fprintf(fPXL_tables,"%f, %f, %f, %d\n",fHits_xG[i_hit],fHits_yG[i_hit],fHits_zG[i_hit],fHits_Id[i_hit]);
                }

                //if(pixel_hit.perp() > 9.5) cout << "HFT hit radius = " << pixel_hit.perp() << ", fHits_Id = " << fHits_Id[i_hit] << ", fHits_fUniqueID = " << fHits_fUniqueID[i_hit] << endl;
                //if(fHits_Id[i_hit] == 262)
                h_HFT_hits_local     ->Fill(fHits_xL[i_hit],fHits_zL[i_hit]);
                h_HFT_hits_global[0] ->Fill(fHits_xG[i_hit],fHits_yG[i_hit]);

                if(
                   get_HFT_det_index(fHits_Id[i_hit],idx_HFT_det,idx_PXL_in_out_IST) == 1
                  )
                {
                    h_HFT_hits_global[1] ->Fill(fHits_xG[i_hit],fHits_yG[i_hit]);
                }
                if(
                   get_HFT_det_index(fHits_Id[i_hit],idx_HFT_det,idx_PXL_in_out_IST) == 2
                  )
                {
                    h_HFT_hits_global[2] ->Fill(fHits_xG[i_hit],fHits_yG[i_hit]);
                }
                if(
                   get_HFT_det_index(fHits_Id[i_hit],idx_HFT_det,idx_PXL_in_out_IST) == 5
                  )
                {
                    h_HFT_hits_global[3] ->Fill(fHits_xG[i_hit],fHits_yG[i_hit]);
                }


                if(fHits_Id[i_hit] >= 0 && fHits_Id[i_hit] < N_max_sensors)
                {
                    h_HFT_hits_local_X[fHits_Id[i_hit]][0]->Fill(fHits_xL[i_hit]);
                    h_HFT_hits_local_Z[fHits_Id[i_hit]][0]->Fill(fHits_zL[i_hit]);

                    //if(idx_PXL_in_out_IST == 2) cout << "i_hit = " << i_hit << ", x_val = " << fHits_xL[i_hit] << ", z_val = " << fHits_zL[i_hit] << endl;
                }

                if(h_HFT_hits_local_Z[fHits_Id[i_hit]][1]->GetBinContent(h_HFT_hits_local_Z[fHits_Id[i_hit]][1]->FindBin(fHits_zL[i_hit])) < hit_threshold)
                {
                    h_HFT_hits_local_X[fHits_Id[i_hit]][2]->Fill(fHits_xL[i_hit]);
                      h_HFT_hits_local_Z[fHits_Id[i_hit]][2]->Fill(fHits_zL[i_hit]);
                }



                if(anamode == 0)
                {
                    Float_t pathA,dcaA;
                    fHelixAtoPointdca(pixel_hit,helixA,pathA,dcaA);

                    //cout << "i_hit = " << i_hit << ", x_hit = " << fHits_xG[i_hit] << ", y_hit = " << fHits_yG[i_hit] << ", z_hit = " << fHits_zG[i_hit] << ", dcaA = " << dcaA << endl;


                    if(dcaA < 4.0)
                    {
                        if(
                           //h_HFT_hits_local_Z[fHits_Id[i_hit]][1]->GetBinContent(h_HFT_hits_local_Z[fHits_Id[i_hit]][1]->FindBin(fHits_zL[i_hit])) < hit_threshold
                           fHits_nRawHits[i_hit] > 1
                           || idx_PXL_in_out_IST == 2
                          )
                        {
                            if(
                               1
                               //h_HFT_hits_local_X[fHits_Id[i_hit]][3]->GetBinContent(h_HFT_hits_local_X[fHits_Id[i_hit]][3]->FindBin(fHits_xL[i_hit])) < hit_threshold
                               //idx_PXL_in_out_IST == 2
                              )
                            {
                                pixel_hit_match_counter++;
                            }
                        }
                    }
                }
            }
            //cout << "pixel_hit_match_counter = " << pixel_hit_match_counter << endl;


            //-----------------------------------------------------------------
            // 3D plotting
            if(pixel_hit_match_counter > 3 && fNtrack == 2 && i_track == 0 && plotmode == 1)
            {
                //cout << "event = " << jentry << ", i_track = " << i_track << ", N_plot_counter = " << N_plot_counter << endl;

                if(N_plot_counter == 3)
                {
                    // Draw first helix
                    Draw_Helix_3D(helixA,kBlue+1,1,1,0.0,200.0,100,4.0); // helix, color, style, width, min_r, max_r, helix points,  step_size (cm)
                    Draw_Helix_3D(helixA,kBlue+1,2,2,0.0,200.0,100,-4.0); // helix, color, style, width, min_r, max_r, helix points,  step_size (cm)

                    //ThelixA.SetLineColor(2);
                    //ThelixA.Draw();

                    // HFT hit loop
                    for(Int_t i_hit = 0; i_hit < ((Int_t)fNhit); i_hit++)
                    {
                        if(fHits_Id[i_hit] >= 2000) continue;

                        get_HFT_det_index(fHits_Id[i_hit],idx_HFT_det,idx_PXL_in_out_IST);

                        // Remove noise from Z
                        if(
                           fHits_nRawHits[i_hit] > 1
                           //h_HFT_hits_local_Z[fHits_Id[i_hit]][1]->GetBinContent(h_HFT_hits_local_Z[fHits_Id[i_hit]][1]->FindBin(fHits_zL[i_hit])) < hit_threshold
                           || idx_PXL_in_out_IST == 2
                          )
                        {
                            // Remove noise from X
                            if(
                               1
                               //h_HFT_hits_local_X[fHits_Id[i_hit]][3]->GetBinContent(h_HFT_hits_local_X[fHits_Id[i_hit]][3]->FindBin(fHits_xL[i_hit])) < hit_threshold
                               //|| idx_PXL_in_out_IST == 2
                              )
                            {
                                // Set pixel hit vector
                                pixel_hit.set(fHits_xG[i_hit],fHits_yG[i_hit],fHits_zG[i_hit]);

                                // Calculate dca to helix
                                Float_t pathA,dcaA;
                                fHelixAtoPointdca(pixel_hit,helixA,pathA,dcaA);

                                // Cut on dca to helix
                                if(dcaA < 3.0)
                                {
                                    PM_Pixel_hit.SetNextPoint(fHits_xG[i_hit],fHits_yG[i_hit],fHits_zG[i_hit]);
                                }
                            }
                        }
                    }

                    PM_Pixel_hit.SetMarkerStyle(20);
                    PM_Pixel_hit.SetMarkerSize(0.8);
                    PM_Pixel_hit.SetMarkerColor(2);
                    PM_Pixel_hit.DrawClone();

                    // Calculate 2nd helix and draw it
                    Float_t charge = ((Float_t)fTracks_fNpoint[i_track+1])/fabs((Float_t)fTracks_fNpoint[i_track+1]);
                    vec_gMom.set(fTracks_fPxDca[i_track+1],fTracks_fPyDca[i_track+1],fTracks_fPzDca[i_track+1]);
                    vec_Origin.set(fTracks_fOriginXDca[i_track+1],fTracks_fOriginYDca[i_track+1],fTracks_fOriginZDca[i_track+1]);
                    helixA = StPhysicalHelixD(vec_gMom,vec_Origin,fEvtHdr_fField*MAGFIELDFACTOR,charge);
                    Draw_Helix_3D(helixA,kBlue+1,1,1,0.0,200.0,100,4.0); // helix, color, style, width, min_r, max_r, helix points,  step_size (cm)
                    Draw_Helix_3D(helixA,kBlue+1,2,2,0.0,200.0,100,-4.0); // helix, color, style, width, min_r, max_r, helix points,  step_size (cm)
                }

                N_plot_counter++;
            }
            //-----------------------------------------------------------------



            if(anamode == 0)
            {
                //-----------------------------------------------------------------
                // Selecting good pixel hits -> pixel matching

                if(pixel_hit_match_counter > 3 && fNtrack == 2 && i_track == 0) // make sure that there are enough correlations and use only the first TPC track
                {
                    //cout << "Pixel matching" << endl;
                    // Vectors explicit for PXL left to right
                    std::vector< std::vector<StThreeVectorF> > HFT_hits_vector;
                    std::vector< std::vector<Int_t> > HFT_hits_id_vector;

                    HFT_hits_vector.resize(6); // [inner pixel left, outer pixel left, IST left, inner pixel right, outer pixel right, IST right]
                    HFT_hits_id_vector.resize(6); // [inner pixel left, outer pixel left, IST left, inner pixel right, outer pixel right, IST right]

                    // Vectors for PXL + IST, any combination of sectors and halfs
                    std::vector< std::vector<StThreeVectorF> > PXL_IST_hits_vector;
                    std::vector< std::vector<Int_t> > PXL_IST_hits_id_vector;
                    std::vector< std::vector<UInt_t> > PXL_IST_hits_cluster_size;

                    PXL_IST_hits_vector.resize(3); // [inner PXL, outer PXL, IST]
                    PXL_IST_hits_id_vector.resize(3); // [inner PXL, outer PXL, IST]
                    PXL_IST_hits_cluster_size.resize(3); // [inner PXL, outer PXL, IST]

                    // HFT hit loop
                    for(Int_t i_hit = 0; i_hit < ((Int_t)fNhit); i_hit++)
                    {
                        if(fHits_Id[i_hit] >= 2000) continue;

                        Int_t HFT_det_index = get_HFT_det_index(fHits_Id[i_hit],idx_HFT_det,idx_PXL_in_out_IST);
                        //cout << "i_hit = " << i_hit << ", idx_PXL_in_out_IST = " << idx_PXL_in_out_IST << endl;

                        // Remove noise from Z
                        if(
                           fHits_nRawHits[i_hit] > 1
                           //h_HFT_hits_local_Z[fHits_Id[i_hit]][1]->GetBinContent(h_HFT_hits_local_Z[fHits_Id[i_hit]][1]->FindBin(fHits_zL[i_hit])) < hit_threshold
                           || idx_PXL_in_out_IST == 2
                          )
                        {
                            // Remove noise from X
                            if(
                               1
                               //h_HFT_hits_local_X[fHits_Id[i_hit]][3]->GetBinContent(h_HFT_hits_local_X[fHits_Id[i_hit]][3]->FindBin(fHits_xL[i_hit])) < hit_threshold
                               //|| idx_PXL_in_out_IST == 2
                              )
                            {
                                // Set pixel hit vector
                                pixel_hit.set(fHits_xG[i_hit],fHits_yG[i_hit],fHits_zG[i_hit]);

                                // Calculate dca to helix
                                Float_t pathA,dcaA;
                                fHelixAtoPointdca(pixel_hit,helixA,pathA,dcaA);

                                // Cut on dca to helix
                                if(dcaA < 4.0)
                                {
                                    if(HFT_det_index >= 0 && HFT_det_index <= 5)
                                    {
                                        StThreeVectorF HFT_single_hit;
                                        HFT_single_hit.set(fHits_xG[i_hit],fHits_yG[i_hit],fHits_zG[i_hit]);

                                        HFT_hits_vector[HFT_det_index].push_back(HFT_single_hit);
                                        HFT_hits_id_vector[HFT_det_index].push_back(fHits_Id[i_hit]);

                                        if(idx_PXL_in_out_IST >= 0 && idx_PXL_in_out_IST < 3)
                                        {
                                            PXL_IST_hits_vector[idx_PXL_in_out_IST].push_back(HFT_single_hit);
                                            PXL_IST_hits_id_vector[idx_PXL_in_out_IST].push_back(fHits_Id[i_hit]);
                                            PXL_IST_hits_cluster_size[idx_PXL_in_out_IST].push_back(fHits_nRawHits[i_hit]);
                                        }
                                    }
                                }
                            }
                        }
                    }


                    //----------------------------------------------------------------------------------------
                    //cout << "Fill PXL vectors" << endl;
                    // Fill vectors explicit for PXL left to right
                    if( // check that we have at least one pixel hit in each inner/outer for left/right
                       HFT_hits_vector[0].size() > 0 && // inner left
                       HFT_hits_vector[1].size() > 0 && // outer left
                       HFT_hits_vector[3].size() > 0 && // inner right
                       HFT_hits_vector[4].size() > 0    // outer right
                      )
                    {
                        StThreeVectorF base_L, base_IR, base_OR, dir_L, dir_R, dist_vec_IR, dist_vec_OR;
                        for(Int_t i_hit_IL = 0; i_hit_IL < HFT_hits_vector[0].size(); i_hit_IL++)
                        {
                            for(Int_t i_hit_OL = 0; i_hit_OL < HFT_hits_vector[1].size(); i_hit_OL++)
                            {
                                base_L =  HFT_hits_vector[0][i_hit_IL];
                                dir_L  =  HFT_hits_vector[0][i_hit_IL];
                                dir_L  -= HFT_hits_vector[1][i_hit_OL];
                                for(Int_t i_hit_IR = 0; i_hit_IR < HFT_hits_vector[3].size(); i_hit_IR++)
                                {
                                    base_IR =  HFT_hits_vector[3][i_hit_IR];
                                    Double_t dca_IR = calculateMinimumDistanceStraightToPoint(base_L,dir_L,base_IR); // base,dir,point
                                    for(Int_t i_hit_OR = 0; i_hit_OR < HFT_hits_vector[4].size(); i_hit_OR++)
                                    {
                                        base_OR =  HFT_hits_vector[4][i_hit_OR];

                                        Double_t dca_OR = calculateMinimumDistanceStraightToPoint(base_L,dir_L,base_OR); // base,dir,point
                                        dist_vec_OR = calculateDCA_vec_StraightToPoint(base_L,dir_L,base_OR); // base,dir,point

                                        Double_t dca_IR = calculateMinimumDistanceStraightToPoint(base_L,dir_L,base_IR); // base,dir,point
                                        dist_vec_IR = calculateDCA_vec_StraightToPoint(base_L,dir_L,base_IR); // base,dir,point

                                        dir_R  =  HFT_hits_vector[3][i_hit_IR];
                                        dir_R  -= HFT_hits_vector[4][i_hit_OR];

                                        StThreeVectorF vec_dca = calculatePointOfClosestApproach(base_L,dir_L,base_IR,dir_R);

                                        if(dca_IR < 1.0)
                                        {
                                            for(Int_t i_xyz = 0; i_xyz < 3; i_xyz++)
                                            {
                                                h_pixel_line_dca[0][i_xyz]->Fill(dist_vec_OR[i_xyz]);
                                            }

                                            NT_HFT_zero_field_cosmic_array[0]  = HFT_hits_vector[0][i_hit_IL].x();
                                            NT_HFT_zero_field_cosmic_array[1]  = HFT_hits_vector[0][i_hit_IL].y();
                                            NT_HFT_zero_field_cosmic_array[2]  = HFT_hits_vector[0][i_hit_IL].z();
                                            NT_HFT_zero_field_cosmic_array[3]  = HFT_hits_vector[1][i_hit_OL].x();
                                            NT_HFT_zero_field_cosmic_array[4]  = HFT_hits_vector[1][i_hit_OL].y();
                                            NT_HFT_zero_field_cosmic_array[5]  = HFT_hits_vector[1][i_hit_OL].z();
                                            NT_HFT_zero_field_cosmic_array[6]  = HFT_hits_vector[3][i_hit_IR].x();
                                            NT_HFT_zero_field_cosmic_array[7]  = HFT_hits_vector[3][i_hit_IR].y();
                                            NT_HFT_zero_field_cosmic_array[8]  = HFT_hits_vector[3][i_hit_IR].z();
                                            NT_HFT_zero_field_cosmic_array[9]  = HFT_hits_vector[4][i_hit_OR].x();
                                            NT_HFT_zero_field_cosmic_array[10] = HFT_hits_vector[4][i_hit_OR].y();
                                            NT_HFT_zero_field_cosmic_array[11] = HFT_hits_vector[4][i_hit_OR].z();
                                            NT_HFT_zero_field_cosmic_array[12] = HFT_hits_id_vector[0][i_hit_IL];
                                            NT_HFT_zero_field_cosmic_array[13] = HFT_hits_id_vector[1][i_hit_OL];
                                            NT_HFT_zero_field_cosmic_array[14] = HFT_hits_id_vector[3][i_hit_IR];
                                            NT_HFT_zero_field_cosmic_array[15] = HFT_hits_id_vector[4][i_hit_OR];

                                            NT_HFT_zero_field_cosmic->Fill(NT_HFT_zero_field_cosmic_array);
                                        }
                                        for(Int_t i_xyz = 0; i_xyz < 3; i_xyz++)
                                        {
                                            if(dca_IR < 0.5)   h_pixel_line_dca[1][i_xyz]->Fill(dist_vec_OR[i_xyz]);
                                            if(dca_IR < 0.25)  h_pixel_line_dca[2][i_xyz]->Fill(dist_vec_OR[i_xyz]);
                                            if(dca_IR < 0.1)   h_pixel_line_dca[3][i_xyz]->Fill(dist_vec_OR[i_xyz]);
                                            if(dca_IR < 0.5 && dca_OR < 0.5) h_pixel_line_dca[4][i_xyz]->Fill(dist_vec_OR[i_xyz]);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    //----------------------------------------------------------------------------------------



                    //----------------------------------------------------------------------------------------
                    //cout << "Fill PXL + IST vectors" << endl;
                    // Fill vectors for PXL + IST, any combination of sectors and halfs
                    if( // check that we have at least two pixel hits in inner/outer
                       PXL_IST_hits_vector[0].size()    > 1 // inner PXL
                       && PXL_IST_hits_vector[1].size() > 1 // outer PXL

                       && PXL_IST_hits_vector[0].size() < 50  // inner PXL
                       && PXL_IST_hits_vector[1].size() < 50  // outer PXL
                      )
                    {
                        StThreeVectorF base_O, dir_O, dist_vec_IA, dist_vec_IB, dist_vec_IST, base_IA, base_IB, base_IST;
                        for(Int_t i_hit_OA = 0; i_hit_OA < PXL_IST_hits_vector[1].size(); i_hit_OA++) // outer PXL hit loop A
                        {
                            for(Int_t i_hit_OB = (i_hit_OA+1); i_hit_OB < PXL_IST_hits_vector[1].size(); i_hit_OB++) // outer PXL hit loop B
                            {
                                base_O =  PXL_IST_hits_vector[1][i_hit_OA];
                                dir_O  =  PXL_IST_hits_vector[1][i_hit_OA];
                                dir_O  -= PXL_IST_hits_vector[1][i_hit_OB];

                                for(Int_t i_hit_IA = 0; i_hit_IA < PXL_IST_hits_vector[0].size(); i_hit_IA++) // inner PXL loop A
                                {
                                    base_IA =  PXL_IST_hits_vector[0][i_hit_IA];
                                    Double_t dca_IA = calculateMinimumDistanceStraightToPoint(base_O,dir_O,base_IA); // base,dir,point
                                    dist_vec_IA = calculateDCA_vec_StraightToPoint(base_O,dir_O,base_IA); // base,dir,point

                                    for(Int_t i_hit_IB = (i_hit_IA+1); i_hit_IB < PXL_IST_hits_vector[0].size(); i_hit_IB++) // inner PXL loop B
                                    {
                                        base_IB =  PXL_IST_hits_vector[0][i_hit_IB];
                                        Double_t dca_IB = calculateMinimumDistanceStraightToPoint(base_O,dir_O,base_IB); // base,dir,point
                                        dist_vec_IB = calculateDCA_vec_StraightToPoint(base_O,dir_O,base_IB); // base,dir,point

                                        if(dca_IA < 0.3 && dca_IB < 0.3
                                           && PXL_IST_hits_cluster_size[0][i_hit_IA] > 1
                                           && PXL_IST_hits_cluster_size[0][i_hit_IB] > 1
                                           && PXL_IST_hits_cluster_size[1][i_hit_OA] > 1
                                           && PXL_IST_hits_cluster_size[1][i_hit_OB] > 1
                                          )
                                        {
                                            Int_t IST_hit_counter = -1; // later on important to not double count PXL tracks
                                            Int_t N_IST_hits      = 1; // make sure to save PXL hits if there is no IST hit
                                            Int_t IST_flag        = 0; // no IST hit flag

                                            //cout << "N_IST hits in event correlated with TPC track = " << PXL_IST_hits_vector[2].size() << endl;

                                            if(PXL_IST_hits_vector[2].size() > 0) // IST
                                            {
                                                N_IST_hits = PXL_IST_hits_vector[2].size();
                                                IST_flag = 1; // IST hit(s)
                                            }

                                            for(Int_t i_hit_IST = 0; i_hit_IST < N_IST_hits; i_hit_IST++) // IST loop
                                            {
                                                Int_t acc_IST_hit = 0;

                                                Double_t IST_hit_x  = 0.0;
                                                Double_t IST_hit_y  = 0.0;
                                                Double_t IST_hit_z  = 0.0;
                                                Int_t    IST_hit_id = -1;

                                                if(IST_flag) // if there is a IST hit
                                                {
                                                    base_IST =  PXL_IST_hits_vector[2][i_hit_IST];
                                                    Double_t dca_IST = calculateMinimumDistanceStraightToPoint(base_O,dir_O,base_IST); // base,dir,point
                                                    dist_vec_IST = calculateDCA_vec_StraightToPoint(base_O,dir_O,base_IST); // base,dir,point
                                                    if(dca_IST < 1.0)
                                                    {
                                                        acc_IST_hit = 1;
                                                        IST_hit_counter++;

                                                        IST_hit_x  = PXL_IST_hits_vector[2][i_hit_IST].x();
                                                        IST_hit_y  = PXL_IST_hits_vector[2][i_hit_IST].y();
                                                        IST_hit_z  = PXL_IST_hits_vector[2][i_hit_IST].z();
                                                        IST_hit_id = PXL_IST_hits_id_vector[2][i_hit_IST];
                                                    }
                                                }

                                                //cout << "acc_IST_hit = " << acc_IST_hit << endl;

                                                if(i_hit_IST == 0 || (i_hit_IST > 0 && acc_IST_hit == 1) )  // always save the first one and then only good matched ones
                                                {
                                                    // PXL information


                                                    HFT_tree_main.PXL_IA_x   = PXL_IST_hits_vector[0][i_hit_IA].x();
                                                    HFT_tree_main.PXL_IA_y   = PXL_IST_hits_vector[0][i_hit_IA].y();
                                                    HFT_tree_main.PXL_IA_z   = PXL_IST_hits_vector[0][i_hit_IA].z();
                                                    HFT_tree_main.PXL_IB_x   = PXL_IST_hits_vector[0][i_hit_IB].x();
                                                    HFT_tree_main.PXL_IB_y   = PXL_IST_hits_vector[0][i_hit_IB].y();
                                                    HFT_tree_main.PXL_IB_z   = PXL_IST_hits_vector[0][i_hit_IB].z();
                                                    HFT_tree_main.PXL_OA_x   = PXL_IST_hits_vector[1][i_hit_OA].x();
                                                    HFT_tree_main.PXL_OA_y   = PXL_IST_hits_vector[1][i_hit_OA].y();
                                                    HFT_tree_main.PXL_OA_z   = PXL_IST_hits_vector[1][i_hit_OA].z();
                                                    HFT_tree_main.PXL_OB_x   = PXL_IST_hits_vector[1][i_hit_OB].x();
                                                    HFT_tree_main.PXL_OB_y   = PXL_IST_hits_vector[1][i_hit_OB].y();
                                                    HFT_tree_main.PXL_OB_z   = PXL_IST_hits_vector[1][i_hit_OB].z();
                                                    HFT_tree_main.PXL_id_IA  = PXL_IST_hits_id_vector[0][i_hit_IA];
                                                    HFT_tree_main.PXL_id_IB  = PXL_IST_hits_id_vector[0][i_hit_IB];
                                                    HFT_tree_main.PXL_id_OA  = PXL_IST_hits_id_vector[1][i_hit_OA];
                                                    HFT_tree_main.PXL_id_OB  = PXL_IST_hits_id_vector[1][i_hit_OB];

                                                    // IST information
                                                    HFT_tree_main.IST_x       = IST_hit_x;
                                                    HFT_tree_main.IST_y       = IST_hit_y;
                                                    HFT_tree_main.IST_z       = IST_hit_z;
                                                    HFT_tree_main.IST_id      = IST_hit_id;
                                                    HFT_tree_main.IST_counter = IST_hit_counter;
                                                    HFT_tree_main.IST_hit     = i_hit_IST;

                                                    // Track information
                                                    HFT_tree_main.magfac = fEvtHdr_fField*MAGFIELDFACTOR;
                                                    HFT_tree_main.qA     = ((Float_t)fTracks_fNpoint[0])/fabs((Float_t)fTracks_fNpoint[0]); // charge
                                                    HFT_tree_main.pxA    = fTracks_fPxDca[0];
                                                    HFT_tree_main.pyA    = fTracks_fPyDca[0];
                                                    HFT_tree_main.pzA    = fTracks_fPzDca[0];
                                                    HFT_tree_main.oxA    = fTracks_fOriginXDca[0];
                                                    HFT_tree_main.oyA    = fTracks_fOriginYDca[0];
                                                    HFT_tree_main.ozA    = fTracks_fOriginZDca[0];
                                                    HFT_tree_main.qB     = ((Float_t)fTracks_fNpoint[1])/fabs((Float_t)fTracks_fNpoint[1]); // charge
                                                    HFT_tree_main.pxB    = fTracks_fPxDca[1];
                                                    HFT_tree_main.pyB    = fTracks_fPyDca[1];
                                                    HFT_tree_main.pzB    = fTracks_fPzDca[1];
                                                    HFT_tree_main.oxB    = fTracks_fOriginXDca[1];
                                                    HFT_tree_main.oyB    = fTracks_fOriginYDca[1];
                                                    HFT_tree_main.ozB    = fTracks_fOriginZDca[1];

                                                    for(Int_t i_TPC_hit = 0; i_TPC_hit < n_TPC_hits; i_TPC_hit++)
                                                    {
                                                        TPC_hitA_X[i_TPC_hit]      = fTracks_fTpcHitX[0][i_TPC_hit];
                                                        TPC_hitA_Y[i_TPC_hit]      = fTracks_fTpcHitY[0][i_TPC_hit];
                                                        TPC_hitA_Z[i_TPC_hit]      = fTracks_fTpcHitZ[0][i_TPC_hit];

                                                        TPC_errA_X[i_TPC_hit]      = fTracks_fTpcHitEX[0][i_TPC_hit];
                                                        TPC_errA_Y[i_TPC_hit]      = fTracks_fTpcHitEY[0][i_TPC_hit];
                                                        TPC_errA_Z[i_TPC_hit]      = fTracks_fTpcHitEZ[0][i_TPC_hit];

                                                        TPC_hitA_sector[i_TPC_hit] = fTracks_fTpcHitSector[0][i_TPC_hit];
                                                        TPC_hitA_padrow[i_TPC_hit] = fTracks_fTpcHitPadrow[0][i_TPC_hit];
                                                        TPC_hitA_pad[i_TPC_hit]    = fTracks_fTpcHitPad[0][i_TPC_hit];

                                                        TPC_hitB_X[i_TPC_hit]      = fTracks_fTpcHitX[1][i_TPC_hit];
                                                        TPC_hitB_Y[i_TPC_hit]      = fTracks_fTpcHitY[1][i_TPC_hit];
                                                        TPC_hitB_Z[i_TPC_hit]      = fTracks_fTpcHitZ[1][i_TPC_hit];

                                                        TPC_errB_X[i_TPC_hit]      = fTracks_fTpcHitEX[1][i_TPC_hit];
                                                        TPC_errB_Y[i_TPC_hit]      = fTracks_fTpcHitEY[1][i_TPC_hit];
                                                        TPC_errB_Z[i_TPC_hit]      = fTracks_fTpcHitEZ[1][i_TPC_hit];

                                                        TPC_hitB_sector[i_TPC_hit] = fTracks_fTpcHitSector[1][i_TPC_hit];
                                                        TPC_hitB_padrow[i_TPC_hit] = fTracks_fTpcHitPadrow[1][i_TPC_hit];
                                                        TPC_hitB_pad[i_TPC_hit]    = fTracks_fTpcHitPad[1][i_TPC_hit];

                                                    }

                                                    cout << "IA = {" << PXL_IST_hits_vector[0][i_hit_IA].x() << ", " << PXL_IST_hits_vector[0][i_hit_IA].y() << ", " << PXL_IST_hits_vector[0][i_hit_IA].z() << "}, "
                                                        << "IB = {" << PXL_IST_hits_vector[0][i_hit_IB].x() << ", " << PXL_IST_hits_vector[0][i_hit_IB].y() << ", " << PXL_IST_hits_vector[0][i_hit_IB].z() << "}, "
                                                        << "OA = {" << PXL_IST_hits_vector[1][i_hit_OA].x() << ", " << PXL_IST_hits_vector[1][i_hit_OA].y() << ", " << PXL_IST_hits_vector[1][i_hit_OA].z() << "}, "
                                                        << "OB = {" << PXL_IST_hits_vector[1][i_hit_OB].x() << ", " << PXL_IST_hits_vector[1][i_hit_OB].y() << ", " << PXL_IST_hits_vector[1][i_hit_OB].z() << "}, "
                                                        ", i_hit_IST = " << i_hit_IST << ", IST_hit_counter = " << IST_hit_counter << ", dca_IA =" << dca_IA
                                                        << ", dca_IB =" << dca_IB << ", event = " << jentry << ", i_track = " << i_track <<
                                                        ", N_IA = " << PXL_IST_hits_vector[1].size() << ", N_IB = " << PXL_IST_hits_vector[0].size() << ", N_IST = " << N_IST_hits << endl;

                                                    Tree_PXL_IST_zero_field_cosmic->Fill();
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    //----------------------------------------------------------------------------------------



                    //cout << "N_hits = " << HFT_hits_vector[0].size() << endl;
                }
                //-----------------------------------------------------------------



                //-----------------------------------------------------------------
                // Second pixel hit loop
                for(Int_t i_hit = 0; i_hit < ((Int_t)fNhit); i_hit++)
                {
                    if(fHits_Id[i_hit] >= 2000) continue;

                    pixel_hit.set(fHits_xG[i_hit],fHits_yG[i_hit],fHits_zG[i_hit]);
                    Float_t pathA,dcaA;
                    fHelixAtoPointdca(pixel_hit,helixA,pathA,dcaA);
                    helix_point_at_dca = helixA.at(pathA);
                    diff_vector = helix_point_at_dca;
                    diff_vector -= pixel_hit;

                    get_HFT_det_index(fHits_Id[i_hit],idx_HFT_det,idx_PXL_in_out_IST);

                    for(Int_t i_xyz = 0; i_xyz < 3; i_xyz++)
                    {
                        h_pixel_dca[0][i_xyz]->Fill(diff_vector[i_xyz]);
                        if(
                           fHits_nRawHits[i_hit] > 1
                           //h_HFT_hits_local_Z[fHits_Id[i_hit]][1]->GetBinContent(h_HFT_hits_local_Z[fHits_Id[i_hit]][1]->FindBin(fHits_zL[i_hit])) < hit_threshold
                           || idx_PXL_in_out_IST == 2
                          )
                        {
                            h_pixel_dca[1][i_xyz]->Fill(diff_vector[i_xyz]);
                            if(
                               1
                               //h_HFT_hits_local_X[fHits_Id[i_hit]][3]->GetBinContent(h_HFT_hits_local_X[fHits_Id[i_hit]][3]->FindBin(fHits_xL[i_hit])) < hit_threshold
                               //|| idx_PXL_in_out_IST == 2
                              )
                            {
                                h_pixel_dca[2][i_xyz]->Fill(diff_vector[i_xyz]);
                                if(
                                   pixel_hit_match_counter > 1
                                  )
                                {
                                    h_pixel_dca[3][i_xyz]->Fill(diff_vector[i_xyz]);
                                    if(
                                       fabs(diff_vector[0]) < 2.0 &&
                                       fabs(diff_vector[2]) < 4.0
                                      )
                                    {
                                        h_pixel_dca[7][i_xyz]->Fill(diff_vector[i_xyz]);
                                    }
                                    if(
                                       pixel_hit_match_counter > 2
                                      )
                                    {
                                        h_pixel_dca[4][i_xyz]->Fill(diff_vector[i_xyz]);
                                        if(
                                           pixel_hit_match_counter > 3
                                          )
                                        {
                                            h_pixel_dca[5][i_xyz]->Fill(diff_vector[i_xyz]);
                                            if(
                                               pixel_hit_match_counter > 4
                                              )
                                            {
                                                h_pixel_dca[6][i_xyz]->Fill(diff_vector[i_xyz]);
                                                if(
                                                   fabs(diff_vector[0]) < 1.0
                                                  )
                                                {
                                                    h_pixel_dca[8][i_xyz]->Fill(diff_vector[i_xyz]);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                //-----------------------------------------------------------------
            }


        }

        if(0)
        {
            // HFT hit loop
            for(Int_t i_hit = 0; i_hit < ((Int_t)fNhit); i_hit++)
            {
                PM_Pixel_hit.SetNextPoint(fHits_xG[i_hit],fHits_yG[i_hit],fHits_zG[i_hit]);
            }

            PM_Pixel_hit.SetMarkerStyle(20);
            PM_Pixel_hit.SetMarkerSize(0.8);
            PM_Pixel_hit.SetMarkerColor(2);
            PM_Pixel_hit.DrawClone();
        }

    }
}

void StReadHFTtrees::Finish()
{
    cout << "Finish started" << endl;

    //------------------------------------------------------------------------------------
    HistName = "c_pixel_dca";
    TCanvas* c_pixel_dca = new TCanvas(HistName.Data(),HistName.Data(),10,10,1300,700);
    c_pixel_dca->SetFillColor(10);
    c_pixel_dca->SetTopMargin(0.05);
    c_pixel_dca->SetBottomMargin(0.15);
    c_pixel_dca->SetRightMargin(0.05);
    c_pixel_dca->SetLeftMargin(0.15);
    c_pixel_dca->Divide(3,2);

    TString xyz_label[3] = {"#Delta x (cm)","#Delta y (cm)","#Delta z (cm)"};
    for(Int_t i_cut = 0; i_cut < 2; i_cut++)
    {
        for(Int_t i_xyz = 0; i_xyz < 3; i_xyz++)
        {
            c_pixel_dca->cd((i_xyz+i_cut*3)+1)->SetTopMargin(0.2);
            c_pixel_dca->cd((i_xyz+i_cut*3)+1)->SetBottomMargin(0.22);
            c_pixel_dca->cd((i_xyz+i_cut*3)+1)->SetRightMargin(0.05);
            c_pixel_dca->cd((i_xyz+i_cut*3)+1)->SetLeftMargin(0.2);
            c_pixel_dca->cd((i_xyz+i_cut*3)+1)->SetTicks(1,1);
            c_pixel_dca->cd((i_xyz+i_cut*3)+1)->SetGrid(0,0);
            c_pixel_dca->cd((i_xyz+i_cut*3)+1)->SetFillColor(10);

            h_pixel_dca[i_cut][i_xyz]->SetStats(0);
            h_pixel_dca[i_cut][i_xyz]->SetTitle("");
            h_pixel_dca[i_cut][i_xyz]->GetXaxis()->SetTitleOffset(0.85);
            h_pixel_dca[i_cut][i_xyz]->GetYaxis()->SetTitleOffset(1.0);
            h_pixel_dca[i_cut][i_xyz]->GetYaxis()->SetLabelOffset(0.01);
            h_pixel_dca[i_cut][i_xyz]->GetXaxis()->SetLabelSize(0.055);
            h_pixel_dca[i_cut][i_xyz]->GetYaxis()->SetLabelSize(0.055);
            h_pixel_dca[i_cut][i_xyz]->GetXaxis()->SetTitleSize(0.055);
            h_pixel_dca[i_cut][i_xyz]->GetYaxis()->SetTitleSize(0.055);
            h_pixel_dca[i_cut][i_xyz]->GetXaxis()->SetNdivisions(505,'N');
            h_pixel_dca[i_cut][i_xyz]->GetYaxis()->SetNdivisions(505,'N');
            h_pixel_dca[i_cut][i_xyz]->GetXaxis()->CenterTitle();
            h_pixel_dca[i_cut][i_xyz]->GetYaxis()->CenterTitle();
            h_pixel_dca[i_cut][i_xyz]->GetXaxis()->SetTitle(xyz_label[i_xyz]);
            h_pixel_dca[i_cut][i_xyz]->GetYaxis()->SetTitle("counts");
            h_pixel_dca[i_cut][i_xyz]->DrawCopy("");

            if(i_cut == 1)
            {
                h_pixel_dca[2][i_xyz]->SetLineColor(2);
                //h_pixel_dca[2][i_xyz]->DrawCopy("same h");

                h_pixel_dca[3][i_xyz]->SetLineColor(kAzure+2);
                h_pixel_dca[3][i_xyz]->DrawCopy("same h");

                h_pixel_dca[4][i_xyz]->SetLineColor(kAzure+2);
                //h_pixel_dca[4][i_xyz]->DrawCopy("same h");

                h_pixel_dca[5][i_xyz]->SetLineColor(kGreen+2);
                //h_pixel_dca[5][i_xyz]->DrawCopy("same h");

                h_pixel_dca[6][i_xyz]->SetLineColor(kOrange-2);
                //h_pixel_dca[6][i_xyz]->DrawCopy("same h");

                h_pixel_dca[7][i_xyz]->SetLineColor(kOrange+1);
                h_pixel_dca[7][i_xyz]->DrawCopy("same h");

                h_pixel_dca[8][i_xyz]->SetLineColor(kOrange+1);
                //h_pixel_dca[8][i_xyz]->DrawCopy("same h");
            }
        }
    }
    //------------------------------------------------------------------------------------



    //------------------------------------------------------------------------------------
    HistName = "c_pixel_line_dca";
    TCanvas* c_pixel_line_dca = new TCanvas(HistName.Data(),HistName.Data(),10,10,1300,700);
    c_pixel_line_dca->SetFillColor(10);
    c_pixel_line_dca->SetTopMargin(0.05);
    c_pixel_line_dca->SetBottomMargin(0.15);
    c_pixel_line_dca->SetRightMargin(0.05);
    c_pixel_line_dca->SetLeftMargin(0.15);
    c_pixel_line_dca->Divide(3,2);

    Int_t tc_pixel_line_dca[5] = {kGray+2,kRed+1,kAzure+2,kOrange+2,kGreen+2};
    for(Int_t i_cut = 0; i_cut < 5; i_cut++)
    {
        for(Int_t i_xyz = 0; i_xyz < 3; i_xyz++)
        {
            h_pixel_line_dca[i_cut][i_xyz]->SetLineColor(tc_pixel_line_dca[i_cut]);
        }
    }

    for(Int_t i_cut = 0; i_cut < 1; i_cut++)
    {
        for(Int_t i_xyz = 0; i_xyz < 3; i_xyz++)
        {
            c_pixel_line_dca->cd((i_xyz+i_cut*3)+1)->SetTopMargin(0.2);
            c_pixel_line_dca->cd((i_xyz+i_cut*3)+1)->SetBottomMargin(0.22);
            c_pixel_line_dca->cd((i_xyz+i_cut*3)+1)->SetRightMargin(0.05);
            c_pixel_line_dca->cd((i_xyz+i_cut*3)+1)->SetLeftMargin(0.2);
            c_pixel_line_dca->cd((i_xyz+i_cut*3)+1)->SetTicks(1,1);
            c_pixel_line_dca->cd((i_xyz+i_cut*3)+1)->SetGrid(0,0);
            c_pixel_line_dca->cd((i_xyz+i_cut*3)+1)->SetFillColor(10);

            h_pixel_line_dca[i_cut][i_xyz]->SetStats(0);
            h_pixel_line_dca[i_cut][i_xyz]->SetTitle("");
            h_pixel_line_dca[i_cut][i_xyz]->GetXaxis()->SetTitleOffset(0.85);
            h_pixel_line_dca[i_cut][i_xyz]->GetYaxis()->SetTitleOffset(1.0);
            h_pixel_line_dca[i_cut][i_xyz]->GetYaxis()->SetLabelOffset(0.01);
            h_pixel_line_dca[i_cut][i_xyz]->GetXaxis()->SetLabelSize(0.055);
            h_pixel_line_dca[i_cut][i_xyz]->GetYaxis()->SetLabelSize(0.055);
            h_pixel_line_dca[i_cut][i_xyz]->GetXaxis()->SetTitleSize(0.055);
            h_pixel_line_dca[i_cut][i_xyz]->GetYaxis()->SetTitleSize(0.055);
            h_pixel_line_dca[i_cut][i_xyz]->GetXaxis()->SetNdivisions(505,'N');
            h_pixel_line_dca[i_cut][i_xyz]->GetYaxis()->SetNdivisions(505,'N');
            h_pixel_line_dca[i_cut][i_xyz]->GetXaxis()->CenterTitle();
            h_pixel_line_dca[i_cut][i_xyz]->GetYaxis()->CenterTitle();
            h_pixel_line_dca[i_cut][i_xyz]->GetXaxis()->SetTitle(xyz_label[i_xyz]);
            h_pixel_line_dca[i_cut][i_xyz]->GetYaxis()->SetTitle("counts");
            if(i_cut == 0) h_pixel_line_dca[i_cut][i_xyz]->DrawCopy("");
            h_pixel_line_dca[1][i_xyz]->DrawCopy("same");
            h_pixel_line_dca[2][i_xyz]->DrawCopy("same");
            h_pixel_line_dca[3][i_xyz]->DrawCopy("same");

            c_pixel_line_dca->cd((i_xyz+1*3)+1)->SetTopMargin(0.2);
            c_pixel_line_dca->cd((i_xyz+1*3)+1)->SetBottomMargin(0.22);
            c_pixel_line_dca->cd((i_xyz+1*3)+1)->SetRightMargin(0.05);
            c_pixel_line_dca->cd((i_xyz+1*3)+1)->SetLeftMargin(0.2);
            c_pixel_line_dca->cd((i_xyz+1*3)+1)->SetTicks(1,1);
            c_pixel_line_dca->cd((i_xyz+1*3)+1)->SetGrid(0,0);
            c_pixel_line_dca->cd((i_xyz+1*3)+1)->SetFillColor(10);
            h_pixel_line_dca[4][i_xyz]->DrawCopy("");
        }
    }
    //------------------------------------------------------------------------------------



    //------------------------------------------------------------------------------------
    HistName = "c_HFT_hits_local";
    TCanvas* c_HFT_hits_local = new TCanvas(HistName.Data(),HistName.Data(),10,10,900,700);
    c_HFT_hits_local->SetFillColor(10);
    c_HFT_hits_local->SetTopMargin(0.2);
    c_HFT_hits_local->SetBottomMargin(0.22);
    c_HFT_hits_local->SetRightMargin(0.2);
    c_HFT_hits_local->SetLeftMargin(0.15);
    c_HFT_hits_local->Divide(2,2);

    c_HFT_hits_local->cd(1)->SetTicks(1,1);
    c_HFT_hits_local->cd(1)->SetGrid(0,0);
    c_HFT_hits_local->cd(1)->SetFillColor(10);
    c_HFT_hits_local->cd(1)->SetLogz(1);

    h_HFT_hits_local->SetStats(0);
    h_HFT_hits_local->SetTitle("");
    h_HFT_hits_local->GetXaxis()->SetTitleOffset(0.85);
    h_HFT_hits_local->GetYaxis()->SetTitleOffset(0.85);
    h_HFT_hits_local->GetYaxis()->SetLabelOffset(0.01);
    h_HFT_hits_local->GetXaxis()->SetLabelSize(0.055);
    h_HFT_hits_local->GetYaxis()->SetLabelSize(0.055);
    h_HFT_hits_local->GetXaxis()->SetTitleSize(0.055);
    h_HFT_hits_local->GetYaxis()->SetTitleSize(0.055);
    h_HFT_hits_local->GetXaxis()->SetNdivisions(505,'N');
    h_HFT_hits_local->GetYaxis()->SetNdivisions(505,'N');
    h_HFT_hits_local->GetXaxis()->CenterTitle();
    h_HFT_hits_local->GetYaxis()->CenterTitle();
    h_HFT_hits_local->GetXaxis()->SetTitle("x_{local} (cm)");
    h_HFT_hits_local->GetYaxis()->SetTitle("z_{local} (cm)");
    h_HFT_hits_local->DrawCopy("colz");

    c_HFT_hits_local->cd(2)->SetTicks(1,1);
    c_HFT_hits_local->cd(2)->SetGrid(0,0);
    c_HFT_hits_local->cd(2)->SetFillColor(10);
    c_HFT_hits_local->cd(2)->SetLogz(1);

    h_HFT_hits_local_Z[262][0]->SetStats(0);
    h_HFT_hits_local_Z[262][0]->SetTitle("");
    h_HFT_hits_local_Z[262][0]->GetXaxis()->SetTitleOffset(0.85);
    h_HFT_hits_local_Z[262][0]->GetYaxis()->SetTitleOffset(0.85);
    h_HFT_hits_local_Z[262][0]->GetYaxis()->SetLabelOffset(0.01);
    h_HFT_hits_local_Z[262][0]->GetXaxis()->SetLabelSize(0.055);
    h_HFT_hits_local_Z[262][0]->GetYaxis()->SetLabelSize(0.055);
    h_HFT_hits_local_Z[262][0]->GetXaxis()->SetTitleSize(0.055);
    h_HFT_hits_local_Z[262][0]->GetYaxis()->SetTitleSize(0.055);
    h_HFT_hits_local_Z[262][0]->GetXaxis()->SetNdivisions(505,'N');
    h_HFT_hits_local_Z[262][0]->GetYaxis()->SetNdivisions(505,'N');
    h_HFT_hits_local_Z[262][0]->GetXaxis()->CenterTitle();
    h_HFT_hits_local_Z[262][0]->GetYaxis()->CenterTitle();
    h_HFT_hits_local_Z[262][0]->GetXaxis()->SetTitle("z_{local} (cm)");
    h_HFT_hits_local_Z[262][0]->GetYaxis()->SetTitle("counts");
    h_HFT_hits_local_Z[262][0]->DrawCopy("");

    c_HFT_hits_local->cd(3)->SetTicks(1,1);
    c_HFT_hits_local->cd(3)->SetGrid(0,0);
    c_HFT_hits_local->cd(3)->SetFillColor(10);
    c_HFT_hits_local->cd(3)->SetLogz(1);

    h_HFT_hits_local_X[262][0]->SetStats(0);
    h_HFT_hits_local_X[262][0]->SetTitle("");
    h_HFT_hits_local_X[262][0]->GetXaxis()->SetTitleOffset(0.85);
    h_HFT_hits_local_X[262][0]->GetYaxis()->SetTitleOffset(0.85);
    h_HFT_hits_local_X[262][0]->GetYaxis()->SetLabelOffset(0.01);
    h_HFT_hits_local_X[262][0]->GetXaxis()->SetLabelSize(0.055);
    h_HFT_hits_local_X[262][0]->GetYaxis()->SetLabelSize(0.055);
    h_HFT_hits_local_X[262][0]->GetXaxis()->SetTitleSize(0.055);
    h_HFT_hits_local_X[262][0]->GetYaxis()->SetTitleSize(0.055);
    h_HFT_hits_local_X[262][0]->GetXaxis()->SetNdivisions(505,'N');
    h_HFT_hits_local_X[262][0]->GetYaxis()->SetNdivisions(505,'N');
    h_HFT_hits_local_X[262][0]->GetXaxis()->CenterTitle();
    h_HFT_hits_local_X[262][0]->GetYaxis()->CenterTitle();
    h_HFT_hits_local_X[262][0]->GetXaxis()->SetTitle("x_{local} (cm)");
    h_HFT_hits_local_X[262][0]->GetYaxis()->SetTitle("counts");
    h_HFT_hits_local_X[262][0]->DrawCopy("");
    //------------------------------------------------------------------------------------



    //------------------------------------------------------------------------------------
    HistName = "c_HFT_hits_global";
    TCanvas* c_HFT_hits_global = new TCanvas(HistName.Data(),HistName.Data(),10,10,900,700);
    c_HFT_hits_global->SetFillColor(10);
    c_HFT_hits_global->SetTopMargin(0.2);
    c_HFT_hits_global->SetBottomMargin(0.22);
    c_HFT_hits_global->SetRightMargin(0.2);
    c_HFT_hits_global->SetLeftMargin(0.15);
    c_HFT_hits_global->Divide(2,2);

    for(Int_t i_glob = 0; i_glob < 4; i_glob++)
    {
        c_HFT_hits_global->cd(i_glob+1)->SetTicks(1,1);
        c_HFT_hits_global->cd(i_glob+1)->SetGrid(0,0);
        c_HFT_hits_global->cd(i_glob+1)->SetFillColor(10);
        c_HFT_hits_global->cd(i_glob+1)->SetBottomMargin(0.22);
        c_HFT_hits_global->cd(i_glob+1)->SetRightMargin(0.12);
        c_HFT_hits_global->cd(i_glob+1)->SetLogz(1);
        h_HFT_hits_global[i_glob]->SetStats(0);
        h_HFT_hits_global[i_glob]->SetTitle("");
        h_HFT_hits_global[i_glob]->GetXaxis()->SetTitleOffset(0.85);
        h_HFT_hits_global[i_glob]->GetYaxis()->SetTitleOffset(0.85);
        h_HFT_hits_global[i_glob]->GetYaxis()->SetLabelOffset(0.01);
        h_HFT_hits_global[i_glob]->GetXaxis()->SetLabelSize(0.055);
        h_HFT_hits_global[i_glob]->GetYaxis()->SetLabelSize(0.055);
        h_HFT_hits_global[i_glob]->GetXaxis()->SetTitleSize(0.055);
        h_HFT_hits_global[i_glob]->GetYaxis()->SetTitleSize(0.055);
        h_HFT_hits_global[i_glob]->GetXaxis()->SetNdivisions(505,'N');
        h_HFT_hits_global[i_glob]->GetYaxis()->SetNdivisions(505,'N');
        h_HFT_hits_global[i_glob]->GetXaxis()->CenterTitle();
        h_HFT_hits_global[i_glob]->GetYaxis()->CenterTitle();
        h_HFT_hits_global[i_glob]->GetXaxis()->SetTitle("x_{global} (cm)");
        h_HFT_hits_global[i_glob]->GetYaxis()->SetTitle("y_{global} (cm)");
        h_HFT_hits_global[i_glob]->DrawCopy("colz");
    }
    //------------------------------------------------------------------------------------



    //------------------------------------------------------------------------------------
    cout << "Create output file" << endl;
    Outputfile->cd();

    if(SaveTree == 0)
    {
        h_diff_angle    ->Write();
        h_phiA          ->Write();
        h_phiB          ->Write();
        h_thetaA        ->Write();
        h_thetaB        ->Write();
        h_x_dcaA        ->Write();
        h_y_dcaA        ->Write();
        h_z_dcaA        ->Write();
        h_x_dcaB        ->Write();
        h_y_dcaB        ->Write();
        h_z_dcaB        ->Write();

        NT_HFT_zero_field_cosmic       ->Write();
        h_cosmic_momentum              ->Write();
        for(Int_t i_sensor = 0; i_sensor < N_max_sensors; i_sensor++)
        {
            for(Int_t i_corr_loop = 0; i_corr_loop < N_corr_loops; i_corr_loop++)
            {
                h_HFT_hits_local_X[i_sensor][i_corr_loop] ->Write();
                h_HFT_hits_local_Z[i_sensor][i_corr_loop] ->Write();
            }
        }
    }
    Tree_PXL_IST_zero_field_cosmic ->Write();
    //Outputfile->Close();
    //------------------------------------------------------------------------------------



}

