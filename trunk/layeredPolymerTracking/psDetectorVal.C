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

void gammaVal(char* fileName){

    // Getting the files and thickness
    FILE* in = fopen(fileName,"r");
    TObjArray *files = new TObjArray();
    TObjArray *hist = new TObjArray();
    char* histKey = "totEventEDepGap_00";
    TObjArray *thickness = new TObjArray();
    char* token;
    TFile* f = NULL;
    TH1F* h = NULL;
    if (in != NULL){
        char line[128];
        while ( fscanf(in,"%s\n",line) == 1 ){
            f = new TFile(line,"READ");
            h = (TH1F*) f->Get(histKey);
            h->Draw("same");
            files->Add(f);
            hist->Add(h);
            token = strtok(line,"_");
            thickness->Add(new TObjString(token));
        }
        fclose(in);
    }
    else{
        perror(filename);
    }
    fprintf(stdout,"Files:\n");
    files->Print();
    fprintf(stdout,"\nHistograms:\n");
    hist->Print();
    // Plotting
    int numEvents = 1000000000;
    float scale = 1.0/(float)numEvents;
    gStyle->SetOptStat(0);
    for (int i = 0; i < hist->GetEntriesFast(); i ++){
        h = (TH1F*) hist[i];
        //h->Draw();
       // (hist[i])->SetLineColor(i+1);
       // (hist[i])->Scale(scale);
    }
  
    // Getting the number of entries
    
   // fprintf(stdout,"15 um has %5.4e entries\n",(h15->GetEntries()-h15->GetBinContent(1)));
    

    //TAxis *xaxis = h25->GetXaxis();
    //xaxis->SetRangeUser(0,1.34);
    
    //TLegend* leg = new TLegend(0.8,0.7,0.9,0.9);
    //leg->AddEntry(h25,"25 um","l");
   // leg->Draw("same");
/*
    h25->GetXaxis()->SetTitle("Energy Deposition per Event (MeV)");
    h25->GetYaxis()->SetTitle("Frequency");
    h25->SetTitle("Co60 Energy Deposition");
    
    gPad->SetLogy();
*/
    // Energy Deposition
  //  fprintf(stdout,"\nMean \n");
  //  fprintf(stdout,"25 um has %5.4e MeV deposited\n",h25->GetMean());
    
  //  fprintf(stdout,"\nIntegral \n");
  //  fprintf(stdout,"25 um has %5.4e MeV deposited\n",h25->Integral("width"));

/*
    // Saving Histograms
    FILE* outFile = fopen("Co60_HistData.txt","w");
    if (outFile != NULL){
        fprintf(stdout,"Number of bins:\n");
        fprintf(stdout,"\t25um %d\n\t50um %d\n\t100um %d\n\t1mm %d\n\t1cm %d\n",
        h25->GetNbinsX(),h50->GetNbinsX(),h100->GetNbinsX(),
        h1mm->GetNbinsX(), h1cm->GetNbinsX());
        
        fprintf(outFile,"\tbin\t\t25um\t\t50um\t\t100um\t\t1 mm\t\t1 cm\n");
        for(int bin = 2; bin < h25->GetNbinsX(); bin++){
            fprintf(outFile,"%5.4e\t%5.4e\t%5.4e\t%5.4e\t%5.4e\t%5.4e\n",
                h25->GetBinCenter(bin),
                h25->GetBinContent(bin),h50->GetBinContent(bin),
                h100->GetBinContent(bin),h1mm->GetBinContent(bin),
                h1cm->GetBinContent(bin));
        }
        fclose(outFile);
    }
*/
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
    gammaVal();
}
#endif
