/////////////////////////////////////////////////////////
//
// File: 	sb_limits_step8.C 
// Purpose: 	Black Holes limits calculator
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

void bh_limits_step8() {

  gROOT->SetStyle("Plain");
//  gStyle->SetOptStat(0000000);

  //Type in the numbers from previous step in the following BH mass order: 3, 3.5, 4, 4.5, and 5 TeV
  //Replace the stars with real numbers  
  Float_t acc[5]	= {*,*,*,*,*};
  Float_t sacc[5]	= {*,*,*,*,*};
  Float_t bkg[5]	= {*,*,*,*,*};
  Float_t sbkg[5]	= {*,*,*,*,*};
  Float_t obs[5]	= {*,*,*,*,*};
  Float_t theory[5] 	= {6.45, 1.26, 0.20, 0.02, 0.002}; //Do not modify theoretical cross sections
  Float_t MBH[5] 	= {3., 3.5, 4., 4.5, 5.};
  float lumi 		= 35.15;
  float slumi 		= 3.87;

  Float_t cl95[5]	= {0};
  Float_t cla[5]	= {0};

  for (int i = 0; i < 5; i++) {
    cl95[i] = CL95(lumi,slumi,acc[i],sacc[i],bkg[i],sbkg[i],obs[i],true,1);
    cla[i]  = CLA(lumi,slumi,acc[i],sacc[i],bkg[i],sbkg[i],1);
  }

  TGraph *gr_theory = new TGraph(5,MBH,theory);
  TGraph *gr_obs = new TGraph(5,MBH,cl95);
  TGraph *gr_exp = new TGraph(5,MBH,cla);

  gr_theory->SetLineStyle(2);
  gr_theory->SetLineColor(2);
  gr_theory->SetLineWidth(2);

  gr_obs->SetLineStyle(1);
  gr_obs->SetLineColor(3);
  gr_obs->SetLineWidth(2);

  gr_exp->SetLineStyle(3);
  gr_exp->SetLineColor(4);
  gr_exp->SetLineWidth(2);

  gr_theory->GetXaxis()->SetTitle("M_{BH}^{min} [TeV]");
  gr_theory->GetYaxis()->SetTitle("#sigma [pb]");
  gr_theory->SetTitle(0);

  gr_theory->Draw("AL");
  gr_obs->Draw("sameL");
  gr_exp->Draw("sameL");

  TLegend *leg = new TLegend(0.6,0.65,0.90,0.90,"BH M_{D} = 2 TeV, n = 4","brNDC");
  leg->SetTextFont(22);
  leg->SetTextSize(0.03);
  leg->SetLineColor(1);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillStyle(1001);
  leg->SetFillColor(10);
  leg->AddEntry(gr_theory,"Theoretical #sigma","l"); 
  leg->AddEntry(gr_obs,"Observed limit","l");
  leg->AddEntry(gr_exp,"Expected limit","l"); 
  leg->Draw();

  return 0;
}
