#include <iostream.h>
#include <TF1.h>
#include <TH1.h>
int get_correction_common_var(char[200] mydir)
{
  gROOT->Reset();
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(1111);
  gStyle->SetOptFit(111);
  char tmp[100];
  sprintf(tmp,mydir);
  char mycharout[100];
  sprintf(mycharout,"%s%s",tmp,"coefficients_HI2013signaljsonFinal_mom4_rec.txt");
  char mycharin1[100];
  sprintf(mycharin1,"%s%s",tmp,"var_2012Dpedestal_mom4_rec.txt");
  char mycharin2[100];
  sprintf(mycharin2,"%s%s",tmp,"var_HI2013signaljsonFinal_mom4_rec.txt");
  char mycharin3[100];
  sprintf(mycharin3, "%s%s", tmp, "MeanVar_HB.txt");
  Float_t plsignal[50][73][5][5],plnoise[50][73][5][5],plnoiseerr[50][73][5][5];
  Float_t minsignal[50][73][5][5],minnoise[50][73][5][5],minnoiseerr[50][73][5][5];
  Float_t plsignalerr[50][73][5][5],minsignalerr[50][73][5][5];
  Float_t excluded_min[50][73][5][5];
  Float_t plsfit[50][5][5],minfit[50][5][5],plsigma[50][5][5],minsigma[50][5][5];
  
  FILE *Out1 = fopen(mycharout, "w+");  
  std::ifstream in20(mycharin1);
  std::ifstream in21(mycharin2);
  std::ifstream in22(mycharin3);
  
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
		} // l  
	    } // j
	} // i
    } // k 
  
  for(Int_t k=1; k<50; k++)
    {
      for (Int_t j=1; j<5; j++)
	{
	  for(Int_t l=1; l<5; l++)
	    {
	      plsfit[k][j][l]= 0;
	      minfit[k][j][l]= 0;
	      plsigma[k][j][l]= 0;
	      minsigma[k][j][l]=0;
	    }
	}
    }
  std::string line;
  cout<<" Read "<<mycharin1<<endl;
  
  while( std::getline( in20, line)){
    istringstream linestream(line);
    Float_t var,err;
    int subd,eta,phi,dep;
    linestream>>subd>>eta>>phi>>dep>>var>>err;
    
    if( abs(eta) == 42 ) continue; 
    
    if( eta > 0 )
      {
	plnoise[eta][phi][dep][subd] = var;
	plnoiseerr[eta][phi][dep][subd] = err;
      } else
      {
	minnoise[abs(eta)][phi][dep][subd] = var;
	minnoiseerr[abs(eta)][phi][dep][subd] = err;
	cout << "minnoiseerr" << endl; 
      }
  }
  cout<<" End of noise read "<<endl;
  
  while(std::getline( in21, line)){
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
	cout << "minsignalerr" << endl;
      }
  }
  
  cout<<" End of signal read "<<endl;
  
  while( std::getline(in22, line)){
    std::cout << "Hello" << std::endl;
    istringstream linestream(line);
    Float_t meanfit,sigma;
    int subd,eta,depth;
    linestream>>eta>>depth>>subd>>meanfit>>sigma;
    if( eta > 0)
      {
        plsfit[eta][depth][subd] = meanfit;
        plsigma[eta][depth][subd] = sigma;
	cout << "Hello" << endl;
      } else
      {
        minfit[abs(eta)][depth][subd] = meanfit;
        minsigma[abs(eta)][depth][subd] = sigma;
	std::cout<< "Hello I am here" << std::endl;
      }
  }
  cout<<" End of fit read "<<endl;
  for(Int_t k=1; k<5; k++) 
    {
      for(Int_t idep0=1; idep0<5; idep0++) 
	{  
	  for(Int_t i=1; i<42; i++) 
	    {
	      for(Int_t j=1; j<73; j++)
		{
		  
		  //if( i == 14 && j == 31 && idep0 == 1 ) continue;
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
		} // j
	      
	      for(Int_t j=1; j<73; j++)
		{
		  if(k == 1) {
		    if( i <= 16 && i == 55 ) continue;
		    if( i <= 16 && i == 56 ) continue;
		  }
		  if( i == 19 && j == 43 && idep0 == 2 ) continue;
		  if( i == 13 && j == 7 && idep0 == 1 ) continue;
		  if( i == 2 && j == 2 && idep0 == 1 ) continue;
		  if( i == 6 && j == 2 && idep0 == 1 ) continue;
		  if( i == 10 && j == 2 && idep0 == 1 ) continue;
		  if( k == 2 && i <= 20 && i == 54 ) continue;
		  if( k == 2 && i <= 20 && i == 55 ) continue;
		} // j
	      
	      // Do not calibrate HO
	      Float_t err = 0.;
	      if( k == 1 ) {
		for(Int_t j=1;j<73;j++) 
		  {  
		    Float_t tt0 = 1.;err = 0.;
		    if(plsignal[i][j][idep0][k] > 0.) {
		      if(plsignal[i][j][idep0][k] - plnoise[i][j][idep0][k]>0.0000001) {
			Float_t b = plsignal[i][j][idep0][k] - plnoise[i][j][idep0][k];
			if (plsfit[i][idep0][k]>0)
			  {
			    cout << "plsignal" << endl;
			    Float_t lll = plsfit[i][idep0][k]/b; tt0 = sqrt(lll);
			    Float_t myerrora = 0.;
			    if( plsigma[i][idep0][k]> 0.) myerrora = (plsigma[i][idep0][k]);
			    Float_t myerrorb = 0.;
			    if(plsignalerr[i][j][idep0][k] + plnoiseerr[i][j][idep0][k] > 0.) myerrorb = (plsignalerr[i][j][idep0][k] + plnoiseerr[i][j][idep0][k]);
			    err = sqrt(((myerrora)*(myerrora))/(4.0*plsfit[i][idep0][k]*b)+ (plsfit[i][idep0][k]*myerrorb*myerrorb)/(4.0*b*b*b));
			    //err = (0.5)*sqrt((myerrora)/plsfit[i][idep0][k]*(myerrora)/plsfit[i][idep0][k]+((myerrorb)/b)*((myerrorb)/b));
			    fprintf(Out1,"%d %d %d %d %.5f %.5f\n",k,idep0,i,j,tt0,err);
			  }
			/*	else {
				Float_t lll = minfit[i][idep0][k]/b; tt0 = sqrt(lll);
				Float_t myerrora = 0.;
				if( minsigma[i][idep0][k]> 0.) myerrora = (minsigma[i][idep0][k]);
				Float_t myerrorb = 0.;
				if(plsignalerr[i][j][idep0][k] + plnoiseerr[i][j][idep0][k] > 0.) 
				{myerrorb = (plsignalerr[i][j][idep0][k] + plnoiseerr[i][j][idep0][k]);}
				else if(minsignalerr[i][j][idep0][k] + minnoiseerr[i][j][idep0][k] > 0.) 
				{myerrorb = (minsignalerr[i][j][idep0][k] + minnoiseerr[i][j][idep0][k]);}
				err = sqrt(((myerrora)*(myerrora))/(4.0*minfit[i][idep0][k]*b)+ (minfit[i][idep0][k]*myerrorb*myerrorb)/(4.0*b*b*b));
				//err = (0.5)*sqrt((myerrora)/plsfit[i][idep0][k]*(myerrora)/plsfit[i][idep0][k]+((myerrorb)/b)*((myerrorb)/b));                              			  fprintf(Out1,"%d %d %d %d %.5f %.5f\n",k,idep0,(-1*i),j,tt0,err);
				}*/
		      } 
		    } 
		    if(minsignal[i][j][idep0][k] > 0.)
		      {       
			cout<< "minsignal" <<endl;
			if(minsignal[i][j][idep0][k] - minnoise[i][j][idep0][k]>0.) {
			  Float_t b = minsignal[i][j][idep0][k] - minnoise[i][j][idep0][k];
			  if (minfit[i][idep0][k]>0)
			    {
			      cout << "minsignal" << endl;
			      Float_t lll = minfit[i][idep0][k]/b; tt0 = sqrt(lll);
			      Float_t myerrora = 0.;
			      if(minsigma[i][idep0][k]>0.) myerrora = (minsigma[i][idep0][k]);
			      Float_t myerrorb = 0.;
			      if(minsignalerr[i][j][idep0][k] + minnoiseerr[i][j][idep0][k]>0.) myerrorb = (minsignalerr[i][j][idep0][k] + minnoiseerr[i][j][idep0][k]);
			      err = sqrt(((myerrora)*(myerrora))/(4.0*minfit[i][idep0][k]*b)+ (minfit[i][idep0][k]*myerrorb*myerrorb)/(4.0*b*b*b));
			      Int_t ieta = -1*i; 
			      // err = (0.5)*sqrt((myerrora)/minfit[i][idep0][k]*(myerrora)/minfit[i][idep0][k]+((myerrorb)/b)*((myerrorb)/b));
			      fprintf(Out1,"%d %d %d %d %.5f %.5f\n",k,idep0,ieta,j,tt0,err);
			    }
			}
		      }
		  } // j
	      } // TMP  
	    } // i
	} // idep0
    }
  fclose(Out1);
}
