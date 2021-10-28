
//*******************************************************
//
// DicomGeometry.hh :
//	- Start the building of the geometry
//	- Creation of the world and other "mother"(middle) volume
//	- Initialisation of patient geometry
//	- Initialisation of HeaD geometry
// 	- Functions are in DicomGeometry.cc, PatientConstructor.cc
//*******************************************************

#ifndef ncatPhantomGeometry_h
#define ncatPhantomGeometry_h_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"

class DicomConfiguration;
class DicomPatientConstructor;
class G4Material;
class G4LogicalVolume;
class G4PhysicalVolume;
class G4Box;
class G4Tubs;

class ncatPhantomGeometry : public G4VUserDetectorConstruction
{
public:

  ncatPhantomGeometry();
  ~ncatPhantomGeometry();

  G4VPhysicalVolume* Construct();

private:

  void InitialisationOfMaterials();
  void PatientConstruction();
 
  DicomPatientConstructor* patientConstructor;

  //Materials ...
 
  G4Material* leadShield; 
  G4Material* tungstenShield; 
  G4Material* trabecularBone; 
  G4Material* denseBone;
  G4Material* liver; 
  G4Material* muscle; 
  G4Material* phantom; 
  G4Material* breast; 
  G4Material* adiposeTissue; 
  G4Material* lungexhale; 
  G4Material* lunginhale;
  G4Material* air; 
  G4Material* spinebone; 
  G4Material* heart; 
  G4Material* pancreas; 
  G4Material* skull; 
  G4Material* ribbone; 
  G4Material* kidney; 
  G4Material* spleen; 
  G4Material* cartilage; 
  G4Material* blood; 
  G4Material* lymph; 
  G4Material* brainpancreas; 
  G4Material* intestine; 
  G4Material* lung; 

  // World ...

  G4Box* solidWorld;
  G4LogicalVolume* logicWorld;
  G4VPhysicalVolume* physWorld;
  G4VPhysicalVolume* parameterisedPhysVolume;  
  G4VPhysicalVolume*  physicalLungINhale;

  // Screen   
  G4Tubs*            solidScr;   // pointer to the solid screen
  G4LogicalVolume*   logicScr;
  G4VPhysicalVolume* physiScr;

  // Perfect detectors
  G4Box*             solidDet0;   // pointer to the perfect detector 0
  G4LogicalVolume*   logicDet0;
  G4VPhysicalVolume* physiDet0;
  G4Box*             solidDet1;   // pointer to the perfect detector 1
  G4LogicalVolume*   logicDet1;
  G4VPhysicalVolume* physiDet1;
  G4Box*             solidDet2;   // pointer to the perfect detector 2
  G4LogicalVolume*   logicDet2;
  G4VPhysicalVolume* physiDet2;
  G4Box*             solidDet3;   // pointer to the perfect detector 3
  G4LogicalVolume*   logicDet3;
  G4VPhysicalVolume* physiDet3;

  // End shield   
  G4Tubs*            solidEndShield;   // pointer to the solid screen
  G4LogicalVolume*   logicEndShield;
  G4VPhysicalVolume* physiEndShield;

  // Screen taps
  G4Tubs* solidScrTap;
  G4LogicalVolume*   logicScrTap;
  G4VPhysicalVolume* physiScrTapL;
  G4VPhysicalVolume* physiScrTapR;
};

#endif

