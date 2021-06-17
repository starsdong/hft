// Many important notes and comments are included with the #define statements at the top of the file.
// The parameters for the detectors are databased in these #define statements.  The list is prioritized
// so the most important items (i.e. most frequently adjusted items) are at the top of the list.
//
// 09/18/2006   You must compile this macro (eg  root4star> .x DetectorDesign.C++ ).  Use 'starnew' or above.
// 
// 10/10/2006   This technique is precise for rays that are traced from the outside, going inwards.  You may
//              also have a dead detector (ie detector that are physically present in the system but which do
//              not make a measurment.)
//
//              For example: "d-pxl2 ist1 ist2 ssd tpc end kBlue" is the command line to enter to learn
//              about the pointing resolution at the outer surface of the PXL.
//
// 12/11/2006   You can now edit this file, recompile, and put additional graphs on the same plots on the screen
//              by restarting the program.  It will automatically continue plotting into the same canvases as
//              before unless you say 'clear'.
//
// To do:       Add the decay of the pion to the efficiency curves.
//
// All other Notes and Comments moved to the bottom of the file because they are getting too long to be shown here.

#define        ParticleMass             1.865       // Mass of the test particle in GeV/c**2 
#define        Luminosity               8.e27       // Luminosity of the beam (RHIC I == 3.e27, RHIC II == 8.e27 )
#define        SigmaD                   20.0        // Size of the interaction diamond (cm) (assume 30/15 cm RHIC I/II)
#define        dNdEtaMinB               170         // Multiplicity per unit Eta  (AuAu MinBias = 170, Central = 700)
#define        dNdEtaCent               700         // Multiplicity per unit Eta  (AuAu MinBias = 170, Central = 700)
#define        CrossSectionMinB         10          // minB Cross section for event under study (AuAu MinBias = 10 Barns)
#define        CrossSectionUPC          3460        // UPC Cross section for event under study (UPC = 3.6 kBarns)
#define        AcceptanceOfTpcSSD       1.00        // Assumed geometric acceptance of the TPC and SSD combined (eg 0.75)
#define        IntegrationTime          0.200       // Integration time for PXL chips (milliseconds) (eg 4.0 or 0.2)
#define        UPCBackgroundMultiplier  1.0         // Increase multiplicity in detector (0.0 to 1.0 * UPCRate ) (eg 1.0)
#define        OtherBackground          0.0         // Increase multiplicity in detector (0.0 to 1.0 * minBias)  (eg 0.0)
#define        SiScaleFactor            0.288       // For scaling Si pad sizes.  (eg 1/root(12) = 0.288, 0.5 fit ITTF )
#define        EfficiencySearchFlag     1           // Define search method. ChiSquare = 1, Simple = 0.  ChiSq is better
#define        RIDICULOUS               99999.99    // A ridiculously large resolution (cm) to flag a dead detector
#define        BFIELD                   0.5         // Tesla  (test data taken at 0.25 Tesla)
#define        AvgRapidity              0.25        // Avg rapidity, MCS calc is a function of crossing angle 
#define        PionMass                 0.140       // Mass of the Pion
#define        KaonMass                 0.494       // Mass of the Kaon
#define        D0Mass                   1.865       // Mass of the D0; if ParticleMass = D0Mass then plot D0 efficiency
                                                    // only important if RPhi and Z resolution are very assymetric (>3:1)
// Most likely Detector parameters you may want to tune are in the block starting here: 

#define        VtxResolution            0.7000      // cm  Test data wants 3 mm vertex constraint
#define        VtxResolutionZ           0.5000      // cm  Test data wants 3 mm vertex constraint

#define        NewVtxResolution         0.0300      // cm  NewVertex to study effect of a refit vertex 
#define        NewVtxResolutionZ        0.0300      // cm  NewVertex to study effect of a refit vertex 

#define        RefitVtxResolution       0.0030      // cm  Refit Vertex to study effect of HiRes refit
#define        RefitVtxResolutionZ      0.0030      // cm  Refit Vertex to study effect of HiRes refit 

#define        BeamPipe1Resolution      RIDICULOUS  // Beampipe is not active as a detector

#define        Pxl1Resolution           0.0021      // cm  30 x 30   micron pixels  (30 old 21 new)
#define        Pxl1ResolutionZ          0.0021      // cm  30 x 30   micron pixels  (30 old 21 new)

#define        Pxl2Resolution           0.0021      // cm  30 x 30   micron pixels  (30 old 21 new)
#define        Pxl2ResolutionZ          0.0021      // cm  30 x 30   micron pixels  (30 old 21 new)

#define        BeamPipe2Resolution      RIDICULOUS  // Beampipe is not active as a detector

#define        Ist1Resolution           0.0600      // cm  (600 by 6000 microns)
#define        Ist1ResolutionZ          0.6000      // cm  

#define        Ist1PrimeResolution      0.6000      //  
#define        Ist1PrimeResolutionZ     0.0600      // 

#define        Ist2Resolution           0.6000      // 
#define        Ist2ResolutionZ          0.0600      // 

#define        Ist2PrimeResolution      0.0600      // 
#define        Ist2PrimeResolutionZ     0.6000      // 

#define        SsdResolution            0.0095      // cm  95 x 4200  microns double sided crossed strips @ 35 mRad  == 95x2700
#define        SsdResolutionZ           0.2700      // cm  95 x 4200  microns double sided crossed strips @ 35 mRad  == 95x2700

#define        IFCResolution            RIDICULOUS  // IFC is not active as a detector

#define        TpcResolution            0.1000      // cm  500 x 1000 microns ... Test data wants 500 or 575-625 microns 
#define        TpcResolutionZ           0.3000      // cm  500 x 1000 microns ... Based on experience with real data

// End of 'most likely' block, but there are more parameters, below.

#define        VtxIndex                 0
#define        BeamPipe1Index           1
#define        Pxl1Index                2
#define        Pxl2Index                3
#define        BeamPipe2Index           4
#define        Ist1Index                5
#define        Ist1PrimeIndex           6
#define        Ist2Index                7
#define        Ist2PrimeIndex           8
#define        SsdIndex                 9
#define        IFCIndex                 10
#define        TpcIndex                 11
#define        VtxThickness             0.0000  // % Radiation Lengths
#define        BeamPipe1Thickness       0.0024  // % Radiation Lengths (as in 0.01 == 1%) (0.0014 old, 0.0021 new)
#define        Pxl1Thickness            0.0050  // % Radiation Lengths ( Sim 0.0032 new Al 0.0037 new Cu 0.0062)  
#define        Pxl2Thickness            0.0050  // % Radiation Lengths ( Sim 0.0032 new Al 0.0037 new Cu 0.0062)
#define        BeamPipe2Thickness       0.0000  // % Radiation Lengths 
#define        Ist1Thickness            0.0150  // % Radiation Lengths 
#define        Ist1PrimeThickness       0.0150  // % Radiation Lengths 
#define        Ist2Thickness            0.0150  // % Radiation Lengths 
#define        Ist2PrimeThickness       0.0150  // % Radiation Lengths 
#define        SsdThickness             0.0100  // % Radiation Lengths
#define        IFCThickness             0.0052  // % Radiation Lengths 
#define        TpcAvgThickness          0.00026 // % Radiation Lengths ... Average per TPC row  (i.e. total/45 )
#define        VtxRadius                0.0     // cm
#define        BeamPipe1Radius          2.05    // cm (2.05 new 1.50 old)
#define        Pxl1Radius               2.5     // cm (2.5  new 1.55 old) JT Test
#define        Pxl2Radius               8.0     // cm (7.0  new 5.00 old 8.0 proposed ) JT Test
#define        BeamPipe2Radius         11.5     // cm (8.55 new 6.05 old 11.5 proposed) JT Test
#define        Ist1Radius              14.0     // cm (12.0 IST, 10.8 SVT, option 9.5 IST)
#define        Ist1PrimeRadius         14.1     // cm (12.1 IST1Prime)
#define        Ist2Radius              17.0     // cm (17.0 IST, 14.5 SVT)
#define        Ist2PrimeRadius         17.1     // cm (17.1 IST2Prime)
#define        SsdRadius               22.0     // cm
#define        IFCRadius               47.25    // cm  Middle-Radius of the IFC ... its about 1.29 cm thick
#define        TpcInnerRadialPitch1     4.8     // cm
#define        TpcInnerRadialPitch8     5.2     // cm
#define        TpcOuterRadialPitch      2.0     // cm
#define        TpcInnerPadWidth         0.285   // cm
#define        TpcOuterPadWidth         0.620   // cm
#define        InnerRows1               8
#define        InnerRows8               5
#define        InnerRows               (InnerRows1+InnerRows8) 
#define        OuterRows               32 
#define        TpcRows                 (InnerRows1 + InnerRows8 + OuterRows) 
#define        RowOneRadius            60.0     // cm
#define        RowEightRadius          93.6     // cm
#define        RowFourteenRadius      127.195   // cm                       

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <Riostream.h>

#include "TROOT.h"
#include "TSystem.h"
#include "TMath.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TMatrixD.h" 
#include "TH1F.h"

Double_t ThetaMCS          ( Double_t mass, Double_t RadLength, Double_t momentum )    ;
Double_t ProbGoodHit       ( Double_t Radius, Double_t SearchRadiusRPhi, Double_t SearchRadiusZ )   ; 
Double_t ProbGoodChiSqHit  ( Double_t Radius, Double_t SearchRadiusRPhi, Double_t SearchRadiusZ )   ; 
Double_t Dist              ( Double_t Z, Double_t Radius ) ;  // Howard W. hit distribution and convolution integral
Double_t HitDensity        ( Double_t Radius )   ;
Double_t UpcHitDensity     ( Double_t Radius )   ;
Double_t IntegratedHitDensity  ( Double_t Multiplicity, Double_t Radius )   ;
Double_t OneEventHitDensity    ( Double_t Multiplicity, Double_t Radius )   ;
Double_t D0IntegratedEfficiency( Double_t pt, Double_t efficiency[2][400] ) ;

