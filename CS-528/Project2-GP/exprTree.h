/* $begin exprTree.h */
#ifndef __EXPRTREE_H__
#define __EXPRTREE_H__


enum nodetype_t {
	UNARY,
	BINARY,
	UNKOWN
} nodetype;

struct node_t {
	nodetype = UNKOWN;
	double (*unaryfunc) (double);
	double (*binaryfunc)(double,double);
	struct node *right;
	struct node *left;
}

}
#endif
/* $end exprTree.h */
