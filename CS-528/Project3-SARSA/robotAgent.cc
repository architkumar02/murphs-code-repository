/*
 * robotAgent.cc
 *
 * Matthew J. Urffer (matthew.urffer@gmail.com)
 * CS-528 Machine Learning
 * Fall 2012
 *
 * robot agent for SARSA lambda
 *
 * @todo: Improve training
 * @todo: Test UI
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

    control c;
    if (train){
        cout<<"Elected to train robot"<<endl;
    }
    else{
        cout<<"Elected to run trained robot"<<endl;
        std::ifstream in("OptimalStrategy.txt");
        c.readControlStrategy(in);
        in.close();
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

        // Setting up problem parameters
        double wallD = 0.75;        // Wall Distance
        double oD = 0.25;           // Obsticle distance
        cout<<"Wall distance: "<<wallD<<" m"<<endl;
        cout<<"Obstacle distance: "<<oD<<" m"<<endl;

        double maxSpeed = 0.1;
        double maxYaw = 0.1;

        if (train){
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
        if(run){
            action* a = new action();
            state* s =  new state();
            while(true){

                // Getting State
                s->setRMin(lp.GetMinRight());
                s->setLMin(lp.GetMinLeft());
                s->setRMax(lp.GetMaxRange());

                // What action?
                *a = c.getControlAction(*s);

                // Applying action
                pp.SetSpeed(a->getXSpeed(),a->getYawSpeed());
            }
        }
    }
    catch (PlayerCc::PlayerError & e)
    {
        std::cerr << e << std::endl;
        return -1;
    }
}
