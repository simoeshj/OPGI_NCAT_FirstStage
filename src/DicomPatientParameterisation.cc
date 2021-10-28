

#include "DicomPatientParameterisation.hh"
#include "DicomConfiguration.hh"

#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"

DicomPatientParameterisation::DicomPatientParameterisation(G4int, // NoVoxels, 
							   G4double maxDensity, 
							   G4double minDensity ,
							   G4Material* lunginhale,
							   G4Material* lungexhale,
							   G4Material* adipose,
							   G4Material* breast,
							   G4Material* phantom,
							   G4Material* muscle,
							   G4Material* liver,
							   G4Material* denseBone,
							   G4Material* trabecularBone,
                                                           G4Material* lung,
                                                           G4Material* spinebone, 
                                                           G4Material* heart,
                                                           G4Material* pancreas, 
                                                           G4Material* skull,
                                                           G4Material* ribbone, 
                                                           G4Material* kidney, 
                                                           G4Material* spleen,
                                                           G4Material* cartilage, 
                                                           G4Material* blood, 
                                                           G4Material* lymph, 
                                                           G4Material* brainpancreas,
                                                           G4Material* intestine) { 

    lungExhale           = lungexhale;
    lungInhale           = lunginhale;
    adiposeTissue        = adipose;
    breastTissue         = breast;
    phantomTissue        = phantom;
    muscleTissue         = muscle;
    liverTissue          = liver;
    denseBoneTissue      = denseBone;
    trabecularBoneTissue = trabecularBone;
    lungTissue           = lung;
    spineboneTissue      = spinebone; 
    heartTissue          = heart;
    pancreasTissue       = pancreas; 
    skullTissue          = skull;
    ribboneTissue        = ribbone; 
    kidneyTissue         = kidney; 
    spleenTissue         = spleen;
    cartilageTissue      = cartilage; 
    bloodTissue          = blood; 
    lymphTissue          = lymph; 
    brainpancreasTissue  = brainpancreas;
    intestineTissue      = intestine;
 
    density.clear();

    DicomConfiguration dicomConfiguration;

    // images must have the same dimension
    G4int totalNumberOfFile = dicomConfiguration.GetTotalNumberOfFile();
 
    G4double maxsl = -999. , minsl = 999.;
    for( G4int i = 0; i < totalNumberOfFile; i++) {
	G4double sliceLocation = dicomConfiguration.GetSliceLocation()[i];
	if(sliceLocation > maxsl) maxsl = sliceLocation;
	if(sliceLocation < minsl) minsl = sliceLocation;
    }
    middleLocationValue = (maxsl + minsl)*0.5;  

    //
    readColorChart();


    G4double alpha;

    attributeLungINhale = new G4VisAttributes;
    attributeLungINhale->SetColour(getChartColor(ColorChart::CRED, 0.217),
				   getChartColor(ColorChart::CGREEN, 0.217),
				   getChartColor(ColorChart::CBLUE, 1.217),
				   alpha=1.);
    attributeLungINhale->SetForceSolid(true);

    attributeLung = new G4VisAttributes;
    attributeLung->SetColour(getChartColor(ColorChart::CRED, 1.0),
                             getChartColor(ColorChart::CGREEN, 1.0),
                             getChartColor(ColorChart::CBLUE, 0.88),
                             alpha=1.);
    attributeLung->SetForceSolid(true);

    attributeLungEXhale = new G4VisAttributes;
    attributeLungEXhale->SetColour(getChartColor(ColorChart::CRED, 1.508),
				   getChartColor(ColorChart::CGREEN, 0.508),
				   getChartColor(ColorChart::CBLUE, 0.508),
				   alpha=1.);
    attributeLungEXhale->SetForceSolid(true);

    attributeAdipose = new G4VisAttributes;
    attributeAdipose->SetColour(getChartColor(ColorChart::CRED, 0.967),
				getChartColor(ColorChart::CGREEN, 2.967),
				getChartColor(ColorChart::CBLUE, 0.967),
				alpha=1.);
    attributeAdipose->SetForceSolid(true);

    attributeBreast = new G4VisAttributes;
    attributeBreast->SetColour(getChartColor(ColorChart::CRED, 0.99),
			       getChartColor(ColorChart::CGREEN, 0.99),
			       getChartColor(ColorChart::CBLUE, 0.99),
			       alpha=1.);
    attributeBreast->SetForceSolid(true);

    attributePhantom = new G4VisAttributes;
    attributePhantom->SetColour(getChartColor(ColorChart::CRED, 1.018),
				getChartColor(ColorChart::CGREEN, 1.018),
				getChartColor(ColorChart::CBLUE, 2.018),
				alpha=1.);
    attributePhantom->SetForceSolid(true);

    attributeIntestine = new G4VisAttributes;
    attributeIntestine->SetColour(getChartColor(ColorChart::CRED, 2.018),
		   		  getChartColor(ColorChart::CGREEN, 1.018),
				  getChartColor(ColorChart::CBLUE, 1.018),
				  alpha=1.);
    attributeIntestine->SetForceSolid(true);

    attributeBrainPancreas = new G4VisAttributes;
    attributeBrainPancreas->SetColour(getChartColor(ColorChart::CRED, 1.05),
		   	     	      getChartColor(ColorChart::CGREEN, 1.05),
				      getChartColor(ColorChart::CBLUE, 1.05),
				  alpha=1.);
    attributeBrainPancreas->SetForceSolid(true);


    attributeMuscle = new G4VisAttributes;
    attributeMuscle->SetColour(getChartColor(ColorChart::CRED, 2.061),
			       getChartColor(ColorChart::CGREEN, 1.061),
			       getChartColor(ColorChart::CBLUE, 1.061),
			       alpha=1.);
    attributeMuscle->SetForceSolid(true);


    attributeLiver = new G4VisAttributes;
    attributeLiver->SetColour(getChartColor(ColorChart::CRED, 1.1),
			      getChartColor(ColorChart::CGREEN, 1.1),
			      getChartColor(ColorChart::CBLUE, 1.1),
			      alpha=1.);
    attributeLiver->SetForceSolid(true);

    attributeTrabecularBone = new G4VisAttributes;
    attributeTrabecularBone->SetColour(getChartColor(ColorChart::CRED, 1.159),
				       getChartColor(ColorChart::CGREEN, 1.159),
				       getChartColor(ColorChart::CBLUE, 1.159),
				       alpha=1.);
    attributeTrabecularBone->SetForceSolid(true);

    attributeDenseBone = new G4VisAttributes;
    attributeDenseBone->SetColour(getChartColor(ColorChart::CRED, 1.575),
				  getChartColor(ColorChart::CGREEN, 1.575),
				  getChartColor(ColorChart::CBLUE, 1.575),
				  alpha=1.);
    attributeDenseBone->SetForceSolid(true);

    attributeCartilage = new G4VisAttributes;
    attributeCartilage->SetColour(getChartColor(ColorChart::CRED, 1.2),
			          getChartColor(ColorChart::CGREEN, 1.2),
			          getChartColor(ColorChart::CBLUE,1.2),
			          alpha=1.);
    attributeCartilage->SetForceSolid(true);

    attributeSpineBone = new G4VisAttributes;
    attributeSpineBone->SetColour(getChartColor(ColorChart::CRED, 1.42),
		       	          getChartColor(ColorChart::CGREEN, 1.42),
			          getChartColor(ColorChart::CBLUE,1.42),
			          alpha=1.);
    attributeSpineBone->SetForceSolid(true);
  
    attributeSkull = new G4VisAttributes;
    attributeSkull->SetColour(getChartColor(ColorChart::CRED, 1.61),
			    getChartColor(ColorChart::CGREEN, 1.61),
			    getChartColor(ColorChart::CBLUE,1.61),
			    alpha=1.);
    attributeSkull->SetForceSolid(true);
  
    attributeRibBone = new G4VisAttributes;
    attributeRibBone->SetColour(getChartColor(ColorChart::CRED, 1.92),
		 	        getChartColor(ColorChart::CGREEN, 1.92),
			        getChartColor(ColorChart::CBLUE,1.92),
			        alpha=1.);
    attributeRibBone->SetForceSolid(true);
  
    attributeAir = new G4VisAttributes;
    attributeAir->SetColour(getChartColor(ColorChart::CRED, 0.),
			    getChartColor(ColorChart::CGREEN, 0.),
			    getChartColor(ColorChart::CBLUE,0.),
			    alpha=1.);
    attributeAir->SetForceSolid(true);
  

    rows           = dicomConfiguration.GetTotalRows();
    columns        = dicomConfiguration.GetTotalColumns();
    compression    = dicomConfiguration.GetCompressionValue();
    max            = dicomConfiguration.GetTotalNumberOfFile();
    pixelSpacingX  = dicomConfiguration.GetXPixelSpacing();
    pixelSpacingY  = dicomConfiguration.GetYPixelSpacing();
    sliceThickness = dicomConfiguration.GetSliceThickness();

    GetDensity( maxDensity , minDensity );

    dicomConfiguration.ClearDensityData();
}

