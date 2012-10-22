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

node *buildTree(int depth, double pruneProb){
    if (depth == 0){
        return leafNode();
    } 
    else if (  (rand() / (double) RAND_MAX) < pruneProb) {
        return leafNode();
    }
    else{
        node *tree = funcNode();
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

void StressTest(int maxDepth, int numItter){
    int i=0;
    int depth = 0;
    node *t;
    for (i=0; i < numItter; i++){
        depth = rand() % maxDepth;
        t = buildTree(depth,0.5);
        deleteTree(t);
    }
}

void printSet(){
    int i;
    fprintf(stdout,"TERMINAL Set:\n");
    for (i = 0; i < NUMTERMINALS; i++)
        fprintf(stdout,"\t%s",TERMINALS[i]);

    fprintf(stdout,"\n\nFUCNTION Set:\n");
    for (i = 0; i < NUMFUNCTIONS; i++)
        fprintf(stdout,"\t%s",FUNCTIONS[i]);
    fprintf(stdout,"\n");
}
