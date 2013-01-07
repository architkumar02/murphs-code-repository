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


/**
 * saveHistograms
 * @brief - saves the histogram data to a file
 * @param fileName  - name of the file
 * @param hist      - Histograms
 * @param labels    - labels of the histograms
 *
 * Only the energy bin for the first histogram is written. Entries in the
 * files are tab ('\t') seperated.
 */
void saveHistograms(const char *fileName, TObjArray *hist, TObjArray *labels){

    TH1F* h = NULL;
    TH1F* hRef = NULL;
    TObjString *s = NULL;
    FILE* outFile = fopen(fileName,"w");
    printf("Writing histograms to: %s\n",fileName);
    if (outFile != NULL){

        // Creating headers
        fprintf(outFile,"Energy\t");
        for (int i = 0; i<hist->GetEntriesFast(); i++){
            s = (TObjString*) labels->At(i);
            fprintf(outFile,"\t%s\t",s->String().Data());
        }
        fprintf(outFile,"\n");
        
        // Writing the histogram data
        hRef = (TH1F*) hist->At(0);
        for(int bin = 3; bin < hRef->GetNbinsX(); bin++){
            fprintf(outFile,"%5.4e\t",hRef->GetBinCenter(bin));
            for(int i = 0; i < hist->GetEntriesFast(); i++){
                h = (TH1F*) hist->At(i);
                fprintf(outFile,"%5.4e\t",h->GetBinContent(bin));
            }
            fprintf(outFile,"\n");
        }
        fclose(outFile);
        printf("Wrote histograms data to: %s\n",fileName);
    }
    else{
        fprintf(stderr,"Could not open file: %s\n",fileName);
    }
}

/*
 * Sometimes it is VERY, VERY useful to compile
 *  root[#] .L psDetectorVal.C+g
 */
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

    // Plotting
    float scale = 1.0;
    double numEvents;
    gStyle->SetOptStat(0);
    TCanvas* c1 = new TCanvas();
    TLegend* leg = new TLegend(0.8,0.7,0.9,0.9);
    for (int i = 0; i < hist->GetEntriesFast(); i ++){
        h = (TH1F*) hist->At(i);
        s = (TObjString*) thickness->At(i);
        leg->AddEntry(h,s->String().Data(),"l");
        fprintf(stdout,"Plotting histogram %s\n",s->String().Data());
        if (i == 0){
            h->Draw();
            TAxis *xaxis = h->GetXaxis();
            xaxis->SetRangeUser(0,XMAX);
            h->GetXaxis()->SetTitle("Energy Deposition per Event (MeV)");
            h->GetYaxis()->SetTitle("Frequency");
            h->SetTitle("Energy Deposition");
            numEvents = h->GetEntries();
        }
        else{
            numEvents = h->GetEntries();
            h->Draw("same");
        }
        scale = 1.0/numEvents;
        h->SetLineColor(i+1);
        h->Scale(scale);
    }

    // Histogram Prettying
    gPad->SetLogy();
    leg->Draw("same");
    c1->Update();
    char buffer[128];
    sprintf(buffer,"%s_.eps",fileName);
    c1->SaveAs(buffer);

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