DicomPatientParameterisation::~DicomPatientParameterisation() {

    // visualisation attributes ...
    delete attributeAdipose;
    delete attributeLungEXhale;
    delete attributeBreast;
    delete attributePhantom;
    delete attributeMuscle;
    delete attributeLiver;
    delete attributeTrabecularBone;
    delete attributeLungINhale;
    delete attributeDenseBone;
    delete attributeAir;
  
    // materials ...
    delete trabecularBoneTissue;
    delete denseBoneTissue;
    delete liverTissue;
    delete muscleTissue;

    delete breastTissue; 
    delete adiposeTissue;
    delete lungInhale;
    delete lungExhale;
    
}

void DicomPatientParameterisation::ComputeTransformation(const G4int copyNo, G4VPhysicalVolume* physVol) const
{
    G4double originZ = patientPlacementZ[copyNo]*mm-middleLocationValue*mm-sliceThickness/2;
    G4ThreeVector origin( patientPlacementX[copyNo]*mm, 
			  patientPlacementY[copyNo]*mm, 
			  originZ*mm );

    physVol->SetTranslation(origin);
}

void DicomPatientParameterisation::ComputeDimensions(G4Box& voxels, const G4int, const G4VPhysicalVolume*) const
{
    voxels.SetXHalfLength((pixelSpacingX * compression/2.0) * mm);
    voxels.SetYHalfLength((pixelSpacingY * compression/2.0) * mm);
    voxels.SetZHalfLength((sliceThickness / 2.0) * mm);
}

