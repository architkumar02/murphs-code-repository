/** @file tree.c
 * @brief tree and stack implementation
 *
 * @author Matthew J. Urffer
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
        deleteTree(tree->left);
        deleteTree(tree->right);
        free(tree);
        tree = NULL;
    }
}

/**
 * @breif compareTree
 * @param Tree one and tree two to compare
 * @return 0 if they are not structurally identical
 */
int compareTree(node* t1, node *t2){
    // Both Empty -> True
    if ( t1 == NULL && t2 == NULL) {
        return 1 ;
    }
    else if( t1!= NULL && t2 != NULL){
        return ( strcmp(t1->name,t2->name) &&
                compareTree(t1->left,t2->left) &&
                compareTree(t1->right,t2->right));
    }
    // One tree is empty, other is not
    else return 0;
}

/**
 * @brief prints a node
 */
void printNode(node *n){
    fprintf(stdout,"Node %p:\n\tname: %s\n\tdepth: %d\n\tright: %p\n\tLeft:%p\n",n,n->name,n->depth,n->left,n->right);
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
    fprintf(f,"%ld [label=\"%s\"];\n",(long int)tree,tree->name);
    if( tree->left){
        fprintf(f,"%ld -> %ld;\n",(long int)tree->left,(long int)tree);
        writeTreeHelper(tree->left,f);
    }
    if (tree->right){
        fprintf(f,"%ld -> %ld;\n",(long int)tree->right,(long int)tree);
        writeTreeHelper(tree->right,f);
    }
}
