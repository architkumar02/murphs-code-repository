/// \file optical//include/WLSSlab.hh
/// \brief Definition of the WLSSlab class
//
//
#ifndef WLSSlab_H
#define WLSSlab_H 1

#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4OpticalSurface.hh"

#include "DetectorConstruction.hh"

class WLSSlab : public G4PVPlacement
{
  public:

    WLSSlab(G4RotationMatrix *pRot,
               const G4ThreeVector &tlate,
               G4LogicalVolume *pMotherLogical,
               G4bool pMany,
               G4int pCopyNo,
               DetectorConstruction* c);

  private:

    void CopyValues();

    DetectorConstruction* fConstructor;

    G4bool fUpdated;

    static G4LogicalVolume* fScintSlab_log;

    G4int fNfibers;
    G4double fScint_x;
    G4double fScint_y;
    G4double fScint_z;
    G4double fSlab_z;
};

#endif
