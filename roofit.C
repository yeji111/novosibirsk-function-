#include "TF1.h"
#include "TMath.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooNovosibirsk.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TAxis.h"
#include "RooDataHist.h"
using namespace RooFit;

void makeFit(TFile *f){

   //// Get histograms ////
   TH1F *h = (TH1F*) f->Get("h_muon_pt_fake");
   RooRealVar Muon1_pt("Muon1_pt", "P_{T} (GeV)", 30, 150);
   RooDataHist data("data","dataset with muon_pt",Muon1_pt,h); 

   cout << "n. fake = " << h->Integral() << endl;

   //// Set Variables of model function ////
   RooRealVar mass("mass", "mass", 150, 200);
   RooRealVar normalization("normalization","normalization", 0.1);
   RooRealVar mui("mui","mui", 50.2);
   RooRealVar mus("mus","mus", -0.1686);
   RooRealVar sigi("sigi","sigi", -19.91);
   RooRealVar sigs("sigs","sigs", 0.2555);
   RooRealVar taui("taui","taui", 1.257);
   RooRealVar taus("taus","taus", -0.004062);

   RooFormulaVar mu("mu","mui+mass*mus", RooArgList(mui,mass,mus)); 
   RooFormulaVar sigma("sigma","sigi+mass*sigs", RooArgList(sigi,mass,sigs)); 
   RooFormulaVar tau("tau","taui+mass*taus", RooArgList(taui,mass,taus)); 
  
   RooGenericPdf lambda("lambda", "normalization*exp(-0.5*pow(((log(1+(Muon1_pt-mu)*sinh(tau*sqrt(log(4)))/(sigma*sqrt(log(4)))))/tau),2)+pow(tau,2))", RooArgList(normalization, Muon1_pt, mu, tau, sigma));
/*   
   //// create binned dataset & Pdf ////
   RooDataSet *lam_data = lambda.generate(Muon1_pt, 10000);
   TH1 *h1_lam = lam_data->createHistogram("Muon1_pt", 24);
   RooDataHist hist_lam("hist_lam","hist_lam", Muon1_pt, Import(*h1_lam));
   RooHistPdf lam_pdf("lam_pdf", "lam_pdf", Muon1_pt, hist_lam); 
*/   
   //// Fitting data to model ////
   RooPlot *frame = Muon1_pt.frame();
  // RooFitResult* result = lam_pdf.fitTo(data, Range(30,150), Save());
    RooFitResult *result = lambda.fitTo(data, Save());
   data.plotOn(frame);
  // lam_pdf.plotOn(frame, LineColor(kBlue));
   lambda.plotOn(frame, LineColor(kBlue));   

   //// Fitting results ////
   mass.Print();
   result->Print();

   //// create NLL ////
   RooAbsReal *nll = lambda.createNLL(data);
   RooPlot *frame_nll = mass.frame(Range(170, 175));
   nll->plotOn(frame_nll, ShiftToZero());
   frame_nll -> SetMinimum(0);
   frame_nll -> SetMaximum(3);

  //// create distribution subtracted ////
  // RooRealVar mass_frame("mass_frame", "mass_frame", 165, 180);
  // RooPlot *frame_sub = mass_frame.frame(Range(165, 180));
  // RooRealVar real_mass("real_mass", "real_mass", 172.5);
  // real_mass.plotOn(frame_sub, ShiftToZero(), LineColor(kGreen));
  // mass.plotOn(frame_sub);
  // frame_sub -> SetMinimum(-4);
  // frame_sub -> SetMaximum(4);

   frame->Draw();
   frame_nll->Draw();
  // frame_sub->Draw();

   //// plot ////
   TCanvas *c = new TCanvas("c_fake_fit","c",1);
   gStyle->SetOptFit(1);
   frame->Draw();
   c->Print("fitting_fake.pdf");

   TCanvas *c_nll = new TCanvas("c_fake_fit_nll","c",1);
   frame_nll->Draw();
   c_nll->Print("fitting_fake_nll.pdf");

//   TCanvas *c_sub = new TCanvas("c_fake_fit_sub","c",1);
//   frame_sub->Draw();
//   c_sub->Print("fitting_fake_sub.pdf");


   TFile fake("fit_fake.root","update");
   result->Write("fitting_result");
   fake.Close();
}

void roofit(){

   TFile *f = new TFile("fake_data.root");
   makeFit(f);
}
