#include "SessionManager.hh"

#include <iostream>
#include <sstream>
#include <fstream>

#include <stdio.h>
#include "G4ios.hh"

#include "G4ParticleDefinition.hh"
#include "G4UImanager.hh"
#include "G4String.hh"

SessionManager &SessionManager::getInstance()
{
    static SessionManager instance; // Guaranteed to be destroyed, instantiated on first use.
    return instance;
}

SessionManager::SessionManager(){}

SessionManager::~SessionManager()
{
    delete outStream;
}

void SessionManager::startSession()
{
    outStream = new std::ofstream();

    if (bBinaryOutput) outStream->open(FileName_Output, std::ios::out | std::ios::binary);
    else             outStream->open(FileName_Output);

    if (!outStream->is_open())
    {
        std::cerr << "Cannot open file to store generated particles" << std::endl;
        exit(1);
    }

    if (bSaveDose){
        if ((dose3D=(float*) calloc(96000000,sizeof(float))) == NULL ){
         printf("Problem allocating memory for \"dose\". Exiting.\n");
         }

       int i,j,k;
        // set dose array to zero, write "fortran" way
       for (i=0;i<400;i++)
          for (j=0;j<400;j++)
            for (k=0;k<600;k++)
              dose3D[k*400*400 + j*400 + i]=0.;



        dosefh = new std::ofstream();
        dosefh->open(DoseFile, std::ios::out | std::ios::binary);

        if (!dosefh->is_open())
        {
            std::cerr << "Cannot open file to save the dose" << std::endl;
            exit(1);
        }
    }
}

void SessionManager::endSession()
{
    outStream->close();

    if (bSaveDose){
    for (int i=0;i< 96000000;i++)
        dosefh->write((char*)&dose3D[i], sizeof(float));

    dosefh->close();
    }
}

void SessionManager::runSimulation()
{
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    for (int i=0; i<numRuns; i++)
    {
        if (i % 100 == 0) std::cout << "Run # " << i << "  bunch time: " << bunchTime << std::endl;
        saveEventNumber(i);

        UImanager->ApplyCommand("/run/beamOn");

        bunchTime += bunchPeriod;
    }
}

void SessionManager::fillDose(double x, double y, double z, double thisDose)
{
 int   iaux,
       jaux,
       kaux;
 float this_x, this_y, this_z;
 float offsetZ;

 this_x = -x;
 this_y = -y;
 this_z = -z;

 // Zcenter of patient = 205*4.68 = 959.4 mm
 // head irradiation
 // //offsetZ = 655.; //mm
 //offsetZ=780.-200.; // mm
 offsetZ=63.-200.; // mm

 kaux = (int)floor(this_z-offsetZ); //?;
 //kaux=5; //testing only
 if (kaux >= 0 && kaux < 400) {
   jaux = (int)floor(this_y)+200;
   if (jaux >= 0 && jaux < 400) {
     iaux = (int)floor(this_x)+300;
       if (iaux >= 0 && iaux < 600)
         // C-like 3d write-to-disk
         //dose3D[iaux][jaux][kaux] += thisDose;
         // Fortran-like
         // 600*250=150000
         // 600*400
         *(dose3D + kaux*240000 + jaux*600 + iaux) += (G4float)thisDose;
   }
 }
}


void SessionManager::saveEventNumber(int iEvent)
{
    if (bBinaryOutput)
    {
        *outStream << char(0xee);
        outStream->write((char*)&iEvent, sizeof(int));
    }
    else
    {
        *outStream << '#' << iEvent << std::endl;
        //std::stringstream ss;
        //ss << '#' << iEvent;
        //*outStream << ss.rdbuf() << std::endl;
    }
}

void SessionManager::saveParticle(const G4String & particle, double energy, double time, double * PosDir)
{
    if (bBinaryOutput)
    {
        *outStream << char(0xff);
        outStream->write((char*)&energy, sizeof(double));
        outStream->write((char*)&time, sizeof(double));
        outStream->write((char*)PosDir, 6*sizeof(double));
        *outStream << particle << char(0x00);
    }
    else
    {
        std::stringstream ss;
        ss.precision(OutputPrecision);

        ss << particle << ' ';
        ss << energy << ' ';
        ss << time << ' ';
        ss << PosDir[0] << ' ' << PosDir[1] << ' ' << PosDir[2] << ' ';     //position
        ss << PosDir[3] << ' ' << PosDir[4] << ' ' << PosDir[5];            //direction

        *outStream << ss.rdbuf() << std::endl;
    }
}
