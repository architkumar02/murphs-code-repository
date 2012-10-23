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
#include "genetic."

#define NUMPOINTS 101
#define MAXPOP 1000

struct breedingParam{
    double mutationRate;
    double swapRate;
    double touramentFraction;
    double rankFraction;
} ;

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
    int attempt = 0;
    int maxTries = 100;
    node *canidate;
    for(tree = 0; tree < numTrees; tree++){
        attempt = 0;
        canidate = buildTree(NULL,maxDepth,pruneFraction);
        while (uniqueTree(forest,canidate,tree-1)!=0 && attempt < maxTries){
            deleteTree(canidate);
            canidate = buildTree(NULL,maxDepth,pruneFraction);
            attempt ++;
        }
        forest[tree] = canidate;
    }
}

void rampedHalfHalf(node *forest[], int numTrees, int maxDepth, double pruneFraction){

    int tree;
    int attempt = 0;
    int maxTries = 100;
    int depth = 0;
    node *canidate;
    for(tree = 0; tree < numTrees/2; tree++){
        attempt = 0;
        canidate = buildTree(NULL,maxDepth,pruneFraction);
        while (uniqueTree(forest,canidate,tree-1)!=0 && attempt < maxTries){
            deleteTree(canidate);
            canidate = buildTree(NULL,maxDepth,1);
            attempt ++;
        }
        forest[tree] = canidate;
    }
    for( ;tree < numTrees; tree++){
        for (depth = 2; depth < maxDepth; depth++){
            attempt = 0;
            canidate = buildTree(NULL,maxDepth,pruneFraction);
            while (uniqueTree(forest,canidate,tree-1)!=0 && attempt < maxTries){
                deleteTree(canidate);
                canidate = buildTree(NULL,depth,pruneFraction);
                attempt ++;
            }
            forest[tree] = canidate;
        }
    }
}
void bestTreeSummary(FILE* out,char *filename,double val[][2]){

    FILE *bestTreeIn = fopen(filename,"r");
    char postfixExpr[512];
    node *bestTree = NULL;
    if( bestTreeIn){
        fgets(postfixExpr,512,bestTreeIn);
        bestTree = readPostfix(postfixExpr);
        fclose(bestTreeIn);
    }
    if (!bestTree){
        fprintf(stdout,"Could not open or convert %s to a tree\n",filename);
        return;
    }
    fprintf(out,"\nBestTree:\n");
    writePostfixHelper(bestTree,out);
    fprintf(out,"\n");
    fprintf(out," (x,y)  Tree(x)  |y-Tree(x)|\n");
    double e = 0;
    double tVal;
    int i;
    for (i = 0; i < NUMPOINTS; i++){
        tVal = evalTree(bestTree,val[i][0]);
        e += pow(abs(val[i][1]-tVal),2);
        fprintf(out,"% 5.3f \t % 5.3f % 5.3f % 5.3f\n",val[i][0],val[i][1],tVal,fabs(val[i][1]-tVal));
    }
    fprintf(out,"SSE: % 5.3e\n",e);
    deleteTree(bestTree);
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
    int eq[MAXPOP];
    for (t1 = 0; t1 < MAXPOP; t1 ++)
        eq[t1] = 0;
    for( t1 = 0; t1 < numTrees-1; t1++){
        for (t2 = t1+1; t2 < numTrees; t2++)
            if (isequal(forest[t2],forest[t1])){
                eq[t1] = 1; 
                eq[t2] = 1;
            }
    }
    for (t1 = 0; t1 < numTrees; t1++){
        if (eq[t1] == 0)
            numSame ++;
    }

    return ((double) numSame / (double) numTrees);
}
/**
 * @brief Computes the sse
 * @param tree
 * @param 2D array of values
 * @return the Sum Squared Error
 */
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
double SSE(node *forest[], int numTrees,double val[][2],
            double *e,double *sseError, char *bestTreeName){
    int tree;
    e[0] = 0;     // Worst (max)
    e[1] = 0;     // Mean 
    e[2] = DBL_MAX;     // Best (min)
    char buffer[128];
    for( tree = 0; tree < numTrees; tree++){

        sseError[tree] = sse(forest[tree],val);
        if (sseError[tree] <= 0.0){
            writeTree(forest[tree],"zeroTree.dot");
            writePostfix(forest[tree],"zeroTree.postfix");
            bestTreeSummary(stdout,"zeroTree.postfix", val);
            exit(EXIT_FAILURE);
        }
        e[1] += sseError[tree];
        if ( sseError[tree] < e[2]){
            e[2] = sseError[tree];
            
            sprintf(buffer,"%s.dot",bestTreeName);
            writeTree(forest[tree],buffer);
            sprintf(buffer,"%s.postfix",bestTreeName);
            writePostfix(forest[tree],buffer);
        }
        else if ( sseError[tree] > e[0])
            e[0] = sseError[tree];

    }
    e[1] = e[1] / (double) numTrees;
    return e[2];
}
void mutatePop(node *forest[], int numTrees,double mR){
    int tree = 0;
    for (tree = 0; tree < numTrees; tree++)
        mutate(forest[tree],mR);
}
void crossOver(node *forest[], int numTrees, double sR){
    int tree = 0;
    for (tree = 0; tree < floor(numTrees/2); tree++){
        swap(forest[tree],forest[numTrees-tree-1],sR);
    }
}

struct ssePoint {
    double sse;
    int index;
};


