/**
 * Problem 1-4, NE 582, Matthew J. Urffer
 *
 * Given two random deviates, demonstrate that:
 *  a) standard deviation of sum is individual deviates added in quadrature
 *  b) standard deviation of subraction is individual deviates added in quadrature
 * 
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include "TH1D.h"
#include "TCanvas.h"

#define N 100000
#define X1 0
#define X2 1
#define RESULT 2

// Function Protopytes
double addition(double a, double b);
double subtraction(double a, double b);
void Opperation(double (*mathOpp)(double,double), char *description);
using namespace std;

int main(){

    fprintf(stdout,"Part A: Standard Deviation of x1 + x2 with %d trials\n",N);
    Opperation(addition,"Addition");
    
    
    fprintf(stdout,"\nPart B: Standard Deviation of x1 - x2 with %d trials\n",N);
    Opperation(subtraction,"Subtraction");
    return EXIT_SUCCESS;
}
void Opperation(double (*mathOpp)(double,double), char *description){
    
    char str[80]; 
    memset(str,'\0',80*sizeof(char));
    TH1D *h = new TH1D("Result",strcat(str,description),100,-2,2);
    assert(h != NULL);

    double x1[N];
    double x2[N];
    double result[N];
    
    // Bin for the results
    double bin[3] = {0,0,0};       
    double bin2[3] = {0,0,0};

    for(int i=0; i < N; i++){
        // Setting up the X-Bins
        x1[i] = (double) rand()/ (double) RAND_MAX;
        x2[i] = (double) rand()/ (double) RAND_MAX;

        // Preforming the Opperation
        result[i] = (*mathOpp)(x1[i],x2[i]);
        h->Fill(result[i]);

        // Binning the Results
        bin[X1] += x1[i];
        bin[X2] += x2[i];
        bin[RESULT] += result[i];
        bin2[X1] += x1[i]*x1[i];
        bin2[X2] += x2[i]*x2[i];
        bin2[RESULT] += result[i]*result[i];
    }

    // Printing the Results
    double xHat[3];
    double x2Hat[3];
    double sampleVar[3];
    double sampleSTD[3];
    double meanVar[3];
    double meanSTD[3];
    for(int i=0; i<3; i++){
        xHat[i] = bin[i]/(double)N;
        x2Hat[i] = bin2[i]/(double)N;
        sampleVar[i] = (double)N/(double)(N-1)*(x2Hat[i]-xHat[i]*xHat[i]);
        sampleSTD[i] = sqrt(sampleVar[i]);
        meanVar[i] = sampleVar[i]/(double)N;
        meanSTD[i] = sqrt(meanVar[i]);
    }
    fprintf(stdout,"\tx1 is %5.3f +/- %5.3e\n",xHat[X1],meanSTD[X1]);
    fprintf(stdout,"\tx2 is %5.3f +/- %5.3e\n",xHat[X2],meanSTD[X2]);
    fprintf(stdout,"\t%s is %5.3f +/- %5.3e\n",description,xHat[RESULT],meanSTD[RESULT]);
    fprintf(stdout,"\tsqrt(varX1+varX2) = %5.3e\n",sqrt(meanVar[X1]+meanVar[X2]));
    /**
     * Plotting with ROOT
     */
     TCanvas *c = new TCanvas("c","Canvas",400,400);
     h->Draw();
     c->SaveAs(strcat(str,".gif"));
     c->Close();
     
}

double addition(double a, double b){
    return a+b;
}
double subtraction(double a, double b){
    return a-b;
}
