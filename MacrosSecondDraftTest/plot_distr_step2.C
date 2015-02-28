/////////////////////////////////////////////////////////
//
// File: 	plot_distr_step2.C 
// Purpose: 	Distributions plotter
// Analysis:	Black Hole, CMSSW_3_8_7, 7 TeV
// Author: 	Alexey Ferapontov
// Date:	December 19, 2010
// Notes:	2011 Data Analysis School (a.k.a. EJTerm) at FNAL
//
/////////////////////////////////////////////////////////

#include <vector>
#include <set>
#include <string>
#include <assert.h>
#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <TROOT.h>
#include <TKey.h>
#include <TObject.h>
#include <TClass.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TLegend.h>
#include <TH1.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TF2.h>
#include <TPaletteAxis.h>
#include <math.h>
#include <TGraph.h>
#include <TLatex.h>

using namespace std;

void plot_distr_step2(){

  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0000000);

  //Binning rules:
  //ST: 		70 bins 0 - 7000
  //mBH:		70 bins 0 - 7000
  //Multiplicity:	20 bins 0 - 20
  //N objects:		20 bins 0 - 20	
  //the rest:		up to analyzer
    
  const TString var 	= "Multiplicity";
  const TString req 	= "";
  const int nbins 	= 20;
  const float binmin 	= 0;
  const float binmax 	= 20;

  char *namevar = var;
  char *namereq = req;
  
  TH1F *hsignal[8];
  TH1F *hbkg[5];
  TH1F *hdata[0];
  TH1F *htotbkg[1];
  
  hist_qcd(var, req, nbins, binmin, binmax, "hall_qcd");
  hbkg[0]=(TH1F*)hall_qcd->Clone();
  
  hist_photonjet(var, req, nbins, binmin, binmax, "hall_photonjet");
  hbkg[1]=(TH1F*)hall_photonjet->Clone();

  hist_wjet(var, req, nbins, binmin, binmax, "hall_wjet");
  hbkg[2]=(TH1F*)hall_wjet->Clone();

  hist_zjet(var, req, nbins, binmin, binmax, "hall_zjet");
  hbkg[3]=(TH1F*)hall_zjet->Clone();

  hist_ttbar(var, req, nbins, binmin, binmax, "hall_ttbar");
  hbkg[4]=(TH1F*)hall_ttbar->Clone();

  hist_data(var, req, nbins, binmin, binmax, "hall_data");
  hdata[0]=(TH1F*)hall_data->Clone();

  hist_bh234(var, req, nbins, binmin, binmax, "hall_bh234");
  hsignal[0]=(TH1F*)hall_bh234->Clone();

  hist_bh2354(var, req, nbins, binmin, binmax, "hall_bh2354");
  hsignal[1]=(TH1F*)hall_bh2354->Clone();
  
  hist_bh244(var, req, nbins, binmin, binmax, "hall_bh244");
  hsignal[2]=(TH1F*)hall_bh244->Clone();  

  hist_bh2454(var, req, nbins, binmin, binmax, "hall_bh2454");
  hsignal[3]=(TH1F*)hall_bh2454->Clone();

  hist_bh254(var, req, nbins, binmin, binmax, "hall_bh254");
  hsignal[4]=(TH1F*)hall_bh254->Clone();  

  hist_sb11(var, req, nbins, binmin, binmax, "hall_sb11");
  hsignal[5]=(TH1F*)hall_sb11->Clone();
  
  hist_sb12(var, req, nbins, binmin, binmax, "hall_sb12");
  hsignal[6]=(TH1F*)hall_sb12->Clone();
  
  hist_sb13(var, req, nbins, binmin, binmax, "hall_sb13");
  hsignal[7]=(TH1F*)hall_sb13->Clone();
                  
  hbkg[0]->SetLineColor(4);
  hbkg[1]->SetLineColor(3);
  hbkg[2]->SetLineColor(6);
  hbkg[3]->SetLineColor(5);
  hbkg[4]->SetLineColor(2);
   
  hdata[0]->SetMarkerStyle(20);
  hdata[0]->SetMarkerSize(1);

  hsignal[0]->SetLineColor(4);
  hsignal[1]->SetLineColor(14);
  hsignal[2]->SetLineColor(94);
  hsignal[3]->SetLineColor(24);
  hsignal[4]->SetLineColor(34);
  hsignal[5]->SetLineColor(44);
  hsignal[6]->SetLineColor(54);  
  hsignal[7]->SetLineColor(64);
    
  for (int i = 0;i < 8; i++) { 
    hsignal[i]->SetLineWidth(2);
    if (i < 5) hsignal[i]->SetLineStyle(2);
    if (i > 4) hsignal[i]->SetLineStyle(5);
    //normalization to 35.15 pb^-1
    hsignal[i]->Scale(35.15); 
    if (i < 5) { 
      hbkg[i]->SetLineWidth(2);
      //normalization to 35.15 pb^-1
      hbkg[i]->Scale(35.15); 
    }  
  }
  
  TCanvas *c = new TCanvas("c","",800,600);
  c->cd();
  c->SetLogy(1);
  hdata[0]->GetXaxis()->SetTitle(Form("%s",namevar));
  hdata[0]->SetMinimum(1E-5);
  hdata[0]->GetXaxis()->SetNdivisions(505); 
  hdata[0]->Draw("P");
  
  for (int i = 0; i < 5; i++) hbkg[i]->Draw("samehisto");
  for (int i = 0; i < 8; i++) hsignal[i]->Draw("samehisto");
    
  TLegend *leg = new TLegend(0.6,0.50,0.90,0.90,Form("%s",namereq),"brNDC");
  leg->SetTextFont(22);
  leg->SetTextSize(0.025);
  leg->SetLineColor(1);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillStyle(1001);
  leg->SetFillColor(10);
  leg->AddEntry(hdata[0],"Data, 35.15 pb^{-1}","p");
  leg->AddEntry(hbkg[4],"TTbar","l"); 
  leg->AddEntry(hbkg[2],"WJet","l");
  leg->AddEntry(hbkg[3],"ZJet","l"); 
  leg->AddEntry(hbkg[1],"PhotonJet","l");
  leg->AddEntry(hbkg[0],"QCDDiJet PY6","l");
  leg->AddEntry(hsignal[0],"BH MPl2 M3 N4","l");
  leg->AddEntry(hsignal[1],"BH MPl2 M3.5 N4","l");
  leg->AddEntry(hsignal[2],"BH MPl2 M4 N4","l");
  leg->AddEntry(hsignal[3],"BH MPl2 M4.5 N4","l");
  leg->AddEntry(hsignal[4],"BH MPl2 M5 N4","l");      
  leg->AddEntry(hsignal[5],"SB MPl1.3 M3 S1","l");
  leg->AddEntry(hsignal[6],"SB MPl1.6 M3.9 S1.3","l");
  leg->AddEntry(hsignal[7],"SB MPl2.1 M5.1 S1.7","l");    
  leg->Draw();
  
  c->Update();
  c->Print(Form("Results/distributions_initial_%s_%s.pdf",namevar,namereq));
  
  TFile *ff = TFile::Open(Form("Results/distributions_initial_%s_%s.root",namevar,namereq),"RECREATE");
  for (int i = 0; i < 8; i++) { 
    hsignal[i]->Write();
    if (i < 5) hbkg[i]->Write();
  }
  hdata[0]->Write();
  ff->Close();
  
  return; 
}

