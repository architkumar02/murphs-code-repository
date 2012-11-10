/**
  * @file control.hh
  * @author Matthew J. Urffer (matthew.urffer@gmail.com)
  */
#ifndef __CONTORL_H__
#define __CONTROL_H__

#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <utility>

#include "state.hh"
#include "action.hh"

using namespace std;

class control{
    
    map<state,action> qTable;
    string filename;

    public:
        control(string filename);
        ~control();
        
        action getControlAction(state s);
        
        void writeControlStrategy(std::ostream& out);

        void readControlStrategy(ifstream& in);
    private:
        
};
#endif
