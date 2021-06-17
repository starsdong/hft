#include "StTrackingHFT.h"

#include "StTrackingHFT_Func.h"
//#include "/u/aschmah/STAR/Analysis/HFT/StRoot/StHFTtree/StHFTtree.h"

static const Double_t MAGFIELDFACTOR = kilogauss;

// Fixed size dimensions of array or collections stored in the TTree if any.
const Int_t kMaxfVertices = 35;
const Int_t kMaxfTracks = 2131;
const Int_t kMaxfHits = 19563;
const Int_t kMaxfMatchHits = 1;
const Int_t N_max_sensors  = 1500;

const Double_t hit_threshold = 600;

static TString HistName;

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
static Float_t NT_HFT_zero_field_cosmic_array[16];
static Float_t NT_PXL_IST_zero_field_cosmic_array[37];

static const Int_t N_dca_bins = 5; // order of closest hits to track
static TH1F* h_dca_track_PXL[3][N_dca_bins]; // [inner PXL, outer PXL, IST][dca bin]
static TH2F* h_dca_Delta_dca_track_PXL[3][N_dca_bins-1]; // [inner PXL, outer PXL, IST][Delta dca bin]
static TH2F* h_HFT_phi_vs_z[3];
static TH2F* h_nHFT_vs_nTPC[3];

TH3D* h_3D_dummy;
TCanvas* c_3D;

ClassImp(StTrackingHFT)
    //_____________________________________________________________________________
    StTrackingHFT::StTrackingHFT() {

    }
//_____________________________________________________________________________
StTrackingHFT::~StTrackingHFT() {

}

