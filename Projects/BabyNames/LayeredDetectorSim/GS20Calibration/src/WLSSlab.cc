/// \file optical//src/WLSSlab.cc
/// \brief Implementation of the WLSSlab class
//
//
#include "WLSSlab.hh"
#include "WLSFiber.hh"

#include "globals.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4SystemOfUnits.hh"

G4LogicalVolume* WLSSlab::fScintSlab_log=NULL;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

WLSSlab::WLSSlab(G4RotationMatrix *pRot,
                       const G4ThreeVector &tlate,
                       G4LogicalVolume *pMotherLogical,
                       G4bool pMany,
                       G4int pCopyNo,
                       DetectorConstruction* c)
  :G4PVPlacement(pRot,tlate,
                 new G4LogicalVolume(new G4Box("temp",1,1,1),
                                     G4Material::GetMaterial("Vacuum"),
                                     "temp",0,0,0),
                 "Slab",pMotherLogical,pMany,pCopyNo),fConstructor(c)
{
  CopyValues();
 
  if(!fScintSlab_log || fUpdated){

    G4double slab_x = fScint_x/2.;
    G4double slab_y = fScint_y/2.;
 
    G4Box* ScintSlab_box = new G4Box("Slab",slab_x,slab_y,fSlab_z);
 
    fScintSlab_log
      = new G4LogicalVolume(ScintSlab_box,
                            G4Material::GetMaterial("Polystyrene"),
                            "Slab",0,0,0);
 
    G4double spacing = 2*slab_y/fNfibers;
 
    G4RotationMatrix* rm = new G4RotationMatrix();
    rm->rotateY(90*deg);
 
    //Place fibers
    for(G4int i=0;i<fNfibers;i++){
      G4double Y=-(spacing)*(fNfibers-1)*0.5 + i*spacing;
      new WLSFiber(rm,G4ThreeVector(0.,Y,0.),fScintSlab_log,false,0,
                      fConstructor);
    }
 
  }
 
  SetLogicalVolume(fScintSlab_log);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WLSSlab::CopyValues(){
  fUpdated=fConstructor->GetUpdated();
 
  fScint_x=fConstructor->GetScintX();
  fScint_y=fConstructor->GetScintY();
  fScint_z=fConstructor->GetScintZ();
  fNfibers=fConstructor->GetNFibers();
  fSlab_z=fConstructor->GetSlabZ();
}
