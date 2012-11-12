
#include "utility.hh"

double readXMLValue(ifstream& in){
    string line;
    char tmp1[100];
    char tmp2[100];
    char tVal[100];
    getline(in,line);
    sscanf(line.c_str(),"%s %s %s",tmp1,tVal,tmp2);
    return atof(tVal);
}
