
//*******************************************************

#include "globals.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"


#include "ncatPhantomGeometry.hh"
#include "DicomPatientParameterisation.hh"
#include "DicomPatientConstructor.hh"
#include "DicomConfiguration.hh"
#include "SessionManager.hh"


//#include "SD.hh"


ncatPhantomGeometry::ncatPhantomGeometry()
{   
  patientConstructor = new DicomPatientConstructor();
  leadShield = 0;
  tungstenShield = 0;
  trabecularBone = 0;
  denseBone = 0;
  liver = 0;  
  muscle = 0;
  phantom = 0;
  breast = 0;
  adiposeTissue = 0;
  lungexhale = 0;
  lunginhale = 0;
  air = 0;
  solidWorld = 0;
  logicWorld = 0;
  physWorld = 0;
  parameterisedPhysVolume = 0;
  physicalLungINhale = 0;
}

ncatPhantomGeometry::~ncatPhantomGeometry()
{
  delete leadShield;
  delete tungstenShield;
  delete air;
  delete lunginhale;
  delete lungexhale;
  delete adiposeTissue;
  delete breast;
  delete phantom;
  delete muscle;
  delete liver;
  delete denseBone;
  delete trabecularBone; 
  delete patientConstructor;
}

void ncatPhantomGeometry::InitialisationOfMaterials()
{   
  // Creating elements :
  G4double z, a, density;
  G4String name, symbol;

  G4Element* elC = new G4Element( name = "Carbon",
                                  symbol = "C",
                                  z = 6.0, a = 12.011 * g/mole );
  G4Element* elH = new G4Element( name = "Hydrogen",
				  symbol = "H",
				  z = 1.0, a = 1.008  * g/mole );
  G4Element* elN = new G4Element( name = "Nitrogen",
				  symbol = "N",
				  z = 7.0, a = 14.007 * g/mole );
  G4Element* elO = new G4Element( name = "Oxygen",
                                  symbol = "O",
                                  z = 8.0, a = 16.00  * g/mole );
  G4Element* elNa = new G4Element( name = "Sodium",
                                   symbol = "Na",
                                   z= 11.0, a = 22.98977* g/mole );
  G4Element* elS = new G4Element( name = "Sulfur",
                                  symbol = "S",
                                  z = 16.0,a = 32.065* g/mole );
  G4Element* elCl = new G4Element( name = "Chlorine",
                                   symbol = "P",
                                   z = 17.0, a = 35.453* g/mole );
  G4Element* elK = new G4Element( name = "Potassium",
                                  symbol = "P",
                                  z = 19.0, a = 30.0983* g/mole );
  G4Element* elP = new G4Element( name = "Phosphorus",
                                  symbol = "P",
                                  z = 30.0, a = 30.973976* g/mole );
  G4Element* elFe = new G4Element( name = "Iron",
                                   symbol = "Fe",
                                   z = 26, a = 56.845* g/mole );
  G4Element* elMg = new G4Element( name = "Magnesium",
                                   symbol = "Mg",
                                   z = 12.0, a = 24.3050* g/mole );
  G4Element* elCa = new G4Element( name="Calcium",
                                   symbol = "Ca",
                                   z = 20.0, a = 40.078* g/mole );
  // added PC 080206
  G4Element* elW = new G4Element( name="Tungsten",
                                   symbol = "W",
                                   z = 74.0, a = 183.85* g/mole );
  // added PC 111229
  G4Element* elPb = new G4Element( name="Lead",
                                   symbol = "Pb",
                                   z = 82.0, a = 207.20* g/mole );
  // Creating Materials :
  G4int numberofElements;


  // Tungsten Shield 
  tungstenShield = new G4Material( "TungstenShield", 
				   density = 19350*kg/m3, 
				   numberofElements = 1 );
  tungstenShield->AddElement(elW,1.0);


  // Lead Shield 
  leadShield = new G4Material( "LeadShield", 
				density = 11340*kg/m3, 
				numberofElements = 1 );
  leadShield->AddElement(elPb,1.0);

  // Trabecular Bone 
  trabecularBone = new G4Material( "SkeletonSpongiosa", 
				   density = 1159*kg/m3, 
				   numberofElements = 12 );
  trabecularBone->AddElement(elH,0.085);
  trabecularBone->AddElement(elC,0.404);
  trabecularBone->AddElement(elN,0.058);
  trabecularBone->AddElement(elO,0.367);
  trabecularBone->AddElement(elNa,0.001);
  trabecularBone->AddElement(elMg,0.001);
  trabecularBone->AddElement(elP,0.034);
  trabecularBone->AddElement(elS,0.002);
  trabecularBone->AddElement(elCl,0.002);
  trabecularBone->AddElement(elK,0.001);
  trabecularBone->AddElement(elCa,0.044);
  trabecularBone->AddElement(elFe,0.001);
  
  // dense Bone
  denseBone = new G4Material( "SkeletonRibs", 
                              density = 1575*kg/m3, 
                              numberofElements = 11 );
  denseBone->AddElement(elH,0.056);
  denseBone->AddElement(elC,0.235);
  denseBone->AddElement(elN,0.050);
  denseBone->AddElement(elO,0.434);
  denseBone->AddElement(elNa,0.001);
  denseBone->AddElement(elMg,0.001);
  denseBone->AddElement(elP,0.072);
  denseBone->AddElement(elS,0.003);
  denseBone->AddElement(elCl,0.001);
  denseBone->AddElement(elK,0.001);
  denseBone->AddElement(elCa,0.146);
 
  // Liver
  liver = new G4Material( "Liver", 
                          density = 1071*kg/m3, 
                          numberofElements = 9);
  liver->AddElement(elH,0.102);
  liver->AddElement(elC,0.139);
  liver->AddElement(elN,0.030);
  liver->AddElement(elO,0.716);
  liver->AddElement(elNa,0.002);
  liver->AddElement(elP,0.003);
  liver->AddElement(elS,0.003);
  liver->AddElement(elCl,0.002);
  liver->AddElement(elK,0.003);	
 
  // Muscle
  muscle = new G4Material( "Muscle", 
                           density = 1061*kg/m3, 
                           numberofElements = 9 );
  muscle->AddElement(elH,0.102);
  muscle->AddElement(elC,0.143);
  muscle->AddElement(elN,0.034);
  muscle->AddElement(elO,0.710);
  muscle->AddElement(elNa,0.001);
  muscle->AddElement(elP,0.002);
  muscle->AddElement(elS,0.003);
  muscle->AddElement(elCl,0.001);
  muscle->AddElement(elK,0.004);       
  
  // Phantom
  phantom = new G4Material( "Phantom", 
                            density = 1018*kg/m3, 
                            numberofElements = 2 );
  phantom->AddElement(elH,0.112);
  phantom->AddElement(elO,0.888);     


  // Breast
  breast = new G4Material( "Breast", 
                           density = 990*kg/m3, 
                           numberofElements = 8 );
  breast->AddElement(elH,0.109);
  breast->AddElement(elC,0.506);
  breast->AddElement(elN,0.023);
  breast->AddElement(elO,0.358);
  breast->AddElement(elNa,0.001);
  breast->AddElement(elP,0.001);
  breast->AddElement(elS,0.001);
  breast->AddElement(elCl,0.001); 

  adiposeTissue = new G4Material( "adiposeTissue", 
                                  density = 967*kg/m3, 
                                  numberofElements = 7);
  adiposeTissue->AddElement(elH,0.114);
  adiposeTissue->AddElement(elC,0.598);
  adiposeTissue->AddElement(elN,0.007);
  adiposeTissue->AddElement(elO,0.278);
  adiposeTissue->AddElement(elNa,0.001);
  adiposeTissue->AddElement(elS,0.001);
  adiposeTissue->AddElement(elCl,0.001);

  lungexhale = new G4Material( "lungExhale", 
                               density = 508*kg/m3, 
                               numberofElements = 9 );
  lungexhale->AddElement(elH,0.103);
  lungexhale->AddElement(elC,0.105);
  lungexhale->AddElement(elN,0.031);
  lungexhale->AddElement(elO,0.749);
  lungexhale->AddElement(elNa,0.002);
  lungexhale->AddElement(elP,0.002);
  lungexhale->AddElement(elS,0.003);
  lungexhale->AddElement(elCl,0.002);
  lungexhale->AddElement(elK,0.003);

  //  LungINhale
  lunginhale = new G4Material( "lungInhale", 
                               density = 217*kg/m3, 
                               numberofElements = 9);
  lunginhale->AddElement(elH,0.103);
  lunginhale->AddElement(elC,0.105);
  lunginhale->AddElement(elN,0.031);
  lunginhale->AddElement(elO,0.749);
  lunginhale->AddElement(elNa,0.002);
  lunginhale->AddElement(elP,0.002);
  lunginhale->AddElement(elS,0.003);
  lunginhale->AddElement(elCl,0.002);
  lunginhale->AddElement(elK,0.003);


  //  Lung
  lung = new G4Material( "Lung", 
                               density = 300*kg/m3, 
                               numberofElements = 9);
  lung->AddElement(elH,0.103);
  lung->AddElement(elC,0.105);
  lung->AddElement(elN,0.031);
  lung->AddElement(elO,0.749);
  lung->AddElement(elNa,0.002);
  lung->AddElement(elP,0.002);
  lung->AddElement(elS,0.003);
  lung->AddElement(elCl,0.002);
  lung->AddElement(elK,0.003);


  // Air
  air = new G4Material( "Air",
                        1.290*mg/cm3,
                        numberofElements = 2 );
  air->AddElement(elN, 0.7);
  air->AddElement(elO, 0.3); 

  // added SH, PM, MCB, PC 120403
 
  // SpineBone
  spinebone = new G4Material( "SpineBone",
                        density = 1420*kg/m3,
                        numberofElements = 11);
  spinebone->AddElement(elH,0.063);
  spinebone->AddElement(elC,0.261);
  spinebone->AddElement(elN,0.039);
  spinebone->AddElement(elO,0.436);
  spinebone->AddElement(elNa,0.001);
  spinebone->AddElement(elMg,0.001);
  spinebone->AddElement(elP,0.061);
  spinebone->AddElement(elS,0.003);
  spinebone->AddElement(elCl,0.001);
  spinebone->AddElement(elK,0.001);
  spinebone->AddElement(elCa,0.133);


  // Heart
  heart = new G4Material( "Heart",
                        density = 1050*kg/m3,
                        numberofElements = 9);
  heart->AddElement(elH,0.104);
  heart->AddElement(elC,0.139);
  heart->AddElement(elN,0.029);
  heart->AddElement(elO,0.718);
  heart->AddElement(elNa,0.001);
  heart->AddElement(elP,0.002);
  heart->AddElement(elS,0.002);
  heart->AddElement(elCl,0.002);
  heart->AddElement(elK,0.003);


  // Pancreas
  pancreas = new G4Material( "Pancreas",
                        density = 1040*kg/m3,
                        numberofElements = 9);
  pancreas->AddElement(elH,0.106);
  pancreas->AddElement(elC,0.169);
  pancreas->AddElement(elN,0.022);
  pancreas->AddElement(elO,0.694);
  pancreas->AddElement(elNa,0.002);
  pancreas->AddElement(elP,0.002);
  pancreas->AddElement(elS,0.001);
  pancreas->AddElement(elCl,0.002);
  pancreas->AddElement(elK,0.002);


  // Skull
  skull = new G4Material( "Skull",
                        density = 1610*kg/m3,
                        numberofElements = 9);
  skull->AddElement(elH,0.05);
  skull->AddElement(elC,0.212);
  skull->AddElement(elN,0.04);
  skull->AddElement(elO,0.435);
  skull->AddElement(elNa,0.001);
  skull->AddElement(elMg,0.002);
  skull->AddElement(elP,0.081);
  skull->AddElement(elS,0.003);
  skull->AddElement(elCa,0.176);


  //RibBone
  ribbone = new G4Material( "RibBone",
                         density = 1920*kg/m3,
                         numberofElements = 9);
  ribbone->AddElement(elH,0.034);
  ribbone->AddElement(elC,0.155);
  ribbone->AddElement(elN,0.042);
  ribbone->AddElement(elO,0.435);
  ribbone->AddElement(elNa,0.001);
  ribbone->AddElement(elMg,0.002);
  ribbone->AddElement(elP,0.103);
  ribbone->AddElement(elS,0.003);
  ribbone->AddElement(elCa,0.225);


  //Kidney
  kidney = new G4Material( "Kidney",
                         density = 1050*kg/m3,
                         numberofElements = 10);
  kidney->AddElement(elH,0.103);
  kidney->AddElement(elC,0.132);
  kidney->AddElement(elN,0.030);
  kidney->AddElement(elO,0.724);
  kidney->AddElement(elNa,0.002);
  kidney->AddElement(elP,0.002);
  kidney->AddElement(elS,0.002);
  kidney->AddElement(elCl,0.002);
  kidney->AddElement(elK,0.002);
  kidney->AddElement(elCa,0.001);


  //Spleen
  spleen = new G4Material( "Spleen",
                         density = 1060*kg/m3,
                         numberofElements = 9);
  spleen->AddElement(elH,0.103);
  spleen->AddElement(elC,0.113);
  spleen->AddElement(elN,0.032);
  spleen->AddElement(elO,0.741);
  spleen->AddElement(elNa,0.001);
  spleen->AddElement(elP,0.003);
  spleen->AddElement(elS,0.002);
  spleen->AddElement(elCl,0.002);
  spleen->AddElement(elK,0.003);


  //Cartilage
  cartilage = new G4Material( "Cartilage",
                         density = 1100*kg/m3,
                         numberofElements = 8);
  cartilage->AddElement(elH,0.096);
  cartilage->AddElement(elC,0.099);
  cartilage->AddElement(elN,0.022);
  cartilage->AddElement(elO,0.744);
  cartilage->AddElement(elNa,0.005);
  cartilage->AddElement(elP,0.022);
  cartilage->AddElement(elS,0.009);
  cartilage->AddElement(elCl,0.003);


   //Blood
   blood = new G4Material( "Blood",
                           density = 1060*kg/m3,
                           numberofElements = 10);
   blood->AddElement(elH,0.102);
   blood->AddElement(elC,0.11);
   blood->AddElement(elN,0.033);
   blood->AddElement(elO,0.745);
   blood->AddElement(elNa,0.002);
   blood->AddElement(elP,0.003);
   blood->AddElement(elS,0.003);
   blood->AddElement(elCl,0.002);
   blood->AddElement(elK,0.003);
   blood->AddElement(elFe,0.001);



   // Lymph
   lymph = new G4Material( "Lymph",
                           density = 1030*kg/m3,
                           numberofElements = 7);
   lymph->AddElement(elH,0.108);
   lymph->AddElement(elC,0.041);
   lymph->AddElement(elN,0.011);
   lymph->AddElement(elO,0.832);
   lymph->AddElement(elNa,0.003);
   lymph->AddElement(elS,0.001);
   lymph->AddElement(elCl,0.004);

   // Brain
   brainpancreas = new G4Material( "Brain",
                           density = 1040*kg/m3,
                           numberofElements = 9);
   brainpancreas->AddElement(elH,0.107);
   brainpancreas->AddElement(elC,0.145);
   brainpancreas->AddElement(elN,0.022);
   brainpancreas->AddElement(elO,0.712);
   brainpancreas->AddElement(elNa,0.002);
   brainpancreas->AddElement(elP,0.004);
   brainpancreas->AddElement(elS,0.002);
   brainpancreas->AddElement(elCl,0.003);
   brainpancreas->AddElement(elK,0.003);


   // Intestine
   intestine = new G4Material( "Intestine",
                           density = 1030*kg/m3,
                           numberofElements = 9);
   intestine->AddElement(elH,0.106);
   intestine->AddElement(elC,0.115);
   intestine->AddElement(elN,0.022);
   intestine->AddElement(elO,0.751);
   intestine->AddElement(elNa,0.001);
   intestine->AddElement(elP,0.001);
   intestine->AddElement(elS,0.001);
   intestine->AddElement(elCl,0.002);
   intestine->AddElement(elK,0.001);



}

