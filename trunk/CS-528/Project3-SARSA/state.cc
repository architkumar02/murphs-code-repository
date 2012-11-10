#include "state.hh"

state::state(double lmin=0.0, double rmin=0.0, double rmax=0.0){
    this->lmin = lmin;
    this->rmin = rmin;
    this->rmax = rmax;
}

void state::write(std::ostream& out){
    out<<"<state>\n";
    out<<"\t<lmin>"<<lmin<<"</lmin>\n";
    out<<"\t<rmin>"<<rmin<<"</rmin>\n";
    out<<"\t<rmax>"<<rmax<<"</rmax>\n";
    out<<"</state>\n";
}


state::~state(){}
