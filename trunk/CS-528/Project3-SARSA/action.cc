#include "action.hh"


action::action(double xSpeed=0.0, double yawSpeed=0.0){
    this->xSpeed = xSpeed;
    this->yawSpeed = yawSpeed;
}

action::~action(){}

double action::getXAction(){
    return xSpeed;
}

double action::getYawSpeed(){
    return yawSpeed;
}


void action::write(std::ostream& out){
    out<<"<action>\n";
    out<<"\t<xSpeed>"<<xSpeed<<"</xSpeed>\n";
    out<<"\t<yawSpeed>"<<yawSpeed<<"</yaySpeed>\n";
    out<<"</action>\n";
}
