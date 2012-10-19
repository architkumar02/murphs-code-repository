/* $begin exprTree.h */
#ifndef __EXPRTREE_H__
#define __EXPRTREE_H__

#include <math.h>
#include "nodeSet.h"

/**
 * Node structure
 */
struct node_t {
    char *name;
    struct node *right;
    struct node *left;
}

struct node_t* createNode(char* name);
struct node_t* leafNode();
struct node_t* funcNode();

void buildTree(node_t **tree, int depth,double pruneProb);
void deleteTree(node_t **tree);

double evalNode(node_t *n);

#endif
/* $end exprTree.h */
