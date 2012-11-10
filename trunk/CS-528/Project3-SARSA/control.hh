/**
  * @file control.hh
  * @author Matthew J. Urffer (matthew.urffer@gmail.com)
  */
#ifndef CONTORL_HH
#define CONTROL_HH

#include "state.hh"
#include "action.hh"


class control{

    public:
        control();
        ~control();
        
        action getControlAction(state s);
        
    private:
        

};
#endif
