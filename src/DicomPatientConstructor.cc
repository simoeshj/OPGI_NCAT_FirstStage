

//*******************************************************
//
// DicomPatientConstructor.cc :
//	- Initialisation of the construction of DICM images
//	- Reading contour information included in Plan.roi
//	  (Region of interest) *** NOT FULLY WORKING YET ***
//	- Definitions are in DicomGeometry.hh
//
//*******************************************************

#include "DicomConfiguration.hh"
#include "DicomPatientConstructor.hh"

#include "globals.hh"

G4int DicomPatientConstructor::FindingNbOfVoxels(G4double maxDensity , G4double minDensity) {

    DicomConfiguration dicomConfiguration;
    
    G4double density;
    G4int npass = 0;

    G4int npixels = dicomConfiguration.GetTotalPixels();
    for(int i = 0; i < npixels; i++) {
	density = dicomConfiguration.GetDensityValue(i);
	if(density >= minDensity && density <= maxDensity) npass++;
    }

    return npass;

}
 
