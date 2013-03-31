/*
 * Makefile is provided
 */
#include <iostream>
#include <fstream>
#include <string>

#include <TH1F.h>
#include <TFile.h>
#include <TLegend.h>
#include <TAxis.h>
#include <TPad.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TObjString.h>
#include <TObjArray.h>
#include <TNtuple.h>
#include "PlotHistograms.C"

using namespace std;
/**
 * Saving the data
 */
void ExportData(TFile* f,const char* tupleKey,const char* outputfile){
  // Root Setup
  TNtuple* t = (TNtuple*) f->Get(tupleKey);
  int nentries = t->GetEntries();
  Float_t kinE;
  t->SetBranchAddress("kinE",&kinE);
  double* values = t->GetV1();

  // File Setup
  ofstream out;
  out.open(outputfile);
  for (int i = 0; i <nentries; i++){
    t->GetEntry(i);
    out<<kinE<<endl;
  }
  out.close();
}

void DrawNeutronGammaKinESecondary(TFile* fG, TFile* fN){
    cout<<"Plotting Neutron and Gamma Secondary Kinetic Energy"<<endl;
    TObjArray *hist = new TObjArray();
    hist->Add(fG->Get("kinEHist"));
    hist->Add(fN->Get("kinEHist"));
    TObjArray *names = new TObjArray();
    names->Add(new TObjString("{}^{60}Co"));
    names->Add(new TObjString("Neutron"));
    bool logX = true;
    bool logY = true;
    PlotHistogram(hist,names,1E-3,1.5E3,"","Kinetic Energy (MeV)","NGSecElecKinEDist.pdf",logX,logY);
}

void DrawAlphaTritonKinESecondary(TFile* fN){
    cout<<"Plotting Alpha and Triton Secondary Kinetic Energy"<<endl;
    TObjArray *hist = new TObjArray();
    hist->Add(fN->Get("kEAlphaHist"));
    hist->Add(fN->Get("kETritonHist"));
    TObjArray *names = new TObjArray();
    names->Add(new TObjString("Alpha"));
    names->Add(new TObjString("Triton"));
    bool logX = false;
    bool logY = false;
    PlotHistogram(hist,names,0.9*1E3,2*1E3,"","Kinetic Energy (keV)","AlphaTritonSecElecKinEDist.pdf",logX,logY);
}
void DrawNeutronNumSecondary(TFile* fN){
    cout<<"Plotting Alpha and Triton Number of Secondaries"<<endl;
    TObjArray *hist = new TObjArray();
    hist->Add(fN->Get("numSecHist"));
    hist->Add(fN->Get("nSAlphaHist"));
    hist->Add(fN->Get("nSTritonHist"));
    TObjArray *names = new TObjArray();
    names->Add(new TObjString("Total"));
    names->Add(new TObjString("Alpha"));
    names->Add(new TObjString("Triton"));
    bool logX = false;
    bool logY = false;
    PlotHistogram(hist,names,0,120,"","Number of Secondary Electrons","NeutronNumSecElec.pdf",logX,logY);
}

/**
 * Main
 *  root[#] .L SecElectrons.C+g
 */
# ifndef __CINT__
int main(){
    TFile* fG = new TFile("Co60_PS_Detector_5cm.root","r");
    TFile* fN = new TFile("neutron_PS_Detector_5cm.root","r");
    
    DrawNeutronGammaKinESecondary(fG,fN);
    DrawAlphaTritonKinESecondary(fN);
    DrawNeutronNumSecondary(fN);

    std::cout<<"Output data"<<std::endl;
   // ExportData(fG,"kinETuple","Co60.dat");
   // ExportData(fN,"kinETuple","Neutron.dat");
   // ExportData(fN,"tKinETuple","Triton.dat");
   // ExportData(fN,"aKinETuple","Alpha.dat");

}
#endif
