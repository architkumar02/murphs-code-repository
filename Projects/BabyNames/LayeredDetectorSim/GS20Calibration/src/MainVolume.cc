/// \file optical//src/MainVolume.cc
/// \brief Implementation of the MainVolume class
//
//
#include "MainVolume.hh"
#include "globals.hh"
#include "G4SDManager.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "PMTSD.hh"
#include "ScintSD.hh"
#include "G4SystemOfUnits.hh"

ScintSD* MainVolume::fScint_SD=NULL;
PMTSD* MainVolume::fPmt_SD=NULL;

G4LogicalVolume* MainVolume::fHousing_log=NULL;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MainVolume::MainVolume(G4RotationMatrix *pRot,
                             const G4ThreeVector &tlate,
                             G4LogicalVolume *pMotherLogical,
                             G4bool pMany,
                             G4int pCopyNo,
                             DetectorConstruction* c)
  //Pass info to the G4PVPlacement constructor
  :G4PVPlacement(pRot,tlate,
                 //Temp logical volume must be created here
                 new G4LogicalVolume(new G4Box("temp",1,1,1),
                                     G4Material::GetMaterial("Vacuum"),
                                     "temp",0,0,0),
                 "housing",pMotherLogical,pMany,pCopyNo),fConstructor(c)
{
  CopyValues();

  if(!fHousing_log || fUpdated){
 
    G4double housing_x=fScint_x+fD_mtl;
    G4double housing_y=fScint_y+fD_mtl;
    G4double housing_z=fScint_z+fD_mtl;
 
    //*************************** housing and scintillator
    fScint_box = new G4Box("scint_box",fScint_x/2.,fScint_y/2.,fScint_z/2.);
    fHousing_box = new G4Box("housing_box",housing_x/2.,housing_y/2.,
                            housing_z/2.);
 
    fScint_log = new G4LogicalVolume(fScint_box,G4Material::GetMaterial(""),
                                    "scint_log",0,0,0);
    fHousing_log = new G4LogicalVolume(fHousing_box,
                                      G4Material::GetMaterial("Al"),
                                      "housing_log",0,0,0);
 
    new G4PVPlacement(0,G4ThreeVector(),fScint_log,"scintillator",
                                   fHousing_log,false,0);
 
    //*************** Miscellaneous sphere to demonstrate skin surfaces
    fSphere = new G4Sphere("sphere",0.*mm,2.*cm,0.*deg,360.*deg,0.*deg,
                          360.*deg);
    fSphere_log = new G4LogicalVolume(fSphere,G4Material::GetMaterial("Al"),
                                     "sphere_log");
    if(fSphereOn)
      new G4PVPlacement(0,G4ThreeVector(5.*cm,5.*cm,5.*cm),
                                      fSphere_log,"sphere",fScint_log,false,0);
 
    //****************** Build PMTs
    G4double innerRadius_pmt = 0.*cm;
    G4double height_pmt = fD_mtl/2.;
    G4double startAngle_pmt = 0.*deg;
    G4double spanningAngle_pmt = 360.*deg;
 
    fPmt = new G4Tubs("pmt_tube",innerRadius_pmt,fOuterRadius_pmt,
                     height_pmt,startAngle_pmt,spanningAngle_pmt);
 
    //the "photocathode" is a metal slab at the back of the glass that
    //is only a very rough approximation of the real thing since it only
    //absorbs or detects the photons based on the efficiency set below
    fPhotocath = new G4Tubs("photocath_tube",innerRadius_pmt,fOuterRadius_pmt,
                           height_pmt/2,startAngle_pmt,spanningAngle_pmt);
 
    fPmt_log = new G4LogicalVolume(fPmt,G4Material::GetMaterial("Glass"),
                                  "pmt_log");
    fPhotocath_log = new G4LogicalVolume(fPhotocath,
                                        G4Material::GetMaterial("Al"),
                                        "photocath_log");
 
    new G4PVPlacement(0,G4ThreeVector(0,0,-height_pmt/2),
                                       fPhotocath_log,"photocath",
                                       fPmt_log,false,0);
 
    //***********Arrange pmts around the outside of housing**********
    //---pmt sensitive detector
    G4SDManager* SDman = G4SDManager::GetSDMpointer();

    if(!fPmt_SD){
      fPmt_SD = new PMTSD("/Det/pmtSD");
      SDman->AddNewDetector(fPmt_SD);
      //Created here so it exists as pmts are being placed
    }
    fPmt_SD->InitPMTs((fNx*fNy+fNx*fNz+fNy*fNz)*2); //let pmtSD know # of pmts
    //-------
 
    G4double dx = fScint_x/fNx;
    G4double dy = fScint_y/fNy;
    G4double dz = fScint_z/fNz;
 
    G4double x,y,z;
    G4double xmin = -fScint_x/2. - dx/2.;
    G4double ymin = -fScint_y/2. - dy/2.;
    G4double zmin = -fScint_z/2. - dz/2.;
    G4int k=0;
 
    z = -fScint_z/2. - height_pmt;      //front
    PlacePMTs(fPmt_log,0,x,y,dx,dy,xmin,ymin,fNx,fNy,x,y,z,k,fPmt_SD);
    G4RotationMatrix* rm_z = new G4RotationMatrix();
    rm_z->rotateY(180*deg);
    z = fScint_z/2. + height_pmt;       //back
    PlacePMTs(fPmt_log,rm_z,x,y,dx,dy,xmin,ymin,fNx,fNy,x,y,z,k,fPmt_SD);
 
    G4RotationMatrix* rm_y1 = new G4RotationMatrix();
    rm_y1->rotateY(-90*deg);
    x = -fScint_x/2. - height_pmt;      //left
    PlacePMTs(fPmt_log,rm_y1,y,z,dy,dz,ymin,zmin,fNy,fNz,x,y,z,k,fPmt_SD);
    G4RotationMatrix* rm_y2 = new G4RotationMatrix();
    rm_y2->rotateY(90*deg);
    x = fScint_x/2. + height_pmt;      //right
    PlacePMTs(fPmt_log,rm_y2,y,z,dy,dz,ymin,zmin,fNy,fNz,x,y,z,k,fPmt_SD);
 
    G4RotationMatrix* rm_x1 = new G4RotationMatrix();
    rm_x1->rotateX(90*deg);
    y = -fScint_y/2. - height_pmt;     //bottom
    PlacePMTs(fPmt_log,rm_x1,x,z,dx,dz,xmin,zmin,fNx,fNz,x,y,z,k,fPmt_SD);
    G4RotationMatrix* rm_x2 = new G4RotationMatrix();
    rm_x2->rotateX(-90*deg);
    y = fScint_y/2. + height_pmt;      //top
    PlacePMTs(fPmt_log,rm_x2,x,z,dx,dz,xmin,zmin,fNx,fNz,x,y,z,k,fPmt_SD);
 
    //**********Setup Sensitive Detectors***************
    if(!fScint_SD){//determine if it has already been created
      fScint_SD = new ScintSD("/Det/scintSD");
      SDman->AddNewDetector(fScint_SD);
    }
    fScint_log->SetSensitiveDetector(fScint_SD);
 
    //sensitive detector is not actually on the photocathode.
    //processHits gets done manually by the stepping action.
    //It is used to detect when photons hit and get absorbed&detected at the
    //boundary to the photocathode (which doesnt get done by attaching it to a
    //logical volume.
    //It does however need to be attached to something or else it doesnt get
    //reset at the begining of events
    fPhotocath_log->SetSensitiveDetector(fPmt_SD);

    VisAttributes();
    SurfaceProperties();
  }

  SetLogicalVolume(fHousing_log);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MainVolume::CopyValues(){
  fUpdated=fConstructor->GetUpdated();

  fScint_x=fConstructor->GetScintX();
  fScint_y=fConstructor->GetScintY();
  fScint_z=fConstructor->GetScintZ();
  fD_mtl=fConstructor->GetHousingThickness();
  fNx=fConstructor->GetNX();
  fNy=fConstructor->GetNY();
  fNz=fConstructor->GetNZ();
  fOuterRadius_pmt=fConstructor->GetPMTRadius();
  fSphereOn=fConstructor->GetSphereOn();
  fRefl=fConstructor->GetHousingReflectivity();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MainVolume::PlacePMTs(G4LogicalVolume* pmt_log,
                              G4RotationMatrix *rot,
                              G4double &a, G4double &b, G4double da,
                              G4double db, G4double amin,
                              G4double bmin, G4int na, G4int nb,
                              G4double &x, G4double &y, G4double &z,
                              G4int &k,PMTSD* sd){
/*PlacePMTs : a different way to parameterize placement that does not depend on
  calculating the position from the copy number

  pmt_log = logical volume for pmts to be placed
  rot = rotation matrix to apply
  a,b = coordinates to vary(ie. if varying in the xy plane then pass x,y)
  da,db = value to increment a,b by
  amin,bmin = start values for a,b
  na,nb = number of repitions in a and b
  x,y,z = just pass x,y, and z by reference (the same ones passed for a,b)
  k = copy number to start with
  sd = sensitive detector for pmts
*/
  a=amin;
  for(G4int j=1;j<=na;j++){
    a+=da;
    b=bmin;
    for(G4int i=1;i<=nb;i++){
      b+=db;
      new G4PVPlacement(rot,G4ThreeVector(x,y,z),pmt_log,"pmt",
                        fHousing_log,false,k);
      sd->SetPMTPos(k,x,y,z);
      k++;
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MainVolume::VisAttributes(){
  G4VisAttributes* housing_va = new G4VisAttributes(G4Colour(0.8,0.8,0.8));
  fHousing_log->SetVisAttributes(housing_va);

  G4VisAttributes* sphere_va = new G4VisAttributes();
  sphere_va->SetForceSolid(true);
  fSphere_log->SetVisAttributes(sphere_va);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MainVolume::SurfaceProperties(){
  const G4int num = 2;
  G4double ephoton[num] = {7.0*eV, 7.14*eV};

  //**Scintillator housing properties
  G4double reflectivity[num] = {fRefl, fRefl};
  G4double efficiency[num] = {0.0, 0.0};
  G4MaterialPropertiesTable* scintHsngPT = new G4MaterialPropertiesTable();
  scintHsngPT->AddProperty("REFLECTIVITY", ephoton, reflectivity, num);
  scintHsngPT->AddProperty("EFFICIENCY", ephoton, efficiency, num);
  G4OpticalSurface* OpScintHousingSurface =
    new G4OpticalSurface("HousingSurface",unified,polished,dielectric_metal);
  OpScintHousingSurface->SetMaterialPropertiesTable(scintHsngPT);
 
  //**Sphere surface properties
  G4double sphereReflectivity[num] = {1.0, 1.0};
  G4double sphereEfficiency[num] = {0.0, 0.0};
  G4MaterialPropertiesTable* spherePT = new G4MaterialPropertiesTable();
  spherePT->AddProperty("REFLECTIVITY", ephoton, sphereReflectivity, num);
  spherePT->AddProperty("EFFICIENCY", ephoton, sphereEfficiency, num);
  G4OpticalSurface* OpSphereSurface =
    new G4OpticalSurface("SphereSurface",unified,polished,dielectric_metal);
  OpSphereSurface->SetMaterialPropertiesTable(spherePT);
 
  //**Photocathode surface properties
  G4double photocath_EFF[num]={1.,1.}; //Enables 'detection' of photons
  G4double photocath_ReR[num]={1.92,1.92};
  G4double photocath_ImR[num]={1.69,1.69};
  G4MaterialPropertiesTable* photocath_mt = new G4MaterialPropertiesTable();
  photocath_mt->AddProperty("EFFICIENCY",ephoton,photocath_EFF,num);
  photocath_mt->AddProperty("REALRINDEX",ephoton,photocath_ReR,num);
  photocath_mt->AddProperty("IMAGINARYRINDEX",ephoton,photocath_ImR,num);
  G4OpticalSurface* photocath_opsurf=
    new G4OpticalSurface("photocath_opsurf",glisur,polished,
                         dielectric_metal);
  photocath_opsurf->SetMaterialPropertiesTable(photocath_mt);

  //**Create logical skin surfaces
  new G4LogicalSkinSurface("photocath_surf",fHousing_log,
                           OpScintHousingSurface);
  new G4LogicalSkinSurface("sphere_surface",fSphere_log,OpSphereSurface);
  new G4LogicalSkinSurface("photocath_surf",fPhotocath_log,photocath_opsurf);
}
