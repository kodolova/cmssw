#include "TH1F.h"
#include <cmath>
#include "TCanvas.h"
#include "TDirectory.h"
#include "TF1.h"
#include "TFile.h"
#include "TFitResult.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TH2D.h"
#include "THStack.h"
#include "TLegend.h"
#include "TMinuit.h"
#include "TMath.h"
#include "TProfile.h"
#include "TPaveStats.h"
#include "TPaveText.h"
#include "TROOT.h"
#include "TString.h"
#include "TStyle.h"
#include "TH1.h"
#include "TAttText.h"
#include "TAttFill.h"
#include "TAttMarker.h"
#include "TAttLine.h"
#include "TColor.h"
#include <iostream>
#include "TAxis.h"
std::string fileType[2]  = {"plus", "minus"};
void fit(char* type1 = "plus", char* type2 = "minus")
{
  TFile *File = TFile::Open("meanVar_json2013HI.root");
  char hnamepos[200], hnameneg[200];
  char cname[100], title[100],  cname1[100], title1[100];
  TH1F *hpos[41][4], *hneg[41][4];
  char mycharout[100];
  sprintf(mycharout,"MeanVar_HF.txt");
  FILE *Out = fopen(mycharout, "w+");
  if (!File) {
    std::cout << "Cannot find file" << std::endl;
  }
  else {
   	for (int depth=0;depth<4;depth++)
	  {
	    for (int i=0;i<41;i++)
	      {
		sprintf (hnamepos,"meanVar_%s for HF for eta %d depth %d subd 4",type1,(i+1),(depth+1));
		sprintf (hnameneg,"meanVar_%s for HF for eta %d depth %d subd 4",type2,(i+1),(depth+1));
		hpos[i][depth] = (TH1F*)(File->FindObjectAny(hnamepos));
		hneg[i][depth] = (TH1F*)(File->FindObjectAny(hnameneg));
		if ((hpos[i][depth] && (hpos[i][depth])->Integral()!=0) && (hneg[i][depth] && (hneg[i][depth])->Integral()!=0))
		  {
		    TAxis *posxaxis = hpos[i][depth]->GetXaxis();
		    TAxis *posyaxis = hpos[i][depth]->GetYaxis();
		    TAxis *negxaxis = hneg[i][depth]->GetXaxis();
		    TAxis *negyaxis = hneg[i][depth]->GetYaxis();
		    Int_t posnbinsx = posxaxis->GetNbins();
		    Int_t negnbinsx = negxaxis->GetNbins();
		    if (posnbinsx==negnbinsx)
		      {
			int pos_value=0, neg_value=0;
			for(int j=1;j<=posnbinsx;j++)
			  {
			    int value1 = hpos[i][depth]->GetBinContent(j);
			    int value2 = hneg[i][depth]->GetBinContent(j);
			    if (value1>pos_value)
			      {
				pos_value=value1;
			      }
			    if (value2>neg_value)
			      {
				neg_value=value2;
			      }
			  }
			if (pos_value>=20 && pos_value<=40)
			  {
			    posyaxis->SetRangeUser(0,40);
			  }
			else if (pos_value>40)
			  {
			    posyaxis->SetRangeUser(0,70);
			  }
			else if (pos_value<20 && pos_value>=15)
			  {
			    posyaxis->SetRangeUser(0,20);
			  }
			else if (pos_value<15 && pos_value>=8)
			  {
			    posyaxis->SetRangeUser(0,15);
			  }
			else if (pos_value<8)
			  {
			    posyaxis->SetRangeUser(0,8);
			  }
			if (neg_value>=20 && neg_value<=40)
			  {
			    negyaxis->SetRangeUser(0,40);
			  }
			else if (neg_value>40)
			  {
			    negyaxis->SetRangeUser(0,70);
			  }
			else if (neg_value<20 && neg_value>=15)
			  {
			    negyaxis->SetRangeUser(0,20);
			  }
			else if (neg_value<15 && neg_value>=8)
			  {
			    negyaxis->SetRangeUser(0,15);
			  }
			else if (neg_value<8)
                          {
                            negyaxis->SetRangeUser(0,8);
                          }
			double pos_peak = hpos[i][depth]->GetMean();
			double pos_RMS = hpos[i][depth]->GetRMS();
			double neg_peak = hneg[i][depth]->GetMean();
			double neg_RMS = hneg[i][depth]->GetRMS(); 
			double pos_max = pos_peak + 1.5*pos_RMS;
			double pos_min = pos_peak - 1.5*pos_RMS;
			double pos_max1 = pos_max+0.015625;
			double pos_min1= pos_min-0.015625;
			double neg_max = neg_peak + 1.5*pos_RMS;
			double neg_min = neg_peak - 1.5*pos_RMS;
			double neg_max1 = neg_max+0.015625;
			double neg_min1= neg_min-0.015625;
			posxaxis->SetRangeUser(pos_min1,pos_max1);
			negxaxis->SetRangeUser(neg_min1,neg_max1);
			TF1 *pos_gfit = new TF1("Gaussian","gaus",pos_min,pos_max);
			TF1 *neg_gfit = new TF1("Gaussian","gaus",neg_min,neg_max);
			sprintf (cname, "HF_%sCanvas_for_eta%d depth%d subd4" ,type1, (i+1), (depth+1));
			sprintf (title, "meanVar_%s for HF for eta%d depth%d subd4",type1, (i+1), (depth+1));
			sprintf (cname1, "HF_%sCanvas_for_eta%d depth%d subd4" ,type2, (i+1), (depth+1));
			sprintf (title1, "meanVar_%s for HF for eta%d depth%d subd4",type2, (i+1), (depth+1));
			TCanvas * c1= new TCanvas(cname,title,8,8,1000,800);
			hpos[i][depth]->Fit(pos_gfit,"RQ");
			double pos_chisq=pos_gfit->GetChisquare(); 
			double pos_ndf=pos_gfit->GetNDF();                                                                                                                   double pos_chisqdf=pos_chisq/pos_ndf;                        
			double pos_mean = pos_gfit->GetParameter(1);
			double pos_sigma = pos_gfit->GetParameter(2);       
			gStyle->SetOptStat(111111);
			hpos[i][depth]->Draw();
			TCanvas * c2= new TCanvas(cname1,title1,8,8,1000,800);
			hneg[i][depth]->Fit(neg_gfit,"RQ");
			double neg_chisq=neg_gfit->GetChisquare();
			double neg_ndf=neg_gfit->GetNDF();                                                                                       			         double neg_chisqdf=neg_chisq/neg_ndf;
			double neg_mean= pos_gfit->GetParameter(1);
			double neg_sigma = neg_gfit->GetParameter(2);
			hneg[i][depth]->Draw();
			if (type1 && type2)
			  {
			    fprintf(Out,"%d %d 4 %f %f\n",(i+1),(depth+1),pos_mean,pos_sigma);
			    fprintf(Out,"%d %d 4 %f %f\n",(-1)*(i+1),(depth+1),neg_mean,neg_sigma);
			  }
		      }
		  }
	      }
	  }
	fclose(Out);
  }
}
