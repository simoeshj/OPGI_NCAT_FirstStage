
#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <string>

#include "globals.hh"
#include <iostream>

class G4Material;
class G4String;

class SessionManager
{
    public:
        static SessionManager& getInstance();

    private:
        SessionManager();
        ~SessionManager();

    public:
        SessionManager(SessionManager const&) = delete;
        void operator=(SessionManager const&) = delete;

        void startSession();
        void endSession();
        void runSimulation();

        bool bGuiMode       = false;
        bool bNcatMode      = false;
        bool bRandSeed      = false;
        bool bSaveDose      = false;
        bool bBinaryOutput  = false;

        std::string FileName_Output;
        std::string DoseFile;

        double PMMAlength       = 0;
        G4Material * dummyMat   = nullptr;
        double CylShift         = 0;

        double ProtonEnergy     = 0;
        int numProtonsPerRun    = 0;
        int numRuns             = 0;

        double bunchPeriod      = 0;

        // run time variables
        double bunchTime        = 0;

        double TimeLimit        = 0;
        int    OutputPrecision  = 6;


        float *dose3D; // 600x400x400

public:
        void saveParticle(const G4String & particle, double energy, double time, double *PosDir);
        void saveEventNumber(int iEvent);
        void fillDose(double x, double y, double z, double thisDose);

private:
        std::ofstream * outStream = nullptr;
        std::ofstream * dosefh = nullptr;

};

#endif // SESSIONMANAGER_H
