/////////////////////////////////////////////////////////
//
// File: 	energyscale.C 
// Purpose: 	Estimation of signal systematics due to energy scales
// Analysis:	Black Hole, CMSSW_3_8_7, 7 TeV
// Author: 	Alexey Ferapontov
// Date:	December 19, 2010
// Notes:	2011 Data Analysis School (a.k.a. EJTerm) at FNAL
//
/////////////////////////////////////////////////////////

#define t_cxx
#include "t.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include "TH1F.h"
#include "TMath.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "TSystem.h"
#include "TMacro.h"
#include "TFile.h"

//const int ANATYPE=atoi(getenv("ANATYPE"));

void energyscale(void)
{
  TChain *ch=new TChain("BHAnalyzer/t");
  
  //ch->Add("../BH_NonRot_BlackMax/nonrot_bh_mpl2_m5_n4_exo_v2_387.root");
  ch->Add("/uscmst1b_scratch/lpc1/lpctrig/aferapon/CMSDataAnalysisSchool2011/BH_NonRot_BlackMax/nonrot_bh_mpl2_m3_n4_exo_v2_387.root");
    
  t* mytree=new t(ch);
  mytree->Loop();
  return;
}

void t::Loop()
{
  if (fChain == 0) return;
 
  float pTcut 	= 50.;
  float STmin 	= 2000.;
  int Nmin 	= 3;
  
  //Recalculate ST and Multiplicity on the fly after corrections  
  float adjST;
  float adjMultiplicity;
  
  Long64_t nentries = fChain->GetEntries();
  
  int newcntr1 = 0;
  int newcntr2 = 0;
   
  Long64_t nbytes = 0;
  for (Long64_t jentry = 0; jentry < 100000; jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nbytes += fChain->GetEntry(jentry);   
    
    if(jentry%10000==0) std::cout << "Processing entry "<<jentry << std::endl;
        
    adjST = 0;
    adjMultiplicity = 0;

    //Energy corrections are applied here. Use factor = 1 for no corrections.
    //Typical correction values:
    //Jets +/- 5%, Ele, Ph, Mu +/- 3%
    //Usage: tweak object pT up and down (separately) by the correction factor. Calculate new acceptance.

    float jetcorr = 1.05;
    float elecorr = 1.00;
    float phcorr  = 1.00;
    float mucorr  = 1.00;
    
    //Loop over all booked objects
    for (unsigned int k = 0; k < 25; ++k) {
            
      JetPt[k] = JetPt[k]* jetcorr;
      ElePt[k] = ElePt[k]* elecorr;
      PhPt[k]  = PhPt[k] * phcorr;
      MuPt[k]  = MuPt[k] * mucorr;
      
      if (JetPt[k] > pTcut) {      	
      	adjMultiplicity += 1;
      	adjST += JetPt[k]; 
      } 
      if (ElePt[k] > pTcut) {
      	adjMultiplicity += 1;
      	adjST += ElePt[k]; 
      } 
      if (PhPt[k] > pTcut) {	
        adjMultiplicity += 1;
        adjST += PhPt[k]; 
      } 
      if (MuPt[k] > pTcut) { 	
     	adjMultiplicity += 1; 
      	adjST += MuPt[k];    	
      }
    }

    adjST += Met;
    
    if (ST > STmin && Multiplicity >=Nmin) ++newcntr1;
    if (adjST > STmin && adjMultiplicity >=Nmin) ++newcntr2;
                
  }

  cout<<endl;
  cout <<"Initial number of events in sample "<<nentries<<endl;
  cout <<"Number of events with ST > "<<STmin<<" and N>= "<<Nmin<<" is "<<newcntr1<<". Acceptance = "<<newcntr1/float(nentries)<<endl;
  cout <<"Energy Corrections Applied: number of events with ST > "<<STmin<<" and N>= "<<Nmin<<" is "<<newcntr2<<". Acceptance = "<<newcntr2/float(nentries)<<endl;

  return;
}
