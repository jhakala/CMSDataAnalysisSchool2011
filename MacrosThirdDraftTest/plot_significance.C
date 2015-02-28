/////////////////////////////////////////////////////////
//
// File: 	plot_significance.C 
// Purpose: 	Significance estimator
// Analysis:	Black Hole, CMSSW_3_8_7, 7 TeV
// Author: 	Alexey Ferapontov
// Date:	December 28, 2010
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

void plot_significance(){

  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0000000);

  //Binning rules:
  //ST:		10 GeV bins in any range
  //suggest:	650 bins in 500 - 7000

  const TString var 	= "ST";
  const TString req 	= ""; // not used in this macro
  const int nbins 	= 650;
  const float binmin 	= 500;
  const float binmax 	= 7000.;

  char *namevar = var;
  char *namereq = req;
  
  TH1F *hsignal[4];
  TH1F *hbkg[4];  
     
  hist_bh234(var, "Multiplicity>=3", nbins, binmin, binmax, "m3p");
  hsignal[0]=(TH1F*)m3p->Clone();

  hist_bh234(var, "Multiplicity>=4", nbins, binmin, binmax, "m4p");
  hsignal[1]=(TH1F*)m4p->Clone();

  hist_bh234(var, "Multiplicity>=5", nbins, binmin, binmax, "m5p");
  hsignal[2]=(TH1F*)m5p->Clone();

  hist_bh234(var, "Multiplicity>=6", nbins, binmin, binmax, "m6p");
  hsignal[3]=(TH1F*)m6p->Clone();

  //Signal normalization to 35.15 pb^-1 of collision data
  for (int i = 0; i < 4; i++) { 
    hsignal[i]->Scale(35.15);
    hbkg[i] = new TH1F(Form("hbkg_%i",i),"",nbins,binmin,binmax);
  }   

  //Use numbers from fit and normalization steps
  float norm2_3p, norm2_3p_unc;
  float norm2_4p, norm2_4p_unc;  
  float norm2_5p, norm2_5p_unc;  
  float norm2_6p, norm2_6p_unc;  
  
  ifstream infile1("Results/normalization_coefficient_Multiplicity>=3.txt");
  infile1 >> norm2_3p >> norm2_3p_unc;
  cout <<"\nUsing normalization coefficient for bkg template Multiplicity==2 to Multiplicity>=3 : "<< norm2_3p <<" +/-"<<norm2_3p_unc<< '\n';

  ifstream infile2("Results/normalization_coefficient_Multiplicity>=4.txt");
  infile2 >> norm2_4p >> norm2_4p_unc;
  cout <<"\nUsing normalization coefficient for bkg template Multiplicity==2 to Multiplicity>=4 : "<< norm2_4p <<" +/-"<<norm2_4p_unc<< '\n';
 
  ifstream infile3("Results/normalization_coefficient_Multiplicity>=5.txt");
  infile3 >> norm2_5p >> norm2_5p_unc;
  cout <<"\nUsing normalization coefficient for bkg template Multiplicity==2 to Multiplicity>=5 : "<< norm2_5p <<" +/-"<<norm2_5p_unc<< '\n';
  
  ifstream infile4("Results/normalization_coefficient_Multiplicity>=6.txt");
  infile4 >> norm2_6p >> norm2_6p_unc;
  cout <<"\nUsing normalization coefficient for bkg template Multiplicity==2 to Multiplicity>=6 : "<< norm2_6p <<" +/-"<<norm2_6p_unc<< '\n';
   
  //Use fit-0 template and rescale it 
  TFile *f1 = TFile::Open("Results/fitfunctions_ST_Multiplicity==2.root");
  TF1 *fit0_scale3p = (TF1*)f1->Get("fit0");
  TF1 *fit0_scale4p = (TF1*)f1->Get("fit0");  
  TF1 *fit0_scale5p = (TF1*)f1->Get("fit0");  
  TF1 *fit0_scale6p = (TF1*)f1->Get("fit0");  

  fit0_scale3p->SetParameter(0,norm2_3p*fit0_scale3p->GetParameter(0));
  fit0_scale4p->SetParameter(0,norm2_4p*fit0_scale4p->GetParameter(0));  
  fit0_scale5p->SetParameter(0,norm2_5p*fit0_scale5p->GetParameter(0));  
  fit0_scale6p->SetParameter(0,norm2_6p*fit0_scale6p->GetParameter(0));  
        
  float binwid = (binmax - binmin)/nbins;
  for (int i = 1; i<nbins+1; i++) { 
    hbkg[0]->SetBinContent(i,fit0_scale3p->Eval(binmin+(i-0.5)*binwid));
    hbkg[1]->SetBinContent(i,fit0_scale4p->Eval(binmin+(i-0.5)*binwid));
    hbkg[2]->SetBinContent(i,fit0_scale5p->Eval(binmin+(i-0.5)*binwid));
    hbkg[3]->SetBinContent(i,fit0_scale6p->Eval(binmin+(i-0.5)*binwid));    
  }    

  //Significance
  Float_t sign[4][nbins]={0};    
  Float_t x[nbins]={0};

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < nbins; j++) {
      if ((hbkg[i]->Integral(j+1,nbins) + hsignal[i]->Integral(j+1,nbins)) != 0) {
        sign[i][j] = hsignal[i]->Integral(j+1,nbins)/sqrt(hbkg[i]->Integral(j+1,nbins) + hsignal[i]->Integral(j+1,nbins));
      } else sign[i][j] = 0;                                                                           
    }
  }
  
  //Find maximum of four significance distributions for drawing
  float max0 = sign[0][0];
  float max1 = sign[1][0];
  float max2 = sign[2][0];
  float max3 = sign[3][0];
    
  for (int i = 1; i < nbins; i++) {
    if (sign[0][i] > max0) max0 = sign[0][i];
    if (sign[1][i] > max1) max1 = sign[1][i];
    if (sign[2][i] > max2) max2 = sign[2][i];
    if (sign[3][i] > max3) max3 = sign[3][i];
  }
  
  float maxsign = max0;
  if (max1 > maxsign) maxsign = max1;
  if (max2 > maxsign) maxsign = max2;
  if (max3 > maxsign) maxsign = max3; 
    
  for (int j = 0; j < nbins; j++) x[j] = binmin + binwid*(j+0.5);
  
  TGraph *signif0 = new TGraph(nbins,x,sign[0]);
  TGraph *signif1 = new TGraph(nbins,x,sign[1]);  
  TGraph *signif2 = new TGraph(nbins,x,sign[2]);
  TGraph *signif3 = new TGraph(nbins,x,sign[3]);
  
  TCanvas *c1 = new TCanvas("c1","",800,600);
  c1->Divide(2,2);
  c1->cd(1);
  c1->cd(1)->SetLogy(1);
  hbkg[0]->SetTitle("Multiplicity >= 3");
  hbkg[0]->GetXaxis()->SetTitle(Form("%s",namevar));
  hbkg[0]->Draw();
  hsignal[0]->Draw("same");
  c1->cd(2);
  c1->cd(2)->SetLogy(1);
  hbkg[1]->SetTitle("Multiplicity >= 4");
  hbkg[1]->GetXaxis()->SetTitle(Form("%s",namevar));
  hbkg[1]->Draw();
  hsignal[1]->Draw("same");  
  c1->cd(3);
  c1->cd(3)->SetLogy(1);
  hbkg[2]->SetTitle("Multiplicity >= 5");
  hbkg[2]->GetXaxis()->SetTitle(Form("%s",namevar));
  hbkg[2]->Draw();
  hsignal[2]->Draw("same");
  c1->cd(4);
  c1->cd(4)->SetLogy(1);
  hbkg[3]->SetTitle("Multiplicity >= 6");
  hbkg[3]->GetXaxis()->SetTitle(Form("%s",namevar));
  hbkg[3]->Draw();
  hsignal[3]->Draw("same");  
    
  c1->Update();
  
  TCanvas *c2 = new TCanvas("c2","",600,600);  
  c2->cd();
  
  signif0->SetTitle(0);
  signif0->GetXaxis()->SetTitle(Form("%s",namevar));
  signif0->GetYaxis()->SetTitle("S/#sqrt{S+B}");
  signif0->SetMarkerStyle(20);
  signif0->SetMarkerColor(2);
  signif0->SetMarkerSize(0.6);
  signif0->SetMaximum(maxsign*1.2);
  signif0->Draw("AP");
  signif1->SetMarkerSize(0.6);  
  signif2->SetMarkerSize(0.6);  
  signif3->SetMarkerSize(0.6);    
  signif1->SetMarkerStyle(21);
  signif2->SetMarkerStyle(22);
  signif3->SetMarkerStyle(23);
  signif1->SetMarkerColor(3);
  signif2->SetMarkerColor(4);
  signif3->SetMarkerColor(6);
  signif1->Draw("sameP");
  signif2->Draw("sameP");    
  signif3->Draw("sameP");
  
  TLegend *leg = new TLegend(0.7,0.65,0.90,0.90,"","brNDC");
  leg->SetTextFont(22);
  leg->SetTextSize(0.03);
  leg->SetLineColor(1);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillStyle(1001);
  leg->SetFillColor(10);
  leg->AddEntry(signif0,"Mul >= 3","p"); 
  leg->AddEntry(signif1,"Mul >= 4","p");
  leg->AddEntry(signif2,"Mul >= 5","p"); 
  leg->AddEntry(signif3,"Mul >= 6","p");
  leg->Draw();
  
  return;     
}

