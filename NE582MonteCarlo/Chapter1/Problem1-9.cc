/**
 * Problem 1-9, NE 582, Matthew J. Urffer
 *
 * Show that the star discrepancy of an LCG is proporitional to 1/Sqrt(n)
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <math.h>
#include <vector>
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TH1.h"
#include "LCG.h"
#include "Discrepancy.h"
using namespace std;

int main(){

    int i;
    int maxPower = 20;     
    int maxHistories = 1000;
    double *nSamples = (double*) malloc(maxPower*sizeof(double));
    double *xHat = (double *)malloc(maxPower*sizeof(double));
    double *meanSTD = (double *)malloc(maxPower*sizeof(double));
    double *xError = (double*) malloc(maxPower*sizeof(double));
    memset(xError,0,maxPower*sizeof(double));

    // Differnet Amount of Samples in the Star Disgrepancy
    for(i=0; i < maxPower; i++){
        double bin = 0;
        double bin2 = 0;
        // Each time doing maxHistories
        for(int k=0; k < maxHistories; k++){
            vector<double> randoms; 
            for(int j=0; j< (1<<i); j++)
                randoms[j] = (double) rand()/(double) RAND_MAX;

            // Computing the Star Disgrepancy for this history
            Discrepancy star = Discrepancy(randoms);
            bin += star.StarDiscrepancy();
            bin2 += bin*bin;
        }
        // Computing the statistics of that trial
        xHat[i] = bin/(double) maxHistories;
        meanSTD[i] = sqrt((double)maxHistories/(double)(maxHistories-1)*(bin2/(double) maxHistories-xHat[i]*xHat[i]) );

        // Printing the results
        fprintf(stdout,"Star disgrepancy of %d samples is %5.3f +/- %5.3f (%d histories)\n",(1<<i),xHat[i],meanSTD[i],maxHistories);
    }
    /*
    TCanvas *c = new TCanvas("c","Canvas",400,400);
    TGraphErrors *g1 = new TGraphErrors(maxPower,xHat,xError,meanSTD);
    g1->Draw("AL");
    g1->GetHistogram()->SetXTitle("Number of Samples in Star Discrepancy");
    g1->GetHistogram()->SetYTitle("Star Discrepancy");
    c->SetLogx();
    c->SetLogy();
    c->SaveAs("Problem1-9.gif");
*/
    // Good Boys and Girls clean up their memory
    free(xHat);
    free(nSamples);
    free(xError);
    free(meanSTD);
    return EXIT_SUCCESS;
}