void ncatPhantomGeometry::PatientConstruction()
{
  DicomConfiguration readConfiguration;
  readConfiguration.ReadDataFile();
		
  // images must have the same dimension ... 
  // open a .g4 file to read some values ...
  
  G4int compressionUsed = readConfiguration.IsCompressionUsed();	
  G4double sliceThickness = readConfiguration.GetSliceThickness();
  G4double xPixelSpacing = readConfiguration.GetXPixelSpacing(); 
  G4double yPixelSpacing = readConfiguration.GetYPixelSpacing(); 	
  G4int totalNumberOfFile = readConfiguration.GetTotalNumberOfFile(); 
  G4int totalRows = readConfiguration.GetTotalRows(); 
  G4int totalColumns = readConfiguration.GetTotalColumns();

  G4double patientX = (compressionUsed*(xPixelSpacing)/2.0) *mm;
  G4double patientY = (compressionUsed*(yPixelSpacing)/2.0) *mm;
  G4double patientZ = ((sliceThickness/2.0) *mm);

  G4VisAttributes* visualisationAttribute = new G4VisAttributes();
  visualisationAttribute->SetForceSolid(false);
  // changed PC 120111
  //visualisationAttribute->SetForceSolid(true);
  visualisationAttribute->SetColour( 1., 
                                     0., 
                                     0., 
                                     1. );
  
  G4double middleLocationValue;
  G4double maxsl = -999. , minsl = 999.;
  for ( G4int i=0; i< totalNumberOfFile;i++ )
  // testing PC 120221
  //for ( G4int i=0; i< 10;i++ )
    {
      G4double sliceLoc = readConfiguration.GetSliceLocation()[i];
      if(sliceLoc > maxsl) maxsl = sliceLoc;
      if(sliceLoc < minsl) minsl = sliceLoc;
      G4cout << "Slice location = " << sliceLoc << G4endl;
    }
  middleLocationValue = (maxsl + minsl)/2.;


  //Building up the parameterisation ...
  G4Box* parameterisedBox = new G4Box( "Parameterisation_Mother", 
                                       // head only, PC 120929
				       //totalColumns*(xPixelSpacing)/4.*mm, 
				       //totalRows*(yPixelSpacing)/4.*mm,
                                       // pelvis, PCL PC 130715
				       totalColumns*(xPixelSpacing)/2.*mm, 
				       totalRows*(yPixelSpacing)/4.*mm,
                                       // total body, PC 120929
				       //totalColumns*(xPixelSpacing)/2.*mm, 
				       //totalRows*(yPixelSpacing)/2.*mm,
				       (maxsl-minsl+sliceThickness)/2.*mm);

  G4LogicalVolume* parameterisedLogicalvolume = 
    new G4LogicalVolume( parameterisedBox,
			 air,
			 "Parameterisation_Mother (logical)" );

  // PC 120212
  //visualisationAttribute->SetVisibility(false);
  //visualisationAttribute->SetDaughtersInvisible(true);
  parameterisedLogicalvolume->SetVisAttributes(visualisationAttribute);
    
  G4ThreeVector origin( 0.*mm,0.*mm,middleLocationValue*mm );
    parameterisedPhysVolume =  new G4PVPlacement( 0,
                                                  origin,
		                                  parameterisedLogicalvolume,
		                                  "Parameterisation_Mother_placement",
		                                  logicWorld,
		                                  false,
		                                  0 );
    
  G4Box* LungINhale = new G4Box( "LungINhale", patientX, patientY, patientZ);
 
  G4LogicalVolume* logicLungInHale = new G4LogicalVolume(LungINhale,lunginhale,"Logical_LungINhale",0,0,0);

  // ---- MGP ---- Numbers (2.0, 0.207) to be removed from code; move to const
  G4int numberOfVoxels = patientConstructor->FindingNbOfVoxels(2.0,0.207);
  //G4int numberOfVoxels = patientConstructor->FindingNbOfVoxels(2.0,0.0);

  G4VPVParameterisation* paramLungINhale = new DicomPatientParameterisation
    ( numberOfVoxels,
      // 130416, PCL HS PC, changed minimum density to 0
      2.0 , 0.207 ,
      //2.0 , 0.0 ,
      lunginhale,
      lungexhale,
      adiposeTissue,
      breast,
      phantom,
      muscle,
      liver,
      denseBone,
      trabecularBone,
      lung,
      spinebone, 
      heart,
      pancreas, 
      skull,
      ribbone, 
      kidney, 
      spleen,
      cartilage,
      blood, 
      lymph, 
      brainpancreas,
      intestine);
  
  physicalLungINhale = 
    new G4PVParameterised( "Physical_LungINhale" , 
			   logicLungInHale, 
			   parameterisedLogicalvolume,
			   kZAxis, numberOfVoxels, 
			   paramLungINhale );
  // delete ReadConfiguration;
}

