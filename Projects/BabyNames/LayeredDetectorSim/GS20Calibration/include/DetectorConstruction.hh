/// \file optical//include/DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class
//
//
#ifndef DetectorConstruction_H
#define DetectorConstruction_H 1

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Box;
class G4Tubs;
class PMTSD;
class ScintSD;
class G4Sphere;

#include "G4Material.hh"
#include "DetectorMessenger.hh"
#include "G4VisAttributes.hh"
#include "G4RotationMatrix.hh"
#include "G4VUserDetectorConstruction.hh"

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:

    DetectorConstruction();
    virtual ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();

    //Functions to modify the geometry
    void SetDimensions(G4ThreeVector );
    void SetHousingThickness(G4double );
    void SetNX(G4int );
    void SetNY(G4int );
    void SetNZ(G4int );
    void SetPMTRadius(G4double );
    void SetDefaults();

    //Get values
    G4double GetScintX(){return fScint_x;}
    G4double GetScintY(){return fScint_y;}
    G4double GetScintZ(){return fScint_z;}
    G4double GetHousingThickness(){return fD_mtl;}
    G4int GetNX(){return fNx;}
    G4int GetNY(){return fNy;}
    G4int GetNZ(){return fNz;}
    G4double GetPMTRadius(){return fOuterRadius_pmt;}
    G4double GetSlabZ(){return fSlab_z;}
 
    //rebuild the geometry based on changes. must be called
    void UpdateGeometry();
    G4bool GetUpdated(){return fUpdated;}

    void SetSphereOn(G4bool b){fSphereOn=b; fUpdated=true;}
    static G4bool GetSphereOn(){return fSphereOn;}

    void SetHousingReflectivity(G4double r){fRefl=r; fUpdated=true;}
    G4double GetHousingReflectivity(){return fRefl;}

    void SetWLSSlabOn(G4bool b){fWLSslab=b; fUpdated=true;}
    G4bool GetWLSSlabOn(){return fWLSslab;}

    void SetMainVolumeOn(G4bool b){fMainVolume=b; fUpdated=true;}
    G4bool GetMainVolumeOn(){return fMainVolume;}

    void SetNFibers(G4int n){fNfibers=n; fUpdated=true;}
    G4int GetNFibers(){return fNfibers;}

    void SetMainScintYield(G4double );
    void SetWLSScintYield(G4double );

  private:

    void DefineMaterials();
    G4VPhysicalVolume* ConstructDetector();

    DetectorMessenger* fDetectorMessenger;

    G4bool fUpdated;
 
    G4Box* fExperimentalHall_box;
    G4LogicalVolume* fExperimentalHall_log;
    G4VPhysicalVolume* fExperimentalHall_phys;

    //Materials & Elements
    G4Material* f;
    G4Material* fAl;
    G4Element* fN;
    G4Element* fO;
    G4Material* fAir;
    G4Material* fVacuum;
    G4Element* fC;
    G4Element* fH;
    G4Material* fGlass;
    G4Material* fPstyrene;
    G4Material* fPMMA;
    G4Material* fPethylene1;
    G4Material* fPethylene2;

    //Geometry
    G4double fScint_x;
    G4double fScint_y;
    G4double fScint_z;
    G4double fD_mtl;
    G4int fNx;
    G4int fNy;
    G4int fNz;
    G4double fOuterRadius_pmt;
    G4int fNfibers;
    static G4bool fSphereOn;
    G4double fRefl;
    G4bool fWLSslab;
    G4bool fMainVolume;
    G4double fSlab_z;

    G4MaterialPropertiesTable* f_mt;
    G4MaterialPropertiesTable* fMPTPStyrene;

};

#endif
