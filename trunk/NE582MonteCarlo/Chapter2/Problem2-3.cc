/**
 * Choosing from Distributions using Probability Mixing.
 *
 * Matthew J. Urffer, NE 582, Problem 2-3.  Febuary 8, 2012
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "TH1D.h"
#include "TF1.h"
#include "TCanvas.h"

using namespace std;
#define NSAMPLES 1000000								// Number of samples

// Function Protoypes
double DistA();
double DistB();
double pdf1A(double x){ return acos(-1)*sqrt(x); }
double pdf1B(double x){ return acos(1-2*x); }
double pdf2A(double x) {return pow(x,(1.0/3.0));}
double pdf2B(double x) {return -0.5*log(-1.0*x*(exp(-2)-exp(-4))+exp(-2)); }
double FuncB(double *x,double *y);

int main(){

	// Decarling Variables
	double distA[NSAMPLES];
	double distB[NSAMPLES];

	// Only Need the Histograms, but keep the NTuple for Kicks
	TH1D *h_a = new TH1D("dist_a","Distribution A",100,-0.1,3.2);
	TH1D *h_b = new TH1D("dist_b","Distribution B",100,-0.1,2.1);
	assert(h_a != NULL && h_b!= NULL);

	
	// Filling Values
	int i;
	for (i=0; i<NSAMPLES; i++){
		distA[i] = DistA();	// Distribution A
		h_a->Fill(distA[i]);
		distB[i] = DistB();	// Distribution B
		h_b->Fill(distB[i]);
	}
	/**
	 * Plotting with ROOT
	 */
	// Normalzign the bins, and then plotting the orignal function 
	TCanvas *c_a = new TCanvas("ca","Canvas",400,400);
	TF1 *funA = new TF1("distA","2*x + sin(x)",0,acos(-1));
	double xValue = acos(-1)*0.5;
	double binValue = h_a->Interpolate(xValue);
	h_a->Scale(funA->Eval(xValue)/binValue);
	h_a->Draw();
	funA->Draw("same");
	c_a->SaveAs("Problem2-3a.gif");

	TCanvas *c_b = new TCanvas("cb","Canvas",400,400);
	TF1 *funB = new TF1("distB",FuncB,0,2,0);
	xValue = 0.99;
	//binValue = h_b->Interpolate(xValue);
	binValue = h_b->GetBinContent(h_b->GetMaximumBin());
	h_b->Scale(funB->Eval(xValue)/binValue);
	h_b->Draw();
	funB->Draw("same");
	c_b->SaveAs("Problem2-3b.gif");
	return EXIT_SUCCESS;
}

/**
 * FuncB - Evaluates function B  at x
 * @param - x
 * @return funcB(x)
 */
double FuncB(double *x,double *y){
	if (*x > 0 && *x <=1)
		return (*x)*(*x);
	else if ( *x>1 && *x <2)
		return exp(-2.0*(*x));
	else
		return 0;
}
	
	
/**
 * Distribution A
 *
 * The distribution is pdf(x) = 2*x+sin(x), 0<x<pi
 */
double DistA(){
	double p = (double) rand()/(double) RAND_MAX;
	double r = (double) rand()/(double) RAND_MAX;
	if (p < pow(acos(-1),2.0)/(pow(acos(-1),2.0)+2.0))
		return pdf1A(r);
	else
		return pdf1B(r);
}

/**
 * Distribution B
 */
double DistB(){

	double p = (double) rand()/(double) RAND_MAX;
	double r = (double) rand()/(double) RAND_MAX;
	//if ( p < (1.0/3.0)/(1.0/3.0+2*exp(-2)-2.0*exp(-4)))
	if ( p < 0.850681)
		return pdf2A(r);
	else
		return pdf2B(r);
}
