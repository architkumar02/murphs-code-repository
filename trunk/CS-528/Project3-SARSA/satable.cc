/**
 * @file satable.cc
 * @author Matthew J. Urffer (matthew.urffer@gmail.com)
 * 
 * @brief satable class for my robot
 */

#include "satable.hh"

satable::satable(){}

/**
 * @brief Deconstructor
 *
 * No memory to free, but write the strategy
 */
satable::~satable(){

    // Do I need to clear all of my table entries?
}

/**
 * @brief Gets a Control Action
 */
action satable::getAction(state s){
    return (table.find(s))->second;
}

/**
 * @brief Adds a <state,action> pair to the table
 */
void satable::addAction(state s, action a){
    table.insert(std::pair<state,action>(s,a));
}

/**
 * @breif readTable 
 * Reads in a control strategy.  Better be formatted correctly as
 * state, followed by an action
 */
void satable::readTable(ifstream& in){
    while(in.good()){
        state* s = new state(0,0,0);
        action* a = new action(0,0);

        s->read(in);
        a->read(in);
        addAction(*s,*a);
    }
}
/**
 * @brief Writes the control strategy out to the file stream
 *
 * Calls the write methods for the state and action 
 */
void satable::writeTable(std::ostream& out){
    state s(0,0,0);
    action a(0,0);
    for (map<state,action>::iterator i = table.begin(); i != table.end(); i++){
        s = i->first;
        a = i->second;
        s.write(out);
        a.write(out);
    }
}