void StTrackingHFT::Init()
{
    cout << "Init started" << endl;

    NT_HFT_zero_field_cosmic     = new TNtuple("NT_HFT_zero_field_cosmic","NT_HFT_zero_field_cosmic Ntuple","x1:y1:z1:x2:y2:z2:x3:y3:z3:x4:y4:z4:id_IL:id_OL:id_IR:id_OR");
    NT_PXL_IST_zero_field_cosmic = new TNtuple("NT_PXL_IST_zero_field_cosmic","NT_PXL_IST_zero_field_cosmic Ntuple","PXL_IA_x:PXL_IA_y:PXL_IA_z:PXL_IB_x:PXL_IB_y:PXL_IB_z:PXL_OA_x:PXL_OA_y:PXL_OA_z:PXL_OB_x:PXL_OB_y:PXL_OB_z:PXL_id_IA:PXL_id_IB:PXL_id_OA:PXL_id_OB:IST_x:IST_y:IST_z:IST_id:IST_counter:IST_hit:magfac:qA:pxA:pyA:pzA:oxA:oyA:ozA:qB:pxB:pyB:pzB:oxB:oyB:ozB");

    for(Int_t i_layer = 0; i_layer < 3; i_layer++)
    {
        HistName = "h_HFT_phi_vs_z_layer_";
        HistName += i_layer;
        h_HFT_phi_vs_z[i_layer] = new TH2F(HistName.Data(),HistName.Data(),700,-35,35,3000,-TMath::Pi()*1.0,TMath::Pi()*1.0);

        HistName = "h_nHFT_vs_nTPC_layer_";
        HistName += i_layer;
        h_nHFT_vs_nTPC[i_layer] = new TH2F(HistName.Data(),HistName.Data(),200,0,2500,400,0,10000);

        for(Int_t i_dca = 0; i_dca < N_dca_bins; i_dca++)
        {
            HistName = "h_dca_track_PXL_layer_";
            HistName += i_layer;
            HistName += "_dca_";
            HistName += i_dca;
            h_dca_track_PXL[i_layer][i_dca] = new TH1F(HistName.Data(),HistName.Data(),100,0,2);

            if(i_dca < (N_dca_bins-1))
            {
                HistName = "h_dca_Delta_dca_track_PXL_";
                HistName += i_layer;
                HistName += "_dca_";
                HistName += i_dca;
                h_dca_Delta_dca_track_PXL[i_layer][i_dca] = new TH2F(HistName.Data(),HistName.Data(),100,0,2,200,0,2);
            }
        }
    }


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

void StTrackingHFT::Make()
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
    Double32_t      fTracks_fNsigmaPi[kMaxfTracks];   //[fTracks_]
    Double32_t      fTracks_fDca2D[kMaxfTracks];   //[fTracks_]
    Double32_t      fTracks_fDca3D[kMaxfTracks];   //[fTracks_]
    UInt_t          fTracks_fPxlHitPattern[kMaxfTracks];   //[fTracks_]
    UInt_t          fTracks_fIstHitPattern[kMaxfTracks];   //[fTracks_]
    UInt_t          fTracks_fSsdHitPattern[kMaxfTracks];   //[fTracks_]
    Double32_t      fTracks_fPxlHitX[kMaxfTracks][3];   //[fTracks_]
    Double32_t      fTracks_fPxlHitY[kMaxfTracks][3];   //[fTracks_]
    Double32_t      fTracks_fPxlHitZ[kMaxfTracks][3];   //[fTracks_]
    Double32_t      fTracks_fIstHitX[kMaxfTracks][2];   //[fTracks_]
    Double32_t      fTracks_fIstHitY[kMaxfTracks][2];   //[fTracks_]
    Double32_t      fTracks_fIstHitZ[kMaxfTracks][2];   //[fTracks_]
    Double32_t      fTracks_fSsdHitX[kMaxfTracks][2];   //[fTracks_]
    Double32_t      fTracks_fSsdHitY[kMaxfTracks][2];   //[fTracks_]
    Double32_t      fTracks_fSsdHitZ[kMaxfTracks][2];   //[fTracks_]
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
    TBranch        *b_fTracks_fNsigmaPi;   //!
    TBranch        *b_fTracks_fDca2D;   //!
    TBranch        *b_fTracks_fDca3D;   //!
    TBranch        *b_fTracks_fPxlHitPattern;   //!
    TBranch        *b_fTracks_fIstHitPattern;   //!
    TBranch        *b_fTracks_fSsdHitPattern;   //!
    TBranch        *b_fTracks_fPxlHitX;   //!
    TBranch        *b_fTracks_fPxlHitY;   //!
    TBranch        *b_fTracks_fPxlHitZ;   //!
    TBranch        *b_fTracks_fIstHitX;   //!
    TBranch        *b_fTracks_fIstHitY;   //!
    TBranch        *b_fTracks_fIstHitZ;   //!
    TBranch        *b_fTracks_fSsdHitX;   //!
    TBranch        *b_fTracks_fSsdHitY;   //!
    TBranch        *b_fTracks_fSsdHitZ;   //!
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

    if(filenumber == 0)
    {
        f = TFile::Open("/star/institutions/lbl_prod/dongx/hft/Run14/AuAu200/Tracking_TPC/output/15081042/0/Event_15081042_raw_1500022.root");
    }

    if(!f || !f->IsOpen())
    {
        cout << "WARNING: File cannot be opened" << endl;
        if(filenumber == 0) f = new TFile("/star/institutions/lbl_prod/dongx/hft/Run14/AuAu200/Tracking_TPC/output/15081042/0/Event_15081042_raw_1500022.root");
    }
    f->GetObject("T",tree);


    cout << "Open HFT sensor hit histogram loop 1" << endl;
    //corr_in_file = TFile::Open("/star/institutions/lbl/aschmah/HFT/Cosmics/Corrections/HFT_local_hit_histograms_loop_cut300.root");
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
    fChain->SetBranchAddress("fTracks.fNsigmaPi", fTracks_fNsigmaPi, &b_fTracks_fNsigmaPi);
    fChain->SetBranchAddress("fTracks.fDca2D", fTracks_fDca2D, &b_fTracks_fDca2D);
    fChain->SetBranchAddress("fTracks.fDca3D", fTracks_fDca3D, &b_fTracks_fDca3D);
    fChain->SetBranchAddress("fTracks.fPxlHitPattern", fTracks_fPxlHitPattern, &b_fTracks_fPxlHitPattern);
    fChain->SetBranchAddress("fTracks.fIstHitPattern", fTracks_fIstHitPattern, &b_fTracks_fIstHitPattern);
    fChain->SetBranchAddress("fTracks.fSsdHitPattern", fTracks_fSsdHitPattern, &b_fTracks_fSsdHitPattern);
    fChain->SetBranchAddress("fTracks.fPxlHitX[3]", fTracks_fPxlHitX, &b_fTracks_fPxlHitX);
    fChain->SetBranchAddress("fTracks.fPxlHitY[3]", fTracks_fPxlHitY, &b_fTracks_fPxlHitY);
    fChain->SetBranchAddress("fTracks.fPxlHitZ[3]", fTracks_fPxlHitZ, &b_fTracks_fPxlHitZ);
    fChain->SetBranchAddress("fTracks.fIstHitX[2]", fTracks_fIstHitX, &b_fTracks_fIstHitX);
    fChain->SetBranchAddress("fTracks.fIstHitY[2]", fTracks_fIstHitY, &b_fTracks_fIstHitY);
    fChain->SetBranchAddress("fTracks.fIstHitZ[2]", fTracks_fIstHitZ, &b_fTracks_fIstHitZ);
    fChain->SetBranchAddress("fTracks.fSsdHitX[2]", fTracks_fSsdHitX, &b_fTracks_fSsdHitX);
    fChain->SetBranchAddress("fTracks.fSsdHitY[2]", fTracks_fSsdHitY, &b_fTracks_fSsdHitY);
    fChain->SetBranchAddress("fTracks.fSsdHitZ[2]", fTracks_fSsdHitZ, &b_fTracks_fSsdHitZ);
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
    fChain->SetBranchAddress("fMatchHits.fUniqueID", &fMatchHits_fUniqueID, &b_fMatchHits_fUniqueID);
    fChain->SetBranchAddress("fMatchHits.fBits", &fMatchHits_fBits, &b_fMatchHits_fBits);
    fChain->SetBranchAddress("fMatchHits.start", &fMatchHits_start, &b_fMatchHits_start);
    fChain->SetBranchAddress("fMatchHits.index2Track", &fMatchHits_index2Track, &b_fMatchHits_index2Track);
    fChain->SetBranchAddress("fMatchHits.index2Hit", &fMatchHits_index2Hit, &b_fMatchHits_index2Hit);
    fChain->SetBranchAddress("fMatchHits.detId", &fMatchHits_detId, &b_fMatchHits_detId);
    fChain->SetBranchAddress("fMatchHits.nRawHits", &fMatchHits_nRawHits, &b_fMatchHits_nRawHits);
    fChain->SetBranchAddress("fMatchHits.xGP", &fMatchHits_xGP, &b_fMatchHits_xGP);
    fChain->SetBranchAddress("fMatchHits.yGP", &fMatchHits_yGP, &b_fMatchHits_yGP);
    fChain->SetBranchAddress("fMatchHits.zGP", &fMatchHits_zGP, &b_fMatchHits_zGP);
    fChain->SetBranchAddress("fMatchHits.xLP", &fMatchHits_xLP, &b_fMatchHits_xLP);
    fChain->SetBranchAddress("fMatchHits.yLP", &fMatchHits_yLP, &b_fMatchHits_yLP);
    fChain->SetBranchAddress("fMatchHits.zLP", &fMatchHits_zLP, &b_fMatchHits_zLP);
    fChain->SetBranchAddress("fMatchHits.tuP", &fMatchHits_tuP, &b_fMatchHits_tuP);
    fChain->SetBranchAddress("fMatchHits.tvP", &fMatchHits_tvP, &b_fMatchHits_tvP);
    fChain->SetBranchAddress("fMatchHits.xG", &fMatchHits_xG, &b_fMatchHits_xG);
    fChain->SetBranchAddress("fMatchHits.yG", &fMatchHits_yG, &b_fMatchHits_yG);
    fChain->SetBranchAddress("fMatchHits.zG", &fMatchHits_zG, &b_fMatchHits_zG);
    fChain->SetBranchAddress("fMatchHits.xL", &fMatchHits_xL, &b_fMatchHits_xL);
    fChain->SetBranchAddress("fMatchHits.yL", &fMatchHits_yL, &b_fMatchHits_yL);
    fChain->SetBranchAddress("fMatchHits.zL", &fMatchHits_zL, &b_fMatchHits_zL);
    fChain->SetBranchAddress("fMatchHits.pT", &fMatchHits_pT, &b_fMatchHits_pT);
    fChain->SetBranchAddress("fMatchHits.eta", &fMatchHits_eta, &b_fMatchHits_eta);
    fChain->SetBranchAddress("fMatchHits.phi", &fMatchHits_phi, &b_fMatchHits_phi);
    fChain->SetBranchAddress("fMatchHits.ox", &fMatchHits_ox, &b_fMatchHits_ox);
    fChain->SetBranchAddress("fMatchHits.oy", &fMatchHits_oy, &b_fMatchHits_oy);
    fChain->SetBranchAddress("fMatchHits.oz", &fMatchHits_oz, &b_fMatchHits_oz);
    fChain->SetBranchAddress("fMatchHits.npoint", &fMatchHits_npoint, &b_fMatchHits_npoint);
    fChain->SetBranchAddress("fMatchHits.firstPointR", &fMatchHits_firstPointR, &b_fMatchHits_firstPointR);
    fChain->SetBranchAddress("fMatchHits.firstPointZ", &fMatchHits_firstPointZ, &b_fMatchHits_firstPointZ);
    fChain->SetBranchAddress("fMatchHits.end", &fMatchHits_end, &b_fMatchHits_end);
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



    //----------------------------------------------------------------------------------------
    Long64_t N_plot_counter = 0;
    for(Long64_t jentry = start_event_use; jentry < stop_event_use; jentry++)
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
            if(filenumber != 2) fEvtHdr_fField = 0.0;
        }

        //cout << "fMatchHits_ = " << fMatchHits_ << ", fEvtHdr_fField = " << fEvtHdr_fField
        //    << ", fNtrack = " << fNtrack << ", fNPTracks = " << fNPTracks << endl;

        StPhysicalHelixD helixA;
        StThreeVectorF vec_gMom, vec_Origin;
        Int_t idx_HFT_det, idx_PXL_in_out_IST;

        cout << "Event: " << jentry << ", Ntracks: " << fNtrack << ", HFThits: " << fNhit << endl;


        // TPC track loop
        for(Int_t i_track = 0; i_track < ((Int_t)fNtrack); i_track++)
        {
            //if(fNtrack > 200) continue;
            Double_t p_total = TMath::Sqrt(fTracks_fPx[i_track]*fTracks_fPx[i_track] + fTracks_fPy[i_track]*fTracks_fPy[i_track] + fTracks_fPz[i_track]*fTracks_fPz[i_track]);

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

            StThreeVectorF pixel_hit, helix_point_at_dca, diff_vector;

            // First pixel hit loop
            Double_t closest_hit_dcas[3][N_dca_bins];
            std::vector< std::vector<TVector3> > closest_hit_vector;
            closest_hit_vector.resize(3);
            for(Int_t i_layer = 0; i_layer < 3; i_layer++)
            {
                closest_hit_vector[i_layer].resize(N_dca_bins);
                for(Int_t i_dca = 0; i_dca < N_dca_bins; i_dca++)
                {
                    closest_hit_dcas[i_layer][i_dca]   = -1;
                    closest_hit_vector[i_layer][i_dca].SetXYZ(0.0,0.0,0.0);
                }
            }

            Double_t phi_track = TMath::ATan2(fTracks_fOriginY[i_track],fTracks_fOriginX[i_track]);

            Long64_t pixel_hit_match_counter = 0;
            // HFT hit loop
            for(Int_t i_hit = 0; i_hit < ((Int_t)fNhit); i_hit++)
            {
                if(fHits_Id[i_hit] >= 2000) continue;
                //if(fHits_nRawHits[i_hit] <= 1) continue;

                get_HFT_det_index(fHits_Id[i_hit],idx_HFT_det,idx_PXL_in_out_IST);

                pixel_hit.set(fHits_xG[i_hit],fHits_yG[i_hit],fHits_zG[i_hit]);

                Double_t phi_hit   = TMath::ATan2(fHits_yG[i_hit],fHits_xG[i_hit]);
                if(i_track == 0) h_HFT_phi_vs_z[idx_PXL_in_out_IST] ->Fill(fHits_zG[i_hit],phi_hit);

                Double_t Delta_phi_hit_track = fabs(phi_hit - phi_track);
                if(Delta_phi_hit_track > TMath::Pi()) Delta_phi_hit_track -= 2.0*TMath::Pi();
                Delta_phi_hit_track = fabs(Delta_phi_hit_track);
                if(Delta_phi_hit_track > TMath::Pi()/4.0) continue;
                Float_t pathA,dcaA;
                fHelixAtoPointdca(pixel_hit,helixA,pathA,dcaA);
                if(idx_PXL_in_out_IST >= 0 && idx_PXL_in_out_IST < 3)
                {
                    for(Int_t i_dca = 0; i_dca < N_dca_bins; i_dca++)
                    {
                        if(closest_hit_dcas[idx_PXL_in_out_IST][i_dca] < 0.0 || dcaA < closest_hit_dcas[idx_PXL_in_out_IST][i_dca])
                        {
                            //cout << "dcaA = " << dcaA << ", closest_hit_dcas = " << closest_hit_dcas[idx_PXL_in_out_IST][i_dca] << endl;
                            // push all previous values one down
                            for(Int_t i_dcaB = (N_dca_bins-1); i_dcaB > i_dca; i_dcaB--)
                            {
                                closest_hit_dcas[idx_PXL_in_out_IST][i_dcaB]   = closest_hit_dcas[idx_PXL_in_out_IST][i_dcaB-1];
                                closest_hit_vector[idx_PXL_in_out_IST][i_dcaB] = closest_hit_vector[idx_PXL_in_out_IST][i_dcaB-1];
                            }
                            // save new dca
                            closest_hit_dcas[idx_PXL_in_out_IST][i_dca]    = dcaA;
                            closest_hit_vector[idx_PXL_in_out_IST][i_dca].SetXYZ(fHits_xG[i_hit],fHits_yG[i_hit],fHits_zG[i_hit]);
                            //cout << "phi_track = " << phi_track << ", phi_hit = " << phi_hit << endl;
                            break;
                        }
                    }
                    //for(Int_t i_dca = 0; i_dca < N_dca_bins; i_dca++)
                    //{
                    //    cout << "closest_hit_dcas[" << idx_PXL_in_out_IST << "][" << i_dca << "] = " << closest_hit_dcas[idx_PXL_in_out_IST][i_dca] << endl;
                    //}
                }

                //cout << "Event: " << jentry << ", track: " << i_track << "/" << fNtrack << ", i_hit: " << i_hit << "/" << fNhit << ", dca: " << dcaA << endl;

            }

            for(Int_t i_layer = 0; i_layer < 3; i_layer++)
            {
                cout << "Event: " << jentry << ", track: " << i_track << "/" << fNtrack << ", layer: " << i_layer << endl;
                for(Int_t i_dca = 0; i_dca < N_dca_bins; i_dca++)
                {
                    //cout << "dca bin: " << i_dca << ", dca = " << closest_hit_dcas[i_layer][i_dca] << endl;
                    h_dca_track_PXL[i_layer][i_dca] ->Fill(closest_hit_dcas[i_layer][i_dca]);
                    if(i_dca < (N_dca_bins-1))
                    {
                        Double_t Delta_dca = closest_hit_dcas[i_layer][i_dca+1] - closest_hit_dcas[i_layer][0];
                        if(i_dca == 0 && i_layer == 2 && Delta_dca < 0.06)
                        {
                            TVector3 diff_vector;
                            diff_vector = closest_hit_vector[i_layer][i_dca];
                            diff_vector -= closest_hit_vector[i_layer][i_dca+1];
                            Double_t mag_diff_vector = diff_vector.Mag();
#if 0
                            cout << "dca bin: " << i_dca << ", dca(0) = " << closest_hit_dcas[i_layer][i_dca] << ", dca(1) = " << closest_hit_dcas[i_layer][i_dca+1]
                                << ", Delta_dca = " << Delta_dca << ", hit(0) = {" << closest_hit_vector[i_layer][i_dca].X()
                                << ", " << closest_hit_vector[i_layer][i_dca].Y() << ", "
                                << closest_hit_vector[i_layer][i_dca].Z() << "}, hit(1) = {" << closest_hit_vector[i_layer][i_dca+1].X()
                                << ", " << closest_hit_vector[i_layer][i_dca+1].Y() << ", "
                                << closest_hit_vector[i_layer][i_dca+1].Z() << "}, |diff_vector| = " << mag_diff_vector << endl;
#endif
                        }
                        h_dca_Delta_dca_track_PXL[i_layer][i_dca] ->Fill(closest_hit_dcas[i_layer][0],Delta_dca);
                    }
                }
            }

        }

    }
    //----------------------------------------------------------------------------------------



    //----------------------------------------------------------------------------------------
    // Calculate HFT hit to TPC track correlation
    start_event_use = 0;
    stop_event_use  = nentries;
    for(Long64_t jentry = start_event_use; jentry < stop_event_use; jentry++)
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

        Long64_t N_HFT_hits[3] = {0,0,0};
        for(Int_t i_hit = 0; i_hit < ((Int_t)fNhit); i_hit++)
        {
            if(fHits_Id[i_hit] >= 2000) continue;

            get_HFT_det_index(fHits_Id[i_hit],idx_HFT_det,idx_PXL_in_out_IST);
            if(idx_PXL_in_out_IST >= 0 && idx_PXL_in_out_IST < 3)
            {
                N_HFT_hits[idx_PXL_in_out_IST]++;
            }
        }
        for(Int_t i_layer = 0; i_layer < 3; i_layer++)
        {
            h_nHFT_vs_nTPC[i_layer] ->Fill(fNtrack,N_HFT_hits[i_layer]);
        }
    }
    //----------------------------------------------------------------------------------------



}

void StTrackingHFT::Finish()
{
    cout << "Finish started" << endl;

    //------------------------------------------------------------------------------------

    //------------------------------------------------------------------------------------



    //------------------------------------------------------------------------------------
    cout << "Create output file" << endl;
    TFile* Outputfile = new TFile("/star/institutions/lbl/aschmah/HFT/Cosmics/Corrections/HFT_tracking_201.root","RECREATE");
    Outputfile->cd();

    for(Int_t i_layer = 0; i_layer < 3; i_layer++)
    {
        h_HFT_phi_vs_z[i_layer] ->Write();
        h_nHFT_vs_nTPC[i_layer] ->Write();
        for(Int_t i_dca = 0; i_dca < N_dca_bins; i_dca++)
        {
            h_dca_track_PXL[i_layer][i_dca] ->Write();
            if(i_dca < (N_dca_bins-1))
            {
                h_dca_Delta_dca_track_PXL[i_layer][i_dca] ->Write();
            }
        }
    }
    //Outputfile->Close();
    //------------------------------------------------------------------------------------



}

