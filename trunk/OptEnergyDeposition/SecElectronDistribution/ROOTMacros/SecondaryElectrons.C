/*
 *
 * Validation of the Geant4 Detector 
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <TH1F.h>
#include <TFile.h>
#include <TLegend.h>
#include <TAxis.h>
#include <TPad.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TObjString.h>
#include <TObjArray.h>
#include "PlotHistograms.C"

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
    hist->Add(fN->Get("kEAlphaHist"));
    hist->Add(fN->Get("kETritonHist"));
    TObjArray *names = new TObjArray();
    names->Add(new TObjString("{}^{60}"));
    names->Add(new TObjString("Neutron"));
    names->Add(new TObjString("Alpha"));
    names->Add(new TObjString("Triton"));
    std::cout<<"Calling plot histograms"<<std::endl;
    PlotHistogram(hist,names,"","Kinetic Energy (MeV)","SecondaryElectronKineticEnergyDistribution.pdf");
}
#endif
