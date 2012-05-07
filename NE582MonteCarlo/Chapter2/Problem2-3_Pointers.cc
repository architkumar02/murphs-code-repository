/**
 * Choosing from Distributions using Probability Mixing.
 *
 * Matthew J. Urffer, NE 582, Problem 2-3.  Febuary 8, 2012
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <vector>
#include <math.h>
#include <iostream>
#include "TH1D.h"
#include "TCanvas.h"

using namespace std;
#define NSAMPLES 1000000								// Number of samples
#define RAND (double) rand()/(double) RAND_MAX 	// MACRO RAND!!

// Function Protoypes
typedef double (*PDF_t)(double);
double Dist(std::vector<double> Pi, std::vector<PDF_t> PDF);
void DistA(double*);
void DistB(double*);
double pdf1A(double x){ return sqrt(acos(-1)*x); }
double pdf1B(double x){ return acos(1-2*x); }
double pdf2A(double);		// Dist B's PDF's
double pdf2B(double);


int main(){

	// Decarling Variables
	double distA[NSAMPLES];
	double distB[NSAMPLES];

	// Only Need the Histograms, but keep the NTuple for Kicks
	TH1D *h_a = new TH1D("dist_a","Distribution A",100,-0.1,3.2);
	TH1D *h_b = new TH1D("dist_b","Distribution B",100,-0.1,2.1);
	assert(h_a != NULL && h_b!= NULL);
	
	cout<<"In Main, About to Fill Histograms"<<endl;
	
	// Filling Values
	DistA(distA);
	h_a->FillN(NSAMPLES,distA,NULL);

	cout<<"Filled A"<<endl;
	DistB(distB);
	h_b->FillN(NSAMPLES,distB,NULL);
	cout<<"Filled B"<<endl;

	/**
	 * Plotting with ROOT
	 */
	TCanvas *c_a = new TCanvas("ca","Canvas",400,400);
	h_a->Draw();
	c_a->SaveAs("Problem2-3a.gif");
	TCanvas *c_b = new TCanvas("cb","Canvas",400,400);
	h_b->Draw();
	c_b->SaveAs("Problem2-3b.gif");

	return EXIT_SUCCESS;
}
/**
 * Dist
 * @return - a random number polled from probability mixing given by the
 * @arg Pi - Probability of a PDF
 * @arg PDF - PDF to be sampled from
 */
double Dist(vector<double> Pi,vector<PDF_t> PDF){
	assert(Pi.size() == PDF.size());  //ONLY YOU CAN PREVENT FOREST FIRES!
	
	// Actually generating the random number
	double r = RAND;
	unsigned int i;
	unsigned int size = Pi.size();
	double val;
	for (i=0; i < size; i++){
		val = Pi[i];
		if (r < val)
			return PDF[i](r);

	}
}

/**
 * Distribution A
 *
 * The distribution is pdf(x) = 2*x+sin(x), 0<x<pi
 */
void DistA(double *d){

	// Declaring Variables
	vector <double> Pi;							// Normalized Pi's
	Pi.push_back(acos(-1)*acos(-1));
	Pi.push_back(2);
	vector <PDF_t> PDF;							// Function Pointers to PDF's
	PDF.push_back(pdf1A);
	PDF.push_back(pdf1B);
	
	// Normalizing the Pi Vector
	double sum = 0;
	unsigned int i;
	unsigned int size = Pi.size();
	for (i=0; i < size; i++)
		sum += Pi[i];
	for (i=0; i < size; i++)
		Pi[i] = Pi[i]/sum;

	// Creating the Distribution
	for (i=0; i < NSAMPLES; i++){
			d[i] = Dist(Pi,PDF);
	}
}

/**
 * Disribution B
 */
void DistB(double *d){

	// Declaring Variables
	vector <double> Pi;							// Normalized Pi's
	Pi.push_back(acos(-1)*acos(-1));
	Pi.push_back(2);
	vector <PDF_t> PDF;							// Function Pointers to PDF's
	PDF.push_back(pdf2A);
	PDF.push_back(pdf2B);
	
	// Normalizing the Pi Vector
	double sum = 0;
	unsigned int i;
	unsigned int size = Pi.size();
	for (i=0; i < size; i++)
		sum += Pi[i];
	for (i=0; i < size; i++)
		Pi[i] = Pi[i]/sum;

	// Creating the Distribution
	for (i=0; i < NSAMPLES; i++){
			d[i] = Dist(Pi,PDF);
	}
}

double pdf2A(double x) {return pow(x,(-1/3));}
double pdf2B(double x) {return -0.5*log(x*(exp(-4)-exp(-2))+exp(-2)); }
