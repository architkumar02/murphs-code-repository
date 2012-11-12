#ifndef __ACTION_H__
#define __ACTION_H__

#include <iostream>
#include "utility.hh"

class action {

    public:
        action(double xSpeed, double yawSpeed);
        ~action();

        void setXSpeed(double v)    {xSpeed = v;};
        void setYawSpeed(double w)  {yawSpeed = w; };
    
        double getXSpeed()          {return xSpeed;};
        double getYawSpeed()        {return yawSpeed;};

        void write(std::ostream& out);
        void read(ifstream &in);
    private:
        double xSpeed;
        double yawSpeed;

};
#endif

