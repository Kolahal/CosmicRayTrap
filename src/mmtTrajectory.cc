#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4AttDefStore.hh"

#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"

#include "mmtTrajectory.hh"
#include "mmtTrajectoryPoint.hh"
#include "G4ParticleTable.hh"

#include "G4LogicalVolumeStore.hh"

//#define G4ATTDEBUG
#ifdef G4ATTDEBUG
#include "G4AttCheck.hh"
#endif

G4ThreadLocal G4Allocator<mmtTrajectory>* mmtTrajectoryAllocator=0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

mmtTrajectory::mmtTrajectory()
	:
	fpPointsContainer(0), 
	fTrackID(0), fParentID(0),
	fPDGCharge(0.0), fPDGEncoding(0), fParticleName(""),
	fInitialKineticEnergy(0.),
	fInitialMomentum(G4ThreeVector(0.,0.,0.)),
	fFinalKineticEnergy(0.),
	fFinalMomentum(G4ThreeVector(0.,0.,0.)),
	//fFinalDirection(G4ThreeVector(0.,0.,0.)),
	fFinalTime(0.),
	fTargetVolume(0),
	fState(-999)
	{G4cout<< "mmtTrajectory Constructor 1" <<G4endl;}

	//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

mmtTrajectory::mmtTrajectory(const G4Track* aTrack)
{
	//G4cout<< "mmtTrajectory Constructor 2" <<G4endl;
	G4ParticleDefinition * particleDefinition = aTrack->GetDefinition();
	fParticleName = particleDefinition->GetParticleName();
	fPDGCharge = particleDefinition->GetPDGCharge();
	fPDGEncoding = particleDefinition->GetPDGEncoding();
	fTrackID = aTrack->GetTrackID();
	fParentID = aTrack->GetParentID();
	fInitialMomentum = aTrack->GetMomentum();
	fInitialKineticEnergy = aTrack->GetKineticEnergy();
	
        fFinalKineticEnergy = aTrack->GetKineticEnergy();
        fFinalMomentum = aTrack->GetMomentum();
        //fFinalDirection= aTrack->GetMomentumDirection();
        fFinalTime = aTrack->GetGlobalTime();
	
	fState=-999;

	fpPointsContainer = new TrajectoryPointContainer();
	// Following is for the first trajectory point
	fpPointsContainer->push_back(new mmtTrajectoryPoint(aTrack));
	
	//fFinalKineticEnergy = aTrack->GetKineticEnergy();
	//fFinalMomentum = aTrack->GetMomentum();
	//fFinalDirection= aTrack->GetMomentumDirection();
	//fFinalTime = aTrack->GetGlobalTime();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

mmtTrajectory::mmtTrajectory(mmtTrajectory & right) : G4VTrajectory(right)
{
	//G4cout<< "mmtTrajectory Constructor 3" <<G4endl;
	fParticleName = right.fParticleName;
	fPDGCharge = right.fPDGCharge;
	fPDGEncoding = right.fPDGEncoding;
	fTrackID = right.fTrackID;
	fParentID = right.fParentID;
	fInitialMomentum = right.fInitialMomentum;
	fInitialKineticEnergy = right.fInitialKineticEnergy;
	
	fFinalKineticEnergy = right.fFinalKineticEnergy;
	fFinalMomentum = right.fFinalMomentum;
	//fFinalDirection= right.fFinalDirection;
	fFinalTime = right.fFinalTime;
	
	fState = -999;

	fpPointsContainer = new TrajectoryPointContainer();

	for(size_t i=0;i<right.fpPointsContainer->size();++i) {
		mmtTrajectoryPoint* rightPoint
			= (mmtTrajectoryPoint*)((*(right.fpPointsContainer))[i]);
		fpPointsContainer->push_back(new mmtTrajectoryPoint(*rightPoint));
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

mmtTrajectory::~mmtTrajectory()
{
	for(size_t i=0;i<fpPointsContainer->size();++i){
		delete  (*fpPointsContainer)[i];
	}
	fpPointsContainer->clear();

	delete fpPointsContainer;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void mmtTrajectory::ShowTrajectory(std::ostream& os) const
{
	// Invoke the default implementation in G4VTrajectory...
	G4VTrajectory::ShowTrajectory(os);
	// ... or override with your own code here.
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void mmtTrajectory::AppendStep(const G4Step* aStep)
{
	//G4cout<< "mmtTrajectory::AppendStep" <<G4endl;
	fpPointsContainer->push_back(new mmtTrajectoryPoint(aStep));

	// Except for first step, which is a sort of virtual step to start
	// the track, compute the final values...
	const G4Track* track = aStep->GetTrack();
	
	fTargetVolume = G4LogicalVolumeStore::GetInstance()->GetVolume("ParticleDetector");
	/*
	if (fTargetVolume){
                G4cout<<" mmtTrajectory::AppendStep:- fTargetVolume "<<fTargetVolume->GetName()<<G4endl;
	}
	*/
	G4LogicalVolume* theVolume1 = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume();
	
	//G4LogicalVolume* theVolume2= track->GetVolume()->GetLogicalVolume();
        
	//if (theVolume1)
	//	G4cout<<" mmtTrajectory::AppendStep:- theVolume1 "<<theVolume1->GetName()<<G4endl;
	//if (theVolume2)
	//	G4cout<<" mmtTrajectory::AppendStep:- theVolume2 "<<theVolume2->GetName()<<G4endl;
	
	if ( (fTargetVolume->GetName() == theVolume1->GetName()) )
	{
		G4cout<<"BANG BANG" << aStep->GetPreStepPoint()->GetPosition() <<"     tk id "<<track->GetTrackID()<<G4endl;
		fState = 1;
		( (G4Track*)track )->SetTrackStatus(fStopAndKill);//fStopAndKill//fKillTrackAndSecondaries
		return;
	}
	
	if (track->GetCurrentStepNumber() > 0)
	{
		fFinalKineticEnergy = aStep->GetPreStepPoint()->GetKineticEnergy() - aStep->GetTotalEnergyDeposit();
		fFinalMomentum = aStep->GetPreStepPoint()->GetMomentum();
		fFinalTime = aStep->GetPreStepPoint()->GetGlobalTime();
		//fFinalDirection = aStep->GetPreStepPoint()->GetMomentumDirection();
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ParticleDefinition* mmtTrajectory::GetParticleDefinition()
{
	return (G4ParticleTable::GetParticleTable()->FindParticle(fParticleName));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void mmtTrajectory::MergeTrajectory(G4VTrajectory* secondTrajectory)
{
	G4cout<< "mmtTrajectory::MergeTrajectory" <<G4endl;
	if(!secondTrajectory) return;

	mmtTrajectory* second = (mmtTrajectory*)secondTrajectory;
	G4int ent = second->GetPointEntries();
	
	// initial point of the second trajectory should not be merged
	for(G4int i=1; i<ent; ++i) {
		fpPointsContainer->push_back((*(second->fpPointsContainer))[i]);
	}
	delete (*second->fpPointsContainer)[0];
	second->fpPointsContainer->clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const std::map<G4String,G4AttDef>* mmtTrajectory::GetAttDefs() const
{
	//G4cout<< "mmtTrajectory::GetAttDefs()" <<G4endl;
	G4bool isNew;
	std::map<G4String,G4AttDef>* store
		= G4AttDefStore::GetInstance("Trajectory",isNew);

	if (isNew) {

		G4String ID("ID");
		(*store)[ID] = G4AttDef(ID,"Track ID","Bookkeeping","","G4int");

		G4String PID("PID");
		(*store)[PID] = G4AttDef(PID,"Parent ID","Bookkeeping","","G4int");

		G4String PN("PN");
		(*store)[PN] = G4AttDef(PN,"Particle Name","Physics","","G4String");

		G4String Ch("Ch");
		(*store)[Ch] = G4AttDef(Ch,"Charge","Physics","e+","G4double");

		G4String PDG("PDG");
		(*store)[PDG] = G4AttDef(PDG,"PDG Encoding","Physics","","G4int");

		G4String IMom("IMom");
		(*store)[IMom] = G4AttDef(IMom,
				"Momentum of track at start of trajectory",
				"Physics","G4BestUnit","G4ThreeVector");

		G4String IMag("IMag");
		(*store)[IMag] = G4AttDef(IMag,
				"Magnitude of momentum of track at start of trajectory",
				"Physics","G4BestUnit","G4double");

		G4String IKE("IKE");
		(*store)[IKE] = G4AttDef(IKE,
				"Kinetic energy of track at start of trajectory",
				"Physics","G4BestUnit","G4double");//G4ThreeVector

		G4String NTP("NTP");
		(*store)[NTP] = G4AttDef(NTP,"No. of points","Bookkeeping","","G4int");

		G4String FKE("FKE");
		(*store)[FKE] = G4AttDef(FKE,
				"Kinetic energy of track at the end of trajectory",
				"Physics","G4BestUnit","G4double");

		G4String FMom("FMom");
		(*store)[FMom] = G4AttDef(FMom,
				"Momentum of track at the end of trajectory",
				"Physics","G4BestUnit","G4ThreeVector");
		
		G4String FTm("FTm");
		(*store)[FTm] = G4AttDef(FTm,"Time at final point","Physics","G4BestUnit","G4double");

		//G4String FDir("FDir");
		//(*store)[FDir] = G4AttDef(FDir,
		//		"Direction of track at the end of trajectory",
		//		"Physics","G4BestUnit","G4ThreeVector");
	}
	return store;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

std::vector<G4AttValue>* mmtTrajectory::CreateAttValues() const
{
	//G4cout<< "mmtTrajectory::CreateAttValues()" <<G4endl;

	std::vector<G4AttValue>* values = new std::vector<G4AttValue>;
	//G4cout<< "ID" <<G4endl;
	values->push_back
		(G4AttValue("ID",G4UIcommand::ConvertToString(fTrackID),""));
	//G4cout<< "PID" <<G4endl;
	values->push_back
		(G4AttValue("PID",G4UIcommand::ConvertToString(fParentID),""));
	//G4cout<< "PN" <<G4endl;
	values->push_back(G4AttValue("PN",fParticleName,""));
	//G4cout<< "CH" <<G4endl;
	values->push_back
		(G4AttValue("Ch",G4UIcommand::ConvertToString(fPDGCharge),""));
	//G4cout<< "PDG" <<G4endl;
	values->push_back
		(G4AttValue("PDG",G4UIcommand::ConvertToString(fPDGEncoding),""));
	//G4cout<< "IMom" <<G4endl;
	values->push_back
		(G4AttValue("IMom",G4BestUnit(fInitialMomentum,"Energy"),""));
	//G4cout<< "IMag" <<G4endl;
	values->push_back
		(G4AttValue("IMag",G4BestUnit(fInitialMomentum.mag(),"Energy"),""));
	//G4cout<< "IKE" <<G4endl;
	values->push_back
		(G4AttValue("IKE",G4BestUnit(fInitialKineticEnergy,"Energy"),""));
	//G4cout<< "NTP" <<G4endl;
	values->push_back
		(G4AttValue("NTP",G4UIcommand::ConvertToString(GetPointEntries()),""));
	//G4cout<< "FKE" <<G4endl;
	values->push_back
		(G4AttValue("FKE",G4BestUnit(fFinalKineticEnergy,"Energy"),""));
	//G4cout<< "FMom" <<G4endl;
	values->push_back
		(G4AttValue("FMom",G4BestUnit(fFinalMomentum,"Energy"),""));
	//G4cout<< "FTm" <<G4endl;
	values->push_back
		(G4AttValue("FTm",G4BestUnit(fFinalTime,"Time"),""));
	//G4cout<< "FDir" <<G4endl;
	//values->push_back
	//	(G4AttValue("FDir",G4BestUnit(fFinalDirection,"Direction"),""));

#ifdef G4ATTDEBUG
	G4cout<< "G4AttCheck" <<G4endl;
	G4cout << G4AttCheck(values,GetAttDefs());
#endif
	return values;
}
