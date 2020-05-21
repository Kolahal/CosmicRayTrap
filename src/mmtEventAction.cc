#include "mmtEventAction.hh"
#include "mmtRunAction.hh"
#include "mmtSteppingAction.hh"
#include "mmtAnalysis.hh"
#include "mmtDetectorConstruction.hh"
#include "mmtTrajectory.hh"
#include "mmtTrajectoryPoint.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ParticleTable.hh"
#include "G4ExtrudedSolid.hh"

#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4AttCheck.hh"

#include "Randomize.hh"
#include <iomanip>
#include <iostream>
#include <typeinfo>
using namespace std;

mmtEventAction::mmtEventAction(const mmtDetectorConstruction* detectorconstruction): G4UserEventAction(), fDet(detectorconstruction), fEvent(0), fInParticleID(0), fITime(0.), fOutParticleID(0), fKE_in(0.), fXX_in(0.), fYY_in(0.), fZZ_in(0.), fTX_in(0.), fTY_in(0.), fTZ_in(0.), fKEout(0.), fXXout(0.), fYYout(0.), fZZout(0.), fTXout(0.), fTYout(0.), fTZout(0.)
{
}

mmtEventAction::~mmtEventAction()
{
}

void mmtEventAction::BeginOfEventAction(const G4Event* evt)
{
	std::cout<<"BeginOfEventAction"<<std::endl;
	
	fEvent = 0;
	fInParticleID = 0;
	fOutParticleID= 0;
	fITime = 0.0;
	fKE_in = 0.0;
	fXX_in = 0.0;
	fYY_in = 0.0;
	fZZ_in = 0.0;
	fTX_in = 0.0;
	fTY_in = 0.0;
	fTZ_in = 0.0;
	
	fKEout = 0.0;
	fXXout = 0.0;
	fYYout = 0.0;
	fZZout = 0.0;
	fTXout = 0.0;
	fTXout = 0.0;
	fTZout = 0.0;
	
	G4int evtNb = evt->GetEventID();
	/*
	G4AnalysisManager* analysisManager =   G4AnalysisManager::Instance();
	analysisManager->FillNtupleIColumn(0, 0, evtNb);
	analysisManager->FillNtupleIColumn(0, 1, fInParticleID);
	analysisManager->FillNtupleDColumn(0, 2, fKE_in);
	analysisManager->FillNtupleDColumn(0, 3, fXX_in);
	analysisManager->FillNtupleDColumn(0, 4, fYY_in);
	analysisManager->FillNtupleDColumn(0, 5, fZZ_in);
	analysisManager->FillNtupleDColumn(0, 6, fTX_in);
	analysisManager->FillNtupleDColumn(0, 7, fTY_in);
	analysisManager->FillNtupleDColumn(0, 8, fTZ_in);
	analysisManager->AddNtupleRow(0);
	*/
	if (evtNb%10 == 0)
	G4cout << "\n---> Begin Of Event: " << evtNb << G4endl;
}

