#ifndef __ACTION_H__
#define __ACTION_H__

#include <iostream>

class action {
    double xSpeed;
    double yawSpeed;

    public:
        action(double xSpeed, double yawSpeed);
        ~action();

        void write(std::ostream& out);
    private:

};
#endif

