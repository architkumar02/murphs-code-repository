#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "tree.h"
#include "exprTree.h"

/**
 * Node Functions
 */
#define NUMFUNCTIONS 7
char *FUNCTIONS[NUMFUNCTIONS] = {"+","-","*","/","^","cos","sin"};

/**
 * Terminal Functions 
 */
#define NUMTERMINALS 4 
char *TERMINALS[NUMTERMINALS] = {"0","-1","pi","x"};

node *leafNode(){
    int choice = (rand() % NUMTERMINALS);
    return createNode(TERMINALS[choice]);
}

node *funcNode(){
    int choice = rand() % NUMFUNCTIONS;
    return createNode(FUNCTIONS[choice]);
}

void printSet(){
    int i;
    fprintf(stdout,"TERMINAL Set:\n");
    for (i = 0; i < NUMTERMINALS; i++)
        fprintf(stdout,"\t%s",TERMINALS[i]);

    fprintf(stdout,"\n\nFUCNTION Set:\n");
    for (i = 0; i < NUMFUNCTIONS; i++)
        fprintf(stdout,"\t%s",FUNCTIONS[i]);
    fprintf(stdout,"\n\n");
}

node *buildTree(int depth, double pruneProb){
    node *tree = NULL;
    if (depth == 0 ||(rand() / (double) RAND_MAX) < pruneProb) {
        tree = leafNode();
        tree->depth = depth;
        return tree;
    }
    else{
        node *tree = funcNode();
        tree->depth = depth;
        tree->left = buildTree(depth-1,pruneProb);
        tree->right= buildTree(depth-1,pruneProb);
        return tree;
    }
}

void writePostfix(node *tree, char *filename){
    FILE *f = NULL;
    if (!filename){
        writePostfixHelper(tree,stdout);
    }
    else if ((f = fopen(filename,"w"))){
        writePostfixHelper(tree,f);
        fclose(f);
    }
    else
        fprintf(stderr,"Could not open file %s\n",filename);
}

void writePostfixHelper(node *tree, FILE *f){
    if (tree){
        writePostfixHelper(tree->left,f);
        writePostfixHelper(tree->right,f);
        fprintf(f," %s ",tree->name);
    }
}
node* readExpr(char *postfix){
    struct stack s;
    node *newnode;
    char *p;
    int i;
    p = strtok(postfix," ");
    while (p != NULL){
        // Checking is it is a known terminal
        for (i = 0; i < NUMTERMINALS; i++){
            if (strcmp(p,TERMINALS[i]) == 0){
                newnode = createNode(TERMINALS[i]);
                push(&s,newnode);
                p = strtok(NULL," "); 
                break;
            }
        }
        // Checking if it a known function
        for (i = 0; i < NUMFUNCTIONS; i++){
            if (strcmp(p,FUNCTIONS[i]) == 0){
                newnode = createNode(FUNCTIONS[i]);
                newnode->right = pop(&s);
                newnode->left = pop(&s);
                push(&s,newnode);
                p = strtok(NULL," "); 
                break;
            }
        }
    }
    return pop(&s);
}

double evalTree(node *tree, double x){
    double l;
    double r;
    if (strcmp(tree->name, "x"))      {return x; }
    else if (strcmp(tree->name,"0"))   {return 0;}
    else if (strcmp(tree->name,"-1"))  {return -1;}
    else if (strcmp(tree->name,"pi"))  {return acos(-1);}
    else {
        l = evalTree(tree->left,x);
        r = evalTree(tree->right,x);
        if (strcmp(tree->name,"+")){ return l+r; }
        else if (strcmp(tree->name,"-")){ return l-r; }
        else if (strcmp(tree->name, "*")){ return l*r; }
        else if (strcmp(tree->name, "/")){
            if (r < 1E-6)
                return 0;
            else
                return l/r;
        }
        else if (strcmp(tree->name ,"^")){ return pow(l,r); }
        else if (strcmp(tree->name,"cos")){return l*cos(r);}
        else if (strcmp(tree->name, "sin")){ return l*sin(r); }
        else {
            fprintf(stderr,"No operator for %s\n",tree->name);
            printNode(tree);
            return 0;
        }
    }
}

void mutate(node *tree, double mR){
    if (tree){
        // Leafs have no children, so mutate
        if (tree->left == NULL && tree->right == NULL && rand() < mR){
            tree->name = TERMINALS[ (rand() % NUMTERMINALS)];
        }
        mutate(tree->left,mR);
        mutate(tree->right,mR);
    }
}

void swap(node *t1, node* t2, double swapP){
    node *temp;

    // Swap left and right subtrees
    if (t1->left != NULL && t1->right != NULL &&
            t2->left != NULL && t2->right != NULL){
        if (rand() / (double) RAND_MAX < swapP){
            temp = t1->left;
            t1->left = t2->right;
            t2->right = temp;

            temp = t2->right;
            t1->right = t2->left;
            t2->left = temp;
        }
        swap(t1->right, t2->right,swapP);
        swap(t1->right, t2->left,swapP);
        swap(t1->left, t2->right,swapP);
        swap(t1->left, t2->left,swapP);
    }
}
