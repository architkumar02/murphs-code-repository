#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "tree.h"
#include "exprTree.h"

node *leafNode(){
    int choice = (rand() % NUMTERMINALS);
    return createNode(TERMINALS[choice]);
}

node *funcNode(){
    int choice = rand() % NUMFUNCTIONS;
    return createNode(FUNCTIONS[choice]);
}

node *buildTree(int depth, double pruneProb){
    node* tree = NULL;
    if (depth == 0){
        tree = leafNode();
    } 
    else if (  (rand() / (double) RAND_MAX) < pruneProb) {
        return tree = leafNode();
    }
    else{
        tree = funcNode();
        tree->left = buildTree(depth-1,pruneProb);
        tree->right= buildTree(depth-1,pruneProb);
    }
    return tree;
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
    fprintf(stdout,"Parsing expression %s\n",postfix);
    p = strtok(postfix," ");
    while (p != NULL){

        // Checking is it is a known terminal
        for (i = 0; i < NUMTERMINALS; i++){
            if (strcmp(p,TERMINALS[i]) == 0){
                newnode = createNode(TERMINALS[i]);
                printNode(newnode);
                push(&s,newnode);
                p = strtok(NULL," "); 
                continue;
            }
        }
        // Checking if it a known function
        for (i = 0; i < NUMFUNCTIONS; i++){
            if (strcmp(p,FUNCTIONS[i]) == 0){
                newnode = createNode(FUNCTIONS[i]);
                newnode->right = pop(&s);
                newnode->left = pop(&s);
                printNode(newnode);
                push(&s,newnode);
                p = strtok(NULL," "); 
                continue;
            }
            // Control should only reach here in error
            fprintf(stderr,"Token %s is not reconized\n",p);
        }
        fprintf(stdout,"Completed expression parsing of %s\n",postfix);
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
        t = buildTree(depth,0.2);
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
int main(int argc, char *argv[]){
    srand( time(NULL));
    node *root = NULL;
    int treeDepth = 0;
    int i;
    /* Processing Command Line Inputs */
    for (i = 1;  i < argc; i++){
        if (strcmp(argv[i],"--maxDepth")==0){
            sscanf(argv[++i],"%d",&treeDepth);
        }
    }

    printSet();

    /* Testing Expression Building */
    fprintf(stdout,"Testing Read Expression\n");
    char expr[128] = "0 pi +";
    root = readExpr(expr);
    writePostfix(root,NULL);
    deleteTree(root);

    //StressTest(20,100);

    fprintf(stdout,"Building an expression tree with depth %d\n",treeDepth);
    root = buildTree(treeDepth,0.1);

    fprintf(stdout," Writing a tree\n");
    writeTree(root,"tree.dot");
    writePostfix(root,NULL);
    double x = 0;
    fprintf(stdout," Evaluating a tree\n\tTree(%5.2f)=%5.2f\n",x,evalTree(root,x));


    fprintf(stdout,"Deleting a tree\n");
    deleteTree(root);
    return EXIT_SUCCESS;
}
