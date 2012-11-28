#include <stdio.h>
#include <stdlib.h>

#include <TH1F.h>
#include <TAxis.h>
/**
 * @brief Converts a PDF histogram to a CDF Histogram 
 * @param hist - PDF Histogram
 * @return Pointer to CDF histogram
 */
TH1F* pdf2cdf(TH1F* hist){
    if (!hist){
        fprintf(stderr,"Input histogram is null\n");
        return NULL;
    }
    char buffer[256];
    sprintf(buffer,"%s_cdf",hist->GetName());
    double minVal = hist->GetXaxis()->GetXmin();
    double maxVal = hist->GetXaxis()->GetXmax();
    
    TH1F* cdf = new TH1F(buffer,"CDF",hist->GetNbinsX(),minVal,maxVal);

    // Computing the CDF
    int total = 0;
    for(int i = 1; i < hist->GetNbinsX(); i++){
        total += hist->GetBinContent(i);
        cdf->SetBinContent(i,total);
    }
    cdf->Scale(1.0/(double)total);
    //cdf->Draw();
    return cdf;
}
