#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx"
#include "ROOT/RDF/RInterface.hxx"
#include "TCanvas.h"
#include "TH1D.h"
#include "TLatex.h"
#include "TLegend.h"
#include "Math/Vector4Dfwd.h"
#include "TStyle.h"
#include <vector>

using namespace ROOT::VecOps;
using rvec_i = RVec<int>;

template <typename T>
void plot(T hist, TString name){
    TCanvas *c = new TCanvas("c",Form("c_%s", name.Data()));
    hist->Write();
    hist->DrawClone();
    c->Print(Form("%s.pdf",name.Data()));
}

int goodleading_idx(rvec_i good){
    int idx = -1;
    for( int i = 0; i < good.size(); i++){
        if(good[i]) idx = i;
        break;
    }
    return idx;
}

   //   double L = 59.7;
   //   double cs_sig = 365.34;
   //   double cs_bg = 2793;
   //   L*cs_sig = 21819.798;
   //   L*cs_bg = 166742.1;
   //   n. bg = 98362049;
   //                            n. bg =   98362049;
   //        ns= 4290538, nb=32787350         
   //df_166 : 9767000 & 5476462         
   //df_169 : 28704000 & 24413462      
   //df_171 : 24696000 & 20405462     
   //df_173 : 23892000 & 19601462   
   //df_175 : 29303000 & 25012462  
   //df_178 : 9762000 & 5471462   
   //df_nominal : 101550000 & 97259462 
               

void Tmass(){

        std::vector<string> obj = {"Muon1_pt"};


	ROOT::RDataFrame df_signal("Events", {"/xrootd/store/mc/RunIIAutumn18NanoAODv6/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/230000/*.root", "/xrootd/store/mc/RunIIAutumn18NanoAODv6/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/240000/*.root","/xrootd/store/mc/RunIIAutumn18NanoAODv6/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/30000/*.root"});
	ROOT::RDataFrame df_bg("Events", {"/xrootd/store/mc/RunIIAutumn18NanoAODv6/WJetsToLNu_2J_TuneCP5_13TeV-amcatnloFXFX-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/230000/*.root", "/xrootd/store/mc/RunIIAutumn18NanoAODv6/WJetsToLNu_2J_TuneCP5_13TeV-amcatnloFXFX-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/250000/*.root", "/xrootd/store/mc/RunIIAutumn18NanoAODv6/WJetsToLNu_2J_TuneCP5_13TeV-amcatnloFXFX-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/60000/*.root"});

   //ns= 4290538, nb=32787350         
   //df_166 : 9767000 & 5476462         
   //df_169 : 28704000 & 24413462      
   //df_171 : 24696000 & 20405462     
   //df_173 : 23892000 & 19601462   
   //df_175 : 29303000 & 25012462  
   //df_178 : 9762000 & 5471462   
   //df_nominal : 101550000 & 97259462 
                                   
   //	L*cs_sig = 21819.798;
   //   L*cs_bg = 166742.1;
   
    auto df_s0_bg = df_bg.Range(65574699,0);	        // number of bg = 32797350  
    auto df_s0_sig = df_signal.Range(97259462, 0)	// number of sig = 4290538


    //**** Muon Selection ( tight ) ****//
    auto df_s1_sig = df_s0_sig.Define("goodMuon","Muon_pt >= 30 && abs(Muon_eta) < 2.5 && Muon_tightId && Muon_pfRelIso04_all < 0.15")
                              .Define("n_goodMuon","Sum(goodMuon)")
                              .Define("Muon1_pt","Muon_pt[goodMuon][0]")
                              .Filter("n_goodMuon == 1","Muon Selection")
                              .Define("goodMuon_leading_idx",goodleading_idx,{"goodMuon"});
   auto df_s1_bg = df_s0_bg.Define("goodMuon","Muon_pt >= 30 && abs(Muon_eta) < 2.5 && Muon_tightId && Muon_pfRelIso04_all < 0.15")
                           .Define("n_goodMuon","Sum(goodMuon)")
                           .Define("Muon1_pt","Muon_pt[goodMuon][0]")
                           .Filter("n_goodMuon == 1","Muon Selection")
                           .Define("goodMuon_leading_idx",goodleading_idx,{"goodMuon"});

    //**** Jet Selection ( tight ) ****//
    auto df_s2_sig = df_s1_sig.Define("goodJet","Jet_pt >= 30 && abs(Jet_eta) < 2.5 && Jet_jetId == 6")
                              .Define("n_goodJet","Sum(goodJet)")
                              .Filter("n_goodJet >= 4","Jet Selection");
    auto df_s2_bg = df_s1_bg.Define("goodJet","Jet_pt >= 30 && abs(Jet_eta) < 2.5 && Jet_jetId == 6")
                            .Define("n_goodJet","Sum(goodJet)")
                            .Filter("n_goodJet >= 4","Jet Selection");

    //**** b Tagged Jet selection ( medium )****//
    auto df_s3_sig = df_s2_sig.Define("goodbJet","goodJet && Jet_btagDeepFlavB > 0.277")
                              .Define("n_goodbJet","Sum(goodbJet)")
                              .Filter("n_goodbJet >= 2","bJet Selection");
    auto df_s3_bg = df_s2_bg.Define("goodbJet","goodJet && Jet_btagDeepFlavB > 0.277")
                            .Define("n_goodbJet","Sum(goodbJet)")
                            .Filter("n_goodbJet >= 2","bJet Selection");

   //***** MET selection *****//
    auto df_s4_sig = df_s3_sig.Filter("MET_pt >= 20", "MET Selection");
    auto df_s4_bg = df_s3_bg.Filter("MET_pt >= 20", "MET Selection");

   //***** W boson transverse mass selection *****//
   auto df_s5_sig = df_s4_sig.Define("WTmass","sqrt(Muon_pt[goodMuon_leading_idx]*MET_pt*(1-cos(Muon_phi[goodMuon_leading_idx]-MET_phi)))")
                             .Filter("(WTmass+MET_pt)>=60", "WTmass Selection");
   auto df_s5_bg = df_s4_bg.Define("WTmass","sqrt(Muon_pt[goodMuon_leading_idx]*MET_pt*(1-cos(Muon_phi[goodMuon_leading_idx]-MET_phi)))")
                           .Filter("(WTmass+MET_pt)>=60", "WTmass Selection");

    //histograms 
    auto h_muon_pt_sig = df_s5_sig.Histo1D({"h_muon_pt_sig", "h_muon_pt_sig", 24, 30, 150}, "Muon1_pt");
    auto h_muon_pt_bg = df_s5_bg.Histo1D({"h_muon_pt_bg", "h_muon_pt_bg", 24, 30, 150}, "Muon1_pt");

    auto h_event_sig = df_s0_sig.Histo1D({"h_event_sig","h_event_sig",10,0,10},"nMuon");
    auto h_event_bg = df_s0_bg.Histo1D({"h_event_bg","h_event_bg",10,0,10},"nMuon");


    //ntuple
    df_s5_sig.Snapshot("tree","ntuple_s.root",obj);
    df_s5_bg.Snapshot("tree","ntuple_b.root",obj);
          

    TFile f("out.root", "recreate");
 
    plot(h_muon_pt_sig, "h_muon_pt_sig");
    plot(h_muon_pt_bg, "h_muon_pt_bg");

    plot(h_event_sig, "h_event_sig");
    plot(h_event_bg, "h_event_bg");


    f.Close();

}
