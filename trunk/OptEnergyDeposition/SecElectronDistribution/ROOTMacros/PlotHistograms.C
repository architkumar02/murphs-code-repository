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
 * @brief Logarithmic Binning
 * Setting the histogram to logarithmic binning (rebin)
 * @param hist
 */
void LogarithmicBinning(TH1F* h){
   TAxis *axis = h->GetXaxis(); 
   int bins = axis->GetNbins();

   Axis_t from = axis->GetXmin();
   Axis_t to = axis->GetXmax();
   Axis_t width = (to - from) / bins;
   Axis_t *new_bins = new Axis_t[bins + 1];

   for (int i = 0; i <= bins; i++) {
     new_bins[i] = TMath::Power(10, from + i * width);
   } 
   axis->Set(bins, new_bins); 
   delete new_bins; 
}

/**
 * @brief Plots an array of histogram
 * @param hist  - TObjArray of histograms
 * @param names - TObjArray of histgram names (thickness)
 * @param title - title of the histogram
 * @param xLabel- x axis label 
 */
void PlotHistogram(TObjArray *hist, TObjArray *names, char *title,char *xLabel, char* histFileName){
    TH1F* h = NULL;
    TObjString *s = NULL;
    gStyle->SetOptStat(0);
    TCanvas* c1 = new TCanvas();
    TLegend* leg = new TLegend(0.8,0.7,0.9,0.9);
    leg->SetFillColor(0);
    for (int i = 0; i < hist->GetEntriesFast(); i ++){
        h = (TH1F*) hist->At(i);
        if (!h)
          std::cerr<<"Histogram pointer is NULL"<<std::endl;
        s = (TObjString*) names->At(i);
        leg->AddEntry(h,s->String().Data(),"l");
        std::cout<<"Plotting histogram "<<i<<std::endl;
        if (i == 0){
            h->Sumw2();
            h->Scale(1.0/h->Integral());
            //LogarithmicBinning(h);
            h->Draw();
            TAxis *xaxis = h->GetXaxis();
            xaxis->SetRangeUser(5E-4,5.0);
            h->GetXaxis()->SetTitle(xLabel);
            h->GetYaxis()->SetTitle("Frequency");
            h->SetTitle(title);
        }
        else{
            h->Sumw2();
            h->Scale(1.0/h->Integral());
            //LogarithmicBinning(h);
            h->Draw("same");
        }
        h->SetLineColor(i+1);
    }

    // Histogram Prettying
    gPad->SetLogy();
    gPad->SetLogx();
    leg->Draw("same");
    c1->Update();
    c1->SaveAs(histFileName);
}

