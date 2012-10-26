#ifndef __TREE_H__
#define __TREE_H__

/*
 * Utility Functon
 */
double drand(double min, double max);

/**
 * Node structure
 */
struct node_t{
    char *name;
    double value;
    struct node_t *parent;
    struct node_t *right;
    struct node_t *left;
};

typedef struct node_t node;
struct stack{
    struct node_t* head;
};
/**
 * Stack Functions 
 */
int isempty(struct stack *s);
void push(struct stack *s, node *n);
void printStack(struct stack *s);
node* pop(struct stack *s);

/**
 * Tree Functions
 */
node* createNode(char* name);
node* copy(const node* root);
void printNode(const node *n);
void deleteTree(node *tree);
void writeTree(node *tree,char *filename);
void writeTreeHelper(node *tree, FILE *f);

int isChild(node *n);
int isLeftChild(node *n, node *p);
int isRightChild(node *n, node *p);
int isHead(node *n);
int isParent(node *n);
int isequal(const node* t1, const node *t2);
#endif
