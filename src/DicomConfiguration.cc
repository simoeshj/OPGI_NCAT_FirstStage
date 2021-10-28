
//
//
// DicomConfiguration.cc :
//	- Handling of the header of *.g4 files
// 	- Reading <Data.dat> file

#include "globals.hh"
#include "DicomConfiguration.hh"
#include <fstream>
#include <vector>

short DicomConfiguration::compressionValue = 0;
G4int DicomConfiguration::totalNumberOfFile = 0;
std::vector<G4String> DicomConfiguration::listOfFile;
short DicomConfiguration::totalRows = 0;
short DicomConfiguration::totalColumns = 0;
G4int DicomConfiguration::totalPixels = 0;
G4double DicomConfiguration::xPixelSpacing = 0.;
G4double DicomConfiguration::yPixelSpacing = 0.;
G4double DicomConfiguration::sliceTickness = 0.;
std::vector<G4double> DicomConfiguration::sliceLocation;
short DicomConfiguration::compressionUsed = 0;
std::vector<G4double> DicomConfiguration::densityValue;

//
DicomConfiguration::DicomConfiguration() {
    ReadDataFile();
}

//
G4bool DicomConfiguration::ReadDataFile() {

    if(totalNumberOfFile > 0) return true;

    totalPixels = 0;

    std::ifstream dataFile("Data.dat");
    G4String nameOfFileBuffer;
    if(dataFile.good() != 1 ) return 1;

    dataFile >> compressionValue;
    dataFile >> totalNumberOfFile;
 
    for(G4int i = 0; i < totalNumberOfFile; i++ ) {

	dataFile >> nameOfFileBuffer;
	listOfFile.push_back( nameOfFileBuffer );

	// read densities from .g4 file
	ReadG4File(nameOfFileBuffer);
    }

    dataFile.close();
    return 0;
}

G4int DicomConfiguration::ReadG4File( G4String g4File ) {
    double maxdens=0.;

    //densityValue.clear();

    g4File = g4File + ".g4";
    std::ifstream readingG4FileHeader(g4File.c_str(),
				      std::ios_base::in | std::ios_base::binary);

    if ( readingG4FileHeader.good() != 1 ) return 1;

    readingG4FileHeader.read((char *)&totalRows, 2);
    readingG4FileHeader.read((char *)&totalColumns, 2);
    readingG4FileHeader.read((char *)&xPixelSpacing, 8);
    readingG4FileHeader.read((char *)&yPixelSpacing, 8);
    readingG4FileHeader.read((char *)&sliceTickness, 8);
    G4double sliceLocationBuff;
    readingG4FileHeader.read((char *)&sliceLocationBuff, 8);
    readingG4FileHeader.read((char *)&compressionUsed, 2);

    sliceLocation.push_back(sliceLocationBuff);

    //added PC 080129
/*    std::printf("Tot rows = %d\n",totalRows);
    std::printf("Tot cols = %d\n",totalColumns);
    std::printf("x Pixel spacing = %f\n",xPixelSpacing);
    std::printf("y Pixel spacing = %f\n",yPixelSpacing);
    std::printf("Slice thickness = %f\n",sliceTickness);
    std::printf("Slice location buffer = %f\n",sliceLocationBuff);
    std::printf("Compression = %d\n",compressionUsed);
*/
    G4double density;
    for(int y = 0; y < totalRows/compressionUsed; y++) {
	for(int x = 0; x < totalColumns/compressionUsed; x++) {
	    readingG4FileHeader.read((char *)&density, sizeof(G4double)); // =8
            // added PC 080129
	    //if (density != 0.) std::printf("Density (%d %d) = %f\n",y,x,density);
	    densityValue.push_back(density);
	    totalPixels++; //256*256
            if (density > maxdens) maxdens=density;
	}
    }
    //std::printf("Max density = %f\n",maxdens);
    readingG4FileHeader.close();
    return 0;
}

G4double DicomConfiguration::GetDensityValue(G4int i) {

    G4double value = 0.;

    if (i >= 0) {
	unsigned int j = i;
	//
	if(j > densityValue.size() ) {
	    // Throw exception, return dummy, cerr error message...
	    G4cout << "out of range in GetDensityValue()! : "
		   << j << ", " << totalPixels << G4endl;
	} else {
	    value = densityValue[i];
	}
    } else {
	G4cout << "out of range in GetDensityValue()!"<<G4endl;
    }

    return value;
}
