/**
 * @file genetic
 * @brief Driver for genetic
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <float.h>
#include "exprTree.h"
#include "tree.h"

#define NUMPOINTS 101
#define MAXPOP 1000
/**
 * @brief counts the number of times the tree occurs
 *      in the forest.
 * @param forest    - the forest of trees
 * @param n         - the tree in question
 * @param numTrees  - number of trees in the forest
 * @return number of times the tree is in the forest,
 *      or 0 if it is unique
 */
int uniqueTree(node **forest, node *n, int numTrees){
    int unique = 0;
    int tree;
    for (tree = 0; tree < numTrees; tree++){
        if (isequal(n,forest[tree])==1){
            unique ++;
        }
    }
    return unique;
}
/**
 * @brief Creates a forest of trees
 * @param numTrees  - number of trees in the forest
 * @param maxDepth  - maximum depth of the tree
 * @param pruneFraction - fraction of branches to prune
 */
void createForest(node *forest[],int numTrees,int maxDepth, double pruneFraction){
    int tree;
    node *canidate;
    if (!forest){
        fprintf(stderr,"Could not create a forest of %d trees\n",numTrees);
        exit(EXIT_FAILURE);
    }
    else {
        for(tree = 0; tree < numTrees; tree++){
            canidate = buildTree(NULL,maxDepth,pruneFraction);
               while (uniqueTree(forest,canidate,tree-1)!=0){
               deleteTree(canidate);
               canidate = buildTree(maxDepth,pruneFraction);
               }
            forest[tree] = canidate;
       }
    }
}
/**
 * @brief deletes a forest of trees
 * @param forest    - the forest of trees
 * @param numTrees  - number of trees in the forest
 */
void deleteForest(node *forest[], int numTrees){
    int tree;
    for( tree = 0; tree < numTrees; tree++){
        deleteTree(forest[tree]);
    }
}
void importData(char *filename, double val[][2]){
    int lineNum = 0;
    FILE *f = fopen(filename,"r");
    char line[128];
    char *p;
    if (f){
        while( fgets(line,sizeof line,f) != NULL && lineNum < NUMPOINTS){
            val[lineNum][0] = strtod(line,&p);
            val[lineNum][1] = strtod(p,NULL);
            lineNum ++;
        }
    }
    else
        fprintf(stderr,"Could not open %s for data import\n",filename);
}
/**
 * @brief Computes the precentage of the population that is unique
 * @param forest    - the forest of trees
 * @param numTrees  - the number of trees in the population
 */