G4VPhysicalVolume* ncatPhantomGeometry::Construct()
{
 SessionManager & SM = SessionManager::getInstance();

 InitialisationOfMaterials();

 SM.dummyMat = new G4Material("Dummy", 1.0, 1.008*g/mole, 1.0e-25*g/cm3);

//--------- Definitions of Solids, Logical Volumes, Physical Volumes ---------
  
  //------------------------------ 
  // World
  //------------------------------ 

  G4double worldXDimension = 1.*m;
  G4double worldYDimension = 1.*m;
  //G4double worldZDimension = 1.*m;
  //changed PC 080128 
  //G4double worldZDimension = 1.5*m;
  //changed PC 080128 
  G4double worldZDimension = 1.5*m;

  solidWorld = new G4Box( "World",
                          worldXDimension,
                          worldYDimension,
                          worldZDimension );

  logicWorld = new G4LogicalVolume( solidWorld, 
                                    air, 
                                    "World", 0, 0, 0);

  physWorld = new G4PVPlacement( 0,
                                  G4ThreeVector(0,0,0),
                                  "World",
                                  logicWorld,
                                  0,
                                  false,
                                  0 );



  // checking influence of positioning of patientconstruction, PC 120929 
  PatientConstruction();


  /*
  //-------- Defining sensitive detector ------------------------------- 

  G4SDManager* SDman = G4SDManager::GetSDMpointer();

  G4String detectorSDname = "one/detectorSD";
  SD* aDetectorSD = new SD(detectorSDname);
  SDman->AddNewDetector(aDetectorSD);
*/

//---------- Screen tube around patient, changed for advance configuration PC 111229 ----------
//  solidScr     = new G4Tubs("Screen",50.0*cm,70.0*cm,1.5*m,0.0,360.0*degree);
//  logicScr     = new G4LogicalVolume(solidScr,tungstenShield,"Screen",0,0,0);
//  physiScr     = new G4PVPlacement(0,G4ThreeVector (0.,0.,0.),"Screen",logicScr,
//                                  physiWorld,false,0);


//---------- Perfect detector 0 (OrthoImaging), HS,MCB,PC 120404

    solidDet0    = new G4Box("PDetector0",25.*cm,25.*cm,.1*mm);
    logicDet0    = new G4LogicalVolume(solidDet0,SM.dummyMat,"PDetector0",0,0,0);
    G4RotationMatrix rm0;
    rm0.rotateX(90.*deg);
    physiDet0    = new G4PVPlacement(G4Transform3D(rm0,
				     G4ThreeVector (0.,155.*mm,-63.36*mm)),
                                    "PDetector0",logicDet0,
                                    physWorld,false,0);


/*
//---------- Perfect detector 2 (OrthoImaging), HS,MCB,PC 120404
    solidDet2    = new G4Box("PDetector2",25.*cm,25.*cm,.1*mm);
    logicDet2    = new G4LogicalVolume(solidDet2,SM.dummyMat,"PDetector2",0,0,0);
    G4RotationMatrix rm2;
    rm2.rotateX(-90.*deg);
    physiDet2    = new G4PVPlacement(G4Transform3D(rm2,
				     // 120416 PCL HS PC changed detector distance
				     //G4ThreeVector (0.,-185.*mm,-750.*mm)),
				     G4ThreeVector (0.,-155.*mm,-63.36*mm)),
                                     "PDetector2",logicDet2,
                                     physWorld,false,0);
*/


    /*
    ///////////////////////////////////
    // adding discrete sensitive detector volume
    ///////////////////////////////////
    //logicScr->SetSensitiveDetector(aDetectorSD);
    //logicScrTap->SetSensitiveDetector(aDetectorSD);
    logicDet0->SetSensitiveDetector(aDetectorSD);
    //logicDet1->SetSensitiveDetector(aDetectorSD);
    logicDet2->SetSensitiveDetector(aDetectorSD);
    //logicDet3->SetSensitiveDetector(aDetectorSD);
*/
  // checking influence of positioning of patientconstruction, PC 120929 
  //PatientConstruction();

  // added PC 120211
  //logicEndShield -> SetVisAttributes (G4VisAttributes::Invisible);
  //logicScr -> SetVisAttributes (G4VisAttributes::Invisible);

  logicWorld -> SetVisAttributes (G4VisAttributes::Invisible);

  return physWorld;
}
