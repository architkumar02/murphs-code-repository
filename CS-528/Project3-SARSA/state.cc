#include "state.hh"


state::state(double lmin=0.0, double rmin=0.0, double rmax=0.0){
    this->lmin = lmin;
    this->rmin = rmin;
    this->rmax = rmax;
}

state::~state(){}
