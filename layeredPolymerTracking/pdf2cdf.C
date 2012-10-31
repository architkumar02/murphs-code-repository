#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <TH1F.h>

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
    TH1F* cdf = new TH1F(buffer,"CDF",hist->GetNbinsX(),hist->GetMinimum(),
            hist->GetMaximum());

    // Computing the CDF
    int total = 0;
    for(int i = 0; i < hist->GetNbinsX(); i++){
        total += hist->GetBinContent(i);
        for (int k = 0; k < total; k++){
            cdf->AddBinContent(i);
        }

    }
    cdf->Scale(1.0/cdf->Integral());
    cdf->Draw();
    return cdf;
}
