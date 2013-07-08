/// \file optical//include/MainVolume.hh
/// \brief Definition of the MainVolume class
//
#ifndef MainVolume_H
#define MainVolume_H 1

#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4OpticalSurface.hh"

#include "DetectorConstruction.hh"

class MainVolume : public G4PVPlacement
{
  public:

    MainVolume(G4RotationMatrix *pRot,
                 const G4ThreeVector &tlate,
                 G4LogicalVolume *pMotherLogical,
                 G4bool pMany,
                 G4int pCopyNo,
                 DetectorConstruction* c);

  private:

    void VisAttributes();
    void SurfaceProperties();

    void PlacePMTs(G4LogicalVolume* pmt_Log,
                   G4RotationMatrix* rot, G4double &a, G4double &b, G4double da,
                   G4double db, G4double amin, G4double bmin, G4int na, G4int nb,
                   G4double &x, G4double &y, G4double &z, G4int &k,PMTSD* sd);

    void CopyValues();

    G4bool fUpdated;
  
    DetectorConstruction* fConstructor;

    G4double fScint_x;
    G4double fScint_y;
    G4double fScint_z;
    G4double fD_mtl;
    G4int fNx;
    G4int fNy;
    G4int fNz;
    G4double fOuterRadius_pmt;
    G4bool fSphereOn;
    G4double fRefl;

    //Basic Volumes
    //
    G4Box* fScint_box;
    G4Box* fHousing_box;
    G4Tubs* fPmt;
    G4Tubs* fPhotocath;
    G4Sphere* fSphere;

    // Logical volumes
    //
    G4LogicalVolume* fScint_log;
    static G4LogicalVolume* fHousing_log;
    G4LogicalVolume* fPmt_log;
    G4LogicalVolume* fPhotocath_log;
    G4LogicalVolume* fSphere_log;

    //Sensitive Detectors
    static ScintSD* fScint_SD;
    static PMTSD* fPmt_SD;
};

#endif