TH1F* hist_qcd(const TString var, const TString req, const int nbins, const float binmin, const float binmax, const TString hname){
  
  TFile *_file5 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/QCD/qcd_50_80_exo_v2_387.root");
  _file5->cd("BHAnalyzer");
  temphist = (TH1F*)_file5->Get("BHAnalyzer/No_Cut/ST");
  double w5 = (6.358E6)/111289.;//temphist->Integral();
  TH1F *h5 = new TH1F("h5","",nbins, binmin, binmax);
  t->Draw(var+">>h5",req);
  
  TFile *_file6 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/QCD/qcd_80_120_exo_v2_387.root");
  _file6->cd("BHAnalyzer");
  temphist = (TH1F*)_file6->Get("BHAnalyzer/No_Cut/ST");
  double w6 = (7.849E5)/606771.;//temphist->Integral();
  TH1F *h6 = new TH1F("h6","",nbins, binmin, binmax);
  t->Draw(var+">>h6",req);
  
  TFile *_file7 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/QCD/qcd_120_170_exo_v2_387.root");
  _file7->cd("BHAnalyzer");
  temphist = (TH1F*)_file7->Get("BHAnalyzer/No_Cut/ST");
  double w7 = (1.151E5)/58888.;//temphist->Integral();
  TH1F *h7 = new TH1F("h7","",nbins, binmin, binmax);
  t->Draw(var+">>h7",req);
  
  TFile *_file8 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/QCD/qcd_170_230_exo_v2_387.root");
  _file8->cd("BHAnalyzer");
  temphist = (TH1F*)_file8->Get("BHAnalyzer/No_Cut/ST");
  double w8 = (2.014E4)/51680.;//temphist->Integral();
  TH1F *h8 = new TH1F("h8","",nbins, binmin, binmax);
  t->Draw(var+">>h8",req);
  
  TFile *_file9 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/QCD/qcd_230_300_exo_v2_387.root");
  _file9->cd("BHAnalyzer");
  temphist = (TH1F*)_file9->Get("BHAnalyzer/No_Cut/ST");
  double w9 = (4.094E3)/52894.;//temphist->Integral();
  TH1F *h9 = new TH1F("h9","",nbins, binmin, binmax);
  t->Draw(var+">>h9",req);

  TFile *_file10 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/QCD/qcd_300_380_exo_v2_387.root");
  _file10->cd("BHAnalyzer");
  temphist = (TH1F*)_file10->Get("BHAnalyzer/No_Cut/ST");
  double w10 = (9.346E2)/64265.;//temphist->Integral();
  TH1F *h10 = new TH1F("h10","",nbins, binmin, binmax);
  t->Draw(var+">>h10",req);

  TFile *_file11 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/QCD/qcd_380_470_exo_v2_387.root");
  _file11->cd("BHAnalyzer");
  temphist = (TH1F*)_file11->Get("BHAnalyzer/No_Cut/ST");
  double w11 = (2.338E2)/52207.;//temphist->Integral();
  TH1F *h11 = new TH1F("h11","",nbins, binmin, binmax);
  t->Draw(var+">>h11",req);

  TFile *_file12 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/QCD/qcd_470_600_exo_v2_387.root");
  _file12->cd("BHAnalyzer");
  temphist = (TH1F*)_file12->Get("BHAnalyzer/No_Cut/ST");
  double w12 = (7.021E1)/20380.;//temphist->Integral();
  TH1F *h12 = new TH1F("h12","",nbins, binmin, binmax);
  t->Draw(var+">>h12",req);

  TFile *_file13 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/QCD/qcd_600_800_exo_v2_387.root");
  _file13->cd("BHAnalyzer");
  temphist = (TH1F*)_file13->Get("BHAnalyzer/No_Cut/ST");
  double w13 = (1.557E1)/22448.;//temphist->Integral();
  TH1F *h13 = new TH1F("h13","",nbins, binmin, binmax);
  t->Draw(var+">>h13",req);

  TFile *_file14 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/QCD/qcd_800_1000_exo_v2_387.root");
  _file14->cd("BHAnalyzer");
  temphist = (TH1F*)_file14->Get("BHAnalyzer/No_Cut/ST");
  double w14 = (1.843)/26000.;//temphist->Integral();
  TH1F *h14 = new TH1F("h14","",nbins, binmin, binmax);
  t->Draw(var+">>h14",req);

  TFile *_file15 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/QCD/qcd_1000_1400_exo_v2_387.root");
  _file15->cd("BHAnalyzer");
  temphist = (TH1F*)_file15->Get("BHAnalyzer/No_Cut/ST");
  double w15 = (3.318E-1)/23956.;//temphist->Integral();
  TH1F *h15 = new TH1F("h15","",nbins, binmin, binmax);
  t->Draw(var+">>h15",req);
  
  TFile *_file16 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/QCD/qcd_1400_1800_exo_v2_387.root");
  _file16->cd("BHAnalyzer");
  temphist = (TH1F*)_file16->Get("BHAnalyzer/No_Cut/ST");
  double w16 = (1.086E-2)/20575.;//temphist->Integral();
  TH1F *h16 = new TH1F("h16","",nbins, binmin, binmax);
  t->Draw(var+">>h16",req);

  TFile *_file17 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/QCD/qcd_1800_2200_exo_v2_387.root");
  _file17->cd("BHAnalyzer");
  temphist = (TH1F*)_file17->Get("BHAnalyzer/No_Cut/ST");
  double w17 = (3.499E-4)/33070.;//temphist->Integral();
  TH1F *h17 = new TH1F("h17","",nbins, binmin, binmax);
  t->Draw(var+">>h17",req);
  
  TFile *_file18 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/QCD/qcd_2200_2600_exo_v2_387.root");
  _file18->cd("BHAnalyzer");
  temphist = (TH1F*)_file18->Get("BHAnalyzer/No_Cut/ST");
  double w18 = (7.549E-6)/22580.;//temphist->Integral();
  TH1F *h18 = new TH1F("h18","",nbins, binmin, binmax);
  t->Draw(var+">>h18",req);
  
  TFile *_file19 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/QCD/qcd_2600_3000_exo_v2_387.root");
  _file19->cd("BHAnalyzer");
  temphist = (TH1F*)_file19->Get("BHAnalyzer/No_Cut/ST");
  double w19 = (6.465E-8)/20644.;//temphist->Integral();
  TH1F *h19 = new TH1F("h19","",nbins, binmin, binmax);
  t->Draw(var+">>h19",req);
  
  TFile *_file20 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/QCD/qcd_3000_3500_exo_v2_387.root");
  _file20->cd("BHAnalyzer");
  temphist = (TH1F*)_file20->Get("BHAnalyzer/No_Cut/ST");
  double w20 = (6.295E-11)/23460.;//temphist->Integral();
  TH1F *h20 = new TH1F("h20","",nbins, binmin, binmax);
  t->Draw(var+">>h20",req);

  TH1F *hall_qcd = new TH1F(hname,"",nbins, binmin, binmax);
  
  hall_qcd->Sumw2();
  h5->Sumw2();
  h6->Sumw2();
  h7->Sumw2();
  h8->Sumw2();
  h9->Sumw2();
  h10->Sumw2();
  h11->Sumw2();
  h12->Sumw2();
  h13->Sumw2();
  h14->Sumw2();
  h15->Sumw2();
  h16->Sumw2();
  h17->Sumw2();
  h18->Sumw2();
  h19->Sumw2();
  h20->Sumw2();

  hall_qcd->Add(h5,h6,w5,w6);
  _file5->Close();
  _file6->Close();
  hall_qcd->Add(h7,hall_qcd,w7,1);
  _file7->Close();  
  hall_qcd->Add(h8,hall_qcd,w8,1);
  _file8->Close();  
  hall_qcd->Add(h9,hall_qcd,w9,1);
  _file9->Close();  
  hall_qcd->Add(h10,hall_qcd,w10,1);
  _file10->Close();  
  hall_qcd->Add(h11,hall_qcd,w11,1);
  _file11->Close(); 
  hall_qcd->Add(h12,hall_qcd,w12,1);
  _file12->Close();  
  hall_qcd->Add(h13,hall_qcd,w13,1);
  _file13->Close();  
  hall_qcd->Add(h14,hall_qcd,w14,1);
  _file14->Close();  
  hall_qcd->Add(h15,hall_qcd,w15,1);
  _file15->Close();  
  hall_qcd->Add(h16,hall_qcd,w16,1);
  _file16->Close();  
  hall_qcd->Add(h17,hall_qcd,w17,1);
  _file17->Close(); 
  hall_qcd->Add(h18,hall_qcd,w18,1);
  _file18->Close();   
  hall_qcd->Add(h19,hall_qcd,w19,1);
  _file19->Close();   
  hall_qcd->Add(h20,hall_qcd,w20,1);	

  return hall_qcd;
}