void DetectorDesignV5HFT( ) 
{

  TCanvas *c1, *c2, *c3, *c4, *c5 ;    // Canvases for drawing graphs
  TH1F*    yreference             ;    // Temporary storage for the reference array

  // List of detector elements.  Note that 'Detectors' and 'DetectorList' must use the same ordering in the list.
  // These do not need to be global in scope but are at the top of the file so you can easily find and edit them.
  
  Int_t NumberOfDetectors  = 20, NumberOfColours = 8 ;
  
  char *DetectorList[] = {   "vtx",   "d-vtx", "newvtx",  "refitvtx", "pxl1", "d-pxl1",  "pxl2", "d-pxl2",  "ist1",
			     "d-ist1",  "ist1prime", "d-ist1prime",  "ist2",   "d-ist2", "ist2prime", "d-ist2prime",
			     "ssd", "d-ssd",    "tpc", "tpc2" } ;
  
  enum       Detectors   {      vtx ,    d_vtx ,  newvtx ,   refitvtx,  pxl1 ,  d_pxl1 ,   pxl2 ,  d_pxl2 ,   ist1 , 
				d_ist1 ,   ist1prime ,  d_ist1prime ,   ist2 ,   d_ist2 , ist2prime, d_ist2prime,
				ssd ,  d_ssd ,     tpc ,  tpc2  } ;
  
  char   *ColourList[] = { "kWhite", "kBlack", "kRed", "kGreen", "kBlue", "kYellow", "kMagenta", "kCyan" } ; 
  

  Double_t ptdots[80]  = { 0.025, 0.075, 0.125, 0.175, 0.225, 0.275, 0.325, 0.375, 0.425, 0.475, 
			   0.525, 0.575, 0.625, 0.675, 0.725, 0.775, 0.825, 0.875, 0.925, 0.975,
			   1.025, 1.075, 1.125, 1.175, 1.225, 1.275, 1.325, 1.375, 1.425, 1.475, 
			   1.525, 1.575, 1.625, 1.675, 1.725, 1.775, 1.825, 1.875, 1.925, 1.975,
			   2.025, 2.075, 2.125, 2.175, 2.225, 2.275, 2.325, 2.375, 2.425, 2.475, 
			   2.525, 2.575, 2.625, 2.675, 2.725, 2.775, 2.825, 2.875, 2.925, 2.975,
			   3.025, 3.075, 3.125, 3.175, 3.225, 3.275, 3.325, 3.375, 3.425, 3.475, 
			   3.525, 3.575, 3.625, 3.675, 3.725, 3.775, 3.825, 3.875, 3.925, 3.975  } ;
  Double_t pbar[80]    = { 0.0, 0.0, 0.0, 0.0, 7.8, 5.6, 5.0, 4.0, 4.0, 3.5,// TPC momentum resolution data at 0.25 Tesla
			   3.3, 3.1, 3.1, 3.0, 3.0, 2.8, 2.8, 2.8, 2.9, 3.0,
			   3.0, 3.1, 3.0, 3.0, 3.1, 3.1, 3.2, 3.3, 3.4, 3.5,
			   3.5, 3.6, 3.7, 3.8, 3.9, 4.0, 4.1, 4.2, 4.3, 4.4,
			   4.4, 4.5, 4.5, 4.6, 4.6, 4.7, 4.8, 4.8, 4.9, 5.0,
			   5.1, 5.1, 5.2, 5.2, 5.3, 5.3, 5.4, 5.4, 5.5, 5.6,
			   5.6, 5.7, 5.8, 5.8, 5.9, 6.0, 6.1, 6.1, 6.2, 6.3,
			   6.3, 6.4, 6.5, 6.5, 6.6, 6.7, 6.7, 6.8, 6.9, 6.9  } ;
  Double_t pion[80]    = { 0.0, 2.8, 2.6, 2.4, 2.4, 2.3, 2.1, 2.4, 2.4, 2.2,// TPC momentum resolution data at 0.25 Tesla
			   2.4, 2.6, 2.8, 3.0, 2.7, 2.8, 2.7, 3.0, 2.9, 2.9,
			   3.0, 3.1, 2.9, 3.0, 3.1, 3.1, 3.2, 3.3, 3.4, 3.5,
			   3.5, 3.6, 3.7, 3.8, 3.9, 4.0, 4.1, 4.2, 4.3, 4.4,
			   4.4, 4.5, 4.5, 4.6, 4.6, 4.7, 4.8, 4.8, 4.9, 5.0,
			   5.1, 5.1, 5.2, 5.2, 5.3, 5.3, 5.4, 5.4, 5.5, 5.6,
			   5.6, 5.7, 5.8, 5.8, 5.9, 6.0, 6.1, 6.1, 6.2, 6.3,
			   6.3, 6.4, 6.5, 6.5, 6.6, 6.7, 6.7, 6.8, 6.9, 6.9  } ;

  TMatrixD BigA(2,2), LittleA(2,2) ;
  TMatrixD Istar(5,5), D(5,5), DT(5,5), A(5,5), M(5,5) , Work(5,5) ;
  TGraph  *graph, *data1, *data2, *data3, *data4, *data5, *data6, *data7, *data8 ; 
  Double_t Radius[100], Resolution[100], ResolutionZ[100], Thickness[100] ;  
  Double_t xpoint[400], ypoint[400], yprime[400], yprimeZ[400], equivalent[400] ;
  Float_t  DetPointRes[100][400], DetPointZRes[100][400] ;
  Double_t ratio[400], PXLReference[400], efficiency[2][400], D0efficiency[400] ;
  Int_t    Clear  = 0 ;    // Clear = 1 means to clear the graphs
  Char_t  *Detector   ;

  // Set default style for all plots
  TStyle*    myStyle = new TStyle("myStyle", "custom style") ;    
  myStyle -> SetLineWidth(2)      ;
  myStyle -> SetFrameLineWidth(2) ;
  myStyle -> SetLineColor(kBlack) ;
  myStyle -> cd() ;
  gROOT   -> ForceStyle();           

  // See if Canvases already exist.  If so, then keep current contents and continue drawing on the same Canvas
  yreference = 0 ;                   // Zero out reference array pointer
  c1 = c2 = c3 = c4 = c5 =  0 ;      // Zero out canvas pointers
  
  TObject* g ;
  g = gROOT->FindObject("c3") ;
  if (g) c3 = (TCanvas*)g ;
  g = gROOT->FindObject("c4") ;
  if (g) c4 = (TCanvas*)g ;
  g = gROOT->FindObject("c1") ;
  if (g) c1 = (TCanvas*)g ;
  g = gROOT->FindObject("c2") ;
  if (g) c2 = (TCanvas*)g ;
  g = gROOT->FindObject("c5") ;
  if (g) c5 = (TCanvas*)g ;
  g = gROOT->FindObject("yreference") ;
  if (g) yreference = (TH1F*)g ;
  
  if ( c2 == 0 )
    { 
      Clear  = 1 ;  // Clear graphs
      c1 = new TCanvas("c1","Momentum Resolution vs Pt", 10, 10, 800, 500) ;
      c5 = new TCanvas("c5","Efficiency vs Pt", 40, 40, 800, 500) ;
      c3 = new TCanvas("c3","Relative Pointing Resolution vs Pt", 430, 430, 800, 500) ;
      c4 = new TCanvas("c4","Z Pointing Resolution vs Pt", 400, 400, 800, 500) ;
      c2 = new TCanvas("c2","R-Phi Pointing Resolution vs Pt", 370, 370, 800, 500) ;
      c1 -> SetFillColor(0) ;
      c5 -> SetFillColor(0) ;
      c3 -> SetFillColor(0) ;
      c4 -> SetFillColor(0) ;
      c2 -> SetFillColor(0) ;
      c1 -> cd() ; c1 -> Draw() ;  c1 -> Update() ;
      c5 -> cd() ; c5 -> Draw() ;  c5 -> Update() ;
      c3 -> cd() ; c3 -> Draw() ;  c3 -> Update() ;  // Create canvases on screen; draw as empty canvases, for now.
      c4 -> cd() ; c4 -> Draw() ;  c4 -> Update() ;
      c2 -> cd() ; c2 -> Draw() ;  c2 -> Update() ;
      yreference = new TH1F("yreference","yreference",400,0,399) ;
    }
  
  // Start of Data Entry loop
  cout << endl
       << "The full list of detectors includes:" << endl ;
  for ( Int_t i = 0  ; i < 10 ; i++ )                { cout << DetectorList[i] << " " ; }
  cout << endl ;
  for ( Int_t i = 10 ; i < NumberOfDetectors ; i++ ) { cout << DetectorList[i] << " " ; }
   cout << endl << endl
       << "The list of colors includes:" << endl ;
  for ( Int_t i = 0 ; i < NumberOfColours ; i++ )    { cout << ColourList[i] << " " ;   }
  cout << endl << endl 
       << "Enter a string of detector names and finish with the word 'end'. Work from the" << endl
       << "vertex and go out along a radial vector. You can/should include dead layers." << endl
       << "Finish with the line color.  Valid colors are shown above." << endl 
       << "Type 'clear' to clear the canvases.  Type 'quit' to quit.  For example:"  << endl << endl
       << "vtx pxl1 pxl2 ist1 d-ist2 ssd tpc end kBlue" << endl << endl ;
  Detector = new Char_t [8] ;
      
  while ( 1 ) 
    {

      Radius     [VtxIndex] = 0 ;
      Resolution [VtxIndex] = RIDICULOUS ;  
      ResolutionZ[VtxIndex] = RIDICULOUS ;  
      Thickness  [VtxIndex] = 0 ; 
      
      Radius     [BeamPipe1Index] = BeamPipe1Radius ;
      Resolution [BeamPipe1Index] = RIDICULOUS ; // Always off
      ResolutionZ[BeamPipe1Index] = RIDICULOUS ; // Always off
      Thickness  [BeamPipe1Index] = BeamPipe1Thickness ;
      
      Radius     [Pxl1Index] = Pxl1Radius ;   
      Resolution [Pxl1Index] = RIDICULOUS ; 
      ResolutionZ[Pxl1Index] = RIDICULOUS ; 
      Thickness  [Pxl1Index] = 0 ;
      
      Radius     [Pxl2Index] = Pxl2Radius ; 
      Resolution [Pxl2Index] = RIDICULOUS ;
      ResolutionZ[Pxl2Index] = RIDICULOUS ;
      Thickness  [Pxl2Index] = 0 ;
      
      Radius     [BeamPipe2Index] = BeamPipe2Radius ;
      Resolution [BeamPipe2Index] = RIDICULOUS ; // Always off
      ResolutionZ[BeamPipe2Index] = RIDICULOUS ; // Always off
      Thickness  [BeamPipe2Index] = BeamPipe2Thickness ;
      
      Radius     [Ist1Index] = Ist1Radius ;
      Resolution [Ist1Index] = RIDICULOUS ;
      ResolutionZ[Ist1Index] = RIDICULOUS ;
      Thickness  [Ist1Index] = 0 ;

      Radius     [Ist1PrimeIndex] = Ist1PrimeRadius ;
      Resolution [Ist1PrimeIndex] = RIDICULOUS ;
      ResolutionZ[Ist1PrimeIndex] = RIDICULOUS ;
      Thickness  [Ist1PrimeIndex] = 0 ;
      
      Radius     [Ist2Index] = Ist2Radius ; 
      Resolution [Ist2Index] = RIDICULOUS ;
      ResolutionZ[Ist2Index] = RIDICULOUS ;
      Thickness  [Ist2Index] = 0 ;
      
      Radius     [Ist2PrimeIndex] = Ist2PrimeRadius ; 
      Resolution [Ist2PrimeIndex] = RIDICULOUS ;
      ResolutionZ[Ist2PrimeIndex] = RIDICULOUS ;
      Thickness  [Ist2PrimeIndex] = 0 ;
      
      Radius     [SsdIndex] = SsdRadius ;  
      Resolution [SsdIndex] = RIDICULOUS ;
      ResolutionZ[SsdIndex] = RIDICULOUS ;
      Thickness  [SsdIndex] = 0 ;
      
      Radius     [IFCIndex] = IFCRadius ;   
      Resolution [IFCIndex] = RIDICULOUS ;
      ResolutionZ[IFCIndex] = RIDICULOUS ;
      Thickness  [IFCIndex] = IFCThickness ;

      for ( Int_t i = 0 ; i < InnerRows1 ; i++ )
	{
	  Radius     [i+TpcIndex]  =  RowOneRadius + i*TpcInnerRadialPitch1 ;
	  Resolution [i+TpcIndex]  =  RIDICULOUS ;
	  ResolutionZ[i+TpcIndex]  =  RIDICULOUS ;
	  Thickness  [i+TpcIndex]  =  0 ;
	}
      for ( Int_t i = InnerRows1 ; i < InnerRows1+InnerRows8 ; i++ )
	{
	  Radius     [i+TpcIndex]  =  RowEightRadius + (i-InnerRows1+1)*TpcInnerRadialPitch8 ;
	  Resolution [i+TpcIndex]  =  RIDICULOUS ;
	  ResolutionZ[i+TpcIndex]  =  RIDICULOUS ;
	  Thickness  [i+TpcIndex]  =  0 ;
	}
      for ( Int_t i = InnerRows1+InnerRows8 ; i < TpcRows ; i++ )
	{
	  Radius     [i+TpcIndex]  =  RowFourteenRadius + (i-InnerRows)*TpcOuterRadialPitch ;
	  Resolution [i+TpcIndex]  =  RIDICULOUS ;
	  ResolutionZ[i+TpcIndex]  =  RIDICULOUS ;
	  Thickness  [i+TpcIndex]  =  0 ;
	}      
    
      // Activate the Detector Layers
      Int_t DetectorIndex = -1 ;
      Int_t BeginIndex    = -1 ;
      Int_t EndIndex      = -1 ;
      while ( 1 )
	{
	  
	  gSystem->DispatchOneEvent();  // Force refresh of the screen so graphs appear immediately
	  DetectorIndex = -1  ;         // Do nothing if a detector is not found.  Handle input errors below the switch.
	  cin >> Detector ;

	  for ( Int_t j = 0 ; j < NumberOfDetectors ; j++ ) { 
	    if ( strcmp(Detector,DetectorList[j]) == 0 ) DetectorIndex = j ; }

	  Int_t skip ;
	  switch ( DetectorIndex ) 
	    {
	    case vtx :
	      Resolution [VtxIndex]  =  VtxResolution  ;
	      ResolutionZ[VtxIndex]  =  VtxResolutionZ ;
	      Thickness  [VtxIndex]  =  VtxThickness   ;
	      EndIndex =  VtxIndex ;    break          ;
	    case d_vtx : 
	      Resolution [VtxIndex]  =  RIDICULOUS    ; 
	      ResolutionZ[VtxIndex]  =  RIDICULOUS    ; 
	      Thickness  [VtxIndex]  =  VtxThickness  ;
	      EndIndex =  VtxIndex ;    break         ; 
	    case newvtx : 
	      Resolution [VtxIndex]  =  NewVtxResolution  ;
	      ResolutionZ[VtxIndex]  =  NewVtxResolutionZ ;
	      Thickness  [VtxIndex]  =  VtxThickness      ;
	      EndIndex =  VtxIndex ;    break             ;
	    case refitvtx : 
	      Resolution [VtxIndex]  =  RefitVtxResolution  ;
	      ResolutionZ[VtxIndex]  =  RefitVtxResolutionZ ;
	      Thickness  [VtxIndex]  =  VtxThickness        ;
	      EndIndex =  VtxIndex ;    break               ;
	    case pxl1 :
	      Resolution [Pxl1Index] =  Pxl1Resolution  ; 
	      ResolutionZ[Pxl1Index] =  Pxl1ResolutionZ ; 
	      Thickness  [Pxl1Index] =  Pxl1Thickness   ;
	      EndIndex =  Pxl1Index ;   break           ;
	    case d_pxl1 : 
 	      Resolution [Pxl1Index] =  RIDICULOUS     ;   
	      ResolutionZ[Pxl1Index] =  RIDICULOUS     ;   
	      Thickness  [Pxl1Index] =  Pxl1Thickness  ;
	      EndIndex =  Pxl1Index ;   break          ;
	    case pxl2 :
	      Resolution [Pxl2Index] =  Pxl2Resolution  ;
	      ResolutionZ[Pxl2Index] =  Pxl2ResolutionZ ;
	      Thickness  [Pxl2Index] =  Pxl2Thickness   ;
	      EndIndex =  Pxl2Index ;   break           ;
	    case d_pxl2 : 
	      Resolution [Pxl2Index] =  RIDICULOUS     ;
	      ResolutionZ[Pxl2Index] =  RIDICULOUS     ;
	      Thickness  [Pxl2Index] =  Pxl2Thickness  ;
	      EndIndex =  Pxl2Index ;   break          ;
	    case ist1 : 
	      Resolution [Ist1Index] =  Ist1Resolution  ;
	      ResolutionZ[Ist1Index] =  Ist1ResolutionZ ;
	      Thickness  [Ist1Index] =  Ist1Thickness   ;
	      EndIndex =  Ist1Index ;   break           ;
	    case d_ist1 : 
	      Resolution [Ist1Index] =  RIDICULOUS     ;
	      ResolutionZ[Ist1Index] =  RIDICULOUS     ;
	      Thickness  [Ist1Index] =  Ist1Thickness  ;
	      EndIndex =  Ist1Index ;   break          ;
	    case ist1prime : 
	      Resolution [Ist1PrimeIndex] =  Ist1PrimeResolution  ;
	      ResolutionZ[Ist1PrimeIndex] =  Ist1PrimeResolutionZ ;
	      Thickness  [Ist1PrimeIndex] =  Ist1PrimeThickness   ;
	      EndIndex =  Ist1PrimeIndex ;   break           ;
	    case d_ist1prime : 
	      Resolution [Ist1PrimeIndex] =  RIDICULOUS     ;
	      ResolutionZ[Ist1PrimeIndex] =  RIDICULOUS     ;
	      Thickness  [Ist1PrimeIndex] =  Ist1PrimeThickness  ;
	      EndIndex =  Ist1PrimeIndex ;   break          ;
	    case ist2 : 
	      Resolution [Ist2Index] =  Ist2Resolution  ;
	      ResolutionZ[Ist2Index] =  Ist2ResolutionZ ;
	      Thickness  [Ist2Index] =  Ist2Thickness   ;
	      EndIndex =  Ist2Index ;   break           ;
	    case d_ist2 : 
	      Resolution [Ist2Index] =  RIDICULOUS     ;
	      ResolutionZ[Ist2Index] =  RIDICULOUS     ;
	      Thickness  [Ist2Index] =  Ist2Thickness  ;
	      EndIndex =  Ist2Index ;   break          ;
	    case ist2prime : 
	      Resolution [Ist2PrimeIndex] =  Ist2PrimeResolution  ;
	      ResolutionZ[Ist2PrimeIndex] =  Ist2PrimeResolutionZ ;
	      Thickness  [Ist2PrimeIndex] =  Ist2PrimeThickness   ;
	      EndIndex =  Ist2PrimeIndex ;   break           ;
	    case d_ist2prime : 
	      Resolution [Ist2PrimeIndex] =  RIDICULOUS     ;
	      ResolutionZ[Ist2PrimeIndex] =  RIDICULOUS     ;
	      Thickness  [Ist2PrimeIndex] =  Ist2PrimeThickness  ;
	      EndIndex =  Ist2PrimeIndex ;   break          ;
	    case ssd :  
	      Resolution [SsdIndex]  =  SsdResolution  ;
	      ResolutionZ[SsdIndex]  =  SsdResolutionZ ;
	      Thickness  [SsdIndex]  =  SsdThickness   ;
	      EndIndex =  SsdIndex ;    break          ;
	    case d_ssd : 
	      Resolution [SsdIndex]  =  RIDICULOUS    ;
	      ResolutionZ[SsdIndex]  =  RIDICULOUS    ;
	      Thickness  [SsdIndex]  =  SsdThickness  ;
	      EndIndex =  SsdIndex ;    break         ;
	    case tpc : 
	      skip          = 1 ; // Skip = 1  Imitates a track with all 45 hits active
	      for ( Int_t k = 0 ; k < TpcRows ; k++ )
		{
		  if ( k%skip == 0 ) Resolution [k+TpcIndex]  =  TpcResolution  ;
		  else               Resolution [k+TpcIndex]  =  RIDICULOUS     ;
		  if ( k%skip == 0 ) ResolutionZ[k+TpcIndex]  =  TpcResolutionZ ;
		  else               ResolutionZ[k+TpcIndex]  =  RIDICULOUS     ;
		  if ( k == 0 || k == 12 ) 
		                   { ResolutionZ[k+TpcIndex]  =  RIDICULOUS ;    
		                     Resolution [k+TpcIndex]  =  RIDICULOUS ;   }  // Always skip pad rows 1 and 13
		  Thickness [k+TpcIndex]  =  TpcAvgThickness ;
		}
	      EndIndex = TpcIndex + TpcRows-1 ;  break ;
	    case tpc2 : 
	      skip          = 2 ; // Skip = 2  Imitates a track with only 23 hits rather than 45
	      for ( Int_t k = 0 ; k < TpcRows ; k++ )
		{
		  if ( k%skip == 0 ) Resolution [k+TpcIndex]  =  TpcResolution  ;
		  else               Resolution [k+TpcIndex]  =  RIDICULOUS     ;
		  if ( k%skip == 0 ) ResolutionZ[k+TpcIndex]  =  TpcResolutionZ ;
		  else               ResolutionZ[k+TpcIndex]  =  RIDICULOUS     ;
		  if ( k == 0 || k == 12 ) 
		                   { ResolutionZ[k+TpcIndex]  =  RIDICULOUS ;    
		                     Resolution [k+TpcIndex]  =  RIDICULOUS ;   }  // Always skip pad rows 1 and 13
		  Thickness [k+TpcIndex]  =  TpcAvgThickness ;
		}
	      EndIndex = TpcIndex + TpcRows-1 ;  break ;
	    default : 
	      break ;

	    }
	  
	  if ( BeginIndex == -1 ) BeginIndex = EndIndex ;  // Set the starting index for the matrix propagation
	  if ( BeginIndex == TpcIndex + TpcRows-1 ) BeginIndex = TpcIndex ;  

	  if ( strcmp(Detector,"quit") == 0 ) 
	    { 
	      cout << endl ; 
	      return ; 
	    }
	  else if ( strcmp(Detector,"clear") == 0 ) 
	    { 
	      Clear  = 1 ;  
	      c1->Clear() ; c1->Update() ;
	      c5->Clear() ; c5->Update() ;
	      c3->Clear() ; c3->Update() ;
	      c4->Clear() ; c4->Update() ;
	      c2->Clear() ; c2->Update() ;
	      cout << endl << "Enter a new detector configuration" << endl ; 
	    }
	  else if ( strcmp(Detector,"update") == 0 ) 
	    gSystem->DispatchOneEvent();  // Force refresh of the screen so graphs appear immediately
	  else if ( strcmp(Detector,"end") == 0 ) 
	    break ;
	  else
	    { 
	      if ( DetectorIndex == -1 ) 
		{ 
		  cout << "Error: Detector\"" << Detector << "\" not defined. Please enter the line again." << endl ;
		  cin.ignore(1000,'\n')      ;   // Ignore any further characters on the input line
		  BeginIndex = EndIndex = -1 ;   // Reset saved indices
		}
	    }
	  
	}

      // Read the color of the line to use in plotting
      gSystem->DispatchOneEvent();  // Force refresh of the screen so graphs appear immediately
      Color_t LineColour    = -1 ;
      cin >> Detector ;

      for ( Int_t j = 0 ; j < NumberOfColours ; j++ ) { 
	if ( strcmp(Detector,ColourList[j]) == 0 ) LineColour = j ; }

      if ( LineColour == -1 )
	{
	  cout << "Error: Colour \"" << Detector << "\" not defined. Please enter the line again." << endl ;
	  continue ;  // Go back to the top and read everything again.
	}

      // Scale Si Detectors Pad Sizes by your favorite estimate of the true resolution (eg 1/root(12) )

      for ( Int_t j = Pxl1Index ; j <= SsdIndex ; j++ )
	{
	  if ( Resolution [j] < RIDICULOUS && Resolution [j] <= 0.100 ) Resolution [j]  *= SiScaleFactor ;
	  if ( Resolution [j] < RIDICULOUS && Resolution [j] >  0.100 ) Resolution [j]  *= 0.288         ;
	  if ( ResolutionZ[j] < RIDICULOUS && ResolutionZ[j] <= 0.100 ) ResolutionZ[j]  *= SiScaleFactor ;
	  if ( ResolutionZ[j] < RIDICULOUS && ResolutionZ[j] >  0.100 ) ResolutionZ[j]  *= 0.288         ;
	} // Don't scale resolutions values greater than 288 microns.  ITTF do these quite well.

      // Convert to Meters, Tesla, and GeV
      for ( Int_t j = 0 ; j < 100 ; j++ ) { Radius[j] /= 100. ;  Resolution[j] /= 100. ;  ResolutionZ[j] /= 100. ;}      

      // End of Data Entry loop

      // Calculate track parameters using Billoirs method of matrices

      Double_t pt, pz, lambda, momentum, rho, DeltaPoverP  ;
      Double_t layerThickness, MCS, mmm, Charge ;
      Double_t F1, F2, F3, G1, G2, G3 ;
      Double_t dx, sigma2, sigma2Z ;
      Double_t mass[2] ;

      for ( Int_t massloop = 0 ; massloop < 2 ; massloop++ )
	{ 

	  lambda = TMath::Pi()/2 - 2*TMath::ATan(TMath::Exp(-1*AvgRapidity))  ;       // PseudoRapidity OK, used as an angle
	  if ( ParticleMass == D0Mass ) { mass[0] = PionMass ; mass[1] = KaonMass ; } // Loop twice for the D0;  first pi then k 
	  else { mass[0] = ParticleMass ; mass[1] = ParticleMass ; }                  // Otherwise just loop twice and ignore 1st loop

	  Double_t y, z, a, b, d, e ;  // Track moves along the x axis and deviations are in the y and z directions.
	                               // a = py/px = tan phi, b = pz/px = tan lambda / cos phi, d = 0.3*Ze/Abs(p)
	  for ( Int_t i = 0 ; i < 400 ; i++ )
	    {
	      xpoint[i] =  ( 0.3*Radius[EndIndex]*TMath::Abs(BFIELD)/2.0 ) + i / 100.0 ;  // JT Starting values based on radius in TPC
	      // Assume track started at (0,0,0) and shoots out on the X axis, and B field is on the Z axis
	      // These are the EndPoint values for y, z, a, b, and d
	      pt  =  xpoint[i]                       ;  // GeV/c
	      rho =  pt / TMath::Abs( 0.3 * BFIELD ) ;  // Radius of curvature of the track (meters)
	      momentum = pt / TMath::Cos(lambda)     ;  // Total momentum  
	      Charge   = -1                          ;  // Assume an electron 
	      pz  =  pt * TMath::Tan(lambda)         ;  // GeV/c
	      d   =  0.3 * Charge / momentum         ;  // Its an electrons so q = -1, which makes d a negative number
	      // Set HFT Efficiency Storage area to unity
	      efficiency[massloop][i] = 1.0 ;
	      // Back-propagate the covariance matrix along the track.  I is the inverse of the covariance matrix. 
	      // Start with small variations to a straight line 'information' matrix.  Tuning this matrix is 
	      // equivalent to 'starting the recursion' in Billoirs paper.  It is a tricky business.  You need test
	      // data and many layers for the matrices to stabilize.  Do not believe results for a small number of layers.
	      // In our case, always include the TPC and this matrix will be well conditioned before it gets to the Si.
	      Istar(0,0) = 1.0   ;  Istar(0,1) = 0.0  ;  Istar(0,2) = 0.0   ;  Istar(0,3) = 0.0   ;  Istar(0,4) = 0.0  ; 
	      Istar(1,0) = 0.0   ;  Istar(1,1) = 1.0  ;  Istar(1,2) = 0.0   ;  Istar(1,3) = 0.0   ;  Istar(1,4) = 0.0  ;
	      Istar(2,0) = 0.0   ;  Istar(2,1) = 0.0  ;  Istar(2,2) = 1.0   ;  Istar(2,3) = 0.0   ;  Istar(2,4) = 0.0  ;
	      Istar(3,0) = 0.0   ;  Istar(3,1) = 0.0  ;  Istar(3,2) = 0.0   ;  Istar(3,3) = 1.0   ;  Istar(3,4) = 0.0  ;
	      Istar(4,0) = 0.0   ;  Istar(4,1) = 0.0  ;  Istar(4,2) = 0.0   ;  Istar(4,3) = 0.0   ;  Istar(4,4) = 1.0  ;
	      // 'A' is the "angle" matrix for MCS at each step.
	      // 'D' is the "distance" matrix at each step.  It propagates the particle backwards along the track. 
	      // 'M' is the "measurement" matrix. It represents the measurement resolution at each step.
	      
	      for ( Int_t j = EndIndex ; j > BeginIndex ; j-- ) 
		{
		  if ( (rho-0.1) <= Radius[j] ) continue ; // If pt is too low, then skip these Radius[j] but keep going // JT test
		  y   =  Radius[j]*Radius[j] / ( 2*rho )                              ; // y      // These are 'ideal' locations
		  a   =  TMath::Sqrt(Radius[j]*Radius[j] - y*y ) / ( rho - y )        ; // py/px  // and not propagated values  
		  b   =  rho * TMath::Tan(lambda) / ( rho - y )                       ; // pz/px  // which should be done if we
		  z   =  rho * TMath::Tan(lambda) * TMath::ATan(a)                    ; // z      // had data. But we don't.
		  e   =  TMath::Sqrt( 1 + a*a + b*b )                                 ; 
		  layerThickness = Thickness[j] / TMath::Sin(TMath::Pi()/2 - lambda) ; 
		  MCS =  ThetaMCS(mass[massloop], layerThickness, momentum)  ; 
		  mmm =  ( 1 + a*a + b*b ) ; 
		  BigA(0,0) = mmm*(1+a*a) ; BigA(0,1) = mmm*a*b     ; LittleA(0,0) = Istar(2,2)  ; LittleA(0,1) = Istar(2,3) ; 
		  BigA(1,0) = mmm*a*b     ; BigA(1,1) = mmm*(1+b*b) ; LittleA(1,0) = Istar(3,2)  ; LittleA(1,1) = Istar(3,3) ; 
		  LittleA = BigA.Invert() + MCS*MCS*LittleA ;
		  LittleA = LittleA.Invert() ;
		  A(0,0) = 0.0 ;  A(0,1) = 0.0  ;  A(0,2) = 0.0          ;  A(0,3) = 0.0          ;  A(0,4) = 0.0  ; 
		  A(1,0) = 0.0 ;  A(1,1) = 0.0  ;  A(1,2) = 0.0          ;  A(1,3) = 0.0          ;  A(1,4) = 0.0  ;
		  A(2,0) = 0.0 ;  A(2,1) = 0.0  ;  A(2,2) = LittleA(0,0) ;  A(2,3) = LittleA(0,1) ;  A(2,4) = 0.0  ;
		  A(3,0) = 0.0 ;  A(3,1) = 0.0  ;  A(3,2) = LittleA(1,0) ;  A(3,3) = LittleA(1,1) ;  A(3,4) = 0.0  ;
		  A(4,0) = 0.0 ;  A(4,1) = 0.0  ;  A(4,2) = 0.0          ;  A(4,3) = 0.0          ;  A(4,4) = 0.0  ;
		  Istar = Istar - MCS*MCS*Istar*A*Istar ;
		  dx     = ( Radius[j] - Radius[j-1] )         ;        // Billoir works with absolute magnitude of distance
		  F3     = e * ( -1 * ( 1 + a*a ) * BFIELD )   ;        // Assume BFIELD is on Z axis, only.
		  F1     = d * ( a*F3/(e*e) - 2*e*a*BFIELD )   ;
		  F2     = d * ( b*F3/(e*e) )                  ;
		  G3     = e * ( -1 * a * b * BFIELD )         ;
		  G1     = d * ( a*G3/(e*e) + e*b*BFIELD )     ;
		  G2     = d * ( b*G3/(e*e) - e*a*BFIELD )     ;	      
		  D(0,0) = 1.0 ; D(0,1) = 0.0 ; D(0,2) = -1*dx+F1*dx*dx/2 ; D(0,3) = F2*dx*dx/2  ;  D(0,4) = F3*dx*dx/2 ;
		  D(1,0) = 0.0 ; D(1,1) = 1.0 ; D(1,2) = G1*dx*dx/2 ;  D(1,3) = -1*dx+G2*dx*dx/2 ;  D(1,4) = G3*dx*dx/2 ;
		  D(2,0) = 0.0 ; D(2,1) = 0.0 ; D(2,2) = 1-F1*dx    ;  D(2,3) = -1*F2*dx         ;  D(2,4) = -1*F3*dx   ;
		  D(3,0) = 0.0 ; D(3,1) = 0.0 ; D(3,2) = -1*G1*dx   ;  D(3,3) = 1-G2*dx          ;  D(3,4) = -1*G3*dx   ;
		  D(4,0) = 0.0 ; D(4,1) = 0.0 ; D(4,2) = 0.0        ;  D(4,3) = 0.0              ;  D(4,4) = 1.0        ;
		  DT.Transpose(D) ;
		  Istar  = DT.Invert() * Istar * D.Invert() ;
		  // Prepare to save Detector efficiencies
		  Work = Istar  ;          // Working copy of matrix 
		  Work.Invert() ;          // Invert the Matrix to recover the convariance matrix
		  DetPointRes [j-1][i]     =  TMath::Sqrt( Work(0,0) )  ;     // result in meters
		  DetPointZRes[j-1][i]     =  TMath::Sqrt( Work(1,1) )  ;     // result in meters
		  // End save
		  sigma2 = ( Resolution[j-1]  * Resolution[j-1]  )   ; 
		  sigma2Z = ( ResolutionZ[j-1] * ResolutionZ[j-1] )   ; 
		  M(0,0) = 1/sigma2 ;  M(0,1) = 0.0  ;        M(0,2) = 0.0  ;  M(0,3) = 0.0  ;  M(0,4) = 0.0  ; 
		  M(1,0) = 0.0 ;       M(1,1) = 1/sigma2Z  ;  M(1,2) = 0.0  ;  M(1,3) = 0.0  ;  M(1,4) = 0.0  ;
		  M(2,0) = 0.0 ;       M(2,1) = 0.0  ;        M(2,2) = 0.0  ;  M(2,3) = 0.0  ;  M(2,4) = 0.0  ;
		  M(3,0) = 0.0 ;       M(3,1) = 0.0  ;        M(3,2) = 0.0  ;  M(3,3) = 0.0  ;  M(3,4) = 0.0  ;
		  M(4,0) = 0.0 ;       M(4,1) = 0.0  ;        M(4,2) = 0.0  ;  M(4,3) = 0.0  ;  M(4,4) = 0.0  ;
		  Istar = Istar + M ;
		}
	      
	      // Invert the Matrix to recover the convariance matrix
	      Istar.Invert() ;
	      // Convert the Convariance matrix parameters into physical quantities
	      // The results are propogated to the previous point but *do not* include the measurement at that point.
	      DeltaPoverP    =  TMath::Sqrt( Istar(4,4) ) * momentum / 0.3  ;  // Absolute magnitude so ignore Charge
	      ypoint[i]      =  100.* TMath::Abs( DeltaPoverP )             ;  // results in percent
	      yprime[i]      =  TMath::Sqrt( Istar(0,0) ) * 1.e6            ;  // result in microns
	      yprimeZ[i]     =  TMath::Sqrt( Istar(1,1) ) * 1.e6            ;  // result in microns
	      equivalent[i]  =  TMath::Sqrt(yprime[i]*yprimeZ[i])           ;  // Equivalent circular radius
	      // Calculate total detector efficiency ... Note: Use centimeters in the ProbGoodHit calculations
	      // JT test
	      if (xpoint[i] < 0.760 && xpoint[i] > 0.750 && massloop == 1)
		{
		  printf("\n") ;
		  printf("Radius Thickness PointResOn PointResOnZ  DetRes  DetResZ  Density Efficiency\n") ;
		}
	      // End JT test
	      for ( Int_t j = EndIndex ; j > BeginIndex ; j-- ) 
		{
		  if ( j < TpcIndex && Thickness[j-1] > 0 && Resolution[j-1] > 0 && Resolution[j-1] < 1.0 
		       && ResolutionZ[j-1] > 0 && ResolutionZ[j-1] < 1.0  ) 
		    { 
		      Double_t RphiError  =  TMath::Sqrt( DetPointRes [j-1][i] * DetPointRes [j-1][i] + 
							  Resolution [j-1] * Resolution [j-1] ) * 100.  ; // work in cm
		      Double_t ZError     =  TMath::Sqrt( DetPointZRes[j-1][i] * DetPointZRes[j-1][i] +
							  ResolutionZ[j-1] * ResolutionZ[j-1] ) * 100.  ; // work in cm
		      // JT test
		      if (xpoint[i] < 0.760 && xpoint[i] > 0.750 && massloop == 1)
			{
			  printf("%6.3f %9.4f %10.0f %11.0f %7.0f %8.0f %8.2f %10.2f\n",
				 Radius[j-1], Thickness[j-1], 
				 DetPointRes[j-1][i]*1.e6, DetPointZRes[j-1][i]*1.e6,
				 Resolution[j-1]*1.e6, ResolutionZ[j-1]*1.e6,
				 HitDensity(Radius[j-1]*100),
				 ProbGoodChiSqHit( Radius[j-1]*100, RphiError , ZError ) ) ;
			}
		      // End JT test
		      if ( EfficiencySearchFlag == 0 )
			efficiency[massloop][i]    *=  ProbGoodHit( Radius[j-1]*100, RphiError , ZError  ) ;
		      else if ( EfficiencySearchFlag == 1 )
			efficiency[massloop][i]    *=  ProbGoodChiSqHit( Radius[j-1]*100, RphiError , ZError  ) ;
		    }
		}
	      if (xpoint[i] < 0.760 && xpoint[i] > 0.750 && massloop == 1)
		{
		  printf("\n") ;
		}
	    }
	}

      // Print out useful comments and/or information
      if ( BeginIndex==VtxIndex && Thickness[Pxl1Index]==Pxl1Thickness && Thickness[Pxl2Index]==Pxl2Thickness  ) 
	cout << "The hit densities on PXL1 & PXL2 are " << HitDensity(100*Radius[Pxl1Index]) << " &  " 
	     << HitDensity(100*Radius[Pxl2Index]) << " per cm**2." << endl ;

      // JT Test
      for ( Int_t i = 0 ; i < 400 ; i++ ) { 
	if (xpoint[i] < 0.760 && xpoint[i] > 0.750) 
	  { 
	    cout << "The single track pointing resolution at " << 1000*xpoint[i] << " MeV is " << endl << yprime[i]
		 << " microns in R-Phi and " << yprimeZ[i] << " microns in Z."  << endl  ;
	    if ( efficiency[1][i] > 0.0 ) cout << "The single track efficiency (all layers combined) is " 
					    << 100*efficiency[1][i] << " percent." << endl ;
	    if ( efficiency[1][i] > 0.0 ) cout << "The table does not include the TPC & SSD acceptance which is assumed \n"
					    << "to be " << 100*AcceptanceOfTpcSSD 
					    << " percent for both detectors combined." << endl;
	    cout << endl ;
	  }
      }
      // End JT Test

      // Start graphing
      Double_t ppoint[400] ;  // Create array of total momentum points
      for ( Int_t i = 0 ; i < 400 ; i++ )  ppoint[i] =  TMath::Cos(lambda) / xpoint[i] ;  //  1./Total momentum  
      c1 -> cd() ;
      c1 -> SetGrid() ;
      myStyle -> SetLineColor(LineColour) ;
      graph =  new TGraph (400, xpoint, ypoint) ;
      graph -> SetMaximum(10) ;
      graph -> SetMinimum(0) ;
      graph -> SetTitle("Momentum Resolution .vs. Pt" ) ;
      if ( Clear == 1 ) 
	{ 
	  graph -> Draw("AC") ; 
	  graph -> GetXaxis()->SetTitle("Transverse Momentum (GeV/c)") ;
	  graph -> GetXaxis()->CenterTitle();
	  graph -> GetYaxis()->SetTitle("Momentum Resolution (percent)") ;
	  graph -> GetYaxis()->CenterTitle();
	  data1 =  new TGraph (40, ptdots , pbar ) ;
	  data1 -> SetMinimum(0)  ;
	  data1 -> SetMaximum(10) ;
	  data1 -> Draw("*") ;
	  data2 =  new TGraph (80, ptdots , pion ) ;
	  data2 -> SetMaximum(10) ;
	  data2 -> SetMinimum(0) ;
	  data2 -> Draw("*") ;
	}
      else 
	{
	  graph -> Draw("C") ; 
	} 
      
      c2 -> cd() ;
      c2 -> SetLogy() ;
      //c2 -> SetLogx() ;
      c2 -> SetGrid() ;
      data3 =  new TGraph ( 400, ppoint, yprime ) ;   // Use 1/p
      //data3 =  new TGraph ( 400, xpoint, yprime ) ; 
      //data3 =  new TGraph ( 400, xpoint, equivalent ) ; // JT Test Use this line if you want equivalent circle
      data3 -> SetMinimum(9.99) ;
      data3 -> SetMaximum(9999.1) ;
      data3 -> SetTitle("R-#phi Pointing Resolution .vs. 1/p" ) ;
      //      data3 -> SetTitle("R-#phi Pointing Resolution .vs. Pt" ) ;
      if ( Clear == 1 )
	{
	  data3 -> GetXaxis()->SetLimits(0.0,5.0) ;
	  data3 -> Draw("AC") ;
	  data3 -> GetXaxis()->SetTitle("1 / Momentum (c/GeV)") ;
	  data3 -> GetXaxis()->CenterTitle();
	  data3 -> GetXaxis()->SetNoExponent(1) ;
	  data3 -> GetXaxis()->SetMoreLogLabels(1) ;
	  data3 -> GetYaxis()->SetTitle("R-#phi Pointing Resolution (microns)") ;
	  data3 -> GetYaxis()->CenterTitle();
	  for ( Int_t i = 0 ; i < 400 ; i++ )
	    {
	      ratio[i] = 0.0 ;
	      yreference->SetBinContent(i,TMath::Sqrt(yprime[i]*yprimeZ[i])) ;  // Store in Global array for later use
	    }
	  // Draw empty plot, for now.  Fill it later, on the 2nd pass.
	  c3 -> cd() ;
	  c3 -> SetLogx() ;
	  c3 -> SetGrid() ;
	  myStyle -> SetLineColor(kBlack) ;
	  data4 =  new TGraph ( 400, xpoint, ratio ) ;
	  data4 -> SetMinimum(0) ;
	  data4 -> SetMaximum(1.3) ;
	  data4 -> SetTitle("Relative Pointing Resolution (#sqrt{R-#phi*Z} / reference ) .vs. Pt" ) ;
	  data4 -> Draw("AC" ) ; 
	  data4 -> GetXaxis()->SetTitle("Transverse Momentum (GeV/c)") ;
	  data4 -> GetXaxis()->CenterTitle();
	  data4 -> GetXaxis()->SetNoExponent(1) ;
	  data4 -> GetXaxis()->SetMoreLogLabels(1) ;
	  data4 -> GetYaxis()->SetTitle("Relative Resolution") ;
	  data4 -> GetYaxis()->CenterTitle();
	  myStyle -> SetLineColor(LineColour) ;
	}
      else
	{ 
	  data3 -> Draw("C") ;
	  for ( Int_t i = 0 ; i < 400 ; i++ )
	    {
	      ratio[i] = 0.0 ;
	      Double_t denominator = yreference->GetBinContent(i) ;
	      if ( denominator != 0 ) ratio[i] = TMath::Sqrt(yprime[i]*yprimeZ[i]) / denominator ;
	    }
	  c3 -> cd() ;
	  c3 -> SetLogx() ;
	  c3 -> SetGrid() ;
	  data4 =  new TGraph ( 400, xpoint, ratio ) ;
	  data4 -> SetMinimum(0) ;
	  data4 -> SetMaximum(1.3) ;
	  data4 -> SetTitle("Relative Pointing Resolution (sqrt[Rphi*Z]/reference) .vs. Pt" ) ;
	  data4 -> Draw("C" )  ;
	  c3 -> Update() ;
	}

      c4 -> cd() ;
      c4 -> SetLogy() ;
      //c4 -> SetLogx() ;
      c4 -> SetGrid() ;
      data6 =  new TGraph ( 400, ppoint, yprimeZ ) ;
      //      data6 =  new TGraph ( 400, xpoint, yprimeZ ) ;
      data6 -> SetMinimum(9.99) ;
      data6 -> SetMaximum(9999.1) ;
      data6 -> SetTitle("Z Pointing Resolution .vs. 1/p" ) ;
      //      data6 -> SetTitle("Z Pointing Resolution .vs. Pt" ) ;
      if ( Clear == 1 )
	{
	  data6 -> GetXaxis()->SetLimits(0.0,5.0) ;
	  data6 -> Draw("AC") ;	  
	  data6 -> GetXaxis()->SetTitle("1 / Momentum (c/GeV)") ;
	  data6 -> GetXaxis()->CenterTitle();
	  data6 -> GetXaxis()->SetNoExponent(1) ;
	  data6 -> GetXaxis()->SetMoreLogLabels(1) ;
	  data6 -> GetYaxis()->SetTitle("Z Pointing Resolution (microns)") ;
	  data6 -> GetYaxis()->CenterTitle();
	}
      else
	{ 
	  data6 -> Draw("C") ;
	}

      // HFT Efficiency Calculation and Plotting
      c5 -> cd() ;
      //c5 -> SetLogx() ;  // JT test
      c5 -> SetGrid() ; 

      Double_t DoNotDecayFactor;
      for ( Int_t massloop = 0 ; massloop < 2 ; massloop++)
	{
	  for ( Int_t j = 0 ; j < 400 ; j++ )
	    { // JT Test Let the kaon decay.  If it decays inside the TPC ... then it is gone; for all decays < 130 cm.
	      Double_t momentum = xpoint[j] / TMath::Cos(lambda)           ;  // Total momentum at average rapidity
	      if ( mass[massloop] == KaonMass ) 
		{
		  DoNotDecayFactor  = TMath::Exp(-130/(371*momentum/KaonMass)) ;  // Decay length for kaon is 371 cm.
		}
	      else
		{
		  DoNotDecayFactor = 1.0 ;
		}
	      efficiency[massloop][j] *= AcceptanceOfTpcSSD*DoNotDecayFactor ;
	    }      
	}

      data7 =  new TGraph ( 400, xpoint, efficiency[1] ) ;
      data7 -> SetMinimum(0.01)  ; 
      data7 -> SetMaximum(1.0)   ; 
      data7 -> SetTitle("Single Track Efficiency for the HFT (D0 Efficiency dashed) .vs. Pt" ) ;
      if ( Clear == 1 )
	{
	  data7 -> Draw("AC") ; 
	  data7 -> GetXaxis()->SetTitle("Transverse Momentum (GeV/c)") ;
	  data7 -> GetXaxis()->CenterTitle();
	  data7 -> GetXaxis()->SetNoExponent(1) ;
	  data7 -> GetXaxis()->SetMoreLogLabels(1) ;
	  data7 -> GetYaxis()->SetTitle("Efficiency (arbitrary units)") ;
	  data7 -> GetYaxis()->CenterTitle();
	}
      else
	{ 
	    data7 -> Draw("C") ;
	}
      // HFT D0 Efficiency
      c5 -> cd() ;
      myStyle -> SetLineWidth(1) ;
      myStyle -> SetLineStyle(kDashed) ;
      if ( ParticleMass == D0Mass ) 
	{
	  for ( Int_t i = 0 ; i < 400 ; i++ ) 
	    { 
	      pt  =  xpoint[i]  ;
	      D0efficiency[i] = D0IntegratedEfficiency(pt,efficiency) ;
	    }
	  data8 =  new TGraph ( 400, xpoint, D0efficiency ) ;
	  data8 -> SetMinimum(0.01) ; 
	  data8 -> SetMaximum(1.0)  ; 
	  if ( BeginIndex==VtxIndex && Thickness[Pxl1Index]==Pxl1Thickness && Thickness[Pxl2Index]==Pxl2Thickness  ) 
	    data8 -> Draw("C") ; // JT Test comment out this line if you want to avoid the estimated D0 efficiency curve
	}      
      myStyle -> SetLineWidth(2) ;
      myStyle -> SetLineStyle(kSolid) ;
      // End Efficiency Calculation and Plotting

      // PXL Reference Section
      c2 -> cd() ;
      myStyle -> SetLineWidth(1) ;
      myStyle -> SetLineStyle(kDashed) ;
      myStyle -> SetLineColor(kBlue) ;

      for ( Int_t i = 0 ; i < 400 ; i++ )  // PXL Reference data as if PXL was acting all alone in the rphi plane
	{
	  pt  =  xpoint[i]  ;
	  momentum = pt / TMath::Cos(lambda)   ;  // Total momentum
	  PXLReference[i] =  Pxl1Resolution*Pxl1Resolution*Pxl2Radius*Pxl2Radius 
	                  +  Pxl2Resolution*Pxl2Resolution*Pxl1Radius*Pxl1Radius ;
	  PXLReference[i] /= ( Pxl2Radius - Pxl1Radius ) * ( Pxl2Radius - Pxl1Radius ) ;
	  PXLReference[i] *= (SiScaleFactor*SiScaleFactor) ;
	  layerThickness = Pxl1Thickness / TMath::Sin(TMath::Pi()/2 - lambda) ;
	  MCS = ThetaMCS(mass[1], layerThickness,momentum) ;
	  PXLReference[i] += Pxl1Radius*Pxl1Radius*MCS*MCS ;
	  PXLReference[i] =  TMath::Sqrt(PXLReference[i]) * 10000.0 ;  // result in microns
	}
      data5 =  new TGraph ( 400, xpoint, PXLReference ) ;
      data5 -> SetMinimum(9.99)    ;
      data5 -> SetMaximum(9999.1) ;
      //data5 -> Draw("C") ;  // JT Test ... comment out this line if you want to avoid the blue dashed PXL Ref line
      c4 -> cd() ;
      //data5 -> Draw("C") ;  // JT Test ... comment out this line if you want to avoid the blue dashed PXL Ref line
      myStyle -> SetLineWidth(2) ;
      myStyle -> SetLineStyle(kSolid) ;
      myStyle -> SetLineColor(LineColour) ;
      // End PXL Reference Section

      c1 -> Update() ;
      c5 -> Update() ;
      c3 -> Update() ;
      c4 -> Update() ;
      c2 -> Update() ;
      Clear = 0 ;  // Reset clear flag to allow multiple graphs on one canvas

    }

}


