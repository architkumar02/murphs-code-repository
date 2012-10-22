#ifndef __TREE_H__
#define __TREE_H__

/**
 * Node structure
 */
struct node_t{
    char *name;
    int depth;
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
node* pop(struct stack *s);

/**
 * Tree Functions
 */
node* createNode(char* name);
void printNode(node *n);
void deleteTree(node *tree);
void writeTree(node *tree,char *filename);
void writeTreeHelper(node *tree, FILE *f);

int compareTree(node* t1, node *t2);
#endif
