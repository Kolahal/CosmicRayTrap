#include "mmtTrajectoryPoint.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"

#include "G4StepStatus.hh"

#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4AttDefStore.hh"

#include "G4UnitsTable.hh"

//#define G4ATTDEBUG
#ifdef G4ATTDEBUG
#include "G4AttCheck.hh"
#endif

G4ThreadLocal G4Allocator<mmtTrajectoryPoint>* mmtTrajPointAllocator=0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

mmtTrajectoryPoint::mmtTrajectoryPoint()
	: fpAuxiliaryPointVector(0), fTime(0.), fMomentum(0.,0.,0.),
	fStepStatus(fUndefined), fVolumeName("") {G4cout<<"mmtConstructor 1"<<G4endl;}

	//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

mmtTrajectoryPoint::mmtTrajectoryPoint(const G4Step* aStep)
	: G4TrajectoryPoint(aStep->GetPostStepPoint()->GetPosition()), fpAuxiliaryPointVector(aStep->GetPointerToVectorOfAuxiliaryPoints())
{
	G4cout<<"mmtTrajectoryPoint Constructor 2"<<G4endl;
	fTime = aStep->GetPostStepPoint()->GetGlobalTime();
	fMomentum = aStep->GetPostStepPoint()->GetMomentum();
	fStepStatus = aStep->GetPostStepPoint()->GetStepStatus();
	if (aStep->GetPostStepPoint()->GetPhysicalVolume())
	{
		fVolumeName = aStep->GetPostStepPoint()->
			GetPhysicalVolume()->GetName();
	} else {
		fVolumeName = " ";
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

mmtTrajectoryPoint::mmtTrajectoryPoint(const G4Track* aTrack)
	: G4TrajectoryPoint(aTrack->GetPosition()), fpAuxiliaryPointVector(0)
{
	G4cout<<"mmtTrajectoryPoint Constructor 3"<<G4endl;
	fTime = aTrack->GetGlobalTime();
	fMomentum = aTrack->GetMomentum();
	fStepStatus = fUndefined;
	fVolumeName = aTrack->GetVolume()->GetName();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

mmtTrajectoryPoint::mmtTrajectoryPoint(mmtTrajectoryPoint &right): G4TrajectoryPoint(right), fpAuxiliaryPointVector(right.fpAuxiliaryPointVector)
{
	G4cout<<"mmtTrajectoryPoint Constructor 4"<<G4endl;
	
	fTime		= right.fTime;
	fMomentum	= right.fMomentum;
	fStepStatus	= right.fStepStatus;
	fVolumeName	= right.fVolumeName;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

mmtTrajectoryPoint::~mmtTrajectoryPoint() {delete fpAuxiliaryPointVector;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const std::map<G4String,G4AttDef>* mmtTrajectoryPoint::GetAttDefs() const
{
	G4cout<<"mmtTrajectoryPoint::GetAttDefs()"<<G4endl;
	G4bool isNew;
	std::map<G4String,G4AttDef>* store
		= G4AttDefStore::GetInstance("TrajectoryPoint",isNew);
	if (isNew) {
		
		G4String Pos("Pos");
		(*store)[Pos] =
			G4AttDef(Pos, "Position", "Physics","G4BestUnit","G4ThreeVector");
		
		G4String Time("Time");
		(*store)[Time] =
			G4AttDef(Time, "Time", "Physics","G4BestUnit","G4double");

		G4String Momentum("Momentum");
		(*store)[Momentum] =
			G4AttDef(Momentum, "Momentum", "Physics","G4BestUnit","G4ThreeVector");

		G4String StepStatus("StepStatus");
		(*store)[StepStatus] =
			G4AttDef(StepStatus, "StepStatus", "Physics", "", "G4StepStatus");

		G4String VolumeName("VolumeName");
		(*store)[VolumeName] =
			G4AttDef(VolumeName, "VolumeName", "Physics", "", "G4String");

	}
	return store;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

std::vector<G4AttValue>* mmtTrajectoryPoint::CreateAttValues() const
{
	G4cout<< "mmtTrajectoryPoint::CreateAttValues()" <<G4endl;

	std::vector<G4AttValue>* values = new std::vector<G4AttValue>;

	values->push_back(G4AttValue("Time",G4BestUnit(fTime,"Time"),""));

	values->push_back(G4AttValue("Momentum",G4BestUnit(fMomentum,"Momentum"),""));

	values->push_back(G4AttValue("StepStatus",fStepStatus,""));

	values->push_back(G4AttValue("VolumeName",fVolumeName,""));

#ifdef G4ATTDEBUG
	G4cout << G4AttCheck(values,GetAttDefs());
#endif

	return values;
}