Double_t ThetaMCS ( Double_t mass, Double_t RadLength, Double_t momentum ) 
{
  Double_t beta  =  momentum / TMath::Sqrt(momentum*momentum+mass*mass)  ;
  Double_t theta =  0.0 ;    // Momentum and mass in GeV
  if ( RadLength > 0 ) theta  =  0.0136 * TMath::Sqrt(RadLength) / ( beta * momentum ) ;  // Without Log term
  //Note that PDG says to use the log term (noted below) but that this does not agree with Geant calculations
  //if ( RadLength > 0 ) theta  =  0.0136 *(1+0.038*TMath::Log(RadLength))* TMath::Sqrt(RadLength) / ( beta * momentum ) ;
  return (theta) ;
}


Double_t ProbGoodHit ( Double_t Radius, Double_t SearchRadiusRPhi, Double_t SearchRadiusZ ) 
{
  // Based on work by Howard Wieman: http://rnc.lbl.gov/~wieman/GhostTracks.htm 
  // and http://rnc.lbl.gov/~wieman/HitFinding2D.htm
  // This is the probability of getting a good hit using 2D Gaussian distribution function and infinite search radius
  Double_t Sx, Sy, GoodHit ;
  Sx = 2 * TMath::Pi() *  SearchRadiusRPhi * SearchRadiusRPhi * HitDensity(Radius) ;
  Sy = 2 * TMath::Pi() *  SearchRadiusZ    * SearchRadiusZ    * HitDensity(Radius) ;
  GoodHit =  TMath::Sqrt(1./((1+Sx)*(1+Sy)))  ;
  return ( GoodHit ) ;
}


