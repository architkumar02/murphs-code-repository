/**
 * @brief Testing the Tally Code
 * 
 * @author Matthew J. Urffer (matthew.urffer@gmail.com)
 */

#include <stdio.h>
#include <stdlib.h>
#include "tally.h"
#include "csapp.h"
#include "math.h"

/**
 * @brief Function Prototypes 
 */
void printTestHeader(int testNum,char* message);

/**
 * @brief Variable Declrations 
 */
tally_t *tally;

int main(){
	int i;
	int numEvents;
	int numBins;

	
	// Test 1
	numEvents = 10000000;
	numBins = 1;
	tally = initilizeTally(numBins,"Constant Values (Test 1)");
	printTestHeader(1,"Constant values");
	for (i=0; i<numEvents; i++) {
		incrementTally(tally, 0, 1);
	}

	fprintf(stdout,"Results from Test 1:\n");
	printTally(stdout,tally,numEvents);
	fprintf(stdout,"\tExpected a mean of 1.0 and a small (zero) precent error\n");
	deleteTally(tally);
	
	// Test 2
	numBins = 10;
	tally = initilizeTally(numBins,"Flat Distribution (Test 2)");
	printTestHeader(2,"Flat Distribution");
	for (i=0; i<numEvents*numBins; i++) {
		incrementTally(tally, rand() % numBins, 1.0);
	}
	fprintf(stdout,"Results from Test 2:\n");
	printTally(stdout,tally,numEvents);
	fprintf(stdout,"\tExpected a mean of 1.0 and a small precent error on each bin\n");
	deleteTally(tally);	
			
	// Test 3
	numBins = 1;
	tally = initilizeTally(numBins,"Uniform Distrubtion (Test 3)");
	printTestHeader(3, "One bin filled with a uniform distrubtion");
	for (i=0; i<numEvents; i++) {
		incrementTally(tally, 0, rand() / (double) RAND_MAX);
	}
	fprintf(stdout, "Results from Test 3:\n");
	printTally(stdout,tally, numEvents);
	fprintf(stdout,"\tExpected a mean of 0.5 and a %2.3f%% error\n",(sqrt(1.0/12.0)/0.5)*100);

	return EXIT_SUCCESS;
}

void printTestHeader(int testNum, char *message){
	
	fprintf(stdout,"--------------------------------\n");
	fprintf(stdout,"   Test %d\n",testNum);
	fprintf(stdout,"   Testing %s\n",message);
	fprintf(stdout,"--------------------------------\n");

	return;
}