/**
 * @file genetic
 * @brief Driver for genetic
 */

#include <stdio.h>
#include "exprTree.h"

void createForest(node && forest,int numTrees)
void importData(char *filename, double *val, double *numPoints){

    FILE *f = fopen("proj2-data.dat","r");
    if (f){


    }
    else
        fprintf(stderr,"Could not open %s for data import\n");


}

int main(int argc, char *argv[]){

    /* Reading in the data file */
    double val[][];
    double numPoints;
    importData("proj2-data.dat",&val,&numPoints);
    return EXIT_SUCCESS;
}