Double_t ProbGoodChiSqHit ( Double_t Radius, Double_t SearchRadiusRPhi, Double_t SearchRadiusZ ) 
{
  // Based on work by Victor Perevoztchikov and Howard Wieman: http://rnc.lbl.gov/~wieman/HitFinding2DXsq.htm
  // This is the probability of getting a good hit using a Chi**2 search on a 2D Gaussian distribution function
  Double_t Sx, GoodHit ;
  Sx = 2 * TMath::Pi() *  SearchRadiusRPhi * SearchRadiusZ * HitDensity(Radius) ;
  GoodHit =  1./(1+Sx) ;
  return ( GoodHit ) ;  
}


Double_t HitDensity ( Double_t Radius ) 
{
  // Background (0-1) is included via 'OtherBackground' which multiplies the minBias rate by a scale factor.
  // UPC electrons is a temporary kludge that is based on Kai Schweda's summary of Kai Hainken's MC results
  // See K. Hencken et al. PRC 69, 054902 (2004) and PPT slides by Kai Schweda.
  // Note that this function assumes we are working in CM and CM**2 [not meters].
  // Based on work by Yan Lu 12/20/2006, all radii and densities in centimeters or cm**2.

  Double_t MaxRadiusSlowDet = BeamPipe2Radius             ;  // Maximum radius for slow detectors.  Fast detectors 
                                                             // and only fast detectors reside outside this radius.
  Double_t ArealDensity = 0                               ;

  if ( Radius > MaxRadiusSlowDet ) 
    {
      ArealDensity  = OneEventHitDensity(dNdEtaCent,Radius)  ; // Fast detectors see central collision density (only)
      ArealDensity += OtherBackground*OneEventHitDensity(dNdEtaMinB,Radius)  ;  // Increase density due to background 
    }

  if (Radius < MaxRadiusSlowDet )
    { // Note that IntegratedHitDensity will always be minB one event, or more, even if integration time => zero.
      ArealDensity  = OneEventHitDensity(dNdEtaCent,Radius) 
	            + IntegratedHitDensity(dNdEtaMinB,Radius) 
	            + UpcHitDensity(Radius) ;
      ArealDensity += OtherBackground*IntegratedHitDensity(dNdEtaMinB,Radius) ;  // Increase density due to background 
    } 

  return ( ArealDensity ) ;  
}


