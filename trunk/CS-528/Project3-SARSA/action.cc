#include "action.hh"

action::action(){
    this->xSpeed = 0.0;
    this->yawSpeed = 0.0;
}

action::action(double xSpeed=0.0, double yawSpeed=0.0){
    this->xSpeed = xSpeed;
    this->yawSpeed = yawSpeed;
}

action::~action(){}


void action::read(ifstream & in){
    string line;
    // Make sure we start on the correct note
    getline(in,line);
    if(line.compare("<action>")!=0){
        cerr<<"Unexeppected start of element (<action> expected): "<<line<<endl;
    }
    // Read in action variables
    this->xSpeed = readXMLValue(in);
    this->yawSpeed = readXMLValue(in);
    
    // Make sure we end on the correct note
    getline(in,line);
    if(line.compare("</action>")!=0){
        cerr<<"Unexeppected end of element (</action> expected): "<<line<<endl;
    }
    return;
}

void action::write(std::ostream& out){
    out<<"<action>\n";
    out<<"\t<xSpeed> "<<xSpeed<<" </xSpeed>\n";
    out<<"\t<yawSpeed> "<<yawSpeed<<" </yaySpeed>\n";
    out<<"</action>\n";
}
