/*
 * psDetectorVal.C
 *
 * Validation of the Geant4 Detector 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <TObjArray.h>
#include <TH1F.h>
#include <TFile.h>

#include "HistUtilities.hh"


/*
 * Sometimes it is VERY, VERY useful to compile
 *  root[#] .L SecondaryElectron
 */
void SecondaryElectronsGamma(const char* fileName,int numEvents){

    double XMAX = 1.34;
    char histKey_1[128] ="secElectronKinE_00_01PID";
    char histKey_2[128] ="secElectronKinE_00_02PID";
    char histKey_3[128] ="secElectronKinE_00_03PID";
    // Getting the files and thickness
    FILE* in = fopen(fileName,"r");
    TObjArray *files = new TObjArray();
    TObjArray *histPID1 = new TObjArray();
    TObjArray *histPID2 = new TObjArray();
    TObjArray *histPID3 = new TObjArray();
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
            files->Add(f);
            // Getting the histograms
            h = (TH1F*) f->Get(histKey_1);
            histPID1->Add(h);
            h = (TH1F*) f->Get(histKey_2);
            histPID2->Add(h);
            h = (TH1F*) f->Get(histKey_3);
            histPID3->Add(h);
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
    fprintf(stdout,"\nThickness:\n");
    thickness->Print();

    plotHistograms("SecondaryElec_Gamma_PID1.eps",histPID1,thickness,XMAX,"PID = 1","Electron Kinetic Energy (MeV)");
    plotHistograms("SecondaryElec_Gamma_PID2.eps",histPID2,thickness,XMAX,"PID = 2","Electron Kinetic Energy (MeV)");
    plotHistograms("SecondaryElec_Gamma_PID3.eps",histPID3,thickness,XMAX,"PID = 3","Electron Kinetic Energy (MeV)");
    
}

void SecondaryElectronsNeutrons(const char* fileName,int numEvents){

    double XMAX = 2.75;
    char histKey_1[128] ="secElectronKinE_00_01PID";
    char histKey_2[128] ="secElectronKinE_00_02PID";
    char histKey_3[128] ="secElectronKinE_00_03PID";
    char histKey_4[128] ="secElectronKinE_00_04PID";
    // Getting the files and thickness
    FILE* in = fopen(fileName,"r");
    TObjArray *files = new TObjArray();
    TObjArray *histPID1 = new TObjArray();
    TObjArray *histPID2 = new TObjArray();
    TObjArray *histPID3 = new TObjArray();
    TObjArray *histPID4 = new TObjArray();
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
            files->Add(f);
            // Getting the histograms
            h = (TH1F*) f->Get(histKey_1);
            histPID1->Add(h);
            h = (TH1F*) f->Get(histKey_2);
            histPID2->Add(h);
            h = (TH1F*) f->Get(histKey_3);
            histPID3->Add(h);
            h = (TH1F*) f->Get(histKey_4);
            histPID4->Add(h);
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
    fprintf(stdout,"\nThickness:\n");
    thickness->Print();

    plotHistograms("SecElec_Neutrons_PID3.eps",histPID3,thickness,XMAX,"PID = 3","Electron Kinetic Energy (MeV)");
    plotHistograms("SecElec_Neturons_PID4.eps",histPID4,thickness,XMAX,"PID = 4","Electron Kinetic Energy (MeV)");
    
    fprintf(stdout,"Code Ran to Completion\n");
}

/**
 * Main
 */
# ifndef __CINT__
int main(){
    SecondaryElectronsGamma("PS_GammaList.txt",1000000);
    SecondaryElectronsNeutrons("PS_NeutronList.txt",100000);
}
#endif