double OneEventHitDensity( Double_t Multiplicity, Double_t Radius )
{
  // This is for one event at the vertex.  No smearing.
  double den   = Multiplicity / (2.*TMath::Pi()*Radius*Radius) ;
  return den ;
} 


double IntegratedHitDensity(Double_t Multiplicity, Double_t Radius)
{ 
  // The integral of minBias events smeared over a gaussian vertex distribution.
  // Based on work by Yan Lu 12/20/2006, all radii in centimeters.

  Double_t ZdcHz = Luminosity * 1.e-24 * CrossSectionMinB ;
  Double_t den   = ZdcHz * IntegrationTime/1000. * Multiplicity * Dist(0., Radius) / (2.*TMath::Pi()*Radius) ;

  // Note that we do not allow the rate*time calculation to fall below one minB event at the vertex.
  if ( den < OneEventHitDensity(Multiplicity,Radius) )  den = OneEventHitDensity(Multiplicity,Radius) ;  

  return den ;
} 


double UpcHitDensity(Double_t Radius)
{ 
  Double_t UPCelectrons ;                                 ;  
  UPCelectrons =  1.23 - Radius/6.5                       ;  // Fit to Kai Schweda summary tables.
//UPCelectrons =  (56.4 - Radius*6.55)                    ;  // Fit to Spiro's new summary tables, not must comment out IntHitDen multiplier, below.
  if ( UPCelectrons < 0 ) UPCelectrons =  0.0             ;  // UPC electrons fall off quickly and don't go to large R
  UPCelectrons *= IntegratedHitDensity(dNdEtaMinB,Radius) ;  // UPCs increase Mulitiplicty ~ proportional to MinBias rate
  UPCelectrons *= UPCBackgroundMultiplier                 ;  // Allow for an external multiplier (eg 0-1) to turn off UPC

  /*
  Double_t ZdcHz = Luminosity * 1.e-24 * CrossSectionUPC ;
  Double_t den   = ZdcHz * IntegrationTime/1000. * Multiplicity * Dist(0., Radius) / (2.*TMath::Pi()*Radius) ;
  // Note that we do not allow the rate*time calculation to fall below one minB event at the vertex.
  if ( den < OneEventHitDensity(Multiplicity,Radius) )  den = OneEventHitDensity(Multiplicity,Radius) ;  
  */ 

  return UPCelectrons ;
} 


