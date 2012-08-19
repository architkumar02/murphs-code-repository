/**
 * LCG Random number generator.
 * Matthew J. Urffer
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "LCG.h"

/**
 * Constructor of the LCG
 */
LCG::LCG(unsigned long a, unsigned long b, unsigned long m, unsigned long i) : a(a), b(b), m(m), i(i) {
	// Used Initilization List, So Empty Constructor Body
}
/**
 * Destructor of the LCG
 */
LCG::~LCG(){}

/**
 * randomLCG() - Generates a random number from the LCG
 */
double LCG::randomLCG(){
	double randValue = (double) i / (double) m;
	incrementLCG();
	return randValue;
}
/**
 * incrementLCG() - Increments the LCG according to inext = (a*icurr+b) mod m
 * @returns - the next seed of the LCG
 */
unsigned long LCG::incrementLCG(){
	i  = (a*i + b) % m;
	return i;
}
/**
 * LCGSequenc() - returns the complete sequenc of the LCG
 */
void LCG::LCGSequence(){
	unsigned long i0 = i;
	unsigned long itter = 0;
	unsigned long val;
	fprintf(stdout,"Full Sequence of LCG with a:%d b:%d m:%d\n",a,b,m);
	fprintf(stdout,"\tItter\t icurr \t (a*icurr+b)\tinext\tx\n");
	do {
		// Ouptput the results
		val = (a*i+b);
		fprintf(stdout,"\t%d\t%d\t%d\t\t%d\t%5.3f\n",itter,i,val,incrementLCG(),(double)i/(double) m);
		itter +=1;
	} while (i != i0);
	fprintf(stdout,"Sequence repeated in %d itterations\n",itter);
}
