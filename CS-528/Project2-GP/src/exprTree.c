#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <fenv.h>

#include "tree.h"
#include "exprTree.h"

/**
 * Node Functions
 */
#define NUMFUNCTIONS 5
char *FUNCTIONS[NUMFUNCTIONS] = {"+","-","*","/","^"};

/**
 * Terminal Functions 
 */
#define NUMTERMINALS 2 
char *TERMINALS[NUMTERMINALS] = {"value","x"};

node *leafNode(double constProb){
    if ( (rand() / (double) RAND_MAX) < constProb){
        return createNode(TERMINALS[0]);   
    }
    else
        return createNode(TERMINALS[1]);
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

node *buildTree(node* parent,int depth, double pruneProb, double constProb){
    node *tree = NULL;
    if (depth == 0 || (drand(0,1) < pruneProb && parent != NULL)) {
        tree = leafNode(constProb);
        tree->parent = parent;
        return tree;
    }
    else{
        node *tree = funcNode();
        tree->parent = parent;
        tree->left = buildTree(tree,depth-1,pruneProb,constProb);
        tree->right= buildTree(tree,depth-1,pruneProb,constProb);
        return tree;
    }
}

void writePostfix(const node *tree, char *filename){
    FILE *f = NULL;
    if (!filename){
        writePostfixHelper(tree,stdout);
    }
    else if ((f = fopen(filename,"w"))){
        writePostfixHelper(tree,f);
        fprintf(f,"\n");
        fclose(f);
    }
    else
        fprintf(stderr,"Could not open file %s\n",filename);
}

void writePostfixHelper(const node *tree, FILE *f){
    if (tree){
        writePostfixHelper(tree->left,f);
        writePostfixHelper(tree->right,f);
        if (strcmp(tree->name,TERMINALS[0])==0)
            fprintf(f," %5.3e ",tree->value);
        else
            fprintf(f," %s ",tree->name);
    }
}
node* readPostfix(char *postfix){
    struct stack s;
    double value;
    node *newnode;
    char *p;
    int i;
    p = strtok(postfix,"   \t");
    while (p != NULL){

        /* Checking is it is a known terminal */
        value = atof(p);
        if ( value != 0.0){
            newnode = createNode(TERMINALS[0]);
            newnode->value = value;
            push(&s,newnode);
        }
        for (i = 0; i < NUMTERMINALS; i++){
            if (strcmp(p,TERMINALS[i]) == 0){
                newnode = createNode(TERMINALS[i]);
                push(&s,newnode);
            }
        }
        /* Checking if it a known function */
        for (i = 0; i < NUMFUNCTIONS; i++){
            if (strcmp(p,FUNCTIONS[i]) == 0){
                newnode = createNode(FUNCTIONS[i]);
                newnode->left = pop(&s);
                newnode->right = pop(&s);
                push(&s,newnode);
            }
        }
        p = strtok(NULL,"   \t"); 
    }
    return  pop(&s);
}

double evalTree(const node *tree, const double x){
    double l;
    double r;
    feenableexcept(FE_INVALID | FE_OVERFLOW);
    
    if (strcmp(tree->name, "x")==0)      {return x; }
    else if (strcmp(tree->name, "value")==0)      {return tree->value; }
    else if (strcmp(tree->name,"0")==0)   {return 0;}
    else if (strcmp(tree->name,"-1")==0)  {return -1;}
    else if (strcmp(tree->name,"pi")==0)  {return acos(-1);}
    else {
        l = evalTree(tree->left,x);
        r = evalTree(tree->right,x);
        if (strcmp(tree->name,"+")==0){ return l+r; }
        else if (strcmp(tree->name,"-")==0){ return l-r; }
        else if (strcmp(tree->name, "*")==0){ return l*r; }
        else if (strcmp(tree->name, "/")==0){
            if (r < 1E-6)
                return 0;
            else
                return l/r;
        }
        else if (strcmp(tree->name ,"^")==0){ 
            if (l <= 0.0)
                return 0.0;
            else if (r == 0.0)
                return 1.0;
            else if (r < 0)
                return pow(l,-1.0*r);
            else
                return pow(l,r); 
        }
        else if (strcmp(tree->name,"cos")==0){return l*cos(r);}
        else if (strcmp(tree->name, "sin")==0){  return l*sin(r); }
        else if (strcmp(tree->name,"sqrt")==0){
            if (r < 0)
                return l*sqrt(-1.0*r);
             else
                return l*sqrt(r);
            }
        else {
            fprintf(stderr,"No operator for %s\n",tree->name);
            printNode(tree);
            return 0;
        }
    }
}

void mutate(node *tree, double mR){
    if (tree){
        /* Mutation of leaves */
        if (isChild(tree)){
            if (drand(0,1) < mR){
                if (rand() % 2 ){
                    if (strcmp(tree->name,"value")==0){
                        tree->name = TERMINALS[1];
                    }
                    else{
                        tree->name = TERMINALS[0];
                        tree->value = rand() / (double) RAND_MAX;
                    }
                }
                else if (strcmp(tree->name,"value")==0){
                    tree->value = rand() / (double) RAND_MAX;
                }
            }
        }
        /* Mutation of the functions */
        else if ( drand(0,1) < mR){
            int choice = rand() % NUMFUNCTIONS;
            tree->name = FUNCTIONS[choice];        
        }
        mutate(tree->left,mR);
        mutate(tree->right,mR);
    }
}

void swap(node *t1, node* t2, double swapP){

    /* Choosing nodes */
    node *temp = NULL;
    int choice = rand() % 4;
    node *p1 = chooseParent(t1,swapP);
    node *p2 = chooseParent(t2,swapP);

    switch (choice){
        case 0:     /* Swap Right with Left (right w/ left) */
        case 1:     /* Swap Left with Right */
            temp = p1->left;
            p1->left = p2->right;
            p2->right = temp;
        case 2:     /* Swap Left with Left */
            temp = p1->left;
            p1->left = p2->left;
            p2->left = temp;
        default:    /* Swap Right with Right */
            temp = p1->right;
            p1->right = p2->right;
            p2->right = temp;
    }
}
node *chooseSubTree(node *tree, double p){
    /* Skipping the head node */
    if (rand() %2)
        tree = tree->left;
    else
        tree = tree->right;
    while(isParent(tree) && (rand()/(double) RAND_MAX < p)){

        /* Pick Left or Right Branch */
        if (rand() % 2)
            tree = tree->left;
        else
            tree = tree->right;
    }
    return tree;
}

node *chooseParent(node *tree, double p){
    node *ptr = tree;    
    while(isParent(ptr)){
        if( rand()/(double) RAND_MAX < p)
            return ptr;
        if (isParent(ptr->left) && isParent(ptr->right)){
            /* Pick Left or Right Branch */
            if (rand() % 2)
                tree = ptr->left;
            else
                tree = ptr->right;
        }
    }
    return ptr;
}
