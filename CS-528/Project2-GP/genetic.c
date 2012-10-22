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
        if (!compareTree(n,forest[tree])){
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
void createForest(node **forest,int numTrees,int maxDepth, double pruneFraction){
    int tree;
    node *canidate;
    *forest = malloc(numTrees*sizeof(node *));
    if (!forest){
        fprintf(stderr,"Could not create a forest of %d trees\n",numTrees);
        exit(EXIT_FAILURE);
    }
    else {
        for(tree = 0; tree < numTrees; tree++){
            canidate = buildTree(maxDepth,pruneFraction);
            /*
            while (uniqueTree(forest,canidate,tree-1)!=0){
                deleteTree(canidate);
                canidate = buildTree(maxDepth,pruneFraction);
            }
            */
            forest[tree] = canidate;
           fprintf(stdout,"Added tree %d\n",tree);
        }
    }
}
/**
 * @brief deletes a forest of trees
 * @param forest    - the forest of trees
 * @param numTrees  - number of trees in the forest
 */
void deleteForest(node **forest, int numTrees){
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
double diversity(node **forest, int numTrees){
    int t1,t2;
    double numDiverse = 0;
    for( t1 = 0; t1 < numTrees; t1++){
        for (t2 = t1+1; t2 < numTrees; t2++)
            if (!compareTree(forest[t1],forest[t2])){
                numDiverse += 1;
            }
    }
    return numDiverse / (double) numTrees;
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
double SSE(node **forest, int numTrees,double val[][2],double *e){
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

    int populationSize = 10;
    int treeDepth = 13;
    int maxGenerations = 1;
    double mseGoal = 0.5;
    double pruneFactor = 0.1;

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
        else if (strcmp(argv[i],"--maxGen")==0){
            maxGenerations = atoi(argv[++i]);
        }
        else if (strcmp(argv[i],"--help")==0){
            fprintf(stderr,"Usage: %s",argv[0]);
            fprintf(stderr,"--maxDepth --pruneFactor # --popSize # --mseGoal # --maxGen #\n");
        }
    }
    /* Run Information */
    FILE *out = stdout;
    splash(out);
    fprintf(out,"Parameters:\n");
    fprintf(out,"\tPopulation Size: %d\n\tMax Tree Depth: %d\n",populationSize,treeDepth);
    fprintf(out,"\tPrune factor: %3.2f\n",pruneFactor);
    fprintf(out,"\tMSE Goal: %3.2f\n\tMax Generations: %d\n",mseGoal,maxGenerations);
    printSet();

    /* Reading in the data file */
    double val[NUMPOINTS][2];
    importData("proj2-data.dat",val);

    /* Creating the intitial population */
    srand( time(NULL));
    node *forest;
    createForest(&forest,populationSize,treeDepth,pruneFactor);

    /* Creating performance variables */
    double genDiv = 0;
    double genSSE[3] = {0,0,0};     // worst, avg, best

    /* Running Generations */
    int gen = 0;
    fprintf(out,"Generation\tDiversity\tMean SSE\tBest SSE\n");
    for(gen = 0; gen < maxGenerations; gen++){
        // Diversity and sse
        genDiv = diversity(&forest,populationSize);
        SSE(&forest,populationSize,val,genSSE); 
        fprintf(out,"\t%d\t%3.2f\t\t%3.3f\t\t%3.2f\n",gen,genDiv,genSSE[1],genSSE[2]);
    }

    // Clean up, clean up, everybody do your share
    deleteForest(&forest,populationSize);

    //free(forest);

    return EXIT_SUCCESS;
}
