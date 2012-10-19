#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "exprTree.h"

void printNode(node *n){
    fprintf(stdout,"Node %X:\n\tname: %s\n\tright: %X\n\tLeft:%X\n",n,
        n->name,n->left,n->right);
}

node *createNode(char *name){
    node *node = NULL;
    if ( (node = malloc(sizeof(node)))){
        node->name = name;
        node->left = NULL;
        node->right = NULL;
        printNode(node);
        return node;
    }
    else
        return node;
}

node *leafNode(){
    int choice = (rand() % NUMTERMINALS);
    return createNode(TERMINALS[choice]);
}

node *funcNode(){
    int choice = rand() % NUMFUNCTIONS;
    return createNode(FUNCTIONS[choice]);
}

void buildTree(node **tree, int depth, double pruneProb){
    if (depth == 0){
        *tree = leafNode();
    }
    else if (  (rand() / (double) RAND_MAX) < pruneProb) {
        *tree = leafNode();
    }
    else{
        buildTree(&(*tree)->left,depth-1,pruneProb);
        buildTree(&(*tree)->right,depth-1,pruneProb);
    }

}

void deleteTree(node **tree){
    if( *tree == NULL)
        return;
    else{
        return(deleteTree(&(*tree)->left));
        return(deleteTree(&(*tree)->right));
        free(*tree);
        *tree = NULL;
        return;
    }
}


int main(int argc, char *argv[]){
    
    //char *TERMINALS[NUMTERMINALS] = {"0","-1","pi","x"};
    //char *FUNCTIONS[NUMFUNCTIONS] = {"+","-","*","/","^","cos","sin"};
    node *root = NULL;
    fprintf(stdout,"\n Building a tree\n");
    buildTree(&root,3,0.1);
    fprintf(stdout,"Deleting a tree\n");
    deleteTree(&root);
    return EXIT_SUCCESS;
}
