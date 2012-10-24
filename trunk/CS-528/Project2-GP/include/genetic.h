#ifndef __GENETIC_H__
#define __GENETIC_H__
/**
 * @file genetic.h
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <float.h>
#include "exprTree.h"
#include "tree.h"

#define NUMPOINTS 101

struct geneticParam{
    double mutationRate;
    double swapRate;
    double touramentFraction;
    double rankFraction;
    int maxDepth;
    double pruneFraction;
    double constProb;
};

/**
 * @brief counts the number of times the tree occurs
 *      in the forest.
 * @param forest    - the forest of trees
 * @param n         - the tree in question
 * @param numTrees  - number of trees in the forest
 * @return number of times the tree is in the forest,
 *      or 0 if it is unique
 */
int uniqueTree(node **forest, node *n, int numTrees);
/**
 * @brief Creates a forest of trees
 * @param forest    - forest of trees
 * @param numTrees  - number of trees in the forest
 * @param geneticParma - pruning, maxDepth, and constant probiblity
 */
void createForest(node *forest[],int numTrees, struct geneticParam *param);
/**
 * @brief rampedHalfHalf
 * @param forest    - forest of trees
 * @param numTrees - number of trees in the population
 * @param geneticParma - pruning, maxDepth, and constant probiblity
 */
void rampedHalfHalf(node *forest[], int numTrees,  struct geneticParam *param);

/**
 * @brief Summerizes the best tree
 * @param out       - output file in which to write
 * @param filename  - filename of the postfix of the best tree
 * @param val       - (x,y) values on which to compare performance
 * @return          - pointer to the best tree
 */
node *bestTreeSummary(FILE* out,char *filename,double val[][2]);
/**
 * @brief deletes a forest of trees
 * @param forest    - the forest of trees
 * @param numTrees  - number of trees in the forest
 */
void deleteForest(node *forest[], int numTrees);
/**
 * @brief Imports data to read
 * @param filename  - name of the input file
 * @param val       - array of (x,y) values to fill
 * 
 * The number of data points are defined by NUMPOINTS
 */
void importData(char *filename, double val[][2]);
/**
 * @brief Computes the precentage of the population that is unique
 * @param forest    - the forest of trees
 * @param numTrees  - the number of trees in the population
 */
double diversity(node *forest[], int numTrees);
/**
 * @brief Computes the sse
 * @param tree
 * @param 2D array of values
 * @return the Sum Squared Error
 */
double sse(node *t, double val[][2]);
/**
 * @brif Computes the SSE of the forest
 * @param forest of trees
 * @param number of trees in forest
 * @param data set
 * @param data sutructre for the generational SSSE, where:
 *          e[0] = worst
 *          e[1] = mean
 *          e[2] = best
 * @param array to hold the sse per index
 * @param bestTreeName - name of the best tree
 * @return the mean SSE
 */
double SSE(node *forest[], int numTrees,double val[][2],double *e,double *sseError, char *bestTreeName);

/**
 * @brief Mutates the Population
 * @param forest    - forest of trees
 * @param numTrees  - number of trees in forest
 * @param mR        - mutation rate (0 < mR < 1)
 */
void mutatePop(node *forest[], int numTrees,double mR);
/**
 * @brief Preforms crossover on the Population
 * @param forest    - forest of trees
 * @param numTrees  - number of trees in forest
 * @param sR        - probability of branch swapping (0 < sR < 1)
 */
void crossOver(node *forest[], int numTrees, double sR);

/**
 * Helper Functions for ranking 
 */
struct ssePoint {
    double sse;
    int index;
};
int compareSSEPoint(const void *a, const void *b);

/**
 * @brief Breeds the next generation
 * @param forest    - forest of trees
 * @param numTrees  - number of trees in forest
 * @param sseError  - array of sse values, each index corrresponds to a tree
 * @param bredingParam - struct which contains breeding arguments
 *
 * The breeding is done in place; i.e. the population is overwritten with the new
 * Options for breeding include:
 *  - tournament selection
 *  - rank selection
 */
void breedGeneration(node *forest[], int numTrees, double sseError[], struct geneticParam *param);
#endif
