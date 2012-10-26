/**
 * @file geneticDriver
 * @brief Driver for genetic
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <float.h>
#include "genetic.h"

#define MAXPOP 10000

void splash(FILE *f){
    fprintf(f,"\n");
    fprintf(f,"----------------------------------------------------------\n");
    fprintf(f,"                                                          \n");
    fprintf(f,"                                                    F FN  \n");
    fprintf(f,"                                         F 5       F U  55\n");
    fprintf(f,"                               F     F 5F  F     FFqBBMG05\n");
    fprintf(f,"                     F       FF1     N05 BBM F   F U8BBBMO\n");
    fprintf(f,"                  FF1       5 ZE     NN NBBB FF    1GMBB0 \n");
    fprintf(f,"                 F50Z5 F   1FMBBOF   F 5M20G  FF  5NBEOM 2\n");
    fprintf(f,"                FFEBBB    5 M NM      5NBOO 8Bq   U8BBBBF5\n");
    fprintf(f,"               1qZN MBZF F5OBME2F    F5Z8EB OZ   5 BBBBB 2\n");
    fprintf(f,"       5 F   F5MG M125   2BGMNF5     2MMM8MMF5   FBBBBBO 5\n");
    fprintf(f,"     UFF0 55  FEBMZGGF F  5BOGGN F   F5ZBNqB 5   1FBBZ GGG\n");
    fprintf(f,"BNF5FGBN5BBNF FBBBBB 8E   5BMZO8q     2BBG 02     5BBB 1uq\n");
    fprintf(f,"B 5q0BBBZqOq F1BBM5 51OZ 5 BB 5UE0     UBZN2     2ZBBBBEUU\n");
    fprintf(f,"B OBBBBBG12 F 1BBO5 2F5   UEBBG2EBF   51BBBNU     JBBBBBG2\n");
    fprintf(f,"BBBOG0BBB1 F  5 B qBB0 F 1uMB8B0FF   F2 BBBB F F U BB2 BBM\n");
    fprintf(f,"B0MBB 0BB1F FFUUGBuNBM1 F ZB0j0OF   1 OBM 80    U8BBE5jNB8\n");
    fprintf(f,"BUZBMZEFMNFF NMBBGFGB2  18BZ1UOB 5   OBq2UBEFF 1 BBq21 UBB\n");
    fprintf(f,"BOG 5B 56BF 1BB0  UMB 5  BNUF FBM51 B8UFF5NBUF5FBB5UF 52MB\n");
    fprintf(f,"BB 2 MZNZB 5 EqU 5 qM EFZGMq F5EBNZ  0 F 5 BMEZ BBB    2EB\n"); 
    fprintf(f,"----------------------------------------------------------\n");
    fprintf(f,"Symbolic Regression with Genetic Programing\n");
    fprintf(f,"\tMatthew J. Urffer (matthew.urffer@gmail.com)\n\n");
}
void test(){
    node *t1 = buildTree(NULL,3,0,0.5);
    node *t2 = buildTree(NULL,3,0,0.5);
    writeTree(t1,"TreeOne.dot");
    writeTree(t2,"TreeTwo.dot");
    /*
    mutate(t1,0.8);
    mutate(t2,0.8);
    writeTree(t1,"TreeOneMutate.dot");
    writeTree(t2,"TreeTwoMutate.dot");
    */
    
    swap(t1,t2,1);
    writeTree(t1,"TreeOneSwap.dot");
    writeTree(t2,"TreeTwoSwap.dot");

    deleteTree(t1);
    deleteTree(t2);
}
void usage(FILE *f, char *name){

    fprintf(f,"Usage: %s ",name);
    fprintf(f,"--maxDepth # --pruneFactor # --constProb # --popSize # --maxGen # ");
    fprintf(f,"--sseGoal # --mutationRate # --swapRate # ");
    fprintf(f,"--tournamentFraction # --rankFraction # --bestTreeName name\n");
    fprintf(f,"\nIf input arguments are specified then it is assumed the user wants to train and test\n");
}
int main(int argc, char *argv[]){

    /* Itteration Variables */
    int i = 1;
    int gen = 0;
    double bestSSE = DBL_MAX; 
    /* Creating performance variables */
    double genDiv = 0;
    double genSSE[3] = {0,0,0};     /* worst, avg, best */
    double sseError[MAXPOP];
    double val[NUMPOINTS][2];
    /* Genetic Paramenters */
    int populationSize = 500;           /* Population size (constant) */
    int treeDepth = 15;                 /* Maximum Tree Depth */
    int maxGenerations = 50;            /* Maximum Number of generations */
    double sseGoal = 0.5;               /* SSE error goal (for spartan) */
    double pruneFactor = 0.0;          /* Probability that a branch will be prunned */
    double constProb = 0.60;            /* Probability that a leaf will be a randomly choose (0,1) constant */
    double mutationRate = 0.20;         /* Mutation Rate (probability that a node will be mutated */
    double swapRate = 0.7;              /* Probability that crossover will occur */
    double tournamentFraction = 0.40;   /* fraction of individuals selected by tournament */
    double rankFraction = 0.50;         /* fraction of individual selected by rank */
    double spartanFraction = 0.1;        /* fraction of individuals selected by rank, copied as is */
    struct geneticParam genParam;       /* compat representation of generation */
    node *forest[MAXPOP];               /* Forest of trees */
    node *bestTree;
    /* Output Variables */
    int train = 1;
    int performance = 1;
    double evalPoint = 0;
    FILE *out = stdout;
    char bestTreeName[128];
    char response[128] = "Y";
    strcpy(bestTreeName,"bestTree");

    /* Processing Command Line Inputs */
    for (i = 1;  i < argc; i++){
        if (strcmp(argv[i],"--maxDepth")==0){
            sscanf(argv[++i],"%d",&treeDepth);
        }
        else if (strcmp(argv[i],"--test")==0){
            test();
            return EXIT_SUCCESS;
        }
        else if (strcmp(argv[i],"--popSize")==0){
            populationSize = atoi(argv[++i]);
        }
        else if (strcmp(argv[i],"--pruneFactor")==0){
            pruneFactor = (double)atof(argv[++i]);
        }
        else if (strcmp(argv[i],"--constProb")==0){
            constProb = (double)atof(argv[++i]);
        }
        else if (strcmp(argv[i],"--sseGoal")==0){
            sseGoal = (double)atof(argv[++i]);
        }
        else if (strcmp(argv[i],"--mutationRate")==0){
            mutationRate = (double)atof(argv[++i]);
        }
        else if (strcmp(argv[i],"--swapRate")==0){
            swapRate = (double)atof(argv[++i]);
        }
        else if (strcmp(argv[i],"--tournamentFraction")==0){
            tournamentFraction = (double) atof(argv[++i]);
            rankFraction = 1.0 - tournamentFraction;
        }
        else if (strcmp(argv[i],"--rankFraction")==0){
            rankFraction = (double) atof(argv[++i]);
        }
        else if (strcmp(argv[i],"--spartanFraction")==0){
            spartanFraction = (double) atof(argv[++i]);
        }
        else if (strcmp(argv[i],"--maxGen")==0){
            maxGenerations = atoi(argv[++i]);
        }
        else if(strcmp(argv[i],"--bestTreeName")==0){
            strcpy(bestTreeName,argv[++i]);
        }
        else if (strcmp(argv[i],"--help")==0){
            usage(stdout,argv[0]);
            exit(EXIT_SUCCESS);
        }
        else{
            fprintf(stderr,"Argument %s is not reconized\n",argv[i]);
            usage(stderr,argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    /* Checking Input Arguments */
    if (populationSize > MAXPOP){
        fprintf(stderr,"population size is set to the max at %d. Change define for larger population\n",MAXPOP);
        populationSize = MAXPOP;
    }
    if (mutationRate > 1)
        mutationRate = 0.1;
    if (swapRate > 1)
        swapRate = 0.1;
    if (tournamentFraction + rankFraction > 1.0){
        fprintf(stderr,"Tournament Fraction %5.3f and rank fraction %5.3f are greater than 1.0\n",
                tournamentFraction,rankFraction);
        fprintf(stderr,"Both are set to default values\n");
        tournamentFraction = 0.9;
        rankFraction = 1.0 - tournamentFraction;
    }
    genParam.mutationRate = mutationRate;
    genParam.swapRate = swapRate;
    genParam.touramentFraction = tournamentFraction;
    genParam.rankFraction = rankFraction;
    genParam.spartanFraction = spartanFraction;
    genParam.constProb = constProb;
    genParam.maxDepth = treeDepth;
    genParam.pruneFraction = pruneFactor;
    splash(out);

    /* Train or Run? */
    if (argc <= 1){
        fprintf(stdout,"Preform symbolic regression with genetic programming [y/n]:\n");
        fscanf(stdin,"%s",response);
        if (strcmp(response,"y")==0 || strcmp(response,"Y")==0)
            train = 1;
        else
            train = 0;

        fprintf(stdout,"Do you want to print the performance of the best tree [y/n]: \n");
        fscanf(stdin,"%s",response);
        if (strcmp(response,"y")==0 || strcmp(response,"Y")==0)
            performance = 1;
        else
            performance = 0;

    }
    /* Run Information */
    fprintf(out,"Parameters:\n");
    fprintf(out,"\tPopulation Size: %d\n\tMax Tree Depth: %d\n",populationSize,treeDepth);
    fprintf(out,"\tPrune factor: %3.2f\n\tConstant Probability: %3.2f\n",pruneFactor,constProb);
    fprintf(out,"\tSSE Goal: %3.2f\n\tMax Generations: %d\n",sseGoal,maxGenerations);
    fprintf(out,"\tSpartan Fraction: %3.2f\n",spartanFraction);
    fprintf(out,"\tTournament Fraction: %3.2f\n\tRank Fraction: %5.2f\n",tournamentFraction,rankFraction);
    fprintf(out,"\tMutation Rate: %3.2f\n\tSwap Rate: %3.2f\n",mutationRate,swapRate);
    printSet();

    /* Reading in the data file */
    importData("proj2-data.dat",val);

    /* Creating the intitial population */
    if (train){
        srand( time(NULL));
        fprintf(stdout,"Creating Intial Population\n");
        rampedHalfHalf(forest,populationSize,&genParam);

        /* Running Generations */
        fprintf(out,"Generation\tDiversity\tMean SSE\tBest SSE\n");
        while(gen < maxGenerations && bestSSE > sseGoal){

            /* Diversity and SSE */
            genDiv = diversity(forest,populationSize);
            bestSSE = SSE(forest,populationSize,val,genSSE,sseError,bestTreeName); 
            fprintf(out,"\t%d\t%3.2f\t\t%3.2e\t%3.2e\n",gen,genDiv,genSSE[1],genSSE[2]);

            /* Genetic Operations */
            breedGeneration(forest,populationSize,sseError,&genParam);
            gen++;
        }
        /* Diversity and SSE */
       genDiv = diversity(forest,populationSize);
       bestSSE = SSE(forest,populationSize,val,genSSE,sseError,bestTreeName); 
       fprintf(out,"\t%d\t%3.2f\t\t%3.2e\t%3.2e\n",gen,genDiv,genSSE[1],genSSE[2]);
        /* Clean up, clean up, everybody do your share */
        deleteForest(forest,populationSize);
    }
    /* Looking at the performance of the best tree */
    if (performance){
        sprintf(response,"%s.postfix",bestTreeName);
        bestTree = bestTreeSummary(out,response,val);
        if (argc <= 1){
        
        fprintf(stdout,"Enter value on which to test the tree [n to escape]: \n");
        while(fscanf(stdin,"%lf",&evalPoint)==1){
            fprintf(stdout,"Tree(%5.3f) = %5.3f\n",evalPoint,evalTree(bestTree,evalPoint));
        }
        deleteTree(bestTree);
    
        }
    }
    return EXIT_SUCCESS;
}
