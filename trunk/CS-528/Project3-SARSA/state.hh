#ifndef __STATE_H__
#define __STATE_H__

#include <iostream>

class state{
    double lmin;
    double rmin;
    double rmax;
    public:
        state(double lmin, double rmin, double rmax);
        ~state();
        
        void write(std::ostream & out);

    private:

};
#endif