TH1F* hist_photonjet(const TString var, const TString req, const int nbins, const float binmin, const float binmax, const TString hname){

  TFile *_file1 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/PhJets/phjet_40_100_exo_v2_387.root");
  _file1->cd("BHAnalyzer");
  temphist = (TH1F*)_file1->Get("BHAnalyzer/No_Cut/ST");
  double w1 = (23620.)/2256818.;//temphist->Integral();
  TH1F *h1 = new TH1F("h1","",nbins, binmin, binmax);
  t->Draw(var+">>h1",req);
  
  TFile *_file2 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/PhJets/phjet_100_200_exo_v2_387.root");
  _file2->cd("BHAnalyzer");
  temphist = (TH1F*)_file2->Get("BHAnalyzer/No_Cut/ST");
  double w2 = (3476.)/1076393.;//temphist->Integral();
  TH1F *h2 = new TH1F("h2","",nbins, binmin, binmax);
  t->Draw(var+">>h2",req);
  
  TFile *_file3 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/PhJets/phjet_200_inf_exo_v2_387.root");
  _file3->cd("BHAnalyzer");
  temphist = (TH1F*)_file3->Get("BHAnalyzer/No_Cut/ST");
  double w3 = (485.)/1026843.;//temphist->Integral();
  TH1F *h3 = new TH1F("h3","",nbins, binmin, binmax);
  t->Draw(var+">>h3",req);
    
  TH1F *hall_photonjet = new TH1F(hname,"",nbins, binmin, binmax);
  
  hall_photonjet->Sumw2();
  h1->Sumw2();
  h2->Sumw2();
  h3->Sumw2();

  hall_photonjet->Add(h1,h2,w1,w2);
  _file1->Close();
  _file2->Close();
  hall_photonjet->Add(h3,hall_photonjet,w3,1);
 
  return hall_photonjet;
}

