#include "mmtRunAction.hh"
#include "mmtAnalysis.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

mmtRunAction::mmtRunAction(): G4UserRunAction(), outputFile(0), outputTree(0)
{
	//AnPointer = this;
	G4cout<<"mmtRunAction::Ctr"<<G4endl;
	// set printing event number per each event
	G4RunManager::GetRunManager()->SetPrintProgress(1);
	
	auto analysisManager = G4AnalysisManager::Instance();
	analysisManager->SetVerboseLevel(1);
	analysisManager->SetNtupleMerging(true);
	
	analysisManager->CreateNtuple("CosmicMuInTuple", "ComicMuInfo");
	analysisManager->CreateNtupleIColumn("InEventID");
	analysisManager->CreateNtupleIColumn("InParticleID");
	analysisManager->CreateNtupleDColumn("ke_in");
	analysisManager->CreateNtupleDColumn("xx_in");
	analysisManager->CreateNtupleDColumn("yy_in");
	analysisManager->CreateNtupleDColumn("zz_in");
	analysisManager->CreateNtupleDColumn("tx_in");
	analysisManager->CreateNtupleDColumn("ty_in");
	analysisManager->CreateNtupleDColumn("tz_in");
	analysisManager->CreateNtupleDColumn("tm_in");
	analysisManager->FinishNtuple();

	analysisManager->CreateNtuple("CosmicMuOutTuple", "ComicMuOutfo");
	analysisManager->CreateNtupleIColumn("OutEventID");
	analysisManager->CreateNtupleIColumn("OutParticleID");
	analysisManager->CreateNtupleDColumn("keEnd");
	analysisManager->CreateNtupleDColumn("xxEnd");
	analysisManager->CreateNtupleDColumn("yyEnd");
	analysisManager->CreateNtupleDColumn("zzEnd");
	analysisManager->CreateNtupleDColumn("txEnd");
	analysisManager->CreateNtupleDColumn("tyEnd");
	analysisManager->CreateNtupleDColumn("tzEnd");
	analysisManager->CreateNtupleDColumn("tmEnd");
	analysisManager->FinishNtuple();
}
mmtRunAction::~mmtRunAction()
{
	G4cout<<"mmtRunAction::Dtr"<<G4endl;
	delete G4AnalysisManager::Instance();
}
void mmtRunAction::BeginOfRunAction(const G4Run* run)
{
	/*
	*/
	G4cout<<"BeginOfRunAction"<<G4endl;
	//inform the runManager to save random number seed
	G4RunManager::GetRunManager()->SetRandomNumberStore(false);

	auto analysisManager = G4AnalysisManager::Instance();
	analysisManager->OpenFile("mmtApplication.root");
}
void mmtRunAction::EndOfRunAction(const G4Run* /*run*/)
{
        G4cout<<"EndOfRunAction"<<G4endl;

        auto analysisManager = G4AnalysisManager::Instance();
        analysisManager->Write();
        analysisManager->CloseFile();
}
/*
mmtRunAction::mmtRunAction(): G4UserRunAction()
{
	// set printing event number per each event
	G4RunManager::GetRunManager()->SetPrintProgress(1);
}
mmtRunAction::~mmtRunAction()
{
}

void mmtRunAction::BeginOfRunAction(const G4Run* //run//)
{
}
void mmtRunAction::EndOfRunAction(const G4Run* //run//)
{
}
*/