double diversity(node *forest[], int numTrees){
    int t1;
    int t2;
    int numSame = 0;
    unsigned short equal[MAXPOP];
    for (t1 = 0; t1 < MAXPOP; t1 ++)
        equal[t1] = 0;
    for( t1 = 0; t1 < numTrees-1; t1++){
        for (t2 = t1+1; t2 < numTrees; t2++)
            if (isequal(forest[t2],forest[t1])){
                equal[t1] = 1; 
                equal[t2] = 1;
            }
    }
    for (t1 = 0; t1 < numTrees; t1++){
        if (equal[t1] == 0)
            numSame ++;
    }
    
    return ((double) numSame / (double) numTrees);
}
double sse(node *t, double val[][2]){
    double e = 0;
    double tVal;
    int i;
    for (i = 0; i < NUMPOINTS; i++){
        tVal = evalTree(t,val[i][0]);
        e += pow(abs(val[i][1]-tVal),2);
    }
    return e;
}
double SSE(node *forest[], int numTrees,double val[][2],double *e){
    double tempSSE;
    int tree;
    e[0] = 0;     // Worst (max)
    e[1] = 0;     // Mean 
    e[2] = DBL_MAX;     // Best (min)

    for( tree = 0; tree < numTrees; tree++){
        
        tempSSE = sse(forest[tree],val);
        e[1] += tempSSE;
        if ( tempSSE < e[2]){
            e[2] = tempSSE;
            writeTree(forest[tree],"bestTree.dot");
        }
        else if ( tempSSE > e[0])
            e[0] = tempSSE;

    }
    e[1] = e[1] / (double) numTrees;
    return e[1];
}
void mutatePop(node *forest[], int numTrees,double mR){
    int tree = 0;
    for (tree = 0; tree < numTrees; tree++)
        mutate(forest[tree],mR);
}
void crossOver(node *forest[], int numTrees, double sR){
    int tree = 0;
    for (tree = 0; tree < floor(numTrees/2); tree++){
        swap(forest[tree],forest[numTrees-tree],sR);
    }
}
void splash(FILE *f){
    fprintf(f,"\n");
    fprintf(f,"----------------------------------------------------------\n");
    fprintf(f,"-                        ---                             -\n");
    fprintf(f,"-           --          /   |                   --       -\n");
    fprintf(f,"-          /  >        |    =                  |   ?     -\n");
    fprintf(f,"-         /   /|       |    |                 |     |    -\n");
    fprintf(f,"-        /    |        /    |                 |     |    -\n");
    fprintf(f,"-       /      |      /     |                 |=====|    -\n");
    fprintf(f,"-      /   ||   |     |      |                |=====|    -\n");
    fprintf(f,"----------------------------------------------------------\n");
    fprintf(f,"Symbolic Regression with Genetic Programing\n");
    fprintf(f,"\tMatthew J. Urffer (matthew.urffer@gmail.com)\n\n");
}
int main(int argc, char *argv[]){

    int populationSize = 500;
    int treeDepth = 10;
    int maxGenerations = 10;
    double mseGoal = 0.5;
    double pruneFactor = 0.8;
    double mutationRate = 0.1;
    double swapRate = 0.1;

    /* Processing Command Line Inputs */
    int i = 1;
    for (i = 1;  i < argc; i++){
        if (strcmp(argv[i],"--maxDepth")==0){
            sscanf(argv[++i],"%d",&treeDepth);
        }
        else if (strcmp(argv[i],"--popSize")==0){
            populationSize = atoi(argv[++i]);
        }
        else if (strcmp(argv[i],"--pruneFactor")==0){
            pruneFactor = (double)atof(argv[++i]);
        }
        else if (strcmp(argv[i],"--mseGoal")==0){
            mseGoal = (double)atof(argv[++i]);
        }
        else if (strcmp(argv[i],"--mutationRate")==0){
            mutationRate = (double)atof(argv[++i]);
        }
        else if (strcmp(argv[i],"--swapRate")==0){
            swapRate = (double)atof(argv[++i]);
        }
        else if (strcmp(argv[i],"--maxGen")==0){
            maxGenerations = atoi(argv[++i]);
        }
        else if (strcmp(argv[i],"--help")==0){
            fprintf(stdout,"Usage: %s ",argv[0]);
            fprintf(stdout,"--maxDepth --pruneFactor # --popSize # --maxGen #");
            fprintf(stdout,"--mseGoal # --mutationRate # --swapRate #\n");
            exit(EXIT_SUCCESS);
        }
    }
    /* Checking Input Arguments */
    if (populationSize > MAXPOP)
        populationSize = MAXPOP;
    if (mutationRate > 1)
        mutationRate = 0.1;
    if (swapRate > 1)
        swapRate = 0.1;

    /* Run Information */
    FILE *out = stdout;
    splash(out);
    fprintf(out,"Parameters:\n");
    fprintf(out,"\tPopulation Size: %d\n\tMax Tree Depth: %d\n",populationSize,treeDepth);
    fprintf(out,"\tPrune factor: %3.2f\n",pruneFactor);
    fprintf(out,"\tMSE Goal: %3.2f\n\tMax Generations: %d\n",mseGoal,maxGenerations);
    fprintf(out,"\tMutation Rate: %3.2f\n\tSwap Rate: %3.2f\n",mutationRate,swapRate);
    printSet();

    /* Reading in the data file */
    double val[NUMPOINTS][2];
    importData("proj2-data.dat",val);

    /* Creating the intitial population */
    srand( time(NULL));
    node *forest[MAXPOP];
    createForest(forest,populationSize,treeDepth,pruneFactor);


    /* Creating performance variables */
    double genDiv = 0;
    double genSSE[3] = {0,0,0};     // worst, avg, best

    /* Running Generations */
    int gen = 0;
    fprintf(out,"Generation\tDiversity\tMean SSE\tBest SSE\n");
    for(gen = 0; gen < maxGenerations; gen++){
        // Diversity and SSE
        genDiv = diversity(forest,populationSize);
        SSE(forest,populationSize,val,genSSE); 
        fprintf(out,"\t%d\t%3.2f\t\t%3.2e\t%3.2e\n",gen,genDiv,genSSE[1],genSSE[2]);
    
        // Genetic Operations
        mutatePop(forest,populationSize,mutationRate);
    //    crossOver(forest,populationSize,swapRate);
    }

    // Clean up, clean up, everybody do your share
    deleteForest(forest,populationSize);


    return EXIT_SUCCESS;
}