TH1F* hist_ttbar(const TString var, const TString req, const int nbins, const float binmin, const float binmax, const TString hname){

  TFile *_file1 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/TTbar/ttbar_exo_v2_387.root");
  _file1->cd("BHAnalyzer");
  temphist = (TH1F*)_file1->Get("BHAnalyzer/No_Cut/ST");
  double w1 = (94.3)/632010.;//temphist->Integral();
  TH1F *hall_ttbar = new TH1F(hname,"",nbins, binmin, binmax);
  t->Draw(var+">>hall_ttbar",req);
  hall_ttbar->Sumw2();  

  hall_ttbar->Scale(w1);
  return hall_ttbar;
}

TH1F* hist_wjet(const TString var, const TString req, const int nbins, const float binmin, const float binmax, const TString hname){

  TFile *_file1 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/WJets/wjets_exo_v2_387.root");
  _file1->cd("BHAnalyzer");
  temphist = (TH1F*)_file1->Get("BHAnalyzer/No_Cut/ST");
  double w1 = (24170.)/10042001.;
  TH1F *hall_wjet = new TH1F(hname,"",nbins, binmin, binmax);
  t->Draw(var+">>hall_wjet",req);
  hall_wjet->Sumw2();  

  hall_wjet->Scale(w1);
  return hall_wjet;
}

