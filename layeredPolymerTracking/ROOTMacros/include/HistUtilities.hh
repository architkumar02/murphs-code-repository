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
        fprintf(outFile,"Energy");
        for (int i = 0; i<hist->GetEntriesFast(); i++){
            s = (TObjString*) labels->At(i);
            fprintf(outFile,"\t\t%s\t\t\t",s->String().Data());
        }
        fprintf(outFile,"\n");
        
        // Writing the histogram data
        hRef = (TH1F*) hist->At(0);
        for(int bin = 3; bin < hRef->GetNbinsX(); bin++){
            fprintf(outFile,"%5.4e\t",hRef->GetBinCenter(bin));
            for(int i = 0; i < hist->GetEntriesFast(); i++){
                h = (TH1F*) hist->At(i);
                fprintf(outFile,"%5.4e\t%5.4e\t",h->GetBinContent(bin),h->GetBinError(bin));
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

/**
 * plotHistograms
 * @brief - plots the histograms
 * @param fileName  - name of the file to save the plots
 * @param hist      - Histograms
 * @param labels    - labels of the histograms
 * @param emax      - Maximum energy value
 * @param title - title of the histogram
 * @param xLabel- x axis label 
 */
void plotHistograms(const char *fileName, TObjArray *hist, TObjArray *labels, double emax, char *title,char *xLabel){
    double numEvents;
    TH1F* h; 
    TObjString *s = NULL;
    float scale = 1.0;
    
    gStyle->SetOptStat(0);
    TCanvas* c1 = new TCanvas();
    TLegend* leg = new TLegend(0.8,0.7,0.9,0.9);
    for (int i = 0; i < hist->GetEntriesFast(); i ++){
        h = (TH1F*) hist->At(i);
        s = (TObjString*) labels->At(i);
        leg->AddEntry(h,s->String().Data(),"l");
        if (i == 0){
            h->Draw();
            TAxis *xaxis = h->GetXaxis();
            xaxis->SetRangeUser(0,emax);
            h->GetXaxis()->SetTitle(xLabel);
            h->GetYaxis()->SetTitle("Frequency");
            h->SetTitle(title);
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
    c1->SaveAs(fileName);
}
