#include "state.hh"

state::state(){
    lmin = 0.0;
    rmin = 0.0;
    rmax = 0.0;

}

state::state(double lmin, double rmin, double rmax){
    this->lmin = lmin;
    this->rmin = rmin;
    this->rmax = rmax;
}
