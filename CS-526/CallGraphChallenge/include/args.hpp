#include <stdlib.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>

std::string filename("LinkAnalyticsData/UTK_problem/Moria_1.graph");
void print_usage(int argc, char** argv);

int parse_args(int argc, char** argv){
  // set the flags
  const char* optflags = "f:";
  int ch;

  // use getopt to parse the flags
  while(-1 != (ch = getopt(argc, argv, optflags))){
    switch(ch){
      case 'f':   // Filename
          if (!optarg){
            std::cerr<<"Argument is null"<<std::endl;
            print_usage(argc,argv);
          }
          else
            filename.assign(optarg); 
           break;
      case '?': // help
      case ':':
      default:  // unknown
        print_usage(argc, argv);
        exit (-1);
    }
  }
  return (0);
} 

void print_usage(int argc, char** argv){
  using namespace std;
  cerr << "USAGE:  " << *argv << " [options]" << endl << endl;
  cerr << "Where [options] can be:" << endl;
  cerr << "  -f <filename> filename of graph (default is "<<filename<<" )"<<endl;
} 
