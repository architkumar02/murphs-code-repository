#ifndef INCLUDE_ROOTIO_HH 
#define INCLUDE_ROOTIO_HH 1

#include "TROOT.h"
#include "TFile.h"
#include "TSystem.h"

#include "TrackerHit.hh"


class RootIO 
{
public: 
  virtual ~RootIO();
  
  static RootIO* GetInstance();
  void Write(std::vector<TrackerHit*>*);
  void Close();

protected:
  RootIO(); 
  
private:

  TFile* fo;
  int Nevents;
  
};
#endif 
