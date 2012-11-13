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
 */

#include <libplayerc++/playerc++.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>

#include "args.h"
#include "state.hh"
#include "action.hh"
#include "satable.hh"

#define RAYS 32
using namespace std;
double minDistance = 0.05;
bool isTerminal(state s){
    return (s.getRMin() < minDistance || s.getLMin() < minDistance);
}
int main(int argc, char **argv) {
    parse_args(argc,argv);
    srand(time(NULL));
    satable c;
    if (train){
        cout<<"Elected to train robot"<<endl;
    }
    else{
        cout<<"Elected to run trained robot"<<endl;
        std::ifstream in("OptimalStrategy.txt");
        c.readTable(in);
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


        if (train){
            // Training Parameters
            double maxSpeed = 0.1;
            double maxYaw = 0.1;
            double epsilon = 0.1;
            double lambda = 0.9;
            int numTries = 100;
            std::cout<<"\nTraining Parameters:\n"
                <<"\tMax Speed: "<<maxSpeed<<" [m/s]\n"
                <<"\tMax Yaw: "<<maxYaw<<" [rad/s]\n"
                <<"\tepsilon: "<<epsilon<<"\n"
                <<"\tlambda: "<<lambda<<"\n"
                <<"\tNum Eposides: "<<numTries<<std::endl;
            for(int i = 0; i <numTries; i++){
                // Initial Coditions
                pp.GoTo(-3,2,0);
                // Initilize State and Action
                state *s = new state(lp.GetMinRight(),lp.GetMinLeft(), lp.GetMaxRange());
                action *a = new action();
                do
                {
                    // this blocks until new data comes; 10Hz by default
                    robot.Read();
                    // Epsilon Greedy Choice
                    if (rand()/((double) RAND_MAX) < epsilon){
                        a = new action( rand()/((double) RAND_MAX)*maxSpeed,
                                    rand()/((double) RAND_MAX)*2*3.14);
                    }
                    else{
                        *a = c.getAction(*s);
                    }

                    // write commands to robot
                    pp.SetSpeed(a->getXSpeed(), a->getYawSpeed());
                }while (!isTerminal(*s));
            }
            std::ofstream out("OptimalStrategy.txt");
            c.writeTable(out);
            out.close();
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
                *a = c.getAction(*s);

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
