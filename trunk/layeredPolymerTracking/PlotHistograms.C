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


/**
 * @brief Plots an array of histogram
 * @param hist  - TObjArray of histograms
 * @param names - TObjArray of histgram names (thickness)
 * @param XMAX  - maximum x value
 * @param title - title of the histogram
 * @param xLabel- x axis label 
 */
void PlotHistogram(TObjArray *hist, TObjArray *names, double XMAX, char *title,char *xLabel){
    double numEvents = 1000000000;
    float scale = 1.0;
    TH1F* h = NULL;
    TObjString *s = NULL;
    gStyle->SetOptStat(0);
    TCanvas* c1 = new TCanvas();
    TLegend* leg = new TLegend(0.8,0.7,0.9,0.9);
    for (int i = 0; i < hist->GetEntriesFast(); i ++){
        h = (TH1F*) hist->At(i);
        s = (TObjString*) names->At(i);
        leg->AddEntry(h,s->String().Data(),"l");
        if (i == 0){
            h->Draw();
            TAxis *xaxis = h->GetXaxis();
            xaxis->SetRangeUser(0,XMAX);
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
}
