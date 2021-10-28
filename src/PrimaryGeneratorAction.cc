#include "PrimaryGeneratorAction.hh"
#include "SessionManager.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"

#include "Randomize.hh"


PrimaryGeneratorAction::PrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction()
{
    SessionManager & SM = SessionManager::getInstance();

    //G4int nofParticles = SM.numProtonsPerRun;
    //fParticleGun = new G4ParticleGun(nofParticles);

    fParticleGun = new G4ParticleGun(1);

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    fParticleGun->SetParticleDefinition(particleTable->FindParticle("proton"));

    //fParticleGun->SetParticlePosition(G4ThreeVector(-10.0*mm, 0, 0));
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1.0, 0, 0));
    fParticleGun->SetParticleEnergy(SM.ProtonEnergy*MeV);
    fParticleGun->SetParticleTime(0*ns);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    SessionManager & SM = SessionManager::getInstance();

    G4double sigmaY, sigmaZ;
    float    x,y,z;

    int inc = 0;


    sigmaY = 9./2.3548; // FWHM = 9.0 mm
    sigmaZ = 9./2.3548; // FWHM = 9.0 mm

    // PCL PC, pelvis irradiation (prostate)
    x = -195.;

    fParticleGun->SetParticleTime(SM.bunchTime*ns);

    for (inc=0;inc<SM.numProtonsPerRun;inc++){
        // "weight-gain" irradiation
        //y =  -30 + G4RandGauss::shoot(0.0, sigmaY );
        // "normal" irradiation
        y =  -20. + G4RandGauss::shoot(0.0, sigmaY );
        z = -63.36 + G4RandGauss::shoot(0.0, sigmaZ );

        //G4cout << y << " " << z << " " << SM.bunchTime*ns << G4endl;

        // "weight-loss" irradiation
        //y =  -10. + G4RandGauss::shoot(0.0, sigmaY );

        fParticleGun->SetParticlePosition(G4ThreeVector(x*mm, y*mm, z*mm));

        fParticleGun->GeneratePrimaryVertex(anEvent);
    }
}
