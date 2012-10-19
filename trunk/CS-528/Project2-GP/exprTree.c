#include <stdio.h>
#include <stdlib.h>
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
    if (depth == 0){
        *tree = leafNode();
    }
    else if ( (1.0 - (rand() / (double) RAND_MAX)) < pruneProb) {
        *tree = leafNode();
    }
    else{
        buildTree(&(*tree)-left,depth-1,pruneProp);
        buildTree(&(*tree)-right,depth-1,pruneProp);
    }

}

void deleteTree(node_t **tree){
    if( *tree == NULL)
        return;
    else{
        return(deleteTree((*node)->left));
        return(deleteTree((*node)->right));
        free(*node);
        *node = NULL:
    }
}


int main(int argc, char *argv[]){
    root = NULL;
    fprintf(stdout,"\n Building a tree\n");
    buildTree(root);
    fprintf(stdout,"Deleting a tree\n");
    deleteTree(root);
}
