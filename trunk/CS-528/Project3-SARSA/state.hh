#ifndef __STATE_H__
#define __STATE_H__

#include <iostream>
#include <fstream>
#include <string>

#include "utility.hh"

using namespace std;
class state{
    public:
        state(double lmin, double rmin, double rmax);
        ~state();
        
        void write(std::ostream & out);
        void read(ifstream &in);
    
        void setLMin(double l)      {lmin = l;};
        void setRMin(double r)      {rmin = r;};
        void setRMax(double r)      {rmax = r;};

        double getLMin()            {return lmin;};
        double getRMin()            {return rmin;};
        double getRMax()            {return rmax;};
    private:
        double lmin;
        double rmin;
        double rmax;

};
#endif
