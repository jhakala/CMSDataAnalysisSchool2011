/////////////////////////////////////////////////////////
//
// File: 	ddm_fit_step3.C 
// Purpose: 	Data-Driven method ST distributions fitting
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

void ddm_fit_step3(){

  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0000000);
  gStyle->SetOptFit(0000000);
  
  //Binning rules:
  //ST:		10 GeV bins in any range
  //suggest:	250 bins in 500 - 3000
  
  const TString var 	= "ST";
  const TString req 	= "Multiplicity==2";
  const int nbins 	= 250;
  const float binmin 	= 500;
  const float binmax 	= 3000;

  char *namevar = var;
  char *namereq = req;
  
  TH1F *hbkg[0];
  TH1F *hdata[0];
  
  hist_qcd(var, req, nbins, binmin, binmax, "hall_qcd");
  hbkg[0]=(TH1F*)hall_qcd->Clone();

  // PY6 QCD normalization to 35.15 pb^-1 of collision data
  hbkg[0]->Scale(35.15);  

  hist_data(var, req, nbins, binmin, binmax, "hall_data");
  cout<<"Data "<<hall_data->Integral()<<endl;
  hdata[0]=(TH1F*)hall_data->Clone();
                
  hbkg[0]->SetLineColor(1);  
  hdata[0]->SetMarkerStyle(20);
  hdata[0]->SetMarkerSize(1);
  hdata[0]->SetMarkerColor(1);
  
  TCanvas *c = new TCanvas("c","",600,600);
  c->cd();
  c->SetLogy(1);
  hdata[0]->GetXaxis()->SetNdivisions(505);
  hdata[0]->GetXaxis()->SetTitle("ST");
  hdata[0]->Draw("P");
    
  //Choose an appropriate parameterization here
  //parameterization-0: default 
  TF1 *fit0    = new TF1("fit0","[0]*pow((1+x*0.001),[1])/pow(x*0.001,[2]+[3]*log(x*0.001))",0,7000);
  
  //parameterization-1: alternative
  TF1 *fit1    = new TF1("fit1","[0]/pow([1]+[2]*x*0.001+x*x*1e-6,[3])",0,7000);
  
  //parameterization-2: alternative
  TF1 *fit2    = new TF1("fit2","[0]/pow([1]+x*0.001,[2])",0,7000);
    
  fit0->SetLineColor(2);
  fit1->SetLineColor(3);
  fit2->SetLineColor(4);
  
  cout<<endl<<"Fitting data ST distribution with fit-0 "<<endl;
  for (int i = 0; i < 20; i++) hdata[0]->Fit("fit0","QR","",600,1100);
  hdata[0]->Fit("fit0","R","",600,1100);
  
  cout<<endl<<"Fitting data ST distribution with fit-1 "<<endl;
  for (int i = 0; i < 20; i++) hdata[0]->Fit("fit1","sameQR","",600,1100);
  hdata[0]->Fit("fit1","sameR","",600,1100);  
  
  cout<<endl<<"Fitting data ST distribution with fit-2 "<<endl;
  for (int i = 0; i < 20; i++) hdata[0]->Fit("fit2","sameQR","",600,1100);
  hdata[0]->Fit("fit2","sameR","",600,1100);      

  TCanvas *c = new TCanvas("c","",800,600);
  c->cd();
  c->SetLogy(1);
  
  hdata[0]->Draw("P");
  hbkg[0]->Draw("samehisto");
  fit0->Draw("sameL");
  fit1->Draw("sameL");
  fit2->Draw("sameL");

  TLegend *leg = new TLegend(0.6,0.50,0.90,0.90,Form("%s",namereq),"brNDC");
  leg->SetTextFont(22);
  leg->SetTextSize(0.025);
  leg->SetLineColor(1);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillStyle(1001);
  leg->SetFillColor(10);
  leg->AddEntry(hdata[0],"Data, 35.15 pb^{-1}","p");
  leg->AddEntry(fit0,"Fit-0","l"); 
  leg->AddEntry(fit1,"Fit-1","l");
  leg->AddEntry(fit2,"Fit-2","l"); 
  leg->Draw();  

  c->Update();
  c->Print(Form("Results/fitfunctions_%s_%s.pdf",namevar,namereq));
  
  TFile *ff = TFile::Open(Form("Results/fitfunctions_%s_%s.root",namevar,namereq),"RECREATE");
  fit0->Write();
  fit1->Write();
  fit2->Write();
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

TH1F* hist_data(const TString var, const TString req, const int nbins, const float binmin, const float binmax, const TString hname){

  TFile *_file1 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/Data/jetenriched_run2010b_exo_v2_387_35pb.root");
  _file1->cd("BHAnalyzer");

  TH1F *hall_data = new TH1F(hname,"",nbins, binmin, binmax);
  t->Draw(var+">>hall_data",req);
  
  hall_data->Sumw2();

  return hall_data;
}