TH1F* hist_bh234(const TString var, const TString req, const int nbins, const float binmin, const float binmax, const TString hname){

  TFile *_file1 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/BH_NonRot_BlackMax/nonrot_bh_mpl2_m3_n4_exo_v2_387.root");
  _file1->cd("BHAnalyzer");
  temphist = (TH1F*)_file1->Get("BHAnalyzer/No_Cut/ST");
  float w1 = 6.45/11993.;
  TH1F *htemp = new TH1F("htemp","",nbins, binmin, binmax);
  t->Draw(var+">>htemp",req);
  
  TH1F *hall_return = new TH1F(hname,"",nbins, binmin, binmax);
  
  hall_return->Sumw2();
  htemp->Sumw2();
  
  hall_return->Add(htemp,htemp,w1,0);

  return hall_return;
}

TH1F* hist_bh2354(const TString var, const TString req, const int nbins, const float binmin, const float binmax, const TString hname){

  TFile *_file1 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/BH_NonRot_BlackMax/nonrot_bh_mpl2_m3.5_n4_exo_v2_387.root");
  _file1->cd("BHAnalyzer");
  temphist = (TH1F*)_file1->Get("BHAnalyzer/No_Cut/ST");
  float w1 = 1.26/11991.;
  TH1F *htemp = new TH1F("htemp","",nbins, binmin, binmax);
  t->Draw(var+">>htemp",req);
  
  TH1F *hall_return = new TH1F(hname,"",nbins, binmin, binmax);
  
  hall_return->Sumw2();
  htemp->Sumw2();
  
  hall_return->Add(htemp,htemp,w1,0);

  return hall_return;
}

