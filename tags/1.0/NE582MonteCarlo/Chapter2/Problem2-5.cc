/**
 * Invent and solve own problem to prove:
 * 	- variance of a sum of distributions is some of variances
 * 	- variance of a product of distributions is cross product of variances
 *
 * Matthew J. Urffer, NE 582, Problem 2-5.  Febuary 10, 2012
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

using namespace std;

#define NSAMPLES 1000								// Number of samples
inline double randn();
inline double gaussian(double mu, double var, double xmin, double xmax);
double variance(double*,int);
double mean(double*,int);

int main(){
	// Decarling Variables
	double sum[NSAMPLES];			// It would be best to only do one test at
	double diff[NSAMPLES];			// a time, and dynamically allocate the mem.
	double prod[NSAMPLES];			// more trials could then be done.  A double
	double distA[NSAMPLES];			// is 8 bytes, so 1 million samples is about
	double distB[NSAMPLES];			// 8 gigabytes of memory.  Or use a float.

	// Filling Distribution A and B
	int i;
	for (i=0; i < NSAMPLES; i++){
		/*
		distA[i] = gaussian(1,1,-5,5);	
		distB[i] = gaussian(2,0.5,-5,5);
		*/
		distA[i] = randn();
		distB[i] = randn();
	}

	// Computing the Sum, Product, and Difference
	for (i=0; i<NSAMPLES; i++){
		sum[i] = distA[i] + distB[i];
		diff[i] = distA[i] - distB[i];
		prod[i] = distA[i]*distB[i];
	}
	// Computing the Mean, Standard Devaition
	double varA = variance(distA,NSAMPLES);
	double muA = mean(distA,NSAMPLES);
	double varB = variance(distB,NSAMPLES);
	double muB = mean(distB,NSAMPLES);
	double varSum = variance(sum,NSAMPLES);
	double varDiff = variance(diff,NSAMPLES);
	double varProd = variance(prod,NSAMPLES);
	
	fprintf(stdout,"Comparison of Standard Deviations (%d samples)\n",(int)NSAMPLES);
	fprintf(stdout,"Sum - Expected:%f  Observed:%f\n",sqrt(varA+varB),sqrt(varSum));
	fprintf(stdout,"Diff - Expected:%f  Observed:%f\n",sqrt(varA+varB),sqrt(varDiff));
	fprintf(stdout,"Prod - Expected:%f  Observed:%f\n",sqrt(muA*muA*varA+muB*muB*varB+varA*varB),sqrt(varProd));
	return EXIT_SUCCESS;
}
double variance(double *d, int n){
	int i;
	double var=0;
	double mu = mean(d,n);
	for(i=0; i<n; i++)
		var += pow((d[i]-mu),2);
	return var / (double) (n-1.0);
}

double mean(double *d, int n){
	int i;
	double m=0;
	for(i=0; i<n; i++)
		m += d[i];
	return m/(double) n;
}
/**
 * randn() - Random Number from a normal distribution
 *
 * Sampling from Box-Muller Transform
 */
inline double randn(){
	double x = (double) rand()/(double) RAND_MAX;
	double y = (double) rand()/(double) RAND_MAX;

	return sqrt(-2.0*log(x))*cos(2*acos(-1.)*y);
}
/**
 * gaussian - Random Number for a Gaussian Distribution in the range [xmin xmax]
 *
 * @arg mu   - the mean of the distribution
 * @arg var  - the standard deviation of the distribution
 *
 * Method is implemented using rejection.
 */
inline double gaussian(double mu, double var, double xmin, double xmax){
	double PI_MAX = 1.0;		// Forgetting the normalization const
	double x;
	double y;
	// WARNING!  DRAGONS!
	do {
		x = xmin+(xmax-xmin)*(double) rand()/(double) RAND_MAX;
		y = (double) rand()/(double) RAND_MAX * PI_MAX;
	} while (y>= exp(-1.0*pow(x-mu,2)/(2*pow(var,2))));
	return x;
}