double Dist(double z, double r)
{
  // Convolute dEta/dZ  distribution with assumed Gaussian of vertex z distribution
  // Based on work by Howard Wieman http://rnc.lbl.gov/~wieman/HitDensityMeasuredLuminosity7.htm
  // Based on work by Yan Lu 12/20/2006, all radii and Z location in centimeters.
  Int_t    Index  =  1     ;     // Start weight at 1 for Simpsons rule integration
  Int_t    NSTEPS =  301   ;     // NSteps must be odd for Simpson's rule to work
  double   dist   =  0.0   ;
  double   dz0    =  ( 4*SigmaD - (-4)*SigmaD ) / (NSTEPS-1)  ;  //cm
  double    z0    =  0.0   ;     //cm
  for(int i=0; i<NSTEPS; i++){
    if ( i == NSTEPS-1 ) Index = 1 ;
    z0 = -4*SigmaD + i*dz0 ;
    dist += Index * (dz0/3.) * (1/sqrt(2.*TMath::Pi())/SigmaD) * exp(-z0*z0/2./SigmaD/SigmaD) * 
      (1/sqrt((z-z0)*(z-z0) + r*r)) ;
    if ( Index != 4 ) Index = 4; else Index = 2 ;
  }
  return dist; 
}


#define  PZero   0.861  // Momentum of back to back decay particles in the CM frame
#define  EPiZero 0.872  // Energy of the pion from a D0 decay at rest
#define  EKZero  0.993  // Energy of the Kaon from a D0 decay at rest

