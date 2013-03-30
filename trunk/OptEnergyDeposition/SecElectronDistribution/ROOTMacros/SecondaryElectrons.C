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

/**
 * Main
 *  root[#] .L SecElectrons.C+g
 */
# ifndef __CINT__
int main(){
    TFile* fG = new TFile("Co60_PS_Detector_2cm.root","r");
    TFile* fN = new TFile("neutron_PS_Detector_2cm.root","r");
    TObjArray *hist = new TObjArray();
    hist->Add(fG->Get("kinEHist"));
    hist->Add(fN->Get("kinEHist"));
  //  hist->Add(fN->Get("kEAlphaHist"));
   // hist->Add(fN->Get("kETritonHist"));
    TObjArray *names = new TObjArray();
    names->Add(new TObjString("{}^{60}Co"));
    names->Add(new TObjString("Neutron"));
   // names->Add(new TObjString("Alpha"));
   // names->Add(new TObjString("Triton"));
    std::cout<<"Calling plot histograms"<<std::endl;
    bool logX = true;
    PlotHistogram(hist,names,"","Kinetic Energy (MeV)","SecondaryElectronKineticEnergyDistribution.pdf",logX);
    std::cout<<"Output data"<<std::endl;
   // ExportData(fG,"kinETuple","Co60.dat");
   // ExportData(fN,"kinETuple","Neutron.dat");
   // ExportData(fN,"tKinETuple","Triton.dat");
   // ExportData(fN,"aKinETuple","Alpha.dat");

}
#endif
