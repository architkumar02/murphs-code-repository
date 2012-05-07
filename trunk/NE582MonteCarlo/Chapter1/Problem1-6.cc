/**
 * Problem 1-6, NE 582, Matthew J. Urffer
 * 
 * Constructs random numbers based on an LCG
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "LCG.h"

int main(const int argc, const char *argv){
	// Part A
	LCG a(9,1,16,5);
	a.LCGSequence();

	// Part B
	LCG b(5,3,32,3);
	b.LCGSequence();
	
	// Part C
	LCG c(13,5,64,1);
	c.LCGSequence();
}
	
