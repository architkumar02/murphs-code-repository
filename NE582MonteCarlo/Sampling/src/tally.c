#include "tally.h"
#include "csapp.h"
#include <string.h>

/**
 * @brief initilize - initilizes the tally by mallocing the space
 *			for the bins and setting them to zero
 * 
 * @param numBins	- the number of bins
 * @param name		- name of the tally
 * @return - an intilized tally
 */
tally_t *initilizeTally(int numBins, char* name){
	int i;
	tally_t *tally;
	tally = (tally_t*) Malloc(sizeof(tally_t));
	tally->nBins = numBins;
	tally->nEntries = 0;
	tally->name = name;
	tally->bins = (bin_t*) Malloc(sizeof(bin_t)*numBins);
	for (i = 0; i< numBins; i++)
		initilizeBin(getBin(tally,i));
	
	// return the intilized bin
	return tally;
}

/**
 * @brief initilizeBin - intilizes a bin to zero
 * 
 * @param bin to initilize
 */
void initilizeBin(bin_t *bin){
	bin->value = 0;
	bin->valueSquared = 0;
	bin->nEntries = 0;
	return;
}

/**
 * @brief deleteTally - deletes the memory associated with a tally struct
 * 
 * @param tally - the tally whose memeory is to be freed
 */
void deleteTally(tally_t *tally){
	// Clearing the memory
	Free(tally->bins);
	tally->nBins = 0;
	tally->nEntries = 0;
}

/**
 * @brief incrementTally - adds a value to a tally
 *
 * @param tally		- the tally to increment 
 * @param bin		- the bin to incrmenent
 * @param value		- the value to add to the bin
 */
int incrementTally(tally_t *tally, int binNum, double val){
	bin_t *bin;		// Bin
	tally->nEntries++;
	bin = getBin(tally,binNum);
	bin->value += val;
	bin->valueSquared += val*val;
	bin->nEntries ++;
	return EXIT_SUCCESS;
}
/**
 * @brief getBin
 *
 * @param tally		- pointer to tally
 * @param bin		- bin number
 * @return pointer to the bin
 */
bin_t *getBin(tally_t *tally, int bin){
	return &(tally->bins[bin]);
}


/**
 * @brief getNormBinValues - normalizes bin values.
 *
 * @param bin	- pointer to the bin
 * @param mean	- pointer to the mean location
 * @param pErr	- pointer to the precent Error
 * @param norm	- normalziation factor
 * @return sucess or failure
 */
int getNormBinValues(bin_t *bin, double *mean, double *pErr, double norm){
	double std;
	*mean = bin->value;
	std = bin->valueSquared;
	
	//Normalizing
	*mean = (*mean) / norm;
	
	std = norm / (norm -1.0)*sqrt( fabs(std/norm - (*mean)*(*mean)));
	*pErr = (std / (*mean))*100;
	return EXIT_SUCCESS;
}
/**
 * @brief printTally - Prints a tally
 *
 * @param tally		- tally to print
 * @param norm		- normalization of the tally
 * @param file		- file to print to
 */
void printTally(FILE* file,tally_t *tally,double norm){
	int i;
	double *values;
	
	if( !file){
		fprintf(stderr, "File does not exist.  Printing tally to stdout\n");
		file = stdout;
	}
	
	values = Malloc(tally->nBins*sizeof(double));
	fprintf(file, "Tally %s\n",tally->name);
	fprintf(file,"Bin\t\tMean\t\t  %% Error\n");
	for(i=0; i<tally->nBins; i++){
		getNormBinValues(getBin(tally, i), values +2*i, values+2*i+1, norm);
		fprintf(file,"%2d\t%1.5e +- %2.3f%%\n",i,values[2*i],values[2*i+1]);
	}
	fprintf(file,"\n");
	Free(values);
	return;
}


