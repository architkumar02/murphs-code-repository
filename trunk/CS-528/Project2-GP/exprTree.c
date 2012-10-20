#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "exprTree.h"

void printNode(node *n){
    fprintf(stdout,"Node %p:\n\tname: %s\n\tright: %p\n\tLeft:%p\n",n,
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
        *tree = funcNode();
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

void writeTree(node **tree, char *filename){

    FILE* f = NULL;
    if ((f = fopen(filename,"w"))){
        fprintf(f,"strict digraph G{\n");
        writeTreeHelper(tree,f);
        fprintf(f,"}\n");
        fclose(f);
    }
    else{
        fprintf(stderr,"Could not open file %s\n",filename);
    }
}

void writeTreeHelper(node **tree, FILE* f){
    if (*tree == NULL)
        return;
    else{
        fprintf(f,"%p [label=\"%s\"];\n",(*tree),(*tree)->name);
        if( (*tree)->left){
            fprintf(f,"%p -> %p;\n",(*tree),(*tree)->left);
            writeTreeHelper(&(*tree)->left,f);
        }
        if ((*tree)->right)
            fprintf(f,"%p -> %p;\n",(*tree),(*tree)->right);
        writeTreeHelper(&(*tree)->right,f);
    }
}

double evalTree(node **tree, double x){

}

int main(int argc, char *argv[]){

    //char *TERMINALS[NUMTERMINALS] = {"0","-1","pi","x"};
    //char *FUNCTIONS[NUMFUNCTIONS] = {"+","-","*","/","^","cos","sin"};
    node *root = NULL;
    fprintf(stdout,"\n Building a tree\n");
    buildTree(&root,3,0.1);

    fprintf(stdout," Writing a tree\n");
    writeTree(&root,"tree.dot");

    fprintf(stdout,"Deleting a tree\n");
    deleteTree(&root);
    return EXIT_SUCCESS;
}
