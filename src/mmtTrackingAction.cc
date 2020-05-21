#include "mmtTrackingAction.hh"
#include "mmtTrajectory.hh"
//#include "mmtTrackInformation.hh"

#include "G4TrackingManager.hh"
#include "G4Track.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
mmtTrackingAction::mmtTrackingAction()
	:G4UserTrackingAction()
{;}

void mmtTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
	fpTrackingManager->SetStoreTrajectory(true);
	fpTrackingManager->SetTrajectory(new mmtTrajectory(aTrack));
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
