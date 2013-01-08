/*
 *
 * Validation of the Geant4 Detector 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <TObjArray.h>
#include <TObjString.h>
#include <TH1F.h>
#include <TFile.h>
#include <TLegend.h>
#include <TAxis.h>
#include <TPad.h>
#include <TStyle.h>
#include <TCanvas.h>

#include "HistUtilities.hh"

void energyDep(const char* fileName,double XMAX){

    // Getting the files and thickness
    char histKey[128] ="totEventEDepGap_00";
    FILE* in = fopen(fileName,"r");
    TObjArray *files = new TObjArray();
    TObjArray *hist = new TObjArray();
    TObjArray *thickness = new TObjArray();
    char* token;
    TFile* f = NULL;
    TH1F* h = NULL;
    TH1F* hRef = NULL;
    TObjString *s = NULL;
    if (in != NULL){
        char line[128];
        while ( fscanf(in,"%s\n",line) == 1 ){
            f = new TFile(line,"READ");
            h = (TH1F*) f->Get(histKey);
            files->Add(f);
            hist->Add(h);
            token = strtok(line,"_");
            s = new TObjString(token);
            thickness->Add(s);
        }
        fclose(in);
    }
    else{
        perror(fileName);
    }
    fprintf(stdout,"Files:\n");
    files->Print();
    fprintf(stdout,"\nHistograms:\n");
    hist->Print();
    fprintf(stdout,"\nThickness:\n");
    thickness->Print();


    // Getting some properities
    fprintf(stdout,"\n\tInter.\t\tMean\t\tWeighted Mean\n");
    for (int i =0; i < hist->GetEntriesFast(); i++){
        h = (TH1F*) hist->At(i);
        double inter = 1.0 - h->GetBinContent(1);
        double mean = h->GetMean();
        double i1 =0;
        double trapEDep;
        for (int bin  = 2; bin < h->GetNbinsX(); bin ++){
            trapEDep = h->GetBinLowEdge(bin) + 0.5*h->GetBinWidth(bin);
            i1 += trapEDep*h->GetBinContent(bin);
        }
        double i2 = h->Integral(2,h->GetNbinsX());
        s = (TObjString*) thickness->At(i);
        fprintf(stdout,"%s\t%5.4e\t%5.4e\t%5.4e\n",s->String().Data(),inter,mean,i1/i2);

    }
    
    char buffer[128];
    // Plotting the Histograms
    sprintf(buffer,"%s_.eps",fileName);
    plotHistograms(buffer,hist,thickness,XMAX,"Energy Deposition","Energy (MeV)");

    // Saving Histograms
    sprintf(buffer,"%s_HistData.txt",fileName);
    saveHistograms(buffer,hist,thickness);

}
// 
// Code used to generate the input files
void generateHADD(){
    system("source MergeRoot.sh run1");
}

/**
 * Main
 */
# ifndef __CINT__
int main(){
    energyDep("PS_GammaList.txt",1.34);
    energyDep("PS_NeutronList.txt",4.8);
}
#endif
