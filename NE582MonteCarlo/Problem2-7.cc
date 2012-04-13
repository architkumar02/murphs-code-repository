/**
 * Halton Sequence - Generates Psuedo random numbers
 *
 * Halton Sequence has a low disgrepancy
 */
#include "HaltonSequence.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
using namespace std;

double computePi(int n);
int main(){
	int nSamples[] = {100, 1000, 10000, 100000, 1000000};
	int n=5;
	int i;
	cout<<"Calculating Pi with Monte Carlo"<<endl;
	for(i=0; i<n; i++){
		cout<<"Error on "<<nSamples[i]<<": "<<(acos(-1) - computePi(nSamples[i]))<<endl;
	}
}
/**
 * Computing Pi Based On the number of hits that fall inside the unit circle
 */
double computePi(int n){
	// Creating the Sequences
	HaltonSequence base2(2,n);
	HaltonSequence base3(3,n);

	int i;
	int hit = 0;
	for(i=0; i<n; i++){
		if ((pow(base3.nextRandom(),2)+pow(base2.nextRandom(),2)) < 1)
			hit+=4;
	}
	return (double) hit / (double) n;
}
