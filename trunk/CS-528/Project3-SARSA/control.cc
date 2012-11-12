/**
 * @file control.cc
 * @author Matthew J. Urffer (matthew.urffer@gmail.com)
 * 
 * @brief Control class for my robot
 */

#include "control.hh"

control::control(string filename="ControlStrategy.txt"){
    this->filename = filename;
}

/**
 * @brief Deconstructor
 *
 * No memory to free, but write the strategy
 */
control::~control(){
    // Need to write out control stragety
    std::ofstream filestream(filename.c_str());
    writeControlStrategy(filestream);
    filestream.close();
}

/**
 * @brief Gets a Control Action
 */
action control::getControlAction(state s){
    return qTable.find(s)->second;
}

/**
 * @brief Adds a <state,action> pair to the qtable
 */
void control::addControlAction(state s, action a){
    qTable.insert(std::pair<state,action>(s,a));
}

/**
 * @breif readControlStrategy 
 * Reads in a control strategy.  Better be formatted correctly as
 * state, followed by an action
 */
void control::readControlStrategy(ifstream& in){
    while(in.good()){
        state* s = new state(0,0,0);
        action* a = new action(0,0);

        s->read(in);
        a->read(in);
        addControlAction(*s,*a);
    }
}
/**
 * @brief Writes the control strategy out to the file stream
 *
 * Calls the write methods for the state and action 
 */
void control::writeControlStrategy(std::ostream& out){
    state s(0,0,0);
    action a(0,0);
    for (map<state,action>::iterator i = qTable.begin(); i != qTable.end(); i++){
       s = i->first;
       a = i->second;
        s.write(out);
        a.write(out);
          // ((*i).first).write(out);  // First is key (state)
        //((*i).second).write(out); // Second is vlaue (action)
    }
}
