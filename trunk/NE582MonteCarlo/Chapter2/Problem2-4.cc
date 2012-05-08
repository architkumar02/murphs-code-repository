/**
 * Choosing from Distributions using the Metropolis-Hastings Method.
 *
 * Matthew J. Urffer, NE 582, Problem 2-4.  May 8th, 2012
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "TH1D.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TRandom.h"

using namespace std;

// Function Protoypes
double PDFA(double);
double PDFB(double);
double FuncB(double *x,double *y);

void Metroplolis(double x0,double *x,int numItter,double (*pdf)(double));


int main(){

	// Decarling Variables
    int numSamples = 100000;
    double *distA  = (double*) malloc(numSamples*sizeof(double));
    double *distB  = (double*) malloc(numSamples*sizeof(double));

	// Only Need the Histograms, but keep the NTuple for Kicks
	TH1D *h_a = new TH1D("dist_a","Distribution A",100,-0.1,3.2);
	TH1D *h_b = new TH1D("dist_b","Distribution B",100,-0.1,2.1);
	assert(h_a != NULL && h_b!= NULL);

	fprintf(stdout,"Running Metraplois Method\n");
    Metroplolis(0.5,distA,numSamples,PDFA);
    Metroplolis(0.5,distB,numSamples,PDFB);
	
    // Filling Values
	int i;
	for (i=100; i<numSamples; i++){
		h_a->Fill(distA[i]);
		h_b->Fill(distB[i]);
	}
    fprintf(stdout,"Starting to Plot\n");
	/**
	 * Plotting with ROOT
	 */
	// Normalzign the bins, and then plotting the orignal function 
	TCanvas *c_a = new TCanvas("ca","Canvas",400,400);
	TF1 *funA = new TF1("distA","2*x + sin(x)",0,acos(-1));
	double xValue = acos(-1)*0.5;
	double binValue = h_a->Interpolate(xValue);
	h_a->Scale(funA->Eval(xValue)/binValue);
	funA->Draw();
	h_a->Draw("same");
	c_a->SaveAs("Problem2-4a.gif");

	TCanvas *c_b = new TCanvas("cb","Canvas",400,400);
	TF1 *funB = new TF1("distB",FuncB,0,2,0);
	xValue = 0.99;
	//binValue = h_b->Interpolate(xValue);
	binValue = h_b->GetBinContent(h_b->GetMaximumBin());
	h_b->Scale(funB->Eval(xValue)/binValue);
	funB->Draw();
	h_b->Draw("same");;
	c_b->SaveAs("Problem2-4b.gif");

    free(distA);
    free(distB);
	return EXIT_SUCCESS;
}

void Metroplolis(double x0,double *x,int numItter,double (*pdf)(double)){
    double *normalRand  = (double*) malloc(numItter*sizeof(double));
    double *uniformRand = (double*) malloc(numItter*sizeof(double));
    
    // Filling the random number distributions
    TRandom *randGenerator = new TRandom();
    for (int i =0 ; i < numItter; i++){
        uniformRand[i] = (double) rand() / (double) RAND_MAX;
        normalRand[i]  = randGenerator->Gaus();
    }

    double xPrev = x0;
    double xTilde;

    // Looping through
    for (int i=0; i < numItter; i++){
        
        // Add normaly distrubited noise
        xTilde = xPrev + normalRand[i];
        
        // Choosing what to return
        if( (*pdf)(xTilde) > (*pdf)(xPrev) )
            x[i] = xTilde;
        else{
            if (uniformRand[i] <= (*pdf)(xPrev)/(*pdf)(xPrev))
                x[i] = xTilde;
            else
                x[i] = xPrev;
        }
        xPrev = x[i];
    }

    // Cleaning up
    free(normalRand);
    free(uniformRand);
    return;
}
	
	
/**
 * PDF A
 *
 * The distribution is pdf(x) = 2*x+sin(x), 0<x<pi
 */
double PDFA(double x){
    if( x < 0 || x > acos(-1))
        return 0;
    else
        return 2.0*x+sin(x);
}

/**
 * PDF B
 *
 * The distrubtion is pdf(x) = x^4          ,0 < x < 1
 *                             (2-x)^4      ,1 < x < 2
 */
double PDFB(double x){
    if (x < 1.0 && x > 0.0)
        return x*x*x*x;
    else if (x> 1.0 && x < 2.0)
        return pow((2.0-x),4);
    else
        return 0.0;
}
// Wrapper for ROOT
double FuncB(double *x,double *y){
    return PDFB(*x);
}
