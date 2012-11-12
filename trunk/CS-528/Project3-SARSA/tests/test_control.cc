#include <iostream>
#include <fstream>


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "control.cc"

int main(){
    
    // Testing Creation
    srand(time(NULL));
    int numActions = 10;
    control c("ControlStrategy.txt");
    std::cout<<"Testing Creation and addition of <state, action>"<<endl;
    for(int i = 0; i < numActions; i++){
        state* s = new state((double)( rand() % 100),
               (double)( rand()%100),(double)(rand()%100));
        action* a = new action((double)rand()/(double)RAND_MAX, (double)rand()/(double)RAND_MAX);
        c.addControlAction(*s,*a);

    }


    // Testing Writing
    std::cout<<"Testing Write \n"<<std::endl;
    std::ofstream filestream("strategy.txt");
    c.writeControlStrategy(filestream);
    filestream.close();

    // Testing Reading
    std::cout<<"\nTesting Read \n"<<std::endl;
    control c1("strategy.txt");
    std::ifstream in("strategy.txt");
    c1.readControlStrategy(in);
    c1.writeControlStrategy(std::cout);
    in.close();
    return 0;
}
