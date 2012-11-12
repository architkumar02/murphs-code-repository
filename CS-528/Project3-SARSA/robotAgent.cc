/*
 * robotAgent.cc
 *
 * Matthew J. Urffer (matthew.urffer@gmail.com)
 * CS-528 Machine Learning
 * Fall 2012
 *
 * robot agent for SARSA lambda
 *
 * @todo: Need to write some state representation
 * @todo: Need to work on saving actions
 */

#include <libplayerc++/playerc++.h>
#include <iostream>

#include "args.h"
#include "state.hh"
#include "action.hh"
#include "control.hh"

#define RAYS 32
using namespace std;
int main(int argc, char **argv) {
    parse_args(argc,argv);
    if (train){
        cout<<"Elected to train robot"<<endl;
    }
    if(run){
        cout<<"Elected to run trained robot"<<endl;
        // Need to read in optimal control policy
    }

    // we throw exceptions on creation if we fail
    try
    {
        using namespace PlayerCc;

        PlayerClient robot(gHostname, gPort);
        Position2dProxy pp(&robot, gIndex);
        LaserProxy lp(&robot, gIndex);

        std::cout << robot << std::endl;

        pp.SetMotorEnable (true);

        // go into read-think-act loop
        for(;;)
        {
            double newspeed = 0;
            double newturnrate = 0;

            // this blocks until new data comes; 10Hz by default
            robot.Read();

            double minR = lp.GetMinRight();
            double minL = lp.GetMinLeft();
            double maxRange = lp.GetMaxRange();

            // laser avoid (stolen from esben's java example)
            std::cout << "minR: " << minR
                << "minL: " << minL
                << "maxRange: "<< maxRange
                << std::endl;

            double l = (1e5*minR)/500-100;
            double r = (1e5*minL)/500-100;

            if (l > 100)
                l = 100;
            if (r > 100)
                r = 100;

            newspeed = (r+l)/1e3;

            newturnrate = (r-l);
            newturnrate = limit(newturnrate, -40.0, 40.0);
            newturnrate = dtor(newturnrate);

            std::cout << "speed: " << newspeed
                << "turn: " << newturnrate
                << std::endl;

            // write commands to robot
            pp.SetSpeed(newspeed, newturnrate);
        }
    }
    catch (PlayerCc::PlayerError & e)
    {
        std::cerr << e << std::endl;
        return -1;
    }
}
