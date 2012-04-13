#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "queue.h"
#include "event.h"
#include "csapp.h"
#include "tally.h";


#define DEBUG 0

/**
 *@brief Function Pointers.  This is ISO C, BABY!
 */
queue_t *generatePrimaries(int n, int eventID, double* limits, double* energyPDF, int energyGroups);
int chooseEnergy(double* energyPDF, int energyGroups);
double choosePosition(double limits[]);
double chooseDirection();
void printEvent(event_t *e);

/**
 * @brief Variable Declarations
 */
queue_t *events;	// Queue for holding events
event_t *e;			// Single Simualtion Event
double mfp;			// Mean Free Path
double pos;			// Proposed Position

double geometry[2] = {0, 50};;			// Problem is on the interval 0 to 50 cm
double sourceLimits[2] = {0, 5};		// Source is from 0 to 5 cm

int energyGroups = 2;					// Number of Energy Groups
double energyPDF[2] ={ 0.4, 0.6};		// Energy Groups (Sorted PDF)

/** Cross Section Locations */
double totxs[2] ={0.1,0.2};				// Total Cross Section
double scat[2][2] = {{0.05, 0.04},		// Group Scattering Cross Sections
					{0.0, 0.1}};			
double *totscat;						// Total Scattering Cross Section

/** Binning Location */
tally_t *leftLeakage;					// left leackage bin
tally_t *rightLeakage;					// right leackage bin
tally_t *flux;							// flux bin
tally_t *collisions;					// number of collisions
tally_t *absorbitions;					// number of absorbitions

int i;									// Itteration Variables
int j;
int numSimulated;			

int main(int argc, char *argv[]){

	// Number of particles to simualte
	int numberParticles;
	if (argc <= 1)
		numberParticles = 1000000;
	else
		numberParticles = atoi(argv[0]);



	// Computing Total Cross Sections
	totscat = (double*) Malloc(energyGroups*sizeof(double));

	for(i = 0; i<energyGroups; i++){
		totscat[i] = 0;
		for(j= 0; j<energyGroups; j++)
			totscat[i] += scat[i][j];
	}

	// Converting Scattering Cross Section to CDF
	for(i=0; i<energyGroups; i++){
		for(j = 1; j<energyGroups; j++)
			scat[i][j] += scat[i][j-1];
		
		for(j = 0; j<energyGroups; j++)
			scat[i][j] = scat[i][j]/scat[i][energyGroups-1];
	}	


	// Setting up bins
	leftLeakage = initilizeTally(energyGroups);
	rightLeakage = initilizeTally(energyGroups);
	flux =  initilizeTally(energyGroups);
	collisions = initilizeTally(energyGroups);
	absorbitions = initilizeTally(energyGroups);

	/**
	 * @brief Monte Carlo Simulation.
	 * There are six decisions that need to be made:
	 *	1) Initial Position
	 *	2) Initial Direction
	 *	3) Initial Energy
	 *	4) Distance to next collision
	 *	5) Type of collision
	 *	6) Outcome of scattering event
	 */

	for (numSimulated=0; numSimulated<numberParticles; numSimulated++){
		// The events holds a particle at a given time.  Once it collides
		// parameters are updated and then repushed onto the queue.
		// All events are not pushed onto the queue at first because a large
		// number of events would need the space to hold it.
		// This is wasteful (kinda) for a single particle case,
		// but allows for addition of other events (splitting)
		// Generating the first three decisions
		events = generatePrimaries(1,numSimulated,sourceLimits,energyPDF,energyGroups);
		while (! isEmpty(events)){
			e = (event_t*) pop(events);
#if DEBUG
			printEvent(e);
#endif

			// Calcating the mean free path and distance
			mfp = log((double) rand()/(double) RAND_MAX);
			pos = e->x + mfp/totxs[e->E]*e->mu;

			/** Particle left the left boundary */
			if (pos < geometry[0]){
#if DEBUG
				fprintf(stdout,"Particle left the left boundary\n");
#endif
				// Recording what happened
				incrementTally(leftLeakage,e->E,e->w);
				incrementTally(collisions,e->E,e->numCollision);
				free(e);
			}
			/** Particle left the right boundary */
			else if( pos > geometry[1]){
#if DEBUG
				fprintf(stdout,"Particle left the right boundary\n");
#endif
				// Recording what happened
				incrementTally(rightLeakage,e->E,e->w);
				incrementTally(collisions,e->E,e->numCollision);
				free(e);
			}
			/** Collisions */
			else {
				// TODO: Implement variance reduction

				// Checking if it was an absorbition
				if( (double) rand() / (double) RAND_MAX > totscat[e->E]/totxs[e->E]){
					incrementTally(absorbitions,e->E,e->w);
					incrementTally(collisions,e->E,e->numCollision);
					free(e);
				}
				else{
					// Not an absorbition, so we simply pick a new energy group and collisison
					e->numCollision++;
					e->E = chooseEnergy(*scat,energyGroups);
					e->mu = chooseDirection();
					push(events,e);
				}

			}
		}
	}


	// Printing out results
	fprintf(stdout,"\nResult of simulating %2.2e particles (normalized per particle)\n",(double) numberParticles);
	fprintf(stdout,"Energy Group \t\t Left Leakage \t Right Leakage \t Flux \t\t Collsions \t Absorbitions \n");
	for (i=0; i<energyGroups; i++)
		fprintf(stdout,"%7d%20f%17f%15f%12f%15f\n",i,leftLeakage[i]/(double) numberParticles, rightLeakage[i]/(double) numberParticles,
				flux[i]/(double) numberParticles, collisions[i]/(double) numberParticles,absorbitions[i]/(double) numberParticles);

	// Cleanliness is next to godliness
	free(totscat);
	free(events);
	deleteTally(leftLeakage);
	deleteTally(rightLeakage);
	deleteTally(flux);
	deleteTally(collisions);
	deleteTally(absorbitions);

	// DONE!  TO THE BEACH!
	return EXIT_SUCCESS;
}
int i;
queue_t* generatePrimaries(int n,int eventID, double limits[],double *energyPDF, int energyGroups){
	queue_t *events = (queue_t*) Malloc(sizeof(queue_t));
	initilizeQueue(events);
	for(i=0; i<n; i++){
		event_t* e = (event_t*) Malloc(sizeof(event_t));
		
		// Setting EventID
		e->eventID = eventID;

		// Setting Weight
		e->w  = 1.0;
		
		// Need to Choose Position.  This is done uniformly on the interval defined in limits
		e->x = choosePosition(limits);

		// Need to Choose Direction.  This is done uniformly
		e->mu = chooseDirection();

		// Need to Choose Energy from two energy groups.  It is assumed that the groups are sorted.
		e->E = chooseEnergy(energyPDF, energyGroups);
		
		// Hasn't collided yet
		e->numCollision = 0;

		// Pushing the result
		push(events,e);
	}
	return events;
}

