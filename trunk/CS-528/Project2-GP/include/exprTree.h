/* $begin exprTree.h */
#ifndef __EXPRTREE_H__
#define __EXPRTREE_H__

#include <stdio.h>
#include <math.h>

/**
 * Node Functions
 */
#define NUMFUNCTIONS 8
extern char* FUNCTIONS[NUMFUNCTIONS];
extern char *FUNCTIONS[NUMFUNCTIONS] = {"+","-","*","/","^","cos","sin"};

/**
 * Terminal Functions 
 */
#define NUMTERMINALS 4 
extern char *TERMINALS[NUMTERMINALS] = {"0","-1","pi","x"};
/**
 * Node structure
 */
typedef struct node_t{
    char *name;
    struct node_t *right;
    struct node_t *left;
} node;

node* createNode(char* name);
node* leafNode();
node* funcNode();

void buildTree(node **tree, int depth,double pruneProb);
void deleteTree(node **tree);

double evalNode(node *n);
void printNode(node *n);

void writeTree(node **tree,char *filename);
void writeTreeHelper(node **tree, FILE *f);
#endif
/* $end exprTree.h */
