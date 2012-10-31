/*
 * psDetectorVal.C
 *
 * Validation of the Geant4 Detector 
 */

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

#include "pdf2cdf.C"

/*
 * Sometimes it is VERY, VERY useful to compile
 *  root[#] .L psDetectorVal.C+g
 */
void energyDep(const char* fileName){
    char histKey[128] ="totEventEDepGap_00";
    // Getting the files and thickness
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
    int numEvents = 1000000000;
    float scale = 1.0/(float)numEvents;
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
            hRef = h;
            TAxis *xaxis = h->GetXaxis();
            xaxis->SetRangeUser(0,1.34);
            h->GetXaxis()->SetTitle("Energy Deposition per Event (MeV)");
            h->GetYaxis()->SetTitle("Frequency");
            h->SetTitle("Energy Deposition");
        }
        else
            h->Draw("same");
        h->SetLineColor(i+1);
        h->Scale(scale);
    }

    // Histogram Prettying
    gPad->SetLogy();
    leg->Draw("same");
    c1->Update();

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

    // Looking at the CDF
    double lower = 0.01;
    double upper = 0.99;
    TH1F* cdf = NULL;
    TCanvas* c2 = new TCanvas();
    TLegend* legcdf = new TLegend(0.8,0.7,0.9,0.9);
    for (int i = 0; i < hist->GetEntriesFast(); i ++){
        h = (TH1F*) hist->At(i);
        if (cdf)
            delete cdf;
        cdf = pdf2cdf(h);
        s = (TObjString*) thickness->At(i);
        legcdf->AddEntry(h,s->String().Data(),"l");
        fprintf(stdout,"Plotting CDF of histogram %s\n",s->String().Data());
        if (i == 0){
            cdf->Draw();
            hRef = cdf;
            TAxis *xaxis = cdf->GetXaxis();
            cdf->GetXaxis()->SetTitle("Energy Deposition per Event (MeV)");
            cdf->GetYaxis()->SetTitle("Probability");
            cdf->SetTitle("Energy Deposition CDF");
        }
        else
            cdf->Draw("same");
        cdf->SetLineColor(i+1);
        cdf->Scale(scale);
    }
    legcdf->Draw("same");
    c2->Update();

    // Saving Histograms
    char buffer[128];
    sprintf(buffer,"%s_HistData.txt",fileName);
    FILE* outFile = fopen(buffer,"w");

    if (outFile != NULL){
        fprintf(outFile,"\t\t");
        for (int i = 0; i<hist->GetEntriesFast(); i++){
            s = (TObjString*) thickness->At(i);
            fprintf(outFile,"\t%s\t",s->String().Data());
        }
        fprintf(outFile,"\n");

        for(int bin = 2; bin < hRef->GetNbinsX(); bin++){
            fprintf(outFile,"%5.4e\t",hRef->GetBinCenter(bin));
            for(int i = 0; i < hist->GetEntriesFast(); i++){
                h = (TH1F*) hist->At(i);
                fprintf(outFile,"%5.4e\t",h->GetBinContent(bin));
            }
            fprintf(outFile,"\n");
        }
        fclose(outFile);
    }
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
    energyDep("PS_GammaList.txt");
}
#endif
