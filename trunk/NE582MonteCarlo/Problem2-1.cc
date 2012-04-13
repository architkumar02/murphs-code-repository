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
#include "TCanvas.h"
#include "TFile.h"
#include "TNtuple.h"

using namespace std;
#define NSAMPLES 1000000		// Number of samples
double DistA();
double DistB();
int main(int argc, char **argv){

	// Decarling Variables
	double distA[NSAMPLES];
	double distB[NSAMPLES];

	// Only Need the Histograms, but keep the NTuple for Kicks
	TH1D *h_a = new TH1D("dist_a","Distribution A",100,-0.1,3.2);
	TH1D *h_b = new TH1D("dist_b","Distribution B",100,0.9,2.1);
	TFile *tf = new TFile("Problem2-1.root","RECREATE");
	TNtuple *dist = new TNtuple("dist","Distributions","distA:distB");
	assert(dist!=NULL && h_a != NULL && h_b!= NULL && tf != NULL);

	// Filling Values
	int i;
	for (i=0; i<NSAMPLES; i++){
		distA[i] = DistA();	// Distribution A
		h_a->Fill(distA[i]);
		distB[i] = DistB();	// Distribution B
		h_b->Fill(distB[i]);
		dist->Fill(distA[i],distB[i]);
	}

	/**
	 * Plotting with ROOT
	 */
	TCanvas *c_a = new TCanvas("ca","Canvas",400,400);
	h_a->Draw();
	c_a->SaveAs("Problem2-1a.gif");
	TCanvas *c_b = new TCanvas("cb","Canvas",400,400);
	h_b->Draw();
	c_b->SaveAs("Problem2-1b.gif");
	tf->Write();
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
