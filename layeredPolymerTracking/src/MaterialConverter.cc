#include "MaterialConverter.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4ElementVector.hh"
#include "G4Element.hh"
#include "G4IsotopeVector.hh"
#include "G4Isotope.hh"

#include <iomanip>

using namespace std;
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