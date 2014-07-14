#include <iostream.h>
print_coef_vs_phi_vs_eta(char ss0[26] = "HI2013signaljsonFinal_mom4",char ss1[3] = "rec")
{

gROOT->Reset();
gROOT->SetStyle("Plain");

gStyle->SetOptStat(111111);
gStyle->SetOptFit(0);
gStyle->SetOptTitle(0);

   char myin1[100];
   sprintf(myin1,"coefficients_%s_%s.txt",ss0,ss1);
   char myoutroot[100];
   if(ss0 == "HI2013signaljsonFinal_mom4") sprintf(myoutroot,"coefvar_HI2013signaljsonFinal_phi_eta.root");
   if(ss0 == "mom1") sprintf(myoutroot,"coefmean_phi_eta.root");

   std::ifstream in20( myin1 );

   TH1F* hplc[43];
   TH1F* hminc[43];

   TH1F* hplcv[43];
   TH1F* hmincv[43];

   TH1F* histoHFd1 = new TH1F("histoHFd1","histoHFd1",100,0.7,1.4);
   TH1F* histoHFd2 = new TH1F("histoHFd2","histoHFd2",100,0.7,1.4); 

   TH1F* histoHFd1no41no29 = new TH1F("histoHFd1no41no29","histoHFd1no41no29",100,0.7,1.4);
   TH1F* histoHFd2no41no29 = new TH1F("histoHFd2no41no29","histoHFd2no41no29",100,0.7,1.4);
  
   char myhistpl[60];
   char myhistmin[60];
   
   for(Int_t i = 1; i<43; i++) {
     
     sprintf(myhistpl,"hplc_%d",i);
     hplc[i] = new TH1F(myhistpl,myhistpl,72,0.5,72.5);
     sprintf(myhistpl,"hplcv_%d",i);
     hplcv[i] = new TH1F(myhistpl,myhistpl,72,0.7,1.3);

     sprintf(myhistmin,"hminc_%d",i);
     hminc[i] = new TH1F(myhistmin,myhistmin,72,0.5,72.5);
     sprintf(myhistmin,"hmincv_%d",i);
     hmincv[i] = new TH1F(myhistmin,myhistmin,100,0.7,1.3); 
    
   }

//
// Mean values
//

Double_t varpl[43][73],varmin[43][73];
Double_t varpl_err[43][73],varmin_err[43][73];


   for(Int_t i = 0; i<43; i++) {
     for(Int_t j=0; j<73;j++){ 
      varpl[i][j] = 0.;
      varpl_err[i][j] = 0.;
      varmin[i][j] = 0.;
      varmin_err[i][j] = 0.;
     } 
   }

std::string line;

while( std::getline( in20, line)){
int subd,eta,phi,dep;
Float_t var,err;
istringstream linestream(line);
linestream>>subd>>dep>>eta>>phi>>var>>err;
if(dep == 2) {
   if( subd == 4 ) {histoHFd2->Fill(var); if(abs(eta) != 41&&abs(eta) != 29) histoHFd2no41no29->Fill(var);}
}

if( dep > 1 ) continue;

   if( subd == 4 ) {histoHFd1->Fill(var); if(abs(eta) != 41&&abs(eta) != 29) histoHFd1no41no29->Fill(var);}   

if( eta > 0. ) {
 
   varpl[eta][phi] = var;
   varpl_err[eta][phi] = err;
 
}
if( eta < 0. ) {

   varmin[abs(eta)][phi] = var;
   varmin_err[abs(eta)][phi] = err;
 
}
} // read file

   for(Int_t i = 1; i<=42; i++) {
        
// Correction     
     for(Int_t j = 1; j<=72;j++){ 
     
//      if(i == 1) cout<<varpl[i][j]<<" "<<varpl_err[i][j]<<" "<<varmin[i][j]<<" "<<varmin_err[i][j]<<endl;
      
      if(varpl[i][j]>0.) {
	if(varpl_err[i][j]<0.3){ 
        hplc[i]->SetBinContent(j,varpl[i][j]); 
        hplc[i]->SetBinError(j,varpl_err[i][j]); 
        hplcv[i]->Fill(varpl[i][j]);
       } 
      }
      
       if(varmin[i][j]>0.) {
	 if(varmin_err[i][j]<0.3){            
        hminc[i]->SetBinContent(j,varmin[i][j]); 
        hminc[i]->SetBinError(j,varmin_err[i][j]); 
        hmincv[i]->Fill(varmin[i][j]);
       }     
      }
          
     }  // i        
   } // j


   TFile f(myoutroot,"recreate");

   histoHFd1->Write(); 
   histoHFd2->Write();
   histoHFd1no41no29->Write();
   histoHFd2no41no29->Write();

   for(Int_t i = 1; i<42; i++) {

     hplc[i]->Write();
     hminc[i]->Write();
     hplcv[i]->Write();
     hmincv[i]->Write();

   }
//histo30->Draw();
hplc[1]->Draw();
}
