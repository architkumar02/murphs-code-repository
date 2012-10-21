/**
 * @file genetic
 * @brief Driver for genetic
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "exprTree.h"
#include "tree.h"

#define NUMPOINTS 101

void createForest(node **forest,int numTrees,int maxDepth, double pruneFraction){
    int tree;
    forest = (node *)malloc(numTrees*sizeof(node *));
    if (!forest){
        fprintf(stderr,"Could not create a forest of %d trees\n",numTrees);
        exit(EXIT_FAILURE);
    }
    else {
        for(tree = 0; tree < numTrees; tree++){
            forest[tree] = buildTree(maxDepth,pruneFraction);
        }
    }
}
void deleteForest(node **forest, int numTrees){
    int tree;
    for( tree = 0; tree < numTrees; tree++){
        deleteTree(forest[tree]);
    }
    free(forest);
}
void importData(char *filename, double val[][2]){
    int lineNum = 0;
    FILE *f = fopen(filename,"r");
    char line[128];
    char *p;
    if (f){
        while( fgets(line,sizeof line,f) != NULL && lineNum < NUMPOINTS){
            val[lineNum][0] = strtod(line,&p);
            val[lineNum][0] = strtod(p,NULL);
            lineNum ++;
        }
    }
    else
        fprintf(stderr,"Could not open %s for data import\n",filename);
}
void splash(){
    fprintf(stdout,"\n");
    fprintf(stdout,"----------------------------------------------------------\n");
    fprintf(stdout,"-                        ---                             -\n");
    fprintf(stdout,"-           --          /   |                   --       -\n");
    fprintf(stdout,"-          /  >        |    =                  |   ?     -\n");
    fprintf(stdout,"-         /   /|       |    |                 |     |    -\n");
    fprintf(stdout,"-        /    |        /    |                 |     |    -\n");
    fprintf(stdout,"-       /      |      /     |                 |=====|    -\n");
    fprintf(stdout,"-      /   ||   |     |      |                |=====|    -\n");
    fprintf(stdout,"----------------------------------------------------------\n");
    fprintf(stdout,"Symbolic Regression with Genetic Programing\n");
    fprintf(stdout,"\tMatthew J. Urffer (matthew.urffer@gmail.com)\n\n");
}

int main(int argc, char *argv[]){

    int populationSize = 500;
    int treeDepth = 10;
    int maxGenerations = 10;
    double mseGoal = 0.5;
    double pruneFactor = 0.5;

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
    splash();
    fprintf(stdout,"Parameters:\n");
    fprintf(stdout,"\tPopulation Size: %d\n\tMax Tree Depth: %d\n",populationSize,treeDepth);
    fprintf(stdout,"\tPrune factor: %3.2f\n",pruneFactor);
    fprintf(stdout,"\tMSE Goal: %3.2f\n\tMax Generations: %d\n",mseGoal,maxGenerations);

    /* Reading in the data file */
    double val[NUMPOINTS][2];
    importData("proj2-data.dat",val);

    /* Creating the intitial population */
    node **forest;
    createForest(forest,populationSize,treeDepth,pruneFactor);
    deleteForest(forest,populationSize);
    return EXIT_SUCCESS;
}