TH1F* hist_zjet(const TString var, const TString req, const int nbins, const float binmin, const float binmax, const TString hname){

  TFile *_file1 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/ZJets/zjets_exo_v2_387.root");
  _file1->cd("BHAnalyzer");
  temphist = (TH1F*)_file1->Get("BHAnalyzer/No_Cut/ST");
  double w1 = (2400.)/1084921.;//temphist->Integral();
  TH1F *hall_zjet = new TH1F(hname,"",nbins, binmin, binmax);
  t->Draw(var+">>hall_zjet",req);
  hall_zjet->Sumw2();  

  hall_zjet->Scale(w1);
  return hall_zjet;
}


TH1F* hist_data(const TString var, const TString req, const int nbins, const float binmin, const float binmax, const TString hname){

  TFile *_file1 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/Data/jetenriched_run2010b_exo_v2_387_35pb.root");
  _file1->cd("BHAnalyzer");

  TH1F *hall_data = new TH1F(hname,"",nbins, binmin, binmax);
  t->Draw(var+">>hall_data",req);
  
  hall_data->Sumw2();

  return hall_data;
}

TH1F* hist_bh234(const TString var, const TString req, const int nbins, const float binmin, const float binmax, const TString hname){

  TFile *_file1 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/BH_NonRot_BlackMax/nonrot_bh_mpl2_m3_n4_exo_v2_387.root");
  _file1->cd("BHAnalyzer");
  temphist = (TH1F*)_file1->Get("BHAnalyzer/No_Cut/ST");
  float w1 = 6.45/11993.;
  TH1F *hall_bh234 = new TH1F(hname,"",nbins, binmin, binmax);
  t->Draw(var+">>hall_bh234",req);

  hall_bh234->Sumw2();
  hall_bh234->Scale(w1);

  return hall_bh234;
}

TH1F* hist_bh2354(const TString var, const TString req, const int nbins, const float binmin, const float binmax, const TString hname){

  TFile *_file1 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/BH_NonRot_BlackMax/nonrot_bh_mpl2_m3.5_n4_exo_v2_387.root");
  _file1->cd("BHAnalyzer");
  temphist = (TH1F*)_file1->Get("BHAnalyzer/No_Cut/ST");
  float w1 = 1.26/11991.;
  TH1F *hall_bh2354 = new TH1F(hname,"",nbins, binmin, binmax);
  t->Draw(var+">>hall_bh2354",req);

  hall_bh2354->Sumw2();
  hall_bh2354->Scale(w1);

  return hall_bh2354;
}

