/**
 * @file control.cc
 * @author Matthew J. Urffer (matthew.urffer@gmail.com)
 * 
 * @brief Control class for my robot
 */

#include "control.hh"


control::control(string filename="ControlStrategy.txt"){


}

control::~control(){
    // Need to write out control stragety
    writeControlStrategy(filename);
}
