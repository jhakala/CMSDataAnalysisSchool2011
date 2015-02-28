/////////////////////////////////////////////////////////
//
// File: 	sb_limits_step8.C 
// Purpose: 	String Balls limits calculator
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

void sb_limits_step8(){
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0000000);

  //Type in the numbers from previous step in the following SB samples order: SB11, SB12, and SB13 TeV
  //Replace the stars with real numbers
  Float_t acc[3]	= {*,*,*};
  Float_t sacc[3]	= {*,*,*};
  Float_t bkg[3]	= {*,*,*};
  Float_t sbkg[3]	= {*,*,*};
  Float_t obs[3]	= {*,*,*};
  Float_t theory[3] 	= {1.81, 0.047, 0.086e-3}; //Do not modify theoretical cross sections
  float lumi 		= 35.15;
  float slumi 		= 3.87;
  
  Float_t cl95[5]	= {0};
  Float_t cla[5]	= {0};

  for (int i = 0; i < 3; i++) {
    cl95[i] = CL95(lumi,slumi,acc[i],sacc[i],bkg[i],sbkg[i],obs[i],true,1);
    cla[i]  = CLA(lumi,slumi,acc[i],sacc[i],bkg[i],sbkg[i],1);
  }

  printf("\n%15s %15s %15s %15s\n","Sample","Observed xsec","Expected xsec","Theor. xsec");
  printf("%15s %15.3e %15.3e %15.3e\n","StringBall11",cl95[0],cla[0],theory[0]);
  printf("%15s %15.3e %15.3e %15.3e\n","StringBall12",cl95[1],cla[1],theory[1]);
  printf("%15s %15.3e %15.3e %15.3e\n","StringBall13",cl95[2],cla[2],theory[2]);

  return;
}