Double_t D0IntegratedEfficiency( Double_t pt, Double_t efficiency[2][400] )
{ // Math from Ron Longacre.  Note hardwired energy to bin conversion for PtK and PtPi.

  Double_t Const1  =  pt / D0Mass ;
  Double_t Const2  =  TMath::Sqrt(pt*pt+D0Mass*D0Mass) / D0Mass ;
  Double_t sum, PtPi, PtK ;
  Double_t effp, effk ;

  sum = 0.0 ;
  for ( Int_t j = 0 ; j < 360 ; j++ )
    {

      Double_t theta = j * TMath::Pi() / 180. ;

      PtPi = TMath::Sqrt( 
			 PZero*PZero*TMath::Cos(theta)*TMath::Cos(theta)*Const2*Const2 +
			 Const1*Const1*EPiZero*EPiZero -
			 2*PZero*TMath::Cos(theta)*Const2*Const1*EPiZero +
			 PZero*PZero*TMath::Sin(theta)*TMath::Sin(theta)
			 ) ;

      PtK = TMath::Sqrt( 
			PZero*PZero*TMath::Cos(theta)*TMath::Cos(theta)*Const2*Const2 +
			Const1*Const1*EKZero*EKZero +
			2*PZero*TMath::Cos(theta)*Const2*Const1*EKZero +
			PZero*PZero*TMath::Sin(theta)*TMath::Sin(theta)
			) ;

      // JT Test Remove 100 MeV/c in pt to simulate eta!=0 decays
      Int_t pionindex = (int)((PtPi-0.1)*100.0 - 65.0*TMath::Abs(BFIELD)) ; 
      Int_t kaonindex = (int)((PtK -0.1)*100.0 - 65.0*TMath::Abs(BFIELD)) ; 
      
      if ( pionindex >= 400 ) pionindex = 399 ;
      if ( pionindex >= 0 )   effp = efficiency[0][pionindex] ;
      if ( pionindex <  0 )   effp = (efficiency[0][1]-efficiency[0][0])*pionindex + efficiency[0][0] ; // Extrapolate if reqd
      if ( effp < 0 )         effp = 0 ;

      if ( kaonindex >= 400 ) kaonindex = 399 ;
      if ( kaonindex >= 0 )   effk = efficiency[1][kaonindex] ;
      if ( kaonindex <  0 )   effk = (efficiency[1][1]-efficiency[1][0])*kaonindex + efficiency[1][0] ; // Extrapolate if reqd
      if ( effk < 0 )         effk = 0 ;

      // Note that we assume that the Kaon Decay efficiency has already been inlcuded in the kaon efficiency used here.
      
      sum += effp * effk ;

     }    
  
  return(sum/360) ;
  
}

