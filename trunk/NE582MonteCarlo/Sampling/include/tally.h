#ifndef tally_h
#define tally_h

#include <stdio.h>

typedef struct {
	double value;			// Running total of value
	double valueSquared;	// Runnint total of value squared
	int nEntries;
} bin_t;

typedef struct {
	int nBins;		// Number of bins
	int nEntries;	// Number of Entries
	char *name;		// Name of the Tally
	bin_t *bins;	// Bins holding tally data
} tally_t;


/**
 * @brief initilize - initilizes the tally by mallocing the space
 *			for the bins and setting them to zero
 * 
 * @param numBins	- the number of bins
 * @param name		- name of the tally
 * @return - an intilized tally
 */
tally_t *initilizeTally(int numBins, char* name);

/**
 * @brief initilizeBin - intilizes a bin to zero
 * 
 * @param bin to initilize
 */
void initilizeBin(bin_t *bin);

/**
 * @breif deleteTally - deletes the memory associated with a tally struct
 * 
 * @param tally - the tally whose memeory is to be freed
 */
void deleteTally(tally_t *tally);

/**
 * @brief incrementTally - adds a value to a tally
 *
 * @param tally		- the tally to increment 
 * @param bin		- the bin to incrmenent
 * @param value		- the value to add to the bin
 */
int incrementTally(tally_t *tally, int bin, double value);

/**
 * @brief getBin
 *
 * @param tally		- pointer to tally
 * @param bin		- bin number
 * @return pointer to the bin
 */
bin_t *getBin(tally_t *tally, int bin);


/**
 * @brief getNormBinValues - normalizes bin values.  Only the mean is normalized.
 *
 * @param bin	- pointer to the bin
 * @param mean	- pointer to the mean location
 * @param std	- pointer to the standard devation
 * @param norm	- normalziation factor
 * @return sucess or failure
 */
int getNormBinValues(bin_t *bin, double *mean, double *std, double norm);
	
/**
 * @brief printTally - Prints a tally
 *
 * @param tally		- tally to print
 * @param norm		- normalization of the tally
 * @param file		- file to print to
 */
void printTally(FILE * file, tally_t *tally,double norm);
#endif