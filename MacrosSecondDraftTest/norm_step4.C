/////////////////////////////////////////////////////////
//
// File: 	norm_step4.C 
// Purpose: 	Data-Driven method background normalization
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

void norm_step4(){

  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(1111111);
  gStyle->SetOptFit(1111111);

  //Binning rules:
  //ST:		10 GeV bins in 1000 - 1100 GeV range

  const TString var 	= "ST";
  const TString req 	= "Multiplicity==2";
  const int nbins 	= 10;
  const float binmin 	= 1000;
  const float binmax 	= 1100;

  char *namevar = var;
  char *namereq = req;
  
  TH1F *hdata[0];
  
  hist_data(var, req, nbins, binmin, binmax, "hall_data");
  hdata[0]=(TH1F*)hall_data->Clone(); 
  
  //Use results from ddm_fit.C step here
  TFile *f1 = TFile::Open("Results/fitfunctions_ST_Multiplicity==2.root");
  TF1 *fit0_mul2 = (TF1*)f1->Get("fit0");
  
  TCanvas *c2 = new TCanvas("c2","",800,600);
  c2->cd();
  hdata[0]->SetMinimum(0);
  hdata[0]->Draw();
  fit0_mul2->Draw("same");
    
  //Normalization coefficient calculation    
  const int arrlen = 300;
  Float_t LLH[arrlen] = {0};
  Float_t lambda[arrlen] = {0};
  Float_t n[arrlen] = {0};
  Float_t f[arrlen] = {0.};
  for (int i = 0; i< 10; i++) {
    n[i] = hdata[0]->GetBinContent(i+1);
    f[i] = fit0_mul2->Eval(1005+(i)*10);
    
  }  
  
  for (int index = 0; index < arrlen; index++) {
    lambda[index] = index*0.02;
    for (int i = 0; i< 10; i++) {
      LLH[index] += n[i]*log(lambda[index]*f[i]) - lambda[index]*f[i];
      //cout<<"LLH "<<index<<" "<<lambda[index]<<" "<<LLH[index]<<endl;
    }  
  }
  
  if (LLH[0] < 0) LLH[0] = 0.001;
  
  TCanvas *c1 = new TCanvas("c1","",800,600);
  c1->cd();
  TGraph *scal = new TGraph(arrlen,lambda,LLH);
  scal->GetXaxis()->SetTitle("lambda");
  scal->GetYaxis()->SetTitle("LLH");
  scal->SetTitle(0); 
  scal->SetMarkerStyle(22);
  scal->Draw("AP");
  
  //finding max of LLH distribution
  int length = arrlen;
  float max = LLH[0];
  int position = 0;
  for(int i = 1; i<arrlen; i++) {
    if(LLH[i] > max) {
      max = LLH[i];
      position = i;
    }
  }
    
  cout<<"\nLLH max is "<<max<<" ; max. position is "<<position<<endl;
  
  int fitrange = 8;
  if (position-fitrange >=0) {
    scal->Fit("pol2","","",lambda[position-fitrange],lambda[position+fitrange]);     
    scal->GetXaxis()->SetLimits(lambda[position-fitrange],lambda[position+fitrange]);
    scal->GetYaxis()->SetRangeUser(LLH[position-fitrange]-5,max+5); 
  } else {
    scal->Fit("pol2","","",lambda[1],lambda[position+fitrange]);     
    scal->GetXaxis()->SetLimits(lambda[0],lambda[position+fitrange]);
    scal->GetYaxis()->SetRangeUser(LLH[0]-5,max+5);  
  }
  scal->Draw("P");
     
  TF1 *fit = scal->GetFunction("pol2");
  Double_t p0 = fit->GetParameter(0);
  Double_t p1 = fit->GetParameter(1);     
  Double_t p2 = fit->GetParameter(2);
  Double_t e0 = fit->GetParError(0);
  Double_t e1 = fit->GetParError(1);     
  Double_t e2 = fit->GetParError(2);
  Double_t chisq = fit->GetChisquare();
  Double_t ndf = fit->GetNDF(); 

  //Cross-check: scale not using fit info, simply normalize to N events in 1000 - 1100 GeV range
  float binwid = (binmax - binmin)/nbins;  
  int ndata_mulX = hdata[0]->Integral();
  float ddm_mul2 = fit0_mul2->Integral(1000,1100)/binwid;
  float scale_nofit = ndata_mulX/ddm_mul2;  
  
  //Solving quadratic equation for maximum position     
  float scale_fit = -1*p1/(2*p2);
  float scale_fit_error1 = scale_fit*sqrt(pow(e1/p1,2) + pow(e2/p2,2));
  float scale_fit_error2 = scale_nofit*sqrt(1/ndata_mulX + 1/ddm_mul2);   
  float scale_fit_error  = sqrt(pow(scale_fit_error1,2) + pow(scale_fit_error2,2));
  
  cout<<"\nResult! Fit function (p0 + p1*x + p2*x*x) parameters: p0 = "<<p0<<" ; p1 = "<<p1<<" ; p2 = "<<p2<<" ; chi2/ndf = "<<chisq/ndf<<endl;
  cout<<"Result! Fit function maximum position (a.k.a. scale) is "<<scale_fit<<" +/- "<<scale_fit_error<<endl;
  if (chisq/ndf > 2) { 
    cout<<"\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! "<<endl;
    cout<<"WARNING! FIT IS PROBABLY BAD! Please, type in the scale into file named "<<Form("Results/normalization_coefficient_%s.txt manually!",namereq)<<endl;  
    cout<<"Estimate the scale from the LLH distribution by locating the maximum without the fit!"<<endl;
    cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! "<<endl;  
  }
     
  cout<<"\nNumber of events in data with "<<Form("%s is ",namereq)<<ndata_mulX<<endl;
  cout<<"Number of events in background template with Multiplicity==2 is "<<ddm_mul2<<endl;
  cout<<"Cross check: the scale w/o LLH fitting is "<<scale_nofit<<" +/- "<<scale_fit_error<<endl;
  
  //Writing scale to text file
  ofstream myfile;
  if (req == "Multiplicity==2") {
    cout<<"\nAttention! Overriding the fitted scale value ("<<scale_fit<<") with 1.0 +/- 0.0. No additional scaling is needed for Multiplicity==2."<<endl; 
    myfile.open (Form("Results/normalization_coefficient_%s.txt",namereq));
    myfile << 1.0 <<" "<<0.0<<endl;
    myfile.close();
  } else if (req != "Multiplicity==2") {
    myfile.open (Form("Results/normalization_coefficient_%s.txt",namereq));
    myfile << scale_fit<<" "<<scale_fit_error<<endl;
    myfile.close();       
  }
  return;
}

TH1F* hist_data(const TString var, const TString req, const int nbins, const float binmin, const float binmax, const TString hname){

  TFile *_file1 = TFile::Open("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/Data/jetenriched_run2010b_exo_v2_387_35pb.root");
  _file1->cd("BHAnalyzer");

  TH1F *hall_data = new TH1F(hname,"",nbins, binmin, binmax);
  t->Draw(var+">>hall_data",req);
  
  hall_data->Sumw2();

  return hall_data;
}
