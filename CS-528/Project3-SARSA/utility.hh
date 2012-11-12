#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>

double readXMLValue(ifstream& in){
    string line;
    char tmp1[100];
    char tmp2[100];
    char tVal[100];
    getline(in,line);
    sscanf(line.c_str(),"%s %s %s",tmp1,tVal,tmp2);
    return atof(tVal);
}
#endif
