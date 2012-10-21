#include <stdio.h>
#include "tree.h"
int isempty(struct stack *s){
    return (s->head == NULL) ? 1:0;
}

void push(struct stack *s, node *n){
    fprintf(stdout,"Adding a node to the stack\n");
    if (s->head == NULL){
        n->left = NULL;
    }
    n->left = s->head;
    s->head = n;
    fprintf(stdout,"Added a node to the stack\n");
}

node *pop(struct stack *s){
    node *n = NULL;
    if (s->head == NULL){
        fprintf(stderr,"Stack is empty\n");
    }
    n = s->head;
    s->head = n->left;
    return n;
}


void deleteTree(node *tree){
    if( tree == NULL)
        return;
    else{
        return(deleteTree(tree->left));
        return(deleteTree(tree->right));
        free(tree);
        tree = NULL;
        return;
    }
}

void printNode(node *n){
    fprintf(stdout,"Node %p:\n\tname: %s\n\tright: %p\n\tLeft:%p\n",n,
            n->name,n->left,n->right);
}

node *createNode(char *name){
    node *node = NULL;
    if ( (node = malloc(sizeof(struct node_t)))){
        node->name = name;
        node->left = NULL;
        node->right = NULL;
    }
    return node;
}

void writeTree(node *tree, char *filename){
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

void writeTreeHelper(node *tree, FILE* f){
    fprintf(f,"%d [label=\"%s\"];\n",(long int)tree,tree->name);
    if( tree->left){
        fprintf(f,"%d -> %d;\n",(long int)tree->left,(long int)tree);
        writeTreeHelper(tree->left,f);
    }
    if (tree->right){
        fprintf(f,"%d -> %d;\n",(long int)tree->right,(long int)tree);
        writeTreeHelper(tree->right,f);
    }
}