void mmtEventAction::EndOfEventAction(const G4Event* event)
{
	std::cout<<"EndOfEventAction"<<std::endl;

	G4AnalysisManager* analysisManager =   G4AnalysisManager::Instance();
	G4int eventID = event->GetEventID();
	
	G4TrajectoryContainer* trajectoryContainer = event->GetTrajectoryContainer();
	if (trajectoryContainer){
	G4cout<<trajectoryContainer->entries()<<" trajectories in this event"<<G4endl;
	int ntrj = 0;
	//unsigned ntrj_total = trajectoryContainer->entries();
	for(unsigned i=0; i < trajectoryContainer->entries(); i++)
	{
		//G4VTrajectory* tj = (*trajectoryContainer)[i];
		//G4Trajectory* trj = (G4Trajectory*)( (*(event->GetTrajectoryContainer()))[i] );
		mmtTrajectory* trj = (mmtTrajectory*)( (*(event->GetTrajectoryContainer()))[i] );
		
		//mmtTrajectory* trj = new mmtTrajectory();
		//trj = (mmtTrajectory*) (&trajectoryContainer[i]);
		
		//G4cout<< "At EndOfEventAction, CreateAttValues..." <<G4endl;
                std::vector<G4AttValue>* attValues = ((mmtTrajectory*)trj)->CreateAttValues();

		//G4cout<< "At EndOfEventAction, GetAttDefs..." <<G4endl;
                const std::map<G4String,G4AttDef>* attDefs = ((mmtTrajectory*)trj)->GetAttDefs();
                
		//G4cout<< "At EndOfEventAction, G4AttCheck" <<G4endl;
                if (G4AttCheck(attValues,attDefs).Check("Trajectory"))
                {
                      G4cout<<"yes"<<G4endl;
                      return;
                }

                //os << "Trajectory:";
                G4cout<< "+++++++++++++++++++++++++++++++++++++++++++++++++" <<G4endl;
		G4double finalTrackTime=0.0;
		G4double finalTrackKE = 0.0;
		G4ThreeVector finalTrackMomentum = G4ThreeVector(0.,0.,0.);
                std::vector<G4AttValue>::iterator iAttVal;
                for (iAttVal = attValues->begin(); iAttVal != attValues->end(); ++iAttVal)
                {
                        //G4cout<< iAttVal->GetName() << G4endl;
                        std::map<G4String,G4AttDef>::const_iterator iAttDef = attDefs->find(iAttVal->GetName());
			if (iAttVal->GetName() == "FMom")
				finalTrackMomentum = G4UIcmdWith3VectorAndUnit::GetNew3VectorValue( iAttVal->GetValue() );
			if (iAttVal->GetName() == "FKE")
				finalTrackKE = std::stod(iAttVal->GetValue());
			else if (iAttVal->GetName() == "FTm")
				finalTrackTime = std::stod(iAttVal->GetValue());
                        G4cout<< iAttDef->second.GetDesc() << "    "<< iAttVal->GetName() <<"     "<<iAttVal->GetValue()<<G4endl;
                }
                delete attValues;
                G4cout<< "+++++++++++++++++++++++++++++++++++++++++++++++++" <<G4endl;
		G4cout<<"final Mom " <<finalTrackMomentum/GeV<<",   final KE" <<finalTrackKE<<",   final time "<< finalTrackTime << G4endl;

		//G4cout<<"final KE "<<trj->GetFinalKE()/GeV<<G4endl;
		//G4cout<<"final dirn "<<trj->GetFinalDirection()<<G4endl;
		//G4cout<<"final time "<<trj->GetFinalTime()<<G4endl;
		
		//delete trj;
		/*
		for(int n = 0; n<trj->GetPointEntries(); n++)
		{
			mmtTrajectoryPoint* mmtp = new mmtTrajectoryPoint();

			G4VTrajectoryPoint* aPoint= trj->GetPoint(n);
			G4cout<<"mom["<<n<<"]:"<<((mmtTrajectoryPoint*)aPoint)->findMomentum()<<", time "<<((mmtTrajectoryPoint*)aPoint)->GetTime()/s<<", position "<<aPoint->GetPosition()/m<<G4endl;

			delete mmtp;
		}
		*/
		/*
		if (abs(tj->GetPDGEncoding())==13)
		{
			unsigned npoints = 0;
			npoints = tj->GetPointEntries();
			for (unsigned n=0; n<npoints; n++)
			{
				G4VTrajectoryPoint* aPoint= tj->GetPoint(n);
				G4ThreeVector aPosition = aPoint->GetPosition();
				//G4cout<<":- "<<aPosition<<G4endl;
			}
			//G4cout<<"--------------"<<G4endl;
		}
		*/
		G4VTrajectoryPoint* StartingPoint= trj->GetPoint(0);
		//G4VTrajectoryPoint* StartingPoint= trj->GetPoint(0);

		//mmtTrajectoryPoint* q = (mmtTrajectoryPoint*)(StartingPoint);
		//mmtTrajectoryPoint* testPoint = new mmtTrajectoryPoint(*q);
		//mmtTrajectoryPoint* testPoint = new mmtTrajectoryPoint();
		//*testPoint = *q;
		//fITime = testPoint->GetTime();
		//G4ThreeVector testMom = testPoint->GetMomentum();
		//G4cout<<"VertexTime "<< fITime <<", mom "<<testMom/GeV<<G4endl;
		
		//const std::map< G4String,G4AttDef > *mp = StartingPoint->GetAttDefs();
		//G4cout<<(*mp)["Time"]<<G4endl;

		//auto x = ->GetInstance("TrajectoryPoint", true);
		//G4cout<<"--> "<< typeid((*mp)["Time"]).name() <<G4endl;
		//
		
		//mmtTrajectoryPoint* mmtp = new mmtTrajectoryPoint();
		
		//G4double tt0		= mmtp->findTime( (mmtTrajectoryPoint*)StartingPoint );
		//G4ThreeVector mom0	= mmtp->findMomentum( (mmtTrajectoryPoint*)StartingPoint );
		//G4cout<<"time "<<tt0/s<<",      mom "<<mom0/GeV<<G4endl;
		
		//delete mmtp;
		/*
		std::vector<G4AttValue>* attValues = ((mmtTrajectoryPoint*)StartingPoint)->CreateAttValues();
		const std::map<G4String,G4AttDef>* attDefs = ((mmtTrajectoryPoint*)StartingPoint)->GetAttDefs();
		
		//if (G4AttCheck(attValues,attDefs).Check("Time"))
		//{
		//	G4cout<<"yes"<<G4endl;
		//	return;
		//}

		//os << "Trajectory:";
		G4cout<< "+++++++++++++++++++++++++++++++++++++++++++++++++" <<G4endl;
		std::vector<G4AttValue>::iterator iAttVal;
		for (iAttVal = attValues->begin(); iAttVal != attValues->end(); ++iAttVal)
		{
			//G4cout<< iAttVal->GetName() << G4endl;
			std::map<G4String,G4AttDef>::const_iterator iAttDef = attDefs->find(iAttVal->GetName());
			G4cout<< iAttDef->second.GetDesc() << "    "<< iAttVal->GetName() <<"     "<<iAttVal->GetValue()<<G4endl;
		}
		delete attValues;
		G4cout<< "+++++++++++++++++++++++++++++++++++++++++++++++++" <<G4endl;
		*/
		G4ThreeVector VertexPosition = StartingPoint->GetPosition();
		
		G4VTrajectoryPoint* LastPoint = trj->GetPoint(trj->GetPointEntries()-1);
		G4ThreeVector FinalPosition = LastPoint->GetPosition();
		
		G4ThreeVector momentum = trj->GetInitialMomentum();
		G4int   pid = trj->GetPDGEncoding();
		
		G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
		G4ParticleDefinition* p = particleTable->FindParticle(pid);
		G4double mass = p->GetPDGMass();
		
		G4double _x = VertexPosition.x()/cm;
		G4double _y = VertexPosition.y()/cm;
		G4double _z = VertexPosition.z()/cm;
		G4double xx = FinalPosition.x()/cm;
		G4double yy = FinalPosition.y()/cm;
		G4double zz = FinalPosition.z()/cm;
		G4double ke = sqrt(momentum*momentum + mass*mass) - mass;//since initial momentum was used, what we have here is initialKE
		G4double tx = momentum.x()/momentum.mag();
		G4double ty = momentum.y()/momentum.mag();
		G4double tz = momentum.z()/momentum.mag();
		
		//auto PDVol = fDet->GetG4VPhysicalVolume4ParticleDetector();
		//EInside PointInPD=PDVol->Inside(FinalPosition);
		
		G4double yCutOff = (fDet->ppsThickness/cm + fDet->airThickness/cm + fDet->ppsThickness/cm + fDet->PD_y/cm)/2 - (fDet->ppsThickness/cm + fDet->airThickness/cm + fDet->ppsThickness/cm);
		G4cout<<"yCutOff "<<yCutOff<<G4endl;
		G4cout<<"--------------"<<G4endl;
		/*
		auto SolVol = fDet->GetG4VSolid4ParticleDetector();
		EInside PointInPD=SolVol->Inside(FinalPosition);
		if(PointInPD == kInside)
		{	
			G4cout<<"!!!BANG!!!"<<G4endl;
		}
		*/
		
		if (abs(pid)== 13 || abs(pid)==211 || abs(pid)==11 || pid==22 || pid==2212 || pid==2112)
		{
			if (yy < yCutOff){
				if (abs(xx)<125.0){
					if (abs(zz)<500.0){
			G4cout<<i<<"-th particle in event # "<<eventID<<" is "<<trj->GetPDGEncoding()<<", vtx point at "<<VertexPosition<<", last point at "<<FinalPosition<<", with direction ("<<tx<<", "<<ty<<", "<<tz<<")"<<G4endl;
		
		fEvent		= eventID;
		fOutParticleID	= pid;
		fKEout		= ke/GeV;
		fXXout		= xx;
		fYYout          = yy;
		fZZout          = zz;
		fTXout          = _x;
		fTYout          = _y;
		fTZout          = _z;

		analysisManager->FillNtupleIColumn(1, 0, fEvent);
		analysisManager->FillNtupleIColumn(1, 1, fOutParticleID);
		analysisManager->FillNtupleDColumn(1, 2, fKEout);
		analysisManager->FillNtupleDColumn(1, 3, fXXout);
		analysisManager->FillNtupleDColumn(1, 4, fYYout);
		analysisManager->FillNtupleDColumn(1, 5, fZZout);
		analysisManager->FillNtupleDColumn(1, 6, fTXout);
		analysisManager->FillNtupleDColumn(1, 7, fTYout);
		analysisManager->FillNtupleDColumn(1, 8, fTZout);
		analysisManager->AddNtupleRow(1);
		
		ntrj += 1;
		}//zz
		}//xx
		}//yy
		}//mu
		
	}
	}
}