TH1F* hist_bh244(const TString var, const TString req, const int nbins, const float binmin, const float binmax, const TString hname){

  TFile *_file1 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/BH_NonRot_BlackMax/nonrot_bh_mpl2_m4_n4_exo_v2_387.root");
  _file1->cd("BHAnalyzer");
  temphist = (TH1F*)_file1->Get("BHAnalyzer/No_Cut/ST");
  float w1 = 0.20/11987.;
  TH1F *htemp = new TH1F("htemp","",nbins, binmin, binmax);
  t->Draw(var+">>htemp",req);
  
  TH1F *hall_return = new TH1F(hname,"",nbins, binmin, binmax);
  
  hall_return->Sumw2();
  htemp->Sumw2();
  
  hall_return->Add(htemp,htemp,w1,0);

  return hall_return;
}

TH1F* hist_bh2454(const TString var, const TString req, const int nbins, const float binmin, const float binmax, const TString hname){

  TFile *_file1 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/BH_NonRot_BlackMax/nonrot_bh_mpl2_m4.5_n4_exo_v2_387.root");
  _file1->cd("BHAnalyzer");
  temphist = (TH1F*)_file1->Get("BHAnalyzer/No_Cut/ST");
  float w1 = 0.02/11973.;
  TH1F *htemp = new TH1F("htemp","",nbins, binmin, binmax);
  t->Draw(var+">>htemp",req);
  
  TH1F *hall_return = new TH1F(hname,"",nbins, binmin, binmax);
  
  hall_return->Sumw2();
  htemp->Sumw2();
  
  hall_return->Add(htemp,htemp,w1,0);

  return hall_return;
}

