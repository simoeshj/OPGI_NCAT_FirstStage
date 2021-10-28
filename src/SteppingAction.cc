#include "SteppingAction.hh"
#include "SessionManager.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4ProcessManager.hh"
#include "G4StepPoint.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"

void SteppingAction::UserSteppingAction(const G4Step *step)
{
    SessionManager & SM = SessionManager::getInstance();
    G4Track * theTrack = step->GetTrack();

    const G4StepPoint * postStepPoint  = step->GetPostStepPoint();
    const G4ThreeVector & postPos = postStepPoint->GetPosition();

    G4StepPoint* preStepPoint= step->GetPreStepPoint();
    const G4ThreeVector & prePos = preStepPoint->GetPosition();

    if (postStepPoint->GetMaterial() == SM.dummyMat)
    {
        // Save detected particle to file and kill the track

        const double time = postStepPoint->GetGlobalTime()/ns;
        if (time > SM.TimeLimit) return;

        double buf[6];

        buf[0] = postPos[0]/mm;
        buf[1] = postPos[1]/mm;
        buf[2] = postPos[2]/mm;
        const G4ThreeVector & dir = postStepPoint->GetMomentumDirection();
        buf[3] = dir[0];
        buf[4] = dir[1];
        buf[5] = dir[2];

        SM.saveParticle(step->GetTrack()->GetParticleDefinition()->GetParticleName(),
                        postStepPoint->GetKineticEnergy()/keV,
                        time,
                        buf);


        theTrack->SetTrackStatus(fStopAndKill);
        theTrack->SetTrackStatus(fKillTrackAndSecondaries);

    }

    else {

     if (SM.bSaveDose){
        //Dose computation

         G4double x1pre, y1pre, z1pre;
         G4double x1post, y1post, z1post;
         G4double x1, y1, z1;

         G4String process = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();

         x1pre      = prePos.x();
         y1pre      = prePos.y();
         z1pre      = prePos.z();
         x1post     = postPos.x();
         y1post     = postPos.y();
         z1post     = postPos.z();

         x1=x1pre+(x1post-x1pre)*G4UniformRand();
         y1=y1pre+(y1post-y1pre)*G4UniformRand();
         z1=z1pre+(z1post-z1pre)*G4UniformRand();

         G4double dEStep = step->GetTotalEnergyDeposit();

          //G4cout << x1 << " " << y1 << " " << z1 << " " << dEStep << G4endl;

         G4Material* material     = preStepPoint->GetMaterial();
         G4double materialDensity = material->GetDensity();

         SM.fillDose(x1,y1,z1,dEStep/(1.602e-19*materialDensity*6.25e6));
     }
     else{
         return;
     }
   }
}
