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
        n->parent = NULL;
    }
    n->parent = s->head;
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
    s->head = n->parent;
    return n;
}

void printStack(struct stack *s){
    node *n = NULL;
    n = s->head;
    while (n){
        fprintf(stdout,"[%p (%s)] ->",n,n->name);
        n = n->parent;
    }
    fprintf(stdout,"\n");
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
 * @brief Determines if the nodes is the left child
 * @param n - node in question
 * @param p - parent node
 * @return 1 if the child is the left node, 0 otherwise
 */
int isLeftChild(node *n, node *p){
    if (p->left == n) {return 1;}
    else {return 0;}
}

/**
 * @brief Determines if the nodes is the right child
 * @param n - node in question
 * @param p - parent node
 * @return 1 if the child is the right node, 0 otherwise
 */
int isRightChild(node *n, node *p){
    if (p->right == n) {return 1;}
    else {return 0;}
}

/**
 * @brief Determines if the node is the head node
 * @param n - the node in question
 * @return 1 if the node is the head node, 0 otherwise
 */
int isHead(node *n){
    if (n->parent == NULL){return 1;}
    else {return 0;}
}
/**
 * @brief Determines if the node is a parent
 * @param n - node in question
 * @return 1 if the node has two children, 0 otherwiese
 */
int isParent(node *p){
    if (p == NULL) {return 0;}
    else if (p->left && p->right){return 1;}
    else {return 0;}
}
/**
 * @brief isequal
 * @param Tree one and tree two to compare
 * @return 0 if they are not structurally identical
 */
int isequal(node* t1, node *t2){
    // Both Empty -> True
    if ( t1 == NULL && t2 == NULL) {
        return 1 ;
    }
    else if( t1!= NULL && t2 != NULL){
        return ( strcmp(t1->name,t2->name)==0 &&
                isequal(t1->left,t2->left) &&
                isequal(t1->right,t2->right));
    }
    // One tree is empty, other is not
    else return 0;
}

/**
 * @brief copy 
 * @param tree to copy
 * @return new tree
 */
node *copy(node *tree){
    node *newroot;
    if (tree != NULL){
        newroot = createNode(tree->name);
        newroot->left = copy(tree->left);
        newroot->right = copy(tree->right);
    }else if (tree == NULL) return NULL;
    return newroot;


}
/**
 * @brief prints a node
 */
void printNode(node *n){
    fprintf(stdout,"Node %p:\n\tname: %s\n\tparent: %p\n\tright: %p\n\tLeft:%p\n",n,n->name,n->parent,n->left,n->right);
}

/**
 * @brief Creates a node
 * @return the created node, or NULL is malloc failed
 */
node *createNode(char *name){
    node *node = malloc(sizeof(struct node_t));
    if (node){
        node->name = name;
        node->parent = NULL;
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
 * @brief writes the nodes of the tree to a file
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