TH1F* hist_bh254(const TString var, const TString req, const int nbins, const float binmin, const float binmax, const TString hname){

  TFile *_file1 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/BH_NonRot_BlackMax/nonrot_bh_mpl2_m5_n4_exo_v2_387.root");
  _file1->cd("BHAnalyzer");
  temphist = (TH1F*)_file1->Get("BHAnalyzer/No_Cut/ST");
  float w1 = 0.002/temphist->Integral();
  TH1F *htemp = new TH1F("htemp","",nbins, binmin, binmax);
  t->Draw(var+">>htemp",req);
  
  TH1F *hall_return = new TH1F(hname,"",nbins, binmin, binmax);
  
  hall_return->Sumw2();
  htemp->Sumw2();
  
  hall_return->Add(htemp,htemp,w1,0);

  return hall_return;
}

TH1F* hist_sb11(const TString var, const TString req, const int nbins, const float binmin, const float binmax, const TString hname){

  TFile *_file1 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/StringBalls/stringball_sb11_exo_v2_387.root");
  _file1->cd("BHAnalyzer");
  temphist = (TH1F*)_file1->Get("BHAnalyzer/No_Cut/ST");
  float w1 = 1.81/10000.;
  TH1F *htemp = new TH1F("htemp","",nbins, binmin, binmax);
  t->Draw(var+">>htemp",req);
  
  TH1F *hall_return = new TH1F(hname,"",nbins, binmin, binmax);
  
  hall_return->Sumw2();
  htemp->Sumw2();
  
  hall_return->Add(htemp,htemp,w1,0);

  return hall_return;
}

TH1F* hist_sb12(const TString var, const TString req, const int nbins, const float binmin, const float binmax, const TString hname){

  TFile *_file1 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/StringBalls/stringball_sb12_exo_v2_387.root");
  _file1->cd("BHAnalyzer");
  temphist = (TH1F*)_file1->Get("BHAnalyzer/No_Cut/ST");
  float w1 = 0.047/10000.;
  TH1F *htemp = new TH1F("htemp","",nbins, binmin, binmax);
  t->Draw(var+">>htemp",req);
  
  TH1F *hall_return = new TH1F(hname,"",nbins, binmin, binmax);
  
  hall_return->Sumw2();
  htemp->Sumw2();
  
  hall_return->Add(htemp,htemp,w1,0);

  return hall_return;
}

TH1F* hist_sb13(const TString var, const TString req, const int nbins, const float binmin, const float binmax, const TString hname){

  TFile *_file1 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/StringBalls/stringball_sb13_exo_v2_387.root");
  _file1->cd("BHAnalyzer");
  temphist = (TH1F*)_file1->Get("BHAnalyzer/No_Cut/ST");
  float w1 = 0.086e-3/9500.;
  TH1F *htemp = new TH1F("htemp","",nbins, binmin, binmax);
  t->Draw(var+">>htemp",req);
  
  TH1F *hall_return = new TH1F(hname,"",nbins, binmin, binmax);
  
  hall_return->Sumw2();
  htemp->Sumw2();
  
  hall_return->Add(htemp,htemp,w1,0);

  return hall_return;
}