int compareSSEPoint(const void *a, const void *b){
    const struct ssePoint *a1 = (struct ssePoint *) a;
    const struct ssePoint *b1 = (struct ssePoint *) b;

    if (a1->sse > b1->sse) 
        return 1;
    else if (a1->sse < b1->sse) 
        return -1;
    else 
        return 0;
}
/*
   int compareSSEPoint(struct ssePoint *a, struct ssePoint *b){
   return (a->sse - b->sse);
   }
   */
void breedGeneration(node *forest[], int numTrees, double sseError[], struct breedingParam *param){

    struct ssePoint rankSSE[MAXPOP];
    node *newforest[MAXPOP];

    int tournamentNumber = floor(numTrees* param->touramentFraction);
    int rankNumber = floor(numTrees* param->rankFraction);

    node* temp;
    int tree = 0;
    int t1;
    int t2;
    int i;

    // Tournamnet Selection
    for (tree = 0; tree < tournamentNumber; tree++){
        t1 = rand() % (numTrees- tree) + tree;
        t2 = rand() % (numTrees - tree) + tree;
        if (sseError[t1] >= sseError[t2])
            // Keep t2
            newforest[tree] = copy(forest[t2]);
        else
            // Keep t1
            newforest[tree] = copy(forest[t1]);
    }

    // Rank Selection
    for (i = 0; i < numTrees; i++){
        rankSSE[i].sse = sseError[i];
        rankSSE[i].index = i;
    }
    qsort(rankSSE,numTrees,sizeof(struct ssePoint),compareSSEPoint);
    for (i = 0; i < rankNumber; i ++){
        if (i + tree > numTrees) break;
        newforest[tree+i] = copy(forest[rankSSE[i].index]);    
    }

    // Copying over the trees
    for (tree = 0; tree < numTrees; tree ++){
        temp = forest[tree];
        forest[tree] = newforest[tree];
        newforest[tree] = temp;
    }
    deleteForest(newforest,numTrees);

    // Mutation and cross over on the new generation
    mutatePop(forest,numTrees,param->mutationRate);
    crossOver(forest,numTrees,param->swapRate);

}
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

void usage(FILE *f, char *name){

    fprintf(f,"Usage: %s ",name);
    fprintf(f,"--maxDepth # --pruneFactor # --popSize # --maxGen # ");
    fprintf(f,"--sseGoal # --mutationRate # --swapRate # ");
    fprintf(f,"--tournamentFraction # --rankFraction # --bestTreeName name\n");

}
int main(int argc, char *argv[]){

    int populationSize = 500;
    int treeDepth = 15;
    int maxGenerations = 20;
    double sseGoal = 0.5;
    double pruneFactor = 0.25;
    double mutationRate = 0.80;
    double swapRate = 0.7;
    double tournamentFraction = 0.90;
    double rankFraction = 0.10;
    char bestTreeName[128];
    strcpy(bestTreeName,"bestTree");

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
            tournamentFraction = 1.0 - rankFraction;
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
    if (populationSize > MAXPOP)
        populationSize = MAXPOP;
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

    /* Run Information */
    FILE *out = stdout;
    splash(out);
    fprintf(out,"Parameters:\n");
    fprintf(out,"\tPopulation Size: %d\n\tMax Tree Depth: %d\n",populationSize,treeDepth);
    fprintf(out,"\tPrune factor: %3.2f\n",pruneFactor);
    fprintf(out,"\tSSE Goal: %3.2f\n\tMax Generations: %d\n",sseGoal,maxGenerations);
    fprintf(out,"\tMutation Rate: %3.2f\n\tSwap Rate: %3.2f\n",mutationRate,swapRate);
    printSet();

    /* Reading in the data file */
    double val[NUMPOINTS][2];
    importData("proj2-data.dat",val);

    /* Creating the intitial population */
    srand( time(NULL));
    node *forest[MAXPOP];
    //createForest(forest,populationSize,treeDepth,pruneFactor);
    rampedHalfHalf(forest,populationSize,treeDepth,pruneFactor);

    struct breedingParam genParam = {mutationRate,swapRate,tournamentFraction,rankFraction};

    /* Creating performance variables */
    double genDiv = 0;
    double genSSE[3] = {0,0,0};     // worst, avg, best
    double sseError[MAXPOP];

    /* Running Generations */
    int gen = 0;
    double bestSSE = DBL_MAX; 
    fprintf(out,"Generation\tDiversity\tMean SSE\tBest SSE\n");
    for(gen = 0; (gen < maxGenerations &&  bestSSE > sseGoal); gen++){
        // Diversity and SSE
        genDiv = diversity(forest,populationSize);
        bestSSE = SSE(forest,populationSize,val,genSSE,sseError,bestTreeName); 
        fprintf(out,"\t%d\t\t%3.2f\t\t%3.2e\t%3.2e\n",gen,genDiv,genSSE[1],genSSE[2]);

        // Genetic Operations
        breedGeneration(forest,populationSize,sseError,&genParam);
    }

    /* Looking at the performance of the best tree */
    char response[128] = "Y";
    // fprintf(stdout,"Do you want to print the performance of the best tree [y/n]: \n");
    // fscanf(stdin,"%s",response);
    if (strcmp(response,"y")==0 || strcmp(response,"Y")==0){
        bestTreeSummary(out,"bestTree.postfix",val);
    }
    // Clean up, clean up, everybody do your share
    deleteForest(forest,populationSize);

    return EXIT_SUCCESS;
}
