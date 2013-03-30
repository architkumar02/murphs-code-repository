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
#include <TObjArray.h>
#include <TMath.h>

/**
 * @brief Plots an array of histogram
 * @param hist  - TObjArray of histograms
 * @param names - TObjArray of histgram names (thickness)
 * @param title - title of the histogram
 * @param xLabel- x axis label
 * @param drawLogX - flag to draw logarathmic in X
 * @param drawLogY - flag to draw logarathmic in Y
 */
void PlotHistogram(TObjArray *hist, TObjArray *names,double xMin, double xMax,const char *title,const char *xLabel, const char* histFileName, bool drawLogX, bool drawLogY){
    TH1F* h = NULL;
    TObjString *s = NULL;
    gStyle->SetOptStat(0);
    TCanvas* c1 = new TCanvas();
    TLegend* leg = new TLegend(0.8,0.8,0.95,0.95);
    leg->SetFillColor(0);
    for (int i = 0; i < hist->GetEntriesFast(); i ++){
        h = (TH1F*) hist->At(i);
        if (!h)
          std::cerr<<"Histogram pointer is NULL"<<std::endl;
        s = (TObjString*) names->At(i);
        leg->AddEntry(h,s->String().Data(),"l");
        if (i == 0){
            h->Sumw2();
            //h->Scale(1.0/h->Integral());
            h->Scale(1.0/h->GetEntries());
            h->Draw("HIST,E");
            TAxis *axis = h->GetXaxis();
            axis->SetRange(xMin,xMax);
            h->GetXaxis()->SetTitle(xLabel);
            h->GetYaxis()->SetTitle("Frequency");
            h->SetTitle(title);
        }
        else{
            h->Sumw2();
            //h->Scale(1.0/h->Integral());
            h->Scale(1.0/h->GetEntries());
            h->Draw("HIST,E,same");
        }
        h->SetLineColor(i+1);
    }

    // Histogram Prettying
    if (drawLogY)
      gPad->SetLogy();
    if (drawLogX)
      gPad->SetLogx();
    leg->Draw();
    c1->Update();
    c1->SaveAs(histFileName);
}