G4Material*
DicomPatientParameterisation::ComputeMaterial(const G4int copyNo,
                                              G4VPhysicalVolume* physVol,
                                              const G4VTouchable*)
{
    // added PC 080130
    //std::printf("Density (%d) = %f\n",copyNo,density[copyNo]);
    // changed for checking HS,PM,MCB,PC, 120403
    static float minDensity = 999.;
    static float maxDensity = -999.;
    if (density[copyNo] < minDensity) minDensity = density[copyNo];
    if (density[copyNo] > maxDensity) maxDensity = density[copyNo];
    //std::printf("Min/max density (%d) = %f %f\n\n",copyNo,minDensity,maxDensity);

    if( density[copyNo] >= 0.207 && density[copyNo] <= 0.227 ) {
	physVol->SetName("PhysicalLungINhale");
	physVol->GetLogicalVolume()->SetVisAttributes( attributeLungINhale );
	return lungInhale;
    
    } else if( density[copyNo] >= 0.227 && density[copyNo] < 0.481 ) {
	physVol->SetName("Lung");
	physVol->GetLogicalVolume()->SetVisAttributes( attributeLung );
	return lungTissue;

    } else if( density[copyNo] >= 0.481 && density[copyNo] <= 0.534 ) {
	physVol->SetName("PhysicalLungEXhale");
	physVol->GetLogicalVolume()->SetVisAttributes( attributeLungEXhale );
	return lungExhale;

    } else if( density[copyNo] >= 0.919 && density[copyNo] <= 0.979 ) {
	physVol->SetName("PhysicalAdipose");
	physVol->GetLogicalVolume()->SetVisAttributes( attributeAdipose );
	return adiposeTissue;

    } else if( density[copyNo] > 0.979 && density[copyNo] < 1.03 ) {
	physVol->SetName("PhysicalPhantom");
	physVol->GetLogicalVolume()->SetVisAttributes( attributePhantom );
	return phantomTissue;

    } else if( density[copyNo] >= 1.03 && density[copyNo] < 1.04 ) {
	physVol->SetName("PhysicalIntestine");
	physVol->GetLogicalVolume()->SetVisAttributes( attributeIntestine );
	return intestineTissue;

    } else if( density[copyNo] >= 1.04 && density[copyNo] < 1.05 ) {
	physVol->SetName("PhysicalBrainPancreas");
	physVol->GetLogicalVolume()->SetVisAttributes( attributeBrainPancreas );
	return brainpancreasTissue;

    } else if( density[copyNo] >= 1.05 && density[copyNo] < 1.06 ) {
	physVol->SetName("PhysicalMuscle");
	physVol->GetLogicalVolume()->SetVisAttributes( attributeMuscle );
	return muscleTissue;

    } else if( density[copyNo] >= 1.06 && density[copyNo] < 1.1 ) {
	physVol->SetName("PhysicalLiver");
	physVol->GetLogicalVolume()->SetVisAttributes( attributeLiver );
	return liverTissue;

    } else if( density[copyNo] >= 1.1 && density[copyNo] < 1.42 ) {
	physVol->SetName("PhysicalCartilage");
	physVol->GetLogicalVolume()->SetVisAttributes( attributeCartilage );
	return cartilageTissue;

    } else if( density[copyNo] >= 1.42 && density[copyNo] < 1.61 ) {
	physVol->SetName("PhysicalSpineBone");
	physVol->GetLogicalVolume()->SetVisAttributes( attributeSpineBone );
	return spineboneTissue;

    } else if( density[copyNo] >= 1.61 && density[copyNo] < 1.92 ) {
	physVol->SetName("PhysicalSkull");
	physVol->GetLogicalVolume()->SetVisAttributes( attributeSkull );
	return skullTissue;

    } else if( density[copyNo] >= 1.92 ) {
	physVol->SetName("PhysicalRibBone");
	physVol->GetLogicalVolume()->SetVisAttributes( attributeRibBone );
	return ribboneTissue;
    }

    return physVol->GetLogicalVolume()->GetMaterial();
}

