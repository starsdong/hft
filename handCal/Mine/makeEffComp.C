void makeEffComp()
{
  gROOT->Reset();
  
  const Int_t NConfig = 4;
  const Int_t NParticle = 3;

  const Char_t *inDir[NConfig] = {"Current","New1","New3","New4"};
  const Char_t *particles[NParticle] = {"pion","kaon","proton"};

  TGraph *gr[NConfig][NParticle];

  char inname[100];
  for(int i=0;i<NConfig;i++) {
    for(int j=0;j<NParticle;j++) {
      sprintf(inname,"%s/output_%s.root",inDir[i],particles[j]);
      TFile *fin = new TFile(inname);
      gr[i][j] = (TGraph *)fin->Get("Graph;7");
      fin->Close();
    }
  }

   TCanvas *c1 = new TCanvas("c1", "c1",0,0,800,600);
   gStyle->SetOptFit(0);
   gStyle->SetOptStat(0);
   gStyle->SetEndErrorSize(0.01);
   c1->SetFillColor(10);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetFrameFillColor(0);
   c1->SetFrameBorderMode(0);

   c1->SetGridx();
   c1->SetGridy();

   c1->SetLeftMargin(0.13);
   c1->SetBottomMargin(0.13);
   c1->SetTopMargin(0.02);
   c1->SetRightMargin(0.06);

   double x1 = 0;
   double x2 = 3;
   double y1 = 0;
   double y2 = 1.2;

   TH1D *d0 = new TH1D("d0","",1,x1,x2);
   d0->SetMinimum(y1);
   d0->SetMaximum(y2);
   d0->GetXaxis()->SetNdivisions(208);
   d0->GetXaxis()->SetTitle("p_{T} (GeV/c)");
   d0->GetXaxis()->SetTitleOffset(0.9);
   d0->GetXaxis()->SetTitleSize(0.06);
   d0->GetXaxis()->SetLabelOffset(0.01);
   d0->GetXaxis()->SetLabelSize(0.045);
   d0->GetXaxis()->SetLabelFont(42);
   d0->GetXaxis()->SetTitleFont(42);
   d0->GetYaxis()->SetNdivisions(205);
   d0->GetYaxis()->SetTitle("Single Pion Efficiency (a.u.)");
   d0->GetYaxis()->SetTitleOffset(1.0);
   d0->GetYaxis()->SetTitleSize(0.06);
   d0->GetYaxis()->SetLabelOffset(0.005);
   d0->GetYaxis()->SetLabelSize(0.045);
   d0->GetYaxis()->SetLabelFont(42);
   d0->GetYaxis()->SetTitleFont(42);
   d0->SetLineWidth(2);
   d0->Draw();

   TLine *l1 = new TLine(x1,y1,x2,y1);
   l1->SetLineWidth(3);
   l1->Draw("same");
   TLine *l2 = new TLine(x1,y2,x2,y2);
   l2->SetLineWidth(3);
   l2->Draw("same");
   TLine *l3 = new TLine(x1,y1,x1,y2);
   l3->SetLineWidth(3);
   l3->Draw("same");
   TLine *l4 = new TLine(x2,y1,x2,y2);
   l4->SetLineWidth(3);
   l4->Draw("same");

   TLine *l0 = new TLine(x1,1.,x2,1.);
   l0->SetLineWidth(2);
   l0->SetLineStyle(2);
   l0->Draw("same");

   const Int_t kLineColor[NConfig] = {0, 2, 1, 4};
   double xx[2] = {0.5, 2.};
   double yy[2][NConfig][NParticle];
   for(int i=0;i<NConfig;i++) {
     if(i==0) continue;
     for(int j=0;j<NParticle-2;j++) {
       gr[i][j]->SetLineColor(kLineColor[i]);
       gr[i][j]->SetLineStyle(j+1);
       gr[i][j]->SetLineWidth(2);
       gr[i][j]->Draw("c");

       cout << i << "/" << j << "\t" << gr[i][j]->Eval(xx[0]) << "\t" << gr[i][j]->Eval(xx[1]) << endl;
       yy[0][i][j] = gr[i][j]->Eval(xx[0]);
       yy[1][i][j] = gr[i][j]->Eval(xx[1]);
     }
   }

   TLatex *tex = new TLatex(2.45, 0.92, "HFT^{+}(10#mus)");
   tex->SetTextSize(0.04);
   tex->SetTextFont(12);
   tex->SetTextColor(4);
   tex->Draw("same");

   TLatex *tex = new TLatex(2.45, 0.84, "HFT^{+}(40#mus)");
   tex->SetTextSize(0.04);
   tex->SetTextFont(12);
   tex->SetTextColor(1);
   tex->Draw("same");

   TLatex *tex = new TLatex(2.45, 0.73, "HFT(200#mus)");
   tex->SetTextSize(0.04);
   tex->SetTextFont(12);
   tex->SetTextColor(2);
   tex->Draw("same");

   TArrow *a1 = new TArrow(xx[0], yy[0][1][0], xx[0], yy[0][2][0], 0.03,">");
   a1->SetAngle(30);
   //   a1->SetLineStyle(7);
   a1->Draw();
   TLatex *tex = new TLatex(xx[0]+0.02, (yy[0][1][0]+yy[0][2][0])/2.,"50%");
   tex->SetTextFont(22);
   tex->SetTextSize(0.04);
   tex->Draw("same");

   TArrow *a2 = new TArrow(xx[1], yy[1][1][0], xx[1], yy[1][2][0], 0.03,">");
   a2->SetAngle(30);
   //   a2->SetLineStyle(7);
   a2->Draw();
   TLatex *tex = new TLatex(xx[1]+0.02, (yy[1][1][0]+yy[1][2][0])/2.-0.02,"18%");
   tex->SetTextFont(22);
   tex->SetTextSize(0.04);
   tex->Draw("same");

   TLatex *tex = new TLatex(1.6, 0.25, "Au+Au 200 GeV @ 2020");
   tex->SetTextFont(22);
   tex->SetTextSize(0.05);
   tex->Draw("same");
   TLatex *tex = new TLatex(2., 0.13, "ZDCx = 100 kHz");
   tex->SetTextFont(22);
   tex->SetTextSize(0.05);
   tex->Draw("same");

   // TLegend *leg = new TLegend(0.60, 0.40, 0.9, 0.54);
   // leg->SetFillColor(10);
   // leg->SetFillStyle(10);
   // leg->SetLineStyle(4000);
   // leg->SetLineColor(10);
   // leg->SetLineWidth(0.);
   // leg->SetTextFont(42);
   // leg->SetTextSize(0.045);
   // for(int j=0;j<NParticle-2;j++) {
   //   char label[100];
   //   sprintf(label,"HFT' %s", particles[j]);
   //   leg->AddEntry(gr[2][j],label,"l");
   // }
   // leg->Draw();

   // TLegend *leg = new TLegend(0.60, 0.16, 0.9, 0.30);
   // leg->SetFillColor(10);
   // leg->SetFillStyle(10);
   // leg->SetLineStyle(4000);
   // leg->SetLineColor(10);
   // leg->SetLineWidth(0.);
   // leg->SetTextFont(42);
   // leg->SetTextSize(0.045);
   // for(int j=0;j<NParticle-2;j++) {
   //   char label[100];
   //   sprintf(label,"HFT %s", particles[j]);
   //   leg->AddEntry(gr[1][j],label,"l");
   // }
   // leg->Draw();

   const Char_t *inNewDir = "New34";
   
   char outname[100];
   sprintf(outname,"EfficiencyComp_%s.eps",inNewDir);
   c1->SaveAs(outname);
   sprintf(outname,"EfficiencyComp_%s.png",inNewDir);
   c1->SaveAs(outname);

}
