#include <stdio.h>
#include <math.h>

#include "exprTree.h"

struct node_t *createNode(char *name){
    struct node_t *node = NULL;
    if ( node = malloc(sizeof(node_t))){
        node->name = name;
        node->left = NULL;
        node-right = NULL;
        return node;
        }
        else
            return node;
}

struct node_t *leafNode(){
    char* TERMINALS;
    int choice = rand() % NUMTERMINALS;
    return createNode(TERMINALS[choice]);
}

struct node_t *funcNode(){
    char* FUNCTIONS;
    int choice = rand() % NUMFUNCTIONS;
    return createNode(FUNCTIONS[choice]);
}

void buildTree(node_t **tree, int depth, double pruneProb){
    

}

