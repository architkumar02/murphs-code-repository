/* $begin exprTree.h */
#ifndef __EXPRTREE_H__
#define __EXPRTREE_H__

#include <stdio.h>
#include <math.h>
#include "tree.h"

void printSet(); 
node* leafNode();
node* funcNode();

node* buildTree(node* parent,int depth,double pruneProb);

/**
 * Evaluation and Write
 */
double evalTree(node *n, double x);
void writePostfix(node *tree, char *filename);
void writePostfixHelper(node *tree, FILE *f);
node* readPostfix(char *filename);

/**
 * Genetic Methods
 */
void swap(const node *t1,const node *t2, double swapProp);
void mutate(node *tree,double mutateRate);
node* choosSubTree(node *tree, double p);
node* chooseParent(const node *tree, double p);

#endif
/* $end exprTree.h */
