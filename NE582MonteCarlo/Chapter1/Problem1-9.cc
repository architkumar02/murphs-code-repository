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

void CalcuateStarDisgrepancy(int numSamples, double *mean, double* std);
int main(){
	// Looking at the specific case of 10, 40, 150, and 640
	double mu;
	double std;
	CalcuateStarDisgrepancy(10,&mu,&std);
	CalcuateStarDisgrepancy(40,&mu,&std);
	CalcuateStarDisgrepancy(160,&mu,&std);
	CalcuateStarDisgrepancy(640,&mu,&std);


	int i;
	int maxPower = 20;     
	double *nSamples = (double*) malloc(maxPower*sizeof(double));
	double *xHat = (double *)malloc(maxPower*sizeof(double));
	double *meanSTD = (double *)malloc(maxPower*sizeof(double));
	double *xError = (double*) malloc(maxPower*sizeof(double));
	memset(xError,0,maxPower*sizeof(double));

	// Differnet Amount of Samples in the Star Disgrepancy
	for(i=0; i < maxPower; i++){

		nSamples[i] = (double) (1<<i);
		CalcuateStarDisgrepancy(nSamples[i], xHat+i, meanSTD+i);
	}

	// Plotting Results
	TCanvas *c = new TCanvas("c","Canvas",400,400);
	TGraphErrors *g1 = new TGraphErrors(maxPower,nSamples,xHat,xError,meanSTD);
	g1->Draw("ALP");
	g1->SetTitle("Star Disgrepancy as a function of Samples");
	g1->GetHistogram()->SetXTitle("Number of Samples in Star Discrepancy ");
	g1->GetHistogram()->SetYTitle("Star Discrepancy");
	c->SetLogx();
	c->SetLogy();
	c->SaveAs("Problem1-9.gif");

	// Good Boys and Girls clean up their memory
	free(xHat);
	free(nSamples);
	free(xError);
	free(meanSTD);
	return EXIT_SUCCESS;
}
/**
 * Calculates the Star Disgrepancy for a given number of samples
 * @param numSamples
 * @return mean - the average of the samples
 * @return std - the standard devation of the samples
 */
void CalcuateStarDisgrepancy(int numSamples, double *mean, double* std){
	int maxHistories = 1000;
	double bin = 0;
	double bin2 = 0;
	
	// Each time doing maxHistories
	for(int k=0; k < maxHistories; k++){
		vector<double> randoms; 
		for(int j=0; j< numSamples; j++)
			randoms.push_back((double) rand()/(double) RAND_MAX);

		// Computing the Star Disgrepancy for this history
		Discrepancy star = Discrepancy(randoms);
		bin += star.StarDiscrepancy();
		bin2 += star.StarDiscrepancy()*star.StarDiscrepancy();
	}
	
	// Computing the statistics of that trial
	*mean = bin/(double) maxHistories;
	double x2hat = bin2/(double) maxHistories;
	double sampleVar = ((double) maxHistories / (double) (maxHistories-1)) * (x2hat-(*mean)*(*mean));
	double meanVar = sampleVar / (float) maxHistories;
	*std = sqrt(meanVar);

	// Printing the results
	fprintf(stdout,"Star disgrepancy of %d samples is %5.3e +/- %5.3e (%d histories)\n",
			numSamples,*mean,*std,maxHistories);

	return;
}
