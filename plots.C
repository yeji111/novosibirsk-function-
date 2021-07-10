#include "tdrstyle.C"

void makePlot(TFile *f){

   TH1F *h1 = (TH1F*) f->Get("h_muon_pt_W_1345");
   TH1F *h2 = (TH1F*) f->Get("h_muon_pt_W_359");
   TH1F *h3 = (TH1F*) f->Get("h_muon_pt_W_48");
   TH1F *h4 = (TH1F*) f->Get("h_muon_pt_W_12");
   TH1F *h5 = (TH1F*) f->Get("h_muon_pt_W_5");
   TH1F *h6 = (TH1F*) f->Get("h_muon_pt_W_1");
   TH1F *h7 = (TH1F*) f->Get("h_muon_pt_W_0");

   THStack *hs = new THStack("hs","h_muon_pt_bg");

   hs->Add(h1);
   hs->Add(h2);
   hs->Add(h3);
   hs->Add(h4);
   hs->Add(h5);
   hs->Add(h6);
   hs->Add(h7);

   hs->Draw();
   TH1F *hist = (TH1F*) hs->GetHistogram();
 
   TFile file("out.root", "recreate");
   TCanvas *c = new TCanvas("c","h_muon_pt_bg");
   hist->Write();
   hist->DrawClone();
   c->Print("h_muon_pt_bg.pdf");
   file.Close();
   
}
void plots(){

   setTDRStyle();
   TFile *f_muon = new TFile("out_W.root");

   makePlot(f_muon);
}
