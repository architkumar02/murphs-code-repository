/**
 * Monte Carlo Code for the Transport of a Two Group particle in a 1 D Slap
 * @author matthew.urffer@gmail.com
 * @bugs: 
 */
#include <cstdio>
#include <iostream>
#include <cmath>


/**
 * Material Variable Declarations
 * @variable totxs(ig)      - Total cross section in group ig, 2x1
 * @variable scat(ig,jg)    - Scatter cross ection for group ig to group jg
 *                              this is a 2x2 matrix
 * @variable totscat(ig)    - Total scattering cross section in group
 * @variable sour(ig)       - Source in group ig
 */
int ng = 2;
double totxs[2] = {0.1, 0.2};
double scat[2][2] = {0.05, 0.04, 0, 0.1};
double totscat[2];
double sour[2] = {4,6};

/**
 * Geometry Variable Declaritions
 * @variable width          - width of the slab
 */
double width=50;
int main(){ 
    // Total Scattering Cross Section for Each Group
    for(int ig=0; ig < ng; ig++){
        totscat[ig] = 0;
        for(int jg = 0; jg < ng; jg++){
            totscat[ig] += scat[ig][jg];
        }
    }

    // Convert source to CDF
    for(int ig=0; ig<ng; ig++)
        sour[ig] +=sour[ig-1];
}
/*
