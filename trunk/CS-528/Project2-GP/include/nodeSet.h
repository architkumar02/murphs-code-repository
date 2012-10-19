#ifndef __NODESET_H__
#define __NODESET_H__

/**
 * Node Functions
 */
#define NUMFUNCTIONS 8
extern char FUNCTIONS[NUMFUNCTIONS][4];// = {"+","-","*","/","^","cos","sin"};


/**
 * Terminal Functions 
 */
#define NUMTERMINALS 
extern char TERMINALS[NUMTERMINALS][4]; // = {"0","-1","pi","x"};
/*
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
*/
#endif
