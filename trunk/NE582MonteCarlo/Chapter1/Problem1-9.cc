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
#include "TGraph.h"
#include "TCanvas.h"
#include "TH1.h"
#include "LCG.h"
#include "Discrepancy.h"
using namespace std;
double* Star_LCG(LCG *lcg, int trials);

int main(){

	
	// Looking at the Star Disgrepancy of a low stride LCG
	LCG low_stride(9,1,16,1);		// LCG of Stride 16
	LCG med_stride(13,5,64,5);		// LCG of Stide 64
	LCG super_duper(6909,1,LONG_MAX,3);	// There is actually a LCG called 
										// super duper. It's m is 2^32, which 
										// needs to be a long (Marsaglia)
	
	int trials = 10000;
	double *star1 = Star_LCG(&low_stride,trials);		
	double *star2 = Star_LCG(&med_stride,trials);		
	double *star3 = Star_LCG(&super_duper,trials);		

	// Plotting the results
	double* x = (double*) malloc(trials*sizeof(double));
	int i;
	TCanvas *c = new TCanvas("c","Canvas",400,400);
	TH1F* h_low = new TH1F("low","Low Stride",100,0,1);
	TH1F* h_med = new TH1F("med","Medium Stride",100,0,1);
	TH1F* h_high = new TH1F("high","High Stride",100,0,1);
	for(i=0; i<trials; i++){
		h_low->Fill(low_stride.randomLCG());
		h_med->Fill(med_stride.randomLCG());
		h_high->Fill(super_duper.randomLCG());
	}
	h_low->Draw();
	c->SaveAs("Problem1-9_LowStride.gif");
	h_med->Draw();
	c->SaveAs("Problem1-9_MedStride.gif");
	h_high->Draw();
	c->SaveAs("Problem1-9_HighStride.gif");

	for(i=0; i < trials; i++)					// Plotting as 1/sqrt(x)
		x[i] = *(star1+trials+i);

	TGraph *g1 = new TGraph(trials,x,star1);
	TGraph *g2 = new TGraph(trials,x,star2);
	TGraph *g3 = new TGraph(trials,x,star3);
	g1->Draw("AL");
	g1->GetHistogram()->SetXTitle("Number of Samples");
	g1->GetHistogram()->SetYTitle("Star Discrepancy");
	g2->SetLineColor(2);
	g2->Draw("L");
	g3->SetLineColor(3);
	g3->Draw("L");
	c->SetLogx();
	c->SetLogy();
	c->SaveAs("Problem1-9.gif");

	// Good Boys and Girls clean up their memory
	free(x);
	free(star1);
	free(star2);
	free(star3);
	
	return EXIT_SUCCESS;
}
/**
 * Star_LCG - Calculates the Star Discrepancy of an LCG for a given number of
 * 		trials (n)
 *
 * @returns An array containing the star discrepancies, followed by an array
 * 		of samples that produced the discrepancy.
 */
double *Star_LCG(LCG *lcg, int trials){
	vector<double> r;		// Random Number From LCG
	int i,j;
	double *star = (double*) malloc(2*trials*sizeof(double));
	assert(star!=NULL);
	
	for (i = 0; i < trials; i++){
		star[i+trials] =  (double) i+1;		// 0th i is first trial	
		// Pulling i random numbers for the LCG
		for (j=0; j <i; j++)
			r.push_back(lcg->randomLCG());
		// Computing the Star Disgrepancy
		Discrepancy d = Discrepancy(r);		// I know this could use less mem.
		star[i] = d.StarDiscrepancy();
	}
	
	return star;
}
