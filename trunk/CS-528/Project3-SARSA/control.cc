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
    std::ofstream filestream(filename);
    writeControlStrategy(filestream);
    filestream.close();
}
/**
 * @brief Writes the control strategy out to the file stream
 *
 * Calls the write methods for the state and action 
 */
void control::writeControlStrategy(std::ostream& out){
    
    for (map<state,action>::iterator i = qTable.begin(); i != qTable.end(); i++){
        (*i).first.write(out);  // First is key
        (*i).second.write(out); // Second is vlaue
    }
}
