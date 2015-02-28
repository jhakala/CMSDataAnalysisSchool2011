/////////////////////////////////////////////////////////
//
// File: 	shape_syst_step5.C 
// Purpose: 	Data-Driven method shape systematics
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

void shape_syst_step5(){

  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0000000);
  gStyle->SetOptFit(1111111);
  
  //Binning rules:
  //ST:		10 GeV bins in any range
  //suggest:	250 bins in 500 - 3000
  
  const TString var 	= "ST";
  const TString req 	= "Multiplicity==3";
  const int nbins 	= 250;
  const float binmin 	= 500;
  const float binmax 	= 3000;

  TH1F *hdata[0];

  hist_data(var, req, nbins, binmin, binmax, "hall_data");
  cout<<"Data "<<hall_data->Integral()<<endl;
  hdata[0]=(TH1F*)hall_data->Clone();
  
  float norm2_3, norm2_3_unc;    
  ifstream infile1("Results/normalization_coefficient_Multiplicity==3.txt");
  infile1 >> norm2_3 >> norm2_3_unc;
   
  //Use results from ddm_fit.C step here
  TFile *f1 = TFile::Open("Results/fitfunctions_ST_Multiplicity==2.root");
  TF1 *fit0_mul2 = (TF1*)f1->Get("fit0");
  TF1 *fit1_mul2 = (TF1*)f1->Get("fit1");
  TF1 *fit2_mul2 = (TF1*)f1->Get("fit2");
  
  fit0_mul2->SetParameter(0,norm2_3*fit0_mul2->GetParameter(0));
  fit1_mul2->SetParameter(0,norm2_3*fit1_mul2->GetParameter(0));  
  fit2_mul2->SetParameter(0,norm2_3*fit2_mul2->GetParameter(0));  
  f1->Close();

  //Multiplicity == 3 fits
  TFile *f2 = TFile::Open("Results/fitfunctions_ST_Multiplicity==3.root");
  TF1 *fit0_mul3 = (TF1*)f2->Get("fit0");
  TF1 *fit1_mul3 = (TF1*)f2->Get("fit1");  
  TF1 *fit2_mul3 = (TF1*)f2->Get("fit2");
  f2->Close();
   
  cout<<"checks "<<fit0_mul2->GetParameter(0)<<" "<<fit0_mul3->GetParameter(0)<<endl;
  cout<<"checks "<<fit0_mul2->GetParameter(1)<<" "<<fit0_mul3->GetParameter(1)<<endl;
  
  //Systematics calculation 
  Float_t val[6][70] = {0};
  Float_t diff[70] = {0};
  for (int i = 0; i < 70; i++) {
    val[0][i] = fit0_mul2->Eval((i+0.5)*100);
    val[1][i] = fit1_mul2->Eval((i+0.5)*100);
    val[2][i] = fit2_mul2->Eval((i+0.5)*100);
    val[3][i] = fit0_mul3->Eval((i+0.5)*100);
    val[4][i] = fit1_mul3->Eval((i+0.5)*100);
    val[5][i] = fit2_mul3->Eval((i+0.5)*100);
  }
  
  Float_t max[70]; 
  Float_t min[70]; 
  TH1F *shape_unc = new TH1F("shape_unc","",70,0,7000);
  for (int j = 0; j < 70; j++) {
    max[j] = val[0][j];
    min[j] = val[0][j];
    for (i = 0; i < 6; i++) {
      if (val[i][j] > max[j]) {
        max[j] = val[i][j];
      } else if (val[i][j] < min[j]) {
          min[j] = val[i][j];
        }
    }
  shape_unc -> SetBinContent(j+1,(max[j]-min[j])/(val[0][j]));
  }

  //Drawing  
  TCanvas *c = new TCanvas("c","",600,600);
  c->cd();
  c->SetLogy(1);
  hdata[0]->SetMarkerStyle(20);
  hdata[0]->SetMarkerSize(1);
  hdata[0]->SetMarkerColor(1);  
  hdata[0]->GetXaxis()->SetNdivisions(505);
  hdata[0]->GetXaxis()->SetTitle("ST");
  hdata[0]->SetMinimum(1e-2);
  hdata[0]->Draw("P");   
  fit0_mul2->SetLineStyle(1);
  fit1_mul2->SetLineStyle(1);
  fit2_mul2->SetLineStyle(1);
  fit0_mul2->SetLineColor(2);
  fit1_mul2->SetLineColor(3);
  fit2_mul2->SetLineColor(4);
  fit0_mul3->SetLineStyle(2);
  fit1_mul3->SetLineStyle(2);
  fit2_mul3->SetLineStyle(2);
  fit0_mul3->SetLineColor(2);
  fit1_mul3->SetLineColor(3);
  fit2_mul3->SetLineColor(4);
  shape_unc->SetLineWidth(2);
  
  fit0_mul2->Draw("same");
  fit1_mul2->Draw("same");
  fit2_mul2->Draw("same");
  fit0_mul3->Draw("same");
  fit1_mul3->Draw("same");
  fit2_mul3->Draw("same");  
  shape_unc->Draw("same");
    
  TLegend *leg = new TLegend(0.4,0.6,0.90,0.90,"","brNDC");
  leg->SetTextFont(22);
  leg->SetTextSize(0.03);
  leg->SetLineColor(1);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillStyle(1001);
  leg->SetFillColor(10);
  leg->AddEntry(hdata[0],"Data, Multiplicity = 3","p");
  leg->AddEntry(fit0_mul2,"Fit-0 Mul 2","l");
  leg->AddEntry(fit1_mul2,"Fit-1 Mul 2","l");
  leg->AddEntry(fit2_mul2,"Fit-2 Mul 2","l");
  leg->AddEntry(fit0_mul3,"Fit-0 Mul 3","l");
  leg->AddEntry(fit1_mul3,"Fit-1 Mul 3","l");  
  leg->AddEntry(fit2_mul3,"Fit-2 Mul 3","l");
  leg->AddEntry(shape_unc,"Shape uncertainty","l");    
  leg->Draw();
  c->Update();
  
  c->Print("Results/background_shape_uncertainty.pdf");
  
  TFile *ff = TFile::Open("Results/background_shape_uncertainty.root","RECREATE");
  shape_unc -> Write();
  ff->Close();  
}

void hist_data(const TString var, const TString req, const int nbins, const float binmin, const float binmax, const TString hname){

  TFile *_file1 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/Data/jetenriched_run2010b_exo_v2_387_35pb.root");
  _file1->cd("BHAnalyzer");

  TH1F *hall_data = new TH1F(hname,"",nbins, binmin, binmax);
  t->Draw(var+">>hall_data",req);
  
  hall_data->Sumw2();

  return hall_data;
}
