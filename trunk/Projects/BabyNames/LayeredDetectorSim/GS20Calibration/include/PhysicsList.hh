/// \file optical//include/PhysicsList.hh
/// \brief Definition of the PhysicsList class
//
//
#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class PhysicsList: public G4VModularPhysicsList
{
  public:

    PhysicsList();
    virtual ~PhysicsList();

  public:

    // SetCuts()
    virtual void SetCuts();

};

#endif
