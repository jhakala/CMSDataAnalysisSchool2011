/////////////////////////////////////////////////////////
//
// File: 	plot_final_distr_step7.C 
// Purpose: 	Distributions plotter
// Analysis:	Black Hole, CMSSW_3_8_7, 7 TeV
// Author: 	Alexey Ferapontov
// Date:	December 19, 2010
// Notes:	2011 Data Analysis School (a.k.a. EJTerm) at FNAL
//
/////////////////////////////////////////////////////////

//Written in form of plain ROOT macros to run many times for various multiplicities 
//without recompiling

{
  gROOT->SetStyle("Plain");

  char *namereq = "Multiplicity>=3";

  //Normalization numbers for signal acceptance calculation
  TFile *f0 = TFile::Open("Results/distributions_initial_Multiplicity_.root");
  float data_nocut	= hall_data->Integral();
  float ttbar_nocut 	= hall_ttbar->Integral();
  float phjet_nocut 	= hall_photonjet->Integral();
  float wjet_nocut 	= hall_wjet->Integral();
  float zjet_nocut 	= hall_zjet->Integral();
  float qcd_nocut	= hall_qcd->Integral();
  float bh234_nocut 	= hall_bh234->Integral();
  float bh2354_nocut 	= hall_bh2354->Integral();
  float bh244_nocut 	= hall_bh244->Integral();
  float bh2454_nocut 	= hall_bh2454->Integral();
  float bh254_nocut 	= hall_bh254->Integral();
  float sb11_nocut 	= hall_sb11->Integral();
  float sb12_nocut 	= hall_sb12->Integral();
  float sb13_nocut 	= hall_sb13->Integral();

  //Getting histograms for chosen Multiplicity
  TFile *f1 = TFile::Open(Form("Results/distributions_initial_ST_%s.root",namereq));
  TH1F *hall_data 	= (TH1F*)f1->Get("hall_data");
  TH1F *hall_ttbar 	= (TH1F*)f1->Get("hall_ttbar");
  TH1F *hall_phjet 	= (TH1F*)f1->Get("hall_photonjet");
  TH1F *hall_wjet 	= (TH1F*)f1->Get("hall_wjet");
  TH1F *hall_zjet	= (TH1F*)f1->Get("hall_zjet");
  TH1F *hall_aux_ddm 	= (TH1F*)hall_qcd->Clone("hall_aux_ddm");
  TH1F *hall_aux_ddm_up = (TH1F*)hall_qcd->Clone("hall_aux_ddm_up");
  TH1F *hall_aux_ddm_dn = (TH1F*)hall_qcd->Clone("hall_aux_ddm_dn");
  TH1F *hall_bh234 	= (TH1F*)f1->Get("hall_bh234");
  TH1F *hall_bh2354 	= (TH1F*)f1->Get("hall_bh2354");
  TH1F *hall_bh244 	= (TH1F*)f1->Get("hall_bh244");
  TH1F *hall_bh2454 	= (TH1F*)f1->Get("hall_bh2454");
  TH1F *hall_bh254 	= (TH1F*)f1->Get("hall_bh254");
  TH1F *hall_sb11 	= (TH1F*)f1->Get("hall_sb11");
  TH1F *hall_sb12 	= (TH1F*)f1->Get("hall_sb12");
  TH1F *hall_sb13 	= (TH1F*)f1->Get("hall_sb13");

  //Getting the QCD background template
  TFile *f2 = TFile::Open(Form("Results/fitfunctions_ST_Multiplicity==2.root",namereq));
  TF1 *fit0_mul2 = (TF1*)f2->Get("fit0");
  
  //Getting the QCD background template shape uncertainty
  TFile *f3 = TFile::Open("Results/background_shape_uncertainty.root");
  TH1F *shape_unc = (TH1F*)f3->Get("shape_unc");

  //Getting the QCD background template normalization factor
  ifstream infile(Form("Results/normalization_coefficient_%s.txt",namereq));
  float norm, norm_unc;
  infile >> norm >> norm_unc;
  cout <<"\nNormalization coefficient for bkg template Multiplicity==2 to "<<Form("%s is ",namereq)<< norm <<" +/-"<<norm_unc<< '\n';

  for (int i = 1; i < 71; i++) {
    // Rebinning (10) the template fitting function to match default binning of 100 GeV 
    float sum = 0.;
    for (int j = 0; j < 10; j++) {
      sum += fit0_mul2->Eval(100*(i-1)+(j)*10+5);
    }
    //Normalization of template function to chosen Multiplicity is performed here 
    hall_aux_ddm->SetBinContent(i,norm*sum);
    hall_aux_ddm_up->SetBinContent(i,norm*sum*(1+sqrt(pow(shape_unc->GetBinContent(i),2)+pow(norm_unc/norm,2))));
    hall_aux_ddm_dn->SetBinContent(i,norm*sum*(1-sqrt(pow(shape_unc->GetBinContent(i),2)+pow(norm_unc/norm,2))));
  }
  
  //Statistics block
  cout<<"\nStatistics with no cuts applied [QCD bkg is PY6 based here]"<<endl<<endl;
  printf("%10s %10s %10s %10s %10s %10s %10s %10s %10s %10s %10s %10s %10s %10s %10s %10s\n", 
	 "STmin","Data", "QCD", "syst(QCD)", "PhJet","WJet","ZJet","TTbar","BH234","BH2354","BH244","BH2454","BH254","SB11","SB12","SB13");
  cout<<"----------------------------------------------------------------------------------------";
  cout<<"----------------------------------------------------------------------------------------"<<endl;
  	 
  printf("%10.3i %10.3e %10.3e %10.3e %10.3e %10.3e %10.3e %10.3e %10.3e %10.3e %10.3e %10.3e %10.3e %10.3e %10.3e %10.3e\n",
         0,data_nocut,qcd_nocut,0,phjet_nocut,wjet_nocut,zjet_nocut,ttbar_nocut,
         bh234_nocut,bh2354_nocut,bh244_nocut,bh2454_nocut,bh254_nocut,sb11_nocut,sb12_nocut,sb13_nocut); 

  cout<<"\nStatistics with "<<Form("%s cut applied [QCD bkg is data-driven here]",namereq)<<endl<<endl;

  printf("%10s %10s %10s %10s %10s %10s %10s %10s %10s %10s %10s %10s %10s %10s %10s %10s\n", 
	 "STmin","Data", "QCD", "syst(QCD)", "PhJet","WJet","ZJet","TTbar","BH234","BH2354","BH244","BH2454","BH254","SB11","SB12","SB13");
  cout<<"----------------------------------------------------------------------------------------";
  cout<<"----------------------------------------------------------------------------------------"<<endl;
	 
  printf("%10.3i %10.3e %10.3e %10.3e %10.3e %10.3e %10.3e %10.3e %10.3e %10.3e %10.3e %10.3e %10.3e %10.3e %10.3e %10.3e\n",
	 0,hall_data->Integral(),hall_aux_ddm->Integral(),hall_aux_ddm_up->Integral() - hall_aux_ddm->Integral(), hall_phjet->Integral(),
	 hall_wjet->Integral(),hall_zjet->Integral(),hall_ttbar->Integral(),hall_bh234->Integral(),hall_bh2354->Integral(),
	 hall_bh244->Integral(),hall_bh2454->Integral(),hall_bh254->Integral(),hall_sb11->Integral(),hall_sb12->Integral(),hall_sb13->Integral()); 
	 
  cout<<"\nStatistics with "<<Form("%s and STmin cuts applied [QCD is data-driven here]",namereq)<<endl<<endl;
  printf("%10s %10s %10s %10s %10s %10s %10s %10s %10s %10s %10s %10s %10s %10s %10s %10s\n", 
	 "STmin","Data", "QCD", "syst(QCD)", "PhJet","WJet","ZJet","TTbar","BH234","BH2354","BH244","BH2454","BH254","SB11","SB12","SB13");	 
  cout<<"----------------------------------------------------------------------------------------";
  cout<<"----------------------------------------------------------------------------------------"<<endl;
  
  for (int i = 10; i < 71; i++) {
    printf("%10.3i %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f\n",
	   (i-1)*100.,hall_data->Integral(i,70),hall_aux_ddm->Integral(i,70),hall_aux_ddm_up->Integral(i,70) - hall_aux_ddm->Integral(i,70),
	   hall_phjet->Integral(i,70),hall_wjet->Integral(i,70),hall_zjet->Integral(i,70),hall_ttbar->Integral(i,70),hall_bh234->Integral(i,70),
	   hall_bh2354->Integral(i,70),hall_bh244->Integral(i,70),hall_bh2454->Integral(i,70),hall_bh254->Integral(i,70),
	   hall_sb11->Integral(i,70),hall_sb12->Integral(i,70),hall_sb13->Integral(i,70)); 
  }

  TCanvas *c = new TCanvas("c","",800,600);
  c->cd();
  c->SetLogy(1);
  hall_aux_ddm_up->GetXaxis()->SetRange(10,50);
  hall_aux_ddm_up->SetFillColor(17);
  hall_aux_ddm_up->SetLineWidth(1);
  hall_aux_ddm_up->SetFillStyle(3001); 
  hall_aux_ddm_up->SetLineColor(4);
  hall_aux_ddm_dn->SetLineColor(4);
  hall_aux_ddm_dn->SetLineWidth(1);
  hall_aux_ddm_dn->SetFillColor(10);
  hall_aux_ddm_dn->SetFillStyle(3001);
  
  hall_aux_ddm_up->GetYaxis()->SetTitle("Events");
  hall_aux_ddm_up->Draw("histo");
  hall_aux_ddm_dn->Draw("samehisto");
  hall_wjet->Draw("samehisto");
  hall_zjet->Draw("samehisto");
  hall_phjet->Draw("samehisto");
  hall_ttbar->Draw("samehisto");
  hall_aux_ddm->Draw("samehisto");
  hall_bh234->Add(hall_bh234,hall_aux_ddm);
  hall_bh234->Draw("samehisto");
  hall_bh2354->Add(hall_bh2354,hall_aux_ddm);
  hall_bh2354->Draw("samehisto");
  hall_bh244->Add(hall_bh244,hall_aux_ddm);
  hall_bh244->Draw("samehisto");
  hall_bh2454->Add(hall_bh2454,hall_aux_ddm);
  hall_bh2454->Draw("samehisto");
  hall_bh254->Add(hall_bh254,hall_aux_ddm);
  hall_bh254->Draw("samehisto");
  hall_sb11->Add(hall_sb11,hall_aux_ddm);
  hall_sb11->Draw("samehisto");
  hall_sb12->Add(hall_sb12,hall_aux_ddm);
  hall_sb12->Draw("samehisto");
  hall_sb13->Add(hall_sb13,hall_aux_ddm);
  hall_sb13->Draw("samehisto");
  hall_aux_ddm->Draw("samehisto");
  hall_data->Draw("sameAXIS");
  hall_data->Draw("sameP");

  TLegend *leg = new TLegend(0.65,0.50,0.90,0.90,Form("%s",namereq),"brNDC");
  leg->SetTextFont(22);
  leg->SetTextSize(0.02);
  leg->SetLineColor(1);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillStyle(1001);
  leg->SetFillColor(10);
  leg->AddEntry(hall_data,"Data, 35.15 pb^{-1}","p");
  leg->AddEntry(hall_ttbar,"TTbar","l"); 
  leg->AddEntry(hall_wjet,"WJet","l");
  leg->AddEntry(hall_zjet,"ZJet","l"); 
  leg->AddEntry(hall_phjet,"PhotonJet","l");
  leg->AddEntry(hall_aux_ddm_up,"Data-driven QCD","lf");
  leg->AddEntry(hall_bh234,"BH MPl2 M3 N4 + BKG","l");
  leg->AddEntry(hall_bh2354,"BH MPl2 M3.5 N4 + BKG","l");
  leg->AddEntry(hall_bh244,"BH MPl2 M4 N4 + BKG","l");
  leg->AddEntry(hall_bh2454,"BH MPl2 M4.5 N4 + BKG","l");
  leg->AddEntry(hall_bh254,"BH MPl2 M5 N4 + BKG","l");      
  leg->AddEntry(hall_sb11,"SB MPl1.3 M3 S1 + BKG","l");
  leg->AddEntry(hall_sb12,"SB MPl1.6 M3.9 S1.3 + BKG","l");
  leg->AddEntry(hall_sb13,"SB MPl2.1 M5.1 S1.7 + BKG","l");    
  leg->Draw();
  c->Update();
  c->Print(Form("Results/distributions_final_ST_%s.pdf",namereq));

}
