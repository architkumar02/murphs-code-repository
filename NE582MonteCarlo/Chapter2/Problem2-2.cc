/**
 * Choosing from Distributions using rejeciton.
 *
 * Matthew J. Urffer, NE 582, Problem 2-2.  Febuary 7, 2012
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "TH1D.h"
#include "TCanvas.h"

using namespace std;
#define NSAMPLES 1000000		// Number of samples
double DistA();
double DistB();
inline double PDFB(double);
int main(int argc, char **argv){

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
	TCanvas *c_a = new TCanvas("ca","Canvas",400,400);
	h_a->Draw();
	c_a->SaveAs("Problem2-2a.gif");
	TCanvas *c_b = new TCanvas("cb","Canvas",400,400);
	h_b->Draw();
	c_b->SaveAs("Problem2-2b.gif");
}

/**
 * Distribution A
 *
 * The distribution is pdf(x) = sin(x), 0<x<pi
 */
inline double DistA(){
	double PI_MAX = 1.0;	// Max is 1.0, sin(pi/2)
	double x;
	double y;
	// WARNING!  Infinate Loop Ahead!
	do{
		x = acos(-1)*(double)rand()/(double) RAND_MAX;
		y = (double) rand()/ (double) RAND_MAX * PI_MAX;
	}while(y >= sin(x) );

	return x;
}

/**
 * Distrubtion B
 *
 * The distribution is pdf(x) = x^2 if 0 < x < 1
 *                              exp(-2x) if 1 < x < 2
 */
inline double DistB(){
	double PI_MAX = 1.0;	// Max is 1.0, (x=1)^2
	double x;
	double y;
	// WARNING!  Infinate Loop Ahead!
	do{
		x = 2*(double)rand()/(double) RAND_MAX;
		y = (double) rand()/ (double) RAND_MAX * PI_MAX;
	}while(y >= PDFB(x) );

	return x;
}
/**
 * PDFB
 * The distribution is pdf(x) = x^2 if 0 < x < 1
 *                              exp(-2x) if 1 < x < 2
 */
inline double PDFB(double x){
	if (x > 0 && x < 1)
		return x*x;
	else if (x > 1 && x < 2)
		return exp(-2*x);
	else if (x !=1 ){
		fprintf(stderr,"x: %f should have been in the range 0 to 2, excluding 1\n",x);
		exit(1);
	}

}