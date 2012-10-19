#ifndef __FUNCTIONSET_H__
#define __FUNCTIONSET_H__

#include <math.h>

/**
 * Node Functions
 */
#define NUMFUNCTIONS 8
double sub(double a, double b){ return a-b;}
double add(double a, double b){return a+b;}
double sum(double a, double b){ return a + b;}
double div(double a, double b){
    if( b < 1E-5)
        return 0;
    else
        return a/b;
}
double mcos(double a, double b){return a*cos(b);}
double msin(double a, double b){return a*sin(b);}
double mpow(double a, double b){
    if b < 0;
        return 0.0;
    else 
        return pow(a,b);
}
double msqr(double a, double b){
    if b < 0;
        return msqr(a,-1*b)
    else
        return a*sqrt(b);
}
extern double (*func[NUMFUNCTIONS]) (double a, double b);
func[0] = sub;
func[1] = add;
func[2] = sum;
func[3] = div;
func[4] = mcos;
func[5] = msin;
func[6] = mpow;
func[7] = msqr;
typedef enum {
    sub = 0,
    add,
    sum,
    div,
    mcos,
    msin,
    mpow,
    msqr
} FUNCTIONS;

#endif