TH1F* hist_bh244(const TString var, const TString req, const int nbins, const float binmin, const float binmax, const TString hname){

  TFile *_file1 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/BH_NonRot_BlackMax/nonrot_bh_mpl2_m4_n4_exo_v2_387.root");
  _file1->cd("BHAnalyzer");
  temphist = (TH1F*)_file1->Get("BHAnalyzer/No_Cut/ST");
  float w1 = 0.20/11987.;
  TH1F *hall_bh234 = new TH1F(hname,"",nbins, binmin, binmax);
  t->Draw(var+">>hall_bh244",req);

  hall_bh244->Sumw2();
  hall_bh244->Scale(w1);

  return hall_bh244;
}

TH1F* hist_bh2454(const TString var, const TString req, const int nbins, const float binmin, const float binmax, const TString hname){

  TFile *_file1 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/BH_NonRot_BlackMax/nonrot_bh_mpl2_m4.5_n4_exo_v2_387.root");
  _file1->cd("BHAnalyzer");
  temphist = (TH1F*)_file1->Get("BHAnalyzer/No_Cut/ST");
  float w1 = 0.02/11973.;
  TH1F *hall_bh2454 = new TH1F(hname,"",nbins, binmin, binmax);
  t->Draw(var+">>hall_bh2454",req);

  hall_bh2454->Sumw2();
  hall_bh2454->Scale(w1);

  return hall_bh2454;
}

TH1F* hist_bh254(const TString var, const TString req, const int nbins, const float binmin, const float binmax, const TString hname){

  TFile *_file1 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/BH_NonRot_BlackMax/nonrot_bh_mpl2_m5_n4_exo_v2_387.root");
  _file1->cd("BHAnalyzer");
  temphist = (TH1F*)_file1->Get("BHAnalyzer/No_Cut/ST");
  float w1 = 0.002/temphist->Integral();
  TH1F *hall_bh254 = new TH1F(hname,"",nbins, binmin, binmax);
  t->Draw(var+">>hall_bh254",req);

  hall_bh254->Sumw2();
  hall_bh254->Scale(w1);

  return hall_bh254;
}

TH1F* hist_sb11(const TString var, const TString req, const int nbins, const float binmin, const float binmax, const TString hname){

  TFile *_file1 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/StringBalls/stringball_sb11_exo_v2_387.root");
  _file1->cd("BHAnalyzer");
  temphist = (TH1F*)_file1->Get("BHAnalyzer/No_Cut/ST");
  float w1 = 1.81/10000.;
  TH1F *hall_sb11 = new TH1F(hname,"",nbins, binmin, binmax);
  t->Draw(var+">>hall_sb11",req);
  
  hall_sb11->Sumw2();
  hall_sb11->Scale(w1);
 
  return hall_sb11;
}

TH1F* hist_sb12(const TString var, const TString req, const int nbins, const float binmin, const float binmax, const TString hname){

  TFile *_file1 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/StringBalls/stringball_sb12_exo_v2_387.root");
  _file1->cd("BHAnalyzer");
  temphist = (TH1F*)_file1->Get("BHAnalyzer/No_Cut/ST");
  float w1 = 0.047/10000.;
  TH1F *hall_sb12 = new TH1F(hname,"",nbins, binmin, binmax);
  t->Draw(var+">>hall_sb12",req);
  
  hall_sb12->Sumw2();
  hall_sb12->Scale(w1);

  return hall_sb12;
}

TH1F* hist_sb13(const TString var, const TString req, const int nbins, const float binmin, const float binmax, const TString hname){

  TFile *_file1 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/StringBalls/stringball_sb13_exo_v2_387.root");
  _file1->cd("BHAnalyzer");
  temphist = (TH1F*)_file1->Get("BHAnalyzer/No_Cut/ST");
  float w1 = 0.086e-3/9500.;
  TH1F *hall_sb13 = new TH1F(hname,"",nbins, binmin, binmax);
  t->Draw(var+">>hall_sb13",req);
  
  hall_sb13->Sumw2();
  hall_sb13->Scale(w1);

  return hall_sb13;
}
