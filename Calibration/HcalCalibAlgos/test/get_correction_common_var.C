#include <iostream>
#include <TF1.h>
#include <TH1.h>
int get_correction_common_var(char[200] dirout,char[200] dirmom1, char[200] dirmom2)
{
  gROOT->Reset();
  gROOT->SetStyle("Plain");
  
  gStyle->SetOptStat(1111);
  gStyle->SetOptFit(111);
  
  char tmp1[100];
  sprintf(tmp1,dirout);
  char tmp2[100];
  sprintf(tmp2,dirmom1);
  char tmp3[100];
  sprintf(tmp3,dirmom2);

  char mycharout[100];
  sprintf(mycharout,"%s%s",tmp1,"coefficients_HI2013signaljsonFinal_mom4_rec.txt");
  char mycharin1[100];
  sprintf(mycharin1,"%s%s",tmp2,"var_2012Dpedestal_mom4_rec.txt");
  char mycharin2[100];
  sprintf(mycharin2,"%s%s",tmp3,"var_HI2013signaljsonFinal_mom4_rec.txt");

  Float_t plsignal[50][73][5][5],plnoise[50][73][5][5],plnoiseerr[50][73][5][5];
  Int_t numchan_plus[50][73][5][5],numchan_minus[50][73][5][5];
  Float_t minsignal[50][73][5][5],minnoise[50][73][5][5],minnoiseerr[50][73][5][5];
  Float_t plsignalerr[50][73][5][5],minsignalerr[50][73][5][5];
  Float_t excluded_min[50][73][5][5];
  
  //Float_t conv = sqrt(nsamples);  
  
  FILE *Out1 = fopen(mycharout, "w+");  
  std::ifstream in20(mycharin1);
  std::ifstream in21(mycharin2);
  
  for(Int_t k=1; k<50; k++) 
    {
      for(Int_t i=1; i<73; i++) 
	{  
	  for(Int_t j=1; j<5; j++) 
	    {  
	      for(Int_t l=1; l<5; l++) 
		{  
		  excluded_min[k][i][j][l] = 0.;
		  plsignal[k][i][j][l] = 0.;
		  plnoise[k][i][j][l] = 0.;
		  plnoiseerr[k][i][j][l] = 0.;
		  minsignal[k][i][j][l] = 0.;
		  minnoise[k][i][j][l] = 0.;
		  minnoiseerr[k][i][j][l] = 0.;
		  plsignalerr[k][i][j][l] = 0.;
		  minsignalerr[k][i][j][l] = 0.;
		  numchan_plus[k][i][j][l] = 0;
		  numchan_minus[k][i][j][l] = 0;
		} // l  
	    } // j
	} // i
    } // k 
  
  std::string line;
  cout<<" Read "<<mycharin1<<endl;
  
  while( std::getline( in20, line)){
    istringstream linestream(line);
    Float_t var,err;
    int subd,eta,phi,dep;
    linestream>>subd>>eta>>phi>>dep>>var>>err;
    
    // cout<<subd<<" "<<eta<<" "<<phi<<" "<<var<<endl;
    
    if( abs(eta) == 42 ) continue; 
    
    if( eta > 0 )
      {
	plnoise[eta][phi][dep][subd] = var;
	plnoiseerr[eta][phi][dep][subd] = err;
	numchan_plus[eta][phi][dep][subd] = 1;
      } else
      {
	minnoise[abs(eta)][phi][dep][subd] = var;
	minnoiseerr[abs(eta)][phi][dep][subd] = err;
	numchan_minus[abs(eta)][phi][dep][subd] = 1;
      }
  }
  cout<<" End of noise read "<<endl;
  while( std::getline( in21, line)){
    istringstream linestream(line);
    Float_t var,err;
    int subd,eta,phi,dep;
    linestream>>subd>>eta>>phi>>dep>>var>>err;
    if( eta > 0 )
      {
	plsignal[eta][phi][dep][subd] = var;
	plsignalerr[eta][phi][dep][subd] = err;
	
      } else
      {
	minsignal[abs(eta)][phi][dep][subd] = var;
	minsignalerr[abs(eta)][phi][dep][subd] = err;
      }
  }
  
  cout<<" End of signal read "<<endl;
  // 
  // Choose depth
  //   
  TFile* outputFile = new TFile("meanVar_json2013HI.root","RECREATE");
  
  TH1F* meanVar_HBplus[41][4][4], * meanVar_HBminus[41][4][4], * meanVar_HEplus[41][4][4], * meanVar_HEminus[41][4][4], * meanVar_HFplus[41][4][4], *meanVar_HFminus[41][4][4];

  for (int sub=0;sub<4;sub++)
    {
      for (int depth=0;depth<4;depth++)
	{
	  for(int y=0;y<41;y++)
	    {
	      char name[200],title[200];
	      sprintf (name, "meanVar_plus for HB for eta %d depth %d subd %d", (y+1), (depth+1), (sub+1));
	      sprintf (title,"meanVar_plus for HB for eta %d depth %d subd %d", (y+1), (depth+1), (sub+1));
	      meanVar_HBplus[y][depth][sub] = new TH1F(name,title,500,-0.2,0.2);
	      sprintf (name, "meanVar_minus for HB for eta %d depth %d subd %d", (y+1), (depth+1), (sub+1));
	      sprintf (title,"meanVar_minus for HB for eta %d depth %d subd %d", (y+1), (depth+1), (sub+1));
	      meanVar_HBminus[y][depth][sub] = new TH1F(name,title,500,-0.2,0.2);
	      sprintf (name, "meanVar_plus for HE for eta %d depth %d subd %d ", (y+1), (depth+1), (sub+1));
	      sprintf (title,"meanVar_plus for HE for eta %d depth %d subd %d", (y+1), (depth+1), (sub+1));
	      meanVar_HEplus[y][depth][sub] = new TH1F(name,title,500,-0.5,0.5);
	      sprintf (name, "meanVar_minus for HE for eta %d depth %d subd %d", (y+1), (depth+1), (sub+1));
	      sprintf (title,"meanVar_minus for HE for eta %d depth %d subd %d", (y+1), (depth+1), (sub+1));
	      meanVar_HEminus[y][depth][sub] = new TH1F(name,title,500,-0.5,0.5);
	      sprintf (name, "meanVar_plus for HF for eta %d depth %d subd %d ", (y+1), (depth+1), (sub+1));
              sprintf (title,"meanVar_plus for HF for eta %d depth %d subd %d", (y+1), (depth+1), (sub+1));
              meanVar_HFplus[y][depth][sub] = new TH1F(name,title,100,0.0,1.0);
              sprintf (name, "meanVar_minus for HF for eta %d depth %d subd %d", (y+1), (depth+1), (sub+1));
              sprintf (title,"meanVar_minus for HF for eta %d depth %d subd %d", (y+1), (depth+1), (sub+1));
              meanVar_HFminus[y][depth][sub] = new TH1F(name,title,100,0.0,1.0);
	    }
	}
     }
  Float_t plmean[50][5][5]; 
  Float_t minmean[50][5][5];
  Float_t plmeanerr[50][5][5]; 
  Float_t minmeanerr[50][5][5];
  
  Int_t numchan[50][5][5];
  
  for(Int_t k=1; k<5; k++) 
    {
      for(Int_t idep0=1; idep0<5; idep0++) 
	{  
	  for(Int_t i=1; i<42; i++) 
	    {
	      
	      plmean[i][idep0][k] = 0.; 
	      minmean[i][idep0][k] = 0.;
	      plmeanerr[i][idep0][k] = 0.; 
	      minmeanerr[i][idep0][k] = 0.;
	      
	      
	      Int_t nch1 = 0; 
	      Int_t nchp = 0; 
	      Int_t nchm = 0;
	      
	      for(Int_t j=1; j<73; j++)
		{
		  
		  if(k == 4) {
		    if( i == 29 && j == 67 ) continue;
		    if( i == 30 && j == 67 )  continue;
		    if( i == 31 && j == 67 )  continue;
		    if( i == 32 && j == 67 )  continue;
		  }
		  if(k == 1) {
		    if( i <= 16 && i == 51 ) continue;
		    if( i <= 16 && i == 52 ) continue;
		    if( i <= 16 && i == 54 ) continue;
		  }
		  if(plsignal[i][j][idep0][k]>1.) continue;
		  if(plsignal[i][j][idep0][k]>0.) nchp++;
		  plmean[i][idep0][k] = plmean[i][idep0][k] + plsignal[i][j][idep0][k] - plnoise[i][j][idep0][k];
		  plmeanerr[i][idep0][k] = plmeanerr[i][idep0][k] + plsignalerr[i][j][idep0][k] + plnoiseerr[i][j][idep0][k];
		  
		} // j
	      
	      
	      for(Int_t j=1; j<73; j++)
		{
		  if(k == 1) {
		    if( i <= 16 && i == 55 ) continue;
		    if( i <= 16 && i == 56 ) continue;
		  }
		  if( i == 19 && j == 43 && idep0 == 2 ) continue;
		  if( i == 13 && j == 7 && idep0 == 1 ) continue;
		  // bad channels appeared since run 196438
		  if( i == 2 && j == 2 && idep0 == 1 ) continue;
		  if( i == 6 && j == 2 && idep0 == 1 ) continue;
		  if( i == 10 && j == 2 && idep0 == 1 ) continue;
	 
		  if( k == 2 && i <= 20 && i == 54 ) continue;
		  if( k == 2 && i <= 20 && i == 55 ) continue;

		  if(minsignal[i][j][idep0][k]>1.) continue;
		  if(minsignal[i][j][idep0][k]>0.) nch1++;
		  if(minsignal[i][j][idep0][k]>0.) nchm++;
		  
		  minmean[i][idep0][k] = minmean[i][idep0][k] + minsignal[i][j][idep0][k] - minnoise[i][j][idep0][k];
		  minmeanerr[i][idep0][k] = minmeanerr[i][idep0][k] + minsignalerr[i][j][idep0][k] + minnoiseerr[i][j][idep0][k];

		  if( i == 1 && idep0 == 1 && k == 1 ) cout<<" iphi, sig, noise "<<i<<" "<<j<<" "<<minsignal[i][j][idep0][k]<<" "<<
						minnoise[i][j][idep0][k]<<" "<<minsignal[i][j][idep0][k] - minnoise[i][j][idep0][k]<<endl;
		  
		  
		} // j
	      
	      numchan[i][idep0][k] = nch1;    
	      //For the var distributions  
	      if ( nch1 > 0 ) {    
		plmean[i][idep0][k] = plmean[i][idep0][k]/nchp;
		minmean[i][idep0][k] = minmean[i][idep0][k]/nchm;
		/*if (k==1 || k==2)
		  {
		  std::cout << "Hello" << std::endl;
		  meanVar_plus[i-1]->Fill(plmean[i][idep0][k]);
		  }*/
		plmeanerr[i][idep0][k] = plmeanerr[i][idep0][k]/(nchp*nchp);
		minmeanerr[i][idep0][k] = minmeanerr[i][idep0][k]/(nchm*nchm);
	      }
	      //   meanVar_plus[i-1][idep0-1]->Write();
	      // Do not calibrate HO
	      Float_t err = 0.;
	      
	      if( k != 3 ) {
		
		if(nch1 == 0) continue;
		
		for(Int_t j=1;j<73;j++) 
		  {  
		    Float_t tt0 = 1.;err = 0.;
		    // Temporarily (not correct HBHE)
		    //       if(k<3) plsignal[i][j][idep0][k] = -1.;
		    
		    if(k==4) cout<<plsignal[i][j][idep0][k]<<endl;
		    
		    if(plsignal[i][j][idep0][k] > 0.) {              
		      if(plsignal[i][j][idep0][k] - plnoise[i][j][idep0][k]>0.0000001 && plmean[i][idep0][k]>0.) {
			Float_t b = plsignal[i][j][idep0][k] - plnoise[i][j][idep0][k];
			Float_t lll = plmean[i][idep0][k]/b;
			Float_t myerrora = 0.;
			if( plmeanerr[i][idep0][k]> 0.) myerrora = sqrt(plmeanerr[i][idep0][k]);
			Float_t myerrorb = 0.;
			if(plsignalerr[i][j][idep0][k] + plnoiseerr[i][j][idep0][k] > 0.) myerrorb = sqrt(plsignalerr[i][j][idep0][k] + plnoiseerr[i][j][idep0][k]);
			
			tt0 = sqrt(plmean[i][idep0][k]/(plsignal[i][j][idep0][k] - plnoise[i][j][idep0][k]));
			if (k==1)
			  {
			    meanVar_HBplus[i-1][idep0-1][k-1]->Fill(plsignal[i][j][idep0][k] - plnoise[i][j][idep0][k]);
			  }
			else if (k==2)
			  {
			    meanVar_HEplus[i-1][idep0-1][k-1]->Fill(plsignal[i][j][idep0][k] - plnoise[i][j][idep0][k]);
			  }
		      else if (k==4)
			{
                          meanVar_HFplus[i-1][idep0-1][k-1]->Fill(plsignal[i][j][idep0][k] - plnoise[i][j][idep0][k]);
			}
			err = (0.5/tt0)*sqrt((myerrora/b)*(myerrora/b) + lll*lll*(myerrorb/b)*(myerrorb/b));
			
			
		      } else {
			tt0 = 1.;
			err = 0.;
			if (k==1)
			  {
			    meanVar_HBplus[i-1][idep0-1][k-1]->Fill(plsignal[i][j][idep0][k] - plnoise[i][j][idep0][k]);
			  }
			else if (k==2)
			  {
			    meanVar_HEplus[i-1][idep0-1][k-1]->Fill(plsignal[i][j][idep0][k] - plnoise[i][j][idep0][k]);
			  }
			else if (k==4)
			  {
			    meanVar_HFplus[i-1][idep0-1][k-1]->Fill(plsignal[i][j][idep0][k] - plnoise[i][j][idep0][k]);
                          }
		      }
		    } // plnoise
		    //cout<<" tt0 "<<tt0<<endl;
		    std::cout << "Hello" << std::endl;
		    if(numchan_plus[i][j][idep0][k]>0) fprintf(Out1,"%d %d %d %d %.5f %.5f\n",k,idep0,i,j,tt0,err); 
		    
		  } // j
		
		// Write to file
		
		for(Int_t j=1;j<73;j++) 
		  {  
		    Float_t tt0 = 1.;err = 0.; 
		    //      if( j==43 && k==4 && i>28 ) {
		    // Temporarily (not correct HBHE)
		    //       if(k<3) minsignal[i][j][idep0][k] = -1.;
		    
		    if( minsignal[i][j][idep0][k] > 0. ) 
		      {       
			if(minsignal[i][j][idep0][k] - minnoise[i][j][idep0][k]>0.&& minmean[i][idep0][k]>0. ) {
			  Float_t b = minsignal[i][j][idep0][k] - minnoise[i][j][idep0][k];
			  Float_t lll = minmean[i][idep0][k]/b;
			  Float_t myerrora = 0.;
			  if(minmeanerr[i][idep0][k]>0.) myerrora = sqrt(minmeanerr[i][idep0][k]);
			  Float_t myerrorb = 0.;
			  if(minsignalerr[i][j][idep0][k] + minnoiseerr[i][j][idep0][k]>0.) myerrorb = sqrt(minsignalerr[i][j][idep0][k] + minnoiseerr[i][j][idep0][k]);
			  
			  tt0 = sqrt(minmean[i][idep0][k]/(minsignal[i][j][idep0][k] - minnoise[i][j][idep0][k]));
			  if (k==1)
			    {
			      meanVar_HBminus[i-1][idep0-1][k-1]->Fill(minsignal[i][j][idep0][k] - minnoise[i][j][idep0][k]);
			    }
			  else if (k==2)
			    {
			      meanVar_HEminus[i-1][idep0-1][k-1]->Fill(minsignal[i][j][idep0][k] - minnoise[i][j][idep0][k]);
			    }
			  else if (k==4)
			    {
			      meanVar_HFminus[i-1][idep0-1][k-1]->Fill(minsignal[i][j][idep0][k] - minnoise[i][j][idep0][k]);
                            }
			  
			  err = (0.5/tt0)*sqrt((myerrora/b)*(myerrora/b) + lll*lll*(myerrorb/b)*(myerrorb/b));
			  
			} else {  
			  tt0 = 1.;
			  if (k==1)
			    {
			      meanVar_HBminus[i-1][idep0-1][k-1]->Fill(minsignal[i][j][idep0][k] - minnoise[i][j][idep0][k]);
			    }
			  else if (k==2)
			    {
			      meanVar_HEminus[i-1][idep0-1][k-1]->Fill(minsignal[i][j][idep0][k] - minnoise[i][j][idep0][k]);
			    }
			  else if (k==4)
			    {
                              meanVar_HFminus[i-1][idep0-1][k-1]->Fill(minsignal[i][j][idep0][k] - minnoise[i][j][idep0][k]);
                            }
			  
			  err = 0.;
			} 
		      } // minnoise            
		    //       }
		    Int_t ieta = -1*i;
		    cout<<" tt0 "<<tt0<<endl;
		    if(numchan_minus[i][j][idep0][k]>0) fprintf(Out1,"%d %d %d %d %.5f %.5f\n",k,idep0,ieta,j,tt0,err);
		    
		  } // j
	      } // TMP  
	      else { 
		cout<<k<<" tail catcher "<<idep0<<" "<<ieta<<endl;
		if( idep0 == 4 && i < 16 ) {
		  cout<<" Write tail "<<i<<endl;
		  for(Int_t j=1;j<73;j++){   
		    Float_t tt0=1.; 
		    Float_t err=0.;
		    Int_t ieta = i;
		    
		    if(numchan_plus[i][j][idep0][k]>0) fprintf(Out1,"%d %d %d %d %.5f %.5f\n",k,idep0,ieta,j,tt0,err);
		    
		  }// j
		  
		  for(Int_t j=1;j<73;j++){   
		    Float_t tt0=1.; Float_t err=0.;
		    Int_t ieta = -1*i;
		    
		    if(numchan_minus[i][j][idep0][k]>0) fprintf(Out1,"%d %d %d %d %.5f %.5f\n",k,idep0,ieta,j,tt0,err);
		    
		  }// j
		}
		
	      } // k = 3
	      //cout<<" End "<<endl;
	    } // i
	} // idep0
    } // k
  for (int sub=0;sub<4;sub++)
    {
  for (int depth=0;depth<4;depth++)
     {
       for (int eta=0;eta<41;eta++)
	 { if ((meanVar_HBplus[eta][depth][sub]->Integral()!=0) || (meanVar_HBminus[eta][depth][sub]->Integral()!=0) || (meanVar_HEplus[eta][depth][sub]->Integral()!=0) || (meanVar_HEminus[eta][depth][sub]->Integral()!=0) || (meanVar_HFplus[eta][depth][sub]->Integral()!=0) || (meanVar_HFminus[eta][depth][sub]->Integral()!=0))  
	     {meanVar_HBplus[eta][depth][sub]->Write();
	       meanVar_HBminus[eta][depth][sub]->Write();
	       meanVar_HEplus[eta][depth][sub]->Write();
               meanVar_HEminus[eta][depth][sub]->Write();
	       meanVar_HFplus[eta][depth][sub]->Write();
               meanVar_HFminus[eta][depth][sub]->Write();
	     }
	 }
     }
    }
  outputFile->Close();
  fclose(Out1);
}