void DicomPatientParameterisation::GetDensity(G4double maxdensity, G4double mindensity) {

    DicomConfiguration dicomConfiguration;
  
    G4int copyCounter = 0;

    G4int totalNumberOfFile = dicomConfiguration.GetTotalNumberOfFile();

    G4int lenRows       = rows/compression;
    G4int lenColumns    = columns/compression;
    G4double xDimension = (lenColumns*pixelSpacingX)/2;

    G4int i = 0;
    //for( G4int z = 0; z < totalNumberOfFile; z++ ) {
    // head only PC 120214
    //totalNumberOfFile = 10;
    for( G4int z = 0; z < totalNumberOfFile; z++ ) {

	G4double slicePosition = dicomConfiguration.GetSliceLocation()[z];      
	
	for( G4int j = 1; j <= lenRows; j++ ) {
	    for( G4int w = 1; w <= lenColumns; w++ ) {
	      
		G4double tissueDensity = dicomConfiguration.GetDensityValue(i++);

		if( tissueDensity != -1 ) {
		    if( tissueDensity >= mindensity && tissueDensity <= maxdensity ) {

			density.push_back( tissueDensity );
			copyCounter++;
			G4double yPixel = (pixelSpacingY/2 + (w-1)*pixelSpacingY);
			G4double yDimension = ((lenRows*pixelSpacingX)/2)-(pixelSpacingY/2+(j-1)*pixelSpacingY);
                      
			patientPlacementX.push_back( ( compression*(xDimension- yPixel ) ) *mm );
			patientPlacementY.push_back( ( compression* yDimension  ) *mm );
			patientPlacementZ.push_back( ( slicePosition + sliceThickness/2 ) *mm );
		    }
		}
	    }            
	}
    }

}

void DicomPatientParameterisation::readColorChart() {
    std::ifstream cm("Colormap.dat");
    if(!cm) {
	G4cerr << "Colormap.dat couldn't be opened!!" << G4endl;
	numColorChart = 2;
	ColorChart cc;
	cc.density = 0.;
	cc.color[ColorChart::CRED] = 0.;
	cc.color[ColorChart::CGREEN] = 0.;
	cc.color[ColorChart::CBLUE] = 0.;
	cc.alpha = 1.;
	colorChart.push_back(cc);
	cc.density = 4.;
	cc.color[ColorChart::CRED] = 1.;
	cc.color[ColorChart::CGREEN] = 1.;
	cc.color[ColorChart::CBLUE] = 1.;
	cc.alpha = 1.;
	colorChart.push_back(cc);
	return;
    }

    cm >> numColorChart;
    ColorChart cc;
    for(int i = 0; i < numColorChart; i++) {
	cm >> cc.density
	   >> cc.color[ColorChart::CRED]
	   >> cc.color[ColorChart::CGREEN]
	   >> cc.color[ColorChart::CBLUE]
	   >> cc.alpha; 
	colorChart.push_back(cc);
    }

}

G4double DicomPatientParameterisation::getChartColor(G4int CC, G4double density) {
    G4double color = 0.;

    for(int i = 0; i < numColorChart; i++) {
	if(density <= colorChart[i].density) {

	    G4double w = (density - colorChart[i-1].density)
		/(colorChart[i].density - colorChart[i-1].density);
	    color = w*colorChart[i-1].color[CC] + (1-w)*colorChart[i].color[CC];
	    return color;
	}
    }
    return color;
}
