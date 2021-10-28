
//*******************************************************
//
//
// DicomConfiguration.hh :
// - Handling of the header of *.g4 files
// - Reading <Data.dat> file

#ifndef DicomConfiguration_h
#define DicomConfiguration_h 1

#include "globals.hh"
#include <vector>

class DicomConfiguration
{
public:

   DicomConfiguration();
  ~DicomConfiguration() { }

  // This function reads <Data.dat>, return true or false
  // if successfull or not
  G4bool ReadDataFile();

    G4int GetCompressionValue() {return compressionValue;}

  G4int GetTotalNumberOfFile() {return totalNumberOfFile;}

  std::vector<G4String> GetListOfFile() {return listOfFile;}

  G4int GetTotalRows() {return totalRows;}

  G4int GetTotalColumns() {return totalColumns;}  

    G4int GetTotalPixels() {return totalPixels;}  
    //G4int GetTotalPixels() {return densityValue.size();}

  G4double GetXPixelSpacing() {return  xPixelSpacing;}

  G4double GetYPixelSpacing() {return  yPixelSpacing;}

  G4double GetSliceThickness() {return sliceTickness;} 

  std::vector<G4double> GetSliceLocation() {return  sliceLocation;} 

  G4int IsCompressionUsed() {return compressionUsed;}

  G4double GetDensityValue(G4int i);

    void ClearDensityData() {densityValue.clear(); };

    G4int ReadG4File(G4String g4File);
 
private:
    
    static short compressionValue;
    static G4int totalNumberOfFile;
    static std::vector<G4String> listOfFile;
    static short totalRows;
    static short totalColumns;
    static G4int totalPixels;
    static G4double xPixelSpacing;
    static G4double yPixelSpacing;
    static G4double sliceTickness;
    static std::vector<G4double> sliceLocation; 
    static short compressionUsed;
    static std::vector<G4double> densityValue;	

};
#endif

