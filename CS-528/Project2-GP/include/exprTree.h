/* $begin exprTree.h */
#ifndef __EXPRTREE_H__
#define __EXPRTREE_H__

#include <stdio.h>
#include <math.h>
#include "tree.h"


node* leafNode();
node* funcNode();

node* buildTree(int depth,double pruneProb);

double evalNode(node *n);
void printNode(node *n);
void StressTest(int maxDepth, int numItter);


void writePostfix(node *tree, char *filename);
void writePostfixHelper(node *tree, FILE *f);
node* readPostfix(char *filename);

void printSet(); 
#endif
/* $end exprTree.h */
