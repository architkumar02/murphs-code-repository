/**
 *  Choosing From Distributions - Direct Inversion
 *  The inversion was completed analaytically, this code means
 *  to test that inveration.
 * 
 * Matthew J. Urffer, NE 582, Problem 2-1.  Febuary 5, 2012
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "TH1D.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TNtuple.h"

using namespace std;
#define NSAMPLES 1000000		// Number of samples

double FuncA(double x);
double FuncB(double x);
double DistA();
double DistB();
int main(){

	// Decarling Variables
	double distA[NSAMPLES];
	double distB[NSAMPLES];

	// Only Need the Histograms, but keep the NTuple for Kicks
	TH1D *h_a = new TH1D("dist_a","Distribution A",25,-0.1,3.2);
	TH1D *h_b = new TH1D("dist_b","Distribution B",25,0.9,2.1);
	assert(h_a != NULL && h_b!= NULL );

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
	TF1 *funA = new TF1("distA","sin(x)",0,acos(-1));
	double xValue = acos(-1)*0.5;
	double binValue = h_a->Interpolate(xValue);
	h_a->Scale(funA->Eval(xValue)/binValue);
	h_a->Draw();
	funA->Draw("same");
	c_a->SaveAs("Problem2-1a.gif");

	TCanvas *c_b = new TCanvas("cb","Canvas",400,400);
	TF1 *funB = new TF1("distB","1+x",1,2);
	xValue = 1.5;
	binValue = h_b->Interpolate(xValue);
	h_b->Scale(funB->Eval(xValue)/binValue);
	h_b->Draw();
	funB->Draw("same");
	c_b->SaveAs("Problem2-1b.gif");
}

/**
 * FuncA - Evaluates function A (sinx(x)) at x
 * @ param - x
 * @ return funcA(x)
 */
double FuncA(double x){
	return sin(x);
}

/**
 * FuncB - Evaluates function B (1+x) at x
 * @param - x
 * @return funcB(x)
 */
double FuncB(double x){
	return 1+x;
}

/**
 * Distribution A
 *
 * The distribution is pdf(x) = 1/2*sin(x), 0<x<pi
 * The invereted distribution is arccos(1-2rand)
 */
inline double DistA(){
	return acos( 1-2*(double)rand()/(double) RAND_MAX);
}

/**
 * Distrubtion B
 *
 * The distribution is pdf(x) = 1/5*(1+x).
 * The inverted distrubtion is -1 + sqrt(4+5x)
 */
inline double DistB(){
	return sqrt(4+5*(double)rand()/(double) RAND_MAX)-1;
}
