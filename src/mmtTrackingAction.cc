#include "mmtTrackingAction.hh"
#include "mmtTrajectory.hh"
//#include "mmtTrackInformation.hh"

#include "G4TrackingManager.hh"
#include "G4Track.hh"

#include "G4LogicalVolumeStore.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
mmtTrackingAction::mmtTrackingAction()
	:G4UserTrackingAction() //fTargetVolume(NULL)
{;}

void mmtTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
	fpTrackingManager->SetStoreTrajectory(true);
	fpTrackingManager->SetTrajectory(new mmtTrajectory(aTrack));
	/*
	fTargetVolume = G4LogicalVolumeStore::GetInstance()->GetVolume("ParticleDetector");
	auto CurrentTrackStepVolume = aTrack->GetLogicalVolumeAtVertex();
	if (fTargetVolume)
		G4cout<<" mmtTrackingAction::PreUserTrackingAction:- fTargetVolume "<<G4endl;
	//if (CurrentTrackStepVolume)
	//	G4cout<<" mmtTrackingAction::PreUserTrackingAction:- CurrentTrackStepVolume "<<G4endl;

	if (fTargetVolume == CurrentTrackStepVolume)
	{
		//aTrack->SetTrackStatus(fStopAndKill);
		( (G4Track*) aTrack )->SetTrackStatus( fStopAndKill );
		G4cout<<"BANG BANG!!!"<<G4endl;
		return;
	}
	*/
	
	/*
	// Create trajectory only for track in tracking region
	mmtTrackInformation* trackInfo = 
		(mmtTrackInformation*)(aTrack->GetUserInformation());

	if(trackInfo->GetTrackingStatus() > 0)
	{
		fpTrackingManager->SetStoreTrajectory(true);
		fpTrackingManager->SetTrajectory(new mmtTrajectory(aTrack));
	}
	else
	{ fpTrackingManager->SetStoreTrajectory(false); }
	*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
void mmtTrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
	/*
	G4TrackVector* secondaries = fpTrackingManager->GimmeSecondaries();
	if(secondaries)
	{
		mmtTrackInformation* info = 
			(mmtTrackInformation*)(aTrack->GetUserInformation());
		size_t nSeco = secondaries->size();
		if(nSeco>0)
		{
			for(size_t i=0;i<nSeco;i++)
			{ 
				mmtTrackInformation* infoNew = new mmtTrackInformation(info);
				(*secondaries)[i]->SetUserInformation(infoNew);
			}
		}
	}
	*/
}
