#ifndef __TREE_H__
#define __TREE_H__

/**
 * Node structure
 */
struct node_t{
    char *name;
    struct node_t *right;
    struct node_t *left;
};
typedef struct node_t node;
struct stack{
    struct node_t* head;
};

int isempty(struct stack *s);
void push(struct stack *s, node *n);
node* pop(struct stack *s);

node* createNode(char* name);

#endif
