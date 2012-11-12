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
    struct cmp_state{
        bool operator()(const state& lhs, const state& rhs){
            return (lhs.rmin < rhs.rmin);
        }   
    };
    std::map<state,action,cmp_state> qTable;
    string filename;

    public:
        control(string filename);
        ~control();
        
        action getControlAction(state s);
        void addControlAction(state s, action a);

        /* I/O*/
        void writeControlStrategy(std::ostream& out);
        void readControlStrategy(ifstream& in);
    private:
        
};
#endif
