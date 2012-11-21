#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
 
#define MAX 10
#define EMPTY -1
 
 
struct node
{
    char element;
    struct node *left,*right;
};
 
struct stack
{
    struct node *data[MAX];
    int top;
};
 
int isempty(struct stack *s)
{
    return (s->top == EMPTY) ? 1 : 0;
}
 
void emptystack(struct stack* s)
{
    s->top=EMPTY;
}
 
void push(struct stack* s, struct node *item)
{
    if(s->top == (MAX-1))
    {
        printf("\nSTACK FULL");
    }
    else
    {
        ++s->top;
        s->data[s->top]=item;
 
    }
}
 
struct node* pop(struct stack* s)
{
    struct node *ret=NULL;
    if(!isempty(s))
    {
        ret= s->data[s->top];
        --s->top;
    }
    return ret;
}
 
void postfix2exptree(char* postfix, struct node **root)
{
    struct stack X;
    struct node *newnode,*op1,*op2;
    char *p;
    p = &postfix[0];
    emptystack(&X);
    while(*p)
    {
 
        while(*p == ' ' || *p == '\t')
        {
            p++;
        }
 
        if(isalpha(*p) || isdigit(*p))
        {
            newnode = malloc(sizeof(struct node));
            newnode->element = *p;
            newnode->left = NULL;
            newnode->right = NULL;
            push(&X,newnode);
        }
        else
        {
            op1 = pop(&X);
            op2 = pop(&X);
            newnode = malloc(sizeof(struct node));
            newnode->element = *p;
            newnode->left = op2;
            newnode->right = op1;
            push(&X,newnode);
        }
        p++;
    }
    *root = pop(&X);
}
 
void inorder(struct node *x)
{
    if(x != NULL)
    {
        inorder(x->left);
        printf("%c ",x->element);
        inorder(x->right);
    }
}
 
void preorder(struct node *x)
{
    if(x != NULL)
    {
        printf("%c ",x->element);
        preorder(x->left);
        preorder(x->right);
    }
}
 
void postorder(struct node *x)
{
    if(x != NULL)
    {
        postorder(x->left);
        postorder(x->right);
        printf("%c ",x->element);
    }
}