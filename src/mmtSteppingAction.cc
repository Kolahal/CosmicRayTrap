#include "mmtSteppingAction.hh"
#include "mmtRunAction.hh"
#include "mmtEventAction.hh"
#include "mmtDetectorConstruction.hh"
#include "mmtAnalysis.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"

mmtSteppingAction::mmtSteppingAction(const mmtDetectorConstruction* detectorConstruction,mmtEventAction* eventAction): G4UserSteppingAction(), fDetConstruction(detectorConstruction), fEventAction(eventAction)
{
	total_no_of_particles = 0;
	n_final_state = 0;
}

mmtSteppingAction::~mmtSteppingAction()
{
	n_final_state = 0;
}

void mmtSteppingAction::UserSteppingAction(const G4Step* step)
{
	G4Track* tk = step->GetTrack();
	
	if (fEventAction->new_event == 1)
	{
		n_final_state = 0;
		fEventAction->new_event = 0;
	}
	
	if (tk->GetCurrentStepNumber() == 1) // is it first step of particle?
	total_no_of_particles += 1;
	//G4cout<<"total_no_of_particles "<<total_no_of_particles<<G4endl;
	
	// Collect energy and track length step by step
	
	// get volume of the current step
	auto volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
	G4ThreeVector coord1 = step->GetPreStepPoint()->GetPosition();
	//G4cout<<"current coordinate: "<<coord1<<G4endl;
	//G4cout<<"current KE: "<<step->GetPreStepPoint()->GetKineticEnergy()<<G4endl;
	//G4cout<<"Currently, "<<tk->GetDefinition()->GetPDGEncoding()<<" is at "<<step->GetPreStepPoint()->GetPosition()<<" with KE "<<step->GetPreStepPoint()->GetKineticEnergy()<<G4endl;
	// energy deposit
	auto edep = step->GetTotalEnergyDeposit();
	
	// step length
	/*
	G4double stepLength = 0.;
	if ( step->GetTrack()->GetDefinition()->GetPDGCharge() != 0. )
	{
		stepLength = step->GetStepLength();
	}
	
	if ( volume == fDetConstruction->GetMMTAirPV() )
	{
		fEventAction->AddAbs(edep,stepLength);
	}
	
	if ( volume == fDetConstruction->GetPPS1PV() )
	{
		fEventAction->AddGap(edep,stepLength);
	}
	
	if ( volume == fDetConstruction->GetPPS2PV() )
	{
		fEventAction->AddGap(edep,stepLength);
	}
	*/
	G4Track* track = step->GetTrack();
	G4TrackStatus  status = track->GetTrackStatus();
	
	if  ( status != fAlive )
	{
		const G4ThreeVector& pos = track->GetPosition();
		const G4DynamicParticle* dParticle = track->GetDynamicParticle();
		
		G4double endKE = dParticle->GetKineticEnergy();
		G4ThreeVector m = track->GetMomentumDirection();
		G4double mx = m.x();
		G4double my = m.y();
		G4double mz = m.z();
		G4double ang_x=mx/sqrt(mx*mx + my*my + mz*mz);
		G4double ang_y=my/sqrt(mx*mx + my*my + mz*mz);
		G4double ang_z=mz/sqrt(mx*mx + my*my + mz*mz);
		
		//analysisManager->FillNtupleDColumn(1, 0, fEnergyAbs);



		/*
		mmtRunAction* pAnalysis = mmtRunAction::AnPointer;
		pAnalysis->finalpid[n_final_state] = dParticle->GetPDGcode();
		pAnalysis->keEnd[n_final_state] = 1.e-3*endKE;
		pAnalysis->xxEnd[n_final_state] = 1.e-3*pos[0];
		pAnalysis->yyEnd[n_final_state] = 1.e-3*pos[1];
		pAnalysis->zzEnd[n_final_state] = 1.e-3*pos[2];
		pAnalysis->txEnd[n_final_state] = ang_x;
		pAnalysis->tyEnd[n_final_state] = ang_y;
		pAnalysis->tzEnd[n_final_state] = ang_z;
		
		n_final_state += 1;
		G4cout<<"index of current final state particle in this event "<<n_final_state<<G4endl;
		*/
	}
	
	/*
	if  ( status != fAlive )
	{
		const G4ThreeVector& pos = track->GetPosition();
		//G4cout<<"Position at death: "<<pos[0]<<"    "<<pos[1]<<"     "<<pos[2]<<G4endl;
		
		const G4DynamicParticle* dParticle = track->GetDynamicParticle();
		G4double endKE = dParticle->GetKineticEnergy();
		//G4cout<<"Energy at death: "<<endKE<<G4endl;
		
		G4ThreeVector m = track->GetMomentumDirection();
		G4double mx = m.x();
		G4double my = m.y();
		G4double mz = m.z();
		
		G4double ang_x=mx/sqrt(mx*mx + my*my + mz*mz);
		G4double ang_y=my/sqrt(mx*mx + my*my + mz*mz);
		G4double ang_z=mz/sqrt(mx*mx + my*my + mz*mz);
		
		//G4cout<<"Direction at death: "<<ang_x<<"    "<<ang_y<<"     "<<ang_z<<G4endl;
		//G4cout<<"the dying particle PDG code is "<<dParticle->GetPDGcode()<<G4endl;
		if (abs(dParticle->GetPDGcode())==13)
		{
			mmtRunAction* pAnalysis = mmtRunAction::AnPointer;
			pAnalysis->keEnd = 1.e-3*endKE;
			pAnalysis->xxEnd = 1.e-3*pos[0];
			pAnalysis->yyEnd = 1.e-3*pos[1];
			pAnalysis->zzEnd = 1.e-3*pos[2];
			pAnalysis->txEnd = ang_x;
			pAnalysis->tyEnd = ang_y;
			pAnalysis->tzEnd = ang_z;
		}
	}
	*/
}
