#include <stdio.h>
#include <stdlib.h>

#include <TH1F.h>
#include <TAxis.h>
/**
 * @brief Converts a PDF histogram to a CDF Histogram 
 * A weighted product of the bin is used
 * @param hist - PDF Histogram
 * @return Pointer to CDF histogram
 */
TH1F* pdf2cdfWeighted(TH1F* hist){
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
    double total = 0.0;
    double trapEDep;
    for(int i = 0; i < hist->GetNbinsX(); i++){
        trapEDep = hist->GetBinLowEdge(i)+0.5*hist->GetBinWidth(i);
        total += hist->GetBinContent(i)*trapEDep;
        cdf->SetBinContent(i,total);
    }
    cdf->Scale(1.0/(double)total);
    return cdf;
}
