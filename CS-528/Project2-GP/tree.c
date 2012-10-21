/** @file tree.c
 * @brief tree and stack implementation
 *
 * @author Matthew J. Urffer
 */
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

/**
 * @brief  checks
 * @return
 */
int isempty(struct stack *s){
    return (s->head == NULL) ? 1:0;
}

/**
 * @brief pushes onto a stack LIFO
 * @param n the node to be added
 * @return
 */
void push(struct stack *s, node *n){
    if (s->head == NULL){
        n->left = NULL;
    }
    n->left = s->head;
    s->head = n;
}

/**
 * @brief Pops an item from the LIFO stack
 * @return pointer to the node that was the top of the stack
 */
node *pop(struct stack *s){
    node *n = NULL;
    if (s->head == NULL){
        fprintf(stderr,"Stack is empty\n");
    }
    n = s->head;
    s->head = n->left;
    return n;
}

/**
 * @brief Deletes a tree
 */
void deleteTree(node *tree){
    if( tree){
        return(deleteTree(tree->left));
        return(deleteTree(tree->right));
        free(tree->name);
        free(tree);
        tree = NULL;
    }
}

/**
 * @brief prints a node
 */
void printNode(node *n){
    fprintf(stdout,"Node %p:\n\tname: %s\n\tright: %p\n\tLeft:%p\n",n,
            n->name,n->left,n->right);
}

/**
 * @brief Creates a node
 * @return the created node, or NULL is malloc failed
 */
node *createNode(char *name){
    node *node = malloc(sizeof(struct node_t));
    if (node){
        node->name = name;
        node->left = NULL;
        node->right = NULL;
    }
    return node;
}

/**
 * @brief Writes a tree to the provided filename.
 * The format of the file is such that it can be read by Graphviz
 */
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

/**
 * @brief
 */
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
