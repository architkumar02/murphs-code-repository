#include <iostream>
#include <fstream>

#include "state.hh"

int main(){
    
    // Testing Writing
    std::cout<<"Testing Write \n"<<std::endl;
    state s(4.3,5.2,1.0);
    s.write(std::cout);

    std::ofstream filestream("state.txt");
    s.write(filestream);
    filestream.close();

    // Testing Reading
    std::cout<<"\nTesting Read \n"<<std::endl;
    std::ifstream in("state.txt");
    state s1(0,0,0);
    s1.read(in);
    s1.write(std::cout);
    in.close();
    return 0;
}
