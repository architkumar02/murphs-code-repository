/**
  * @author Matthew J. Urffer (matthew.urffer@gmail.com)
  */
#ifndef __SATABLE_H__
#define __SATABLE_H__

#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <utility>

#include "state.hh"
#include "action.hh"

using namespace std;


class satable{
    struct cmp_state{
        bool operator()(const state& lhs, const state& rhs){
            return (lhs.rmin < rhs.rmin);
        }   
    };
    std::map<state,action,cmp_state> table;

    public:
        satable();
        ~satable();
        
        action getAction(state s);
        void addAction(state s, action a);

        /* I/O*/
        void writeTable(std::ostream& out);
        void readTable(ifstream& in);
    private:
        
};
#endif
