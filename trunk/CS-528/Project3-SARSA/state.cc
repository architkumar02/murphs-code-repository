#include "state.hh"

state::state(){
    this->lmin = 0.0;
    this->rmin = 0.0;
    this->rmax = 0.0;
}

state::state(double lmin=0.0, double rmin=0.0, double rmax=0.0){
    this->lmin = lmin;
    this->rmin = rmin;
    this->rmax = rmax;
}
state::~state(){}

void state::read(ifstream & in){
    string line;
    // Make sure we start on the correct note
    getline(in,line);
    if(line.compare("<state>")!=0){
        cerr<<"Unexeppected start of element (<state> expected): "<<line<<endl;
    }
    // Read in state variables
    this->lmin = readXMLValue(in);
    this->rmin = readXMLValue(in);
    this->rmax = readXMLValue(in);
    
    // Make sure we end on the correct note
    getline(in,line);
    if(line.compare("</state>")!=0){
        cerr<<"Unexeppected end of element (</state> expected): "<<line<<endl;
    }
    return;
}

void state::write(std::ostream& out){
    out<<"<state>"<<endl;;
    out<<"\t<lmin> "<<lmin*1.0<<" </lmin>"<<endl;;
    out<<"\t<rmin> "<<rmin*1.0<<" </rmin>"<<endl;
    out<<"\t<rmax> "<<rmax*1.0<<" </rmax>"<<endl;
    out<<"</state>"<<endl;
}