/**
 * @brief chooseEnergy - chooses an energy
 *
 * @param energyPDF - sorted pdf of the energy groups
 * @param energyGroups - number of energy groups
 * @return group number of energy group
 *
 * @bugs The choice of groups might break down.
 */
int j;
double r;
double lower;
int chooseEnergy(double *energyPDF, int energyGroups){
	int j;
	// Randomly pick a value. The energy group that is between that value is returned.
	double r = (double) rand()/(double) RAND_MAX;
	double lower = 0;
	for(j = 0; j<energyGroups-1; j++){
		if( r > lower && energyPDF[j] < r){
			return j;
		}
		lower = energyPDF[j];
	}
	return j;
}

/**
 * @brief choosePosition - picks a position uniformly on the inteval
 *
 * @param limits - a double array of limits, in increasing order
 * @return the starting x position
 */
double choosePosition(double limits[]){
	return (limits[1]-limits[0])*(double) rand()/(double) RAND_MAX -limits[0];
}

/**
 * @brief chooseDirection
 * 
 * Uniformly choses a direcition
 */
double chooseDirection(){
	return 2.0*( (double) rand()/(double) RAND_MAX) -1;
}


/**
 * @brief incrementTally - Increments a tally, accounting for the variance.
 *
 * @param tally - pointer to the tally to increment
 * @param value - value to add to the tally
 * @return pointer to a 2x1 double array of the tally and the varaiance
 */

#if DEBUG
/**
 * Prints Diagnositics of the event 
 */
void printEvent(event_t *e){
	fprintf(stdout,"Event %d:\t%p\n",e->eventID,e);
	fprintf(stdout,"--> Position: %f\n--> Energy: %d\n--> Direction: %f\n",e->x,e->E,e->mu);
	return;
}
#endif
