#include "tdrstyle.C"

void makePlot(TFile *f){

   TH1F *h1 = (TH1F*) f->Get("h_muon_pt_W_1345");
   TH1F *h2 = (TH1F*) f->Get("h_muon_pt_W_359");
   TH1F *h3 = (TH1F*) f->Get("h_muon_pt_W_48");
   TH1F *h4 = (TH1F*) f->Get("h_muon_pt_W_12");
   TH1F *h5 = (TH1F*) f->Get("h_muon_pt_W_5");
   TH1F *h6 = (TH1F*) f->Get("h_muon_pt_W_1");
   TH1F *h7 = (TH1F*) f->Get("h_muon_pt_W_0");

//   # Calculate SF
//histintegral = hist.Integral() # Nocut entry
//sf *= xsec * integrlumi / histintegral # Normalize with xsec, lumi, nocut entries.
//hist.Scale(sf) # Just scaling with SF.

   double lumi = 59.74;
   double sf1 = 1345*1000*lumi/(h1->Integral());
   double sf2 = 359.7*1000*lumi/(h2->Integral());
   double sf3 = 48.91*1000*lumi/(h3->Integral());
   double sf4 = 12.05*1000*lumi/(h4->Integral());
   double sf5 = 5.501*1000*lumi/(h5->Integral());
   double sf6 = 1.329*1000*lumi/(h6->Integral());
   double sf7 = 0.03216*1000*lumi/(h7->Integral());
   
   h1->Scale(sf1);
   h2->Scale(sf2);
   h3->Scale(sf3);
   h4->Scale(sf4);
   h5->Scale(sf5);
   h6->Scale(sf6);
   h7->Scale(sf7);

   h1->Add(h2);
   h1->Add(h3);
   h1->Add(h4);
   h1->Add(h5);
   h1->Add(h6);
   h1->Add(h7);
 
   TFile file("out.root", "recreate");
   TCanvas *c = new TCanvas("c","h_muon_pt_bg");
   h1->Write();
   h1->DrawClone();
   c->Print("h_muon_pt_bg.pdf");
   file.Close();
   
}
void plots(){

   setTDRStyle();
   TFile *f_muon = new TFile("out_W.root");

   makePlot(f_muon);
}