// Notes:
// 09/18/2006   You must compile this macro.  In otherwords: root4star> .x Matrix5Resolution.C++
// 
// 09/18/2006   Code built to include HPD and two IST layers.  Note that input format has changed
//              Past experience has shown that ITTF gets similar results when the detector resolutions 
//              are set to the 3 sigma level.
//
// 09/21/2006   Using Kalman Filter techniques from Billoir NIM 225 (1984) 352.
//
// 10/06/2006   Extend technique from 3x3 matrices (1D) to 5x5 matrices (2D), note that CHARGE sign is important.
//
// 10/10/2006   This technique is only precise for rays that are traced from the outside, going inwards.  It is
//              not exact when you ask it to calculate the resolution beyond the last active detector and it is 
//              not exact when you ask for the pointing resolution between the first and last detectors.  In 
//              particular, this means that a vertex constraint confuses things.  If you want to know the pointing
//              resolution at PXL2 with a vertex constraint, this means you are asking for the resolution at
//              a point that is midway between the first and last 'active' detectors.  Billoirs method was not
//              designed to handle this intermediate case. I have built in a good approximation to handle these
//              special cases, but the approximations will break down under some circumstance.  They are good
//              enough for most anything we will do in STAR. (The approximations break down when you have a lot
//              of multiple scattering between the vertex and the intermediate point of interest.)  In order to 
//              avoid the approximations, you can usually ask a different question and get the right answer. For
//              example, a vertex constraint does not assist the pointing at the SSD ... so don't use it.  Etc.
//              For example: "d-hft2 ist1 ist2 ssd tpc end t-hft2 kBlue" is the most accurate way to inquire
//              about the pointing resolution at the outer surface of the PXL.
//
// 11/10/2006   Add efficiency calculation.  Start with the effeciency (1-ProbGhost) for the target layer.
//
// 12/1/2006    Note ... THE EFFICIENCY CALC IS NOT YET TUNED UP!!!!  The critical work that needs to be done 
//              is to get a precise model of the hit density as a function of radius (including UPC electrons).  
//              The plotting also needs to be cleaned up.  The key functions are at the bottom of this file.
//              [This comment is now obsolete. 12/20/2006]
//  
// 12/10/2006   Add color at the end of the text line.  Use "kBlack", "kBlue", etc to describe the color of the lines
//
// 12/11/2006   You can now edit this file, recompile, and put additional graphs on the same plots on the screen
//              by restarting the program.  It will automatically continue plotting into the same canvases as
//              before unless you say 'clear'.
// 
// 12/12/2006   Add a parameter called 'SiScaleFactor' to scale the Si pad size by 1/root(12).
//
// 12/14/2006   Add Efficiency plot tuned up for the PXL.  It does not apply to any other detector nor tell
//              you whether another detector has a ghosting problem, or not.  Added new parameters:
//              Luminosity == RHIC luminosity (ie 1x10e28 is RHIC II Luminosity)
//              IntegrationTime == Integration time of PXL chips (in milliseconds)
//              BackgroundMultiplier == Extra increase in background if you are a pessimist about rates in the PXL
//
// 12/17/2006   Added UPC electron background to HitDensity() function.  UPC electrons only affect detectors at
//              small radius.  They do not affect fast detectors.  This is built into the code but needs improvement.
//              Added calculated decrease in rate into the PXL due to spread of vertex, and convoluted integrals.
//
// 12/20/2006   Added two dimensional Gaussian distribution of hits on HFT, and an infinite search radius for Ghosts
//
// 01/03/2007   Add D0 efficiency on the HFT efficiency plot.  This is a kludge which entails squaring the single
//              track efficiency and  multiplying by 80% to represent the TPC efficiency.
//
// 01/03/2007   Add Victor and Howards Chi squared search efficiencies
//
// 02/01/2007   Add Ist1Prime and Ist2Prime layers as a way to include extra pad layers (or other possibilities)
//
// 02/13/2007   Add 2x energy scale for D0 efficiency calculation.  The D0 efficiency is the single track efficiency 
//              squared times 0.8, to represent the inefficiency of the rest of the tracking system, and the energy 
//              scale has been doubled to represent the summed energy of two mean pT particles.  
//
// 06/05/2007   Add routine D0IntegratedEfficiency() to do the D0 efficiency properly with 2 body decay kinematics 
//              and an integral over all possible decay angles in the CM frame.  Boost results to lab frame and plot.
//
// 06/06/2007   Single track efficiency calculations now apply to all detectors (not just the HFT Pixel layers).
//              D0 efficiency calculations will only appear when the vertex and the HFT Pixel layers are included 
//              in the detector description line-of-text.  
//
// 06/30/2007   Add AcceptanceOfTPCSsd to represent the combined acceptance of the TPC and SSD (typically 75%).
//              Add table of radii, pointing resolution, and efficiencies at 750 MeV/c.  The table appears in 
//              the command window below the detector description line-of-text.  The tabulated values do not
//              include the acceptance of the TPC and SSD ... you have to do this in your head.  However, the
//              TPC and SSD acceptance factor *is* applied to the values shown on the efficiency plot.
//
// 06/31/2007   Add OtherBackground as a multiplier on top of the minbias rate as a way to load up on extra hits.
//              Drop GlobalTrackMultiplier and re-organize how background is added.  Now it should be more intuitive.
//
// 07/18/2007   Change names so use pxl layers rather than hft layers.  Otherwise same code.
//
// 07/20/2007   Update matrix math to eliminate some inverse of the inverse for 5 dimensional matrices.
//
// 07/22/2007   Delete all code with reference to a 'target'.  The concept of a 'target' is not used anymore.  The 
//              'target' is now the first detector in the list provided by the user as my might intuitively expect.
//
// 09/06/2007   Add two loops through Matrix calculations to handle D0 with two daughters.  One pion, one kion. If
//              you input the D0 mass as the test particle mass, then you will get a D0 efficiency spectrum.  The 
//              single track efficiency that is plotted with the D0 efficiency is the kaon single track efficiency.
//              If you input any other mass, you will only get the efficiency for that one particle.
//           
// 09/20/2007   Allow Kaon to decay ... if it decays inside the detector, then the track is not found. 
//
// 12/04/2008   Fix small bug in blanking out rows 1 and 13.  Now it is blank in both R-Phi and Z.  
