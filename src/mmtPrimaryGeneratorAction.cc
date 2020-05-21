#include "mmtPrimaryGeneratorAction.hh"
#include "mmtAnalysis.hh"
#include "mmtRunAction.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include <iomanip>
#include "TRandom3.h"
#include <ctime>
using namespace std;

mmtPrimaryGeneratorAction::mmtPrimaryGeneratorAction(const char *inputfile): G4VUserPrimaryGeneratorAction(), fParticleGun(nullptr)
{
	cout<<"ctr"<<endl;
	/*
	G4int nofParticles = 1;
	fParticleGun = new G4ParticleGun(nofParticles);
	
	G4ParticleDefinition* particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("mu+");
	fParticleGun->SetParticleDefinition(particleDefinition);
	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,-1.0,0.));
	fParticleGun->SetParticleEnergy(1.0*GeV);
	*/
	
	ifstream in;
	in.open(inputfile);
	char buffer[100000];
	
	TRandom3* r = new TRandom3(0);
	cout<<r->Rndm(0)<<endl;
	
	if (in.fail())
	{
		if( *inputfile !=0)
		G4cout << "PrimaryGeneratorAction: Failed to open CRY input file= " << inputfile << G4endl;
		InputState=-1;
	}
	else
	{
		std::string setupString("");
		while ( !in.getline(buffer,100000).eof() )
		{
			setupString.append(buffer);
			setupString.append(" ");
		}
		cout<<"CRY setup string: "<<setupString<<endl;
		CRYSetup *setup=new CRYSetup(setupString,"/home/kolahalb/Works/Computation/Packages/CRY/cry_v1.7/data/");
		gen = new CRYGenerator(setup);
		RNGWrapper<CLHEP::HepRandomEngine>::set(CLHEP::HepRandom::getTheEngine(),&CLHEP::HepRandomEngine::flat);
		setup->setRandomFunction(RNGWrapper<CLHEP::HepRandomEngine>::rng);
		
		//gen = new CRYGenerator(setup);
		InputState=0;
	}
	
	// create a vector to store the CRY particle properties
	vect=new std::vector<CRYParticle*>;
	
	// Create the table containing all particle names
	particleTable = G4ParticleTable::GetParticleTable();
	
	
	G4int nofParticles = 1;
	fParticleGun = new G4ParticleGun(nofParticles);
	
	// default particle kinematic
	//
	//auto particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("mu-");
	//fParticleGun->SetParticlePosition(G4ThreeVector(0.*m,5.1*m,0.0*m));
	//fParticleGun->SetParticleDefinition(particleDefinition);
	//fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,-1.,0.));
	//fParticleGun->SetParticleEnergy(1.*GeV);
	//
	
	// Create the messenger file
	gunMessenger = new mmtPrimaryGeneratorMessenger(this);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

mmtPrimaryGeneratorAction::~mmtPrimaryGeneratorAction()
{
	cout<<"dtr"<<endl; 
	delete fParticleGun;
}

void mmtPrimaryGeneratorAction::InputCRY()
{
	cout<<"InputCRY"<<endl; 
	InputState=1;
}

void mmtPrimaryGeneratorAction::UpdateCRY(std::string* MessInput)
{
	cout<<"UpdateCRY "<<endl; 
	CRYSetup *setup=new CRYSetup(*MessInput,"/home/kolahalb/Works/Computation/Packages/CRY/cry_v1.7/data/");
	
	gen = new CRYGenerator(setup);
	
	RNGWrapper<CLHEP::HepRandomEngine>::set(CLHEP::HepRandom::getTheEngine(),&CLHEP::HepRandomEngine::flat);
	setup->setRandomFunction(RNGWrapper<CLHEP::HepRandomEngine>::rng);
	InputState=0;

}

void mmtPrimaryGeneratorAction::CRYFromFile(G4String newValue)
{
	cout<<"CRYFromFile"<<endl;
	std::ifstream inputFile;
	inputFile.open(newValue,std::ios::in);
	char buffer[1000];

	if (inputFile.fail()) {
		G4cout << "Failed to open input file " << newValue << G4endl;
		G4cout << "Make sure to define the cry library on the command line" << G4endl;
		InputState=-1;
	}else{
		std::string setupString("");
		while ( !inputFile.getline(buffer,1000).eof()) {
			setupString.append(buffer);
			setupString.append(" ");
		}
		
		CRYSetup *setup=new CRYSetup(setupString,"/home/kolahalb/Works/Computation/Packages/CRY/cry_v1.7/data/");
		
		gen = new CRYGenerator(setup);
		
		RNGWrapper<CLHEP::HepRandomEngine>::set(CLHEP::HepRandom::getTheEngine(),&CLHEP::HepRandomEngine::flat);
		setup->setRandomFunction(RNGWrapper<CLHEP::HepRandomEngine>::rng);
		InputState=0;
	}
}

void mmtPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	// This function is called at the begining of event
	/*
	fParticleGun->SetParticlePosition(G4ThreeVector(0., 5.1*m, 0.));
	fParticleGun->GeneratePrimaryVertex(anEvent);
	*/
	
	
	// In order to avoid dependence of PrimaryGeneratorAction
	// on DetectorConstruction class we get world volume 
	// from G4LogicalVolumeStore
	//
	cout<<"GeneratePrimaries "<<InputState<<endl;
	
	G4String particleName;
	vect->clear();
	gen->genEvent(vect);
	
	//....debug output
	G4cout	<< "\nEvent=" << anEvent->GetEventID() << " "
		<< "CRY generated nparticles=" << vect->size()
		<< G4endl;
	
	//TRandom3 *random=new TRandom3(0);
	auto analysisManager = G4AnalysisManager::Instance();
	
	for ( unsigned j=0; j<vect->size(); j++) // vect->size()
	{
		particleName=CRYUtils::partName((*vect)[j]->id());
		
		//if ( abs( (*vect)[j]->PDGid() ) != 13 )
		//continue;
		//if ((*vect)[j]->ke()*MeV<100.0)
		//continue;
		
		//....debug output
		cout	<< "  "          << particleName << " "
			<< "pdg id "          << (*vect)[j]->PDGid() << "  "
			<< "charge="      << (*vect)[j]->charge() << " "
			<< setprecision(4)
			<< "energy (MeV)=" << (*vect)[j]->ke()*MeV << " "
			<< "pos (m)"
			<< G4ThreeVector((*vect)[j]->y(), (*vect)[j]->z()+6.45, (*vect)[j]->x())
			<< "time (s)"
			<< (*vect)[j]->t()*s
			<< " " << "direction cosines "
			<< G4ThreeVector((*vect)[j]->v(), (*vect)[j]->w(), (*vect)[j]->u())
			<< " " << endl;
		
		// interface of CRY co-ordinate system with GEANT4 co-ordinate system:
		// https://nusoft.fnal.gov/larsoft/doxsvn/html/CRYHelper_8cxx_source.html
		
		////energy
		//analysisManager->FillH1(2, (*vect)[j]->ke()*MeV);
		
		////position
		//analysisManager->FillH1(3, (*vect)[j]->y());
		//analysisManager->FillH1(4, (*vect)[j]->z()+0.0);
		//analysisManager->FillH1(5, (*vect)[j]->x());
		
		////direction
		//analysisManager->FillH1(6, (*vect)[j]->v());
		//analysisManager->FillH1(7, (*vect)[j]->w());
		//analysisManager->FillH1(8, (*vect)[j]->u());
		
		//mmtRunAction* pAnalysis = mmtRunAction::AnPointer;
		//pAnalysis->mcpid_in[j]=(*vect)[j]->PDGid();
		//pAnalysis->ke_in[j]=1.e-5*(*vect)[j]->ke();
		//pAnalysis->xx_in[j]=(*vect)[j]->y();
		//pAnalysis->yy_in[j]=(*vect)[j]->z()+0.0;
		//pAnalysis->zz_in[j]=(*vect)[j]->x();
		//pAnalysis->tx_in[j]=(*vect)[j]->v();
		//pAnalysis->ty_in[j]=(*vect)[j]->w();
		//pAnalysis->tz_in[j]=(*vect)[j]->u();
		
		//pAnalysis->outputTree->Fill();
		
		auto particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle((*vect)[j]->PDGid());
		
		fParticleGun->SetParticleDefinition(particleDefinition);
		fParticleGun->SetParticleEnergy((*vect)[j]->ke()*MeV);
		
		fParticleGun->SetParticlePosition(G4ThreeVector((*vect)[j]->y()*m, (*vect)[j]->z()+6.45*m, (*vect)[j]->x()*m));
		fParticleGun->SetParticleMomentumDirection(G4ThreeVector((*vect)[j]->v(), (*vect)[j]->w(), (*vect)[j]->u()));
                fParticleGun->SetParticleTime((*vect)[j]->t()*s);
		fParticleGun->GeneratePrimaryVertex(anEvent);
                //delete (*vect)[j];
		
	analysisManager->FillNtupleIColumn(0, 0, anEvent->GetEventID());
        analysisManager->FillNtupleIColumn(0, 1, (*vect)[j]->PDGid());
        analysisManager->FillNtupleDColumn(0, 2, (*vect)[j]->ke()/GeV);
        analysisManager->FillNtupleDColumn(0, 3, (*vect)[j]->y()*m);
        analysisManager->FillNtupleDColumn(0, 4, (*vect)[j]->z()+6.45*m);
        analysisManager->FillNtupleDColumn(0, 5, (*vect)[j]->x()*m);
	analysisManager->FillNtupleDColumn(0, 5, (*vect)[j]->t()/s);
        analysisManager->FillNtupleDColumn(0, 6, (*vect)[j]->v());
        analysisManager->FillNtupleDColumn(0, 7, (*vect)[j]->w());
        analysisManager->FillNtupleDColumn(0, 8, (*vect)[j]->u());
        analysisManager->AddNtupleRow(0);

	delete (*vect)[j];
	}
	
	//delete random;
}
