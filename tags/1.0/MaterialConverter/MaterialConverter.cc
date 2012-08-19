#include "MaterialConverter.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4ElementVector.hh"
#include "G4Element.hh"
#include "G4IsotopeVector.hh"
#include "G4Isotope.hh"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

int main(){

	G4String name, symbol;             // a=mass of a mole;
	G4double a, z, density;            // z=mean number of protons;  
	G4int iz, n;                       // iz=nb of protons  in an isotope; 
                                   // n=nb of nucleons in an isotope;
	G4int nAtoms;
	G4int nComponents;
	
	// NIST Material Manager
	G4NistManager* nistManager = G4NistManager::Instance();
	G4bool fromIsotopes = true;

	// Getting Elements
	G4Element* eH = nistManager->FindOrBuildElement("H",fromIsotopes);
	G4Element* eC = nistManager->FindOrBuildElement("C",fromIsotopes);
	G4Element* eO = nistManager->FindOrBuildElement("O",fromIsotopes);
	G4Element* eN = nistManager->FindOrBuildElement("N",fromIsotopes);
	G4Element* eF = nistManager->FindOrBuildElement("F",fromIsotopes);
	G4Element* eGd = nistManager->FindOrBuildElement("Gd",fromIsotopes);

	// defining enriched Lithium 6
	G4double a6Li = 6.015*g/mole;	// Molar Masses (Wolfram Alpha)
	G4double a7Li = 7.016*g/mole;
	G4double enrichement = 110.815*perCent;
	G4double abundance6Li = enrichement*a6Li/a7Li;		// Relative Abudadance
	G4double abundance7Li = 1-abundance6Li;
	
	G4Isotope* Li6 = new G4Isotope(name="Li6", iz=3, n=6, a6Li);
	G4Isotope* Li7 = new G4Isotope(name="Li7", iz=3, n=7, a7Li);
	G4Element* enrichLi  = new G4Element(name="enriched Lithium", symbol="Li", nComponents=2);
	enrichLi->AddIsotope(Li6, abundance6Li);
	enrichLi->AddIsotope(Li7, abundance7Li);

	// Defining 6LiF Absorber
	G4Material* LiAbsorber = new G4Material("6LiF",density=1.0*g/cm3,nComponents=2);
	LiAbsorber->AddElement(enrichLi,nAtoms=1);
	LiAbsorber->AddElement(eF,nAtoms=1);

	// PPO C15H11NO
	G4Material* PPO = new G4Material("PPO",density=1.1*g/cm3,nComponents=4,kStateSolid);
	PPO->AddElement(eC,nAtoms=15);
	PPO->AddElement(eH,nAtoms=11);
	PPO->AddElement(eO,nAtoms=1);
	PPO->AddElement(eN,nAtoms=1);

	// POPOP C24H15N2O2
	G4Material* POPOP = new G4Material("POPOP",density=1.1*g/cm3,nComponents=4,kStateSolid);
	POPOP->AddElement(eC,nAtoms=24);
	POPOP->AddElement(eH,nAtoms=15);
	POPOP->AddElement(eO,nAtoms=2);
	POPOP->AddElement(eN,nAtoms=2);

	// Scintillant
	G4double fractionPPO = 46./(46.+1.36);		// Scintillant is in the ratio of 46 g PPO to 1.36 g POPOP
	G4Material* scintillant = new G4Material("PPO/POPOP",density=1.1*g/cm3,nComponents=2,kStateSolid);
	scintillant->AddMaterial(PPO,fractionPPO);
	scintillant->AddMaterial(POPOP,1-fractionPPO);

	// Polymer PS Based Detector
	G4double fractionPolymer = 0.55;
	G4double fractionScintillant = 0.05;
	G4double fractionAbsorber = 0.40;
    G4cout<<"PS "<<fractionPolymer*100<<"%% LiF "<<fractionAbsorber*100<<"%% Fluor "<<fractionScintillant*100<<"%%"<<G4endl; 
	G4Material* psDetector = new G4Material("PS_Detector",density=1.1*g/cm3,nComponents=3,kStateSolid);
	psDetector->AddMaterial(nistManager->FindOrBuildMaterial("G4_POLYSTYRENE",fromIsotopes),fractionPolymer);
	psDetector->AddMaterial(scintillant,fractionScintillant);
	psDetector->AddMaterial(LiAbsorber,fractionAbsorber);

    // Polymer Based PEN Detector
    G4Material* PEN = new G4Material("PEN",density=1.1*g/cm3,nComponents=3,kStateSolid);
    PEN->AddElement(eC,nAtoms=14);
    PEN->AddElement(eH,nAtoms=10);
    PEN->AddElement(eO,nAtoms=4);
    G4cout<<"PEN "<<fractionPolymer*100<<"%% LiF "<<fractionAbsorber*100<<"%% Fluor "<<fractionScintillant*100<<"%%"<<G4endl; 
	G4Material* penDetector = new G4Material("PEN_Detector",density=1.1*g/cm3,nComponents=3,kStateSolid);
	penDetector->AddMaterial(PEN,fractionPolymer);
	penDetector->AddMaterial(scintillant,fractionScintillant);
	penDetector->AddMaterial(LiAbsorber,fractionAbsorber);
	
	MaterialConverter converter;
	converter.ConvertToMCNP();	
}


MaterialConverter::MaterialConverter() {}
void MaterialConverter::ConvertToMCNP(){
	const G4MaterialTable *matTable = G4Material::GetMaterialTable();

	size_t nmat = matTable->size();
	size_t matItter;
	for(matItter=0; matItter<nmat; matItter++){

		G4Material* mat = (*matTable)[matItter];
		G4cout<<mat->GetName()<<" density: "<<mat->GetDensity()/(g/cm3)<<"g/cm3"<<endl;
		const G4ElementVector* elements = mat->GetElementVector();
		const G4double *matFraction = mat->GetFractionVector();
		size_t nElem = mat->GetNumberOfElements();
		size_t elemItter;
		for(elemItter=0; elemItter<nElem; elemItter++){
			G4Element* e = (*elements)[elemItter];
			G4double* relAbudance = e->GetRelativeAbundanceVector();
			size_t nIso = e->GetNumberOfIsotopes();
			size_t isoItter;
			const G4IsotopeVector *isotopes = e->GetIsotopeVector();
			for(isoItter = 0; isoItter < nIso; isoItter++){
				G4Isotope* iso = (*isotopes)[isoItter];
				// Format is Name, ZZZAAA, prececnet fraction by mass
				G4cout<<iso->GetName()<<"\t"<<iso->GetZ()<<setfill('0')<<setw(3)<<iso->GetN()<<"\t-"<<relAbudance[isoItter]*matFraction[elemItter]<<endl;
			}
		}
	}
}

MaterialConverter::~MaterialConverter(){

}
