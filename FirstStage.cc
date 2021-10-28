#include "SessionManager.hh"
#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#include "ncatPhantomGeometry.hh"

#include <sstream>

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "QGSP_BIC_HP.hh"
#include "G4StepLimiterPhysics.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include <unistd.h>

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// File formats
//   Output
//     for ascii files
//       new event line: #EventNumber
//       new record:     ParticleName Energy[keV] Time[ns] X[mm] Y[mm] Z[mm] DirX DirY DirZ
//     for binary files
//       new event line: 0xEE(char) EventNumber(int)
//       new record:     0xFF(char) Energy(double)[keV] Time(double)[ns] X(double)[mm] Y(double)[mm] Z(double)[mm] DirX(double)[mm] DirY(double)[mm] DirZ(double)[mm] ParticleName(string) 0x00(char)
//
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------

int main(int argc, char** argv)
{
    SessionManager& SM = SessionManager::getInstance();

    // --- User init ---

    // -- Visualization mode?
    SM.bGuiMode     = false; // if true, G4 visualization will start. use beamOn to generate particles


    // -- Save dose?
    SM.bSaveDose    = false; // true, if you want to save the dose

    // -- Seed definition
    SM.bRandSeed    = true; // if true, a random seed (based in the process ID) will be used
    long Seed       = 1111; // Seed variable. If bRandSeed is false, this value will be used as input seed

    if (SM.bRandSeed){
        pid_t pid;
        pid = getpid();
        Seed = pid;
    }

    // -- Phantom definition
    SM.bNcatMode    = true; // if false, a cylindrical phantom will be simulated instead
    SM.PMMAlength       = 200.0; //400.0;
    SM.CylShift         = 0.0; //0 -> 7.5

    // -- Beam parameters
    //SM.numProtonsPerRun = 310;
    SM.numProtonsPerRun = 310;
    SM.numRuns          = 100; //1.0e8 / SM.numProtonsPerRun;
    SM.bunchPeriod      = 10.0;

    SM.ProtonEnergy     = 200.0;


    // -- output files
    SM.DoseFile = "/home/hugos/g4/OPGI_FirstStage/OPGI_FS_output/testDose.dos";



    SM.TimeLimit        = 3.13e6; // ignore all particles appearing 0.00313+ ms after the start of irradiation

    SM.bBinaryOutput    = true;

    std::string WorkingDirectory = "/home/hugos/g4/OPGI_FirstStage/OPGI_FS_output";
    std::string BaseFileName     = "ncat_pelvis";                   // extension is added automatically: .txt or .bin

    SM.OutputPrecision  = 8; // have no effect if bBinaryFile = true

    // --- end of user init ---



    // -- output files
    //SM.DoseFile = "/home/hugos/g4/OPGI_FirstStage/OPGI_FS_output/testDose.dos";

    std::stringstream baseNameStream;
    baseNameStream << WorkingDirectory << '/' << BaseFileName << "_seed" << Seed << "_shift" << SM.CylShift << "_runs" << SM.numRuns;
    if (SM.bBinaryOutput)  baseNameStream << ".bin";
    else                 baseNameStream << ".txt";
    SM.FileName_Output = baseNameStream.str();
    std::cout <<  "Saving data to file: " << SM.FileName_Output << std::endl;

    std::stringstream doseNameStream;
    doseNameStream << "/home/hugos/g4/OPGI_FirstStage/OPGI_FS_output/testDose_seed" << Seed << "_runs" << SM.numRuns << ".dos";
    SM.DoseFile = doseNameStream.str();
    std::cout <<  "Saving dose to file: " << SM.DoseFile << std::endl;

    CLHEP::RanecuEngine* randGen = new CLHEP::RanecuEngine();
    randGen->setSeed(Seed);
    G4Random::setTheEngine(randGen);
    std::cout <<  "Initialized random with seed nb. " << Seed << std::endl;

    G4UIExecutive* ui =  0;
    if (SM.bGuiMode) ui = new G4UIExecutive(argc, argv);

    G4RunManager* runManager = new G4RunManager;

    if (SM.bNcatMode) {
        ncatPhantomGeometry * theDetector = new ncatPhantomGeometry();
        runManager->SetUserInitialization(theDetector);
    }

    else{
       DetectorConstruction * theDetector = new DetectorConstruction();
       runManager->SetUserInitialization(theDetector);
    }


    G4VModularPhysicsList* physicsList = new QGSP_BIC_HP;
    physicsList->RegisterPhysics(new G4StepLimiterPhysics());
    runManager->SetUserInitialization(physicsList);

    runManager->SetUserInitialization(new ActionInitialization());

    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    UImanager->ApplyCommand("/run/initialize");
    UImanager->ApplyCommand("/control/verbose 0");
    UImanager->ApplyCommand("/run/verbose 0");

    if (SM.bGuiMode)
    {
        UImanager->ApplyCommand("/hits/verbose 0"); //2
        UImanager->ApplyCommand("/tracking/verbose 0"); //2
        //UImanager->ApplyCommand("/control/saveHistory");
    }

    UImanager->ApplyCommand("/run/setCut 0.5 mm");
    UImanager->ApplyCommand("/process/em/fluo true");
    UImanager->ApplyCommand("/process/em/auger true");
    UImanager->ApplyCommand("/process/em/augerCascade true");
    UImanager->ApplyCommand("/process/em/pixe true");
    UImanager->ApplyCommand("/process/em/deexcitationIgnoreCut false");

    UImanager->ApplyCommand("/run/initialize");

    SM.startSession();

    G4VisManager* visManager = 0;

    if (SM.bGuiMode)
    {
        visManager = new G4VisExecutive("Quiet");
        visManager->Initialize();
        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
    }
    else
    {
        SM.runSimulation();
    }

    //delete visManager; // delete this is given an error
    delete runManager;
    delete ui;

    SM.endSession();

    std::cout <<  "Data saved to file:" << std::endl << SM.FileName_Output << std::endl;
    std::cout <<  "\nDose saved to file:" << std::endl << SM.DoseFile << std::endl;
}
