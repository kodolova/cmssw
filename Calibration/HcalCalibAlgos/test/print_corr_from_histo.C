void print_corr_from_histo()
{
  gROOT->Reset();
  gStyle->SetPalette(1);
  gStyle                  -> SetOptFit(1111); 
  gStyle                  -> SetOptStat(111111); 


  char mychar[100];

  Int_t cancolor = 10; 
  TCanvas c2h("c2h","2-d options",10,10,800,600); 
  c2h.SetFillColor(cancolor);  
  gPad->SetDrawOption("e1p");
    
//
// Draw with error bars
//


  TH2F* hvpl_var_c[50];
  TH2F* hvmin_var_c[50];
  TH2F* hvpl_var_cv[50];
  TH2F* hvmin_var_cv[50];
 
  TFile f("coefvar_HI2013signaljsonFinal_phi_eta.root");

    TIter nextHist(f.GetListOfKeys());
    TKey* histKey(0);
    Int_t i = 0;
    Int_t j = 0;
    Int_t k = 0;
    Int_t l = 0;
    
    while ((histKey=(TKey*)nextHist())) {

     TH1F* hrsp  = (TH1F*)histKey->ReadObj();  
     string histname(hrsp->GetName());

     cout<<histname<<endl; 

     if (histname.find("hplc_")==0) {hvpl_var_c[i] = (TH2F*)histKey->ReadObj(); i++;}
     if (histname.find("hplcv")==0) {hvpl_var_cv[j] = (TH2F*)histKey->ReadObj();j++;}
     if (histname.find("hminc_")==0) {hvmin_var_c[k] = (TH2F*)histKey->ReadObj(); k++;}
     if (histname.find("hmincv")==0) {hvmin_var_cv[l] = (TH2F*)histKey->ReadObj();l++;} 
    
    }

 


  TCanvas det("det","2-d options",10,10,800,600); 


 for(Int_t i=0; i<41; i++) {
   
  sprintf(mychar,"/home/debarati/debarati/NZS/rebin/h_vplc_%d.gif",i);
 // sprintf(mychar,"GIF10TS6TSALL/h_vplc_%d.gif",i);
  hvpl_var_c[i]->GetXaxis()->SetTitle("iphi");
  hvpl_var_c[i]->GetYaxis()->SetTitle("Corr2");  
  hvpl_var_c[i]->Draw("box");

  det.Print(mychar); 
   sprintf(mychar,"/home/debarati/debarati/NZS/rebin/h_vminc_%d.gif",i);
 // sprintf(mychar,"GIF10TS6TSALL/h_vplc_%d.gif",i);
  hvmin_var_c[i]->GetXaxis()->SetTitle("iphi");
  hvmin_var_c[i]->GetYaxis()->SetTitle("Corr2");  
  hvmin_var_c[i]->Draw("box");

  det.Print(mychar);

 }
  
}
