#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "G4UIcommand.hh"
#include "Randomize.hh"

#include "G4VisExecutive.hh"

#include "G4UIExecutive.hh"

#include "mmtDetectorConstruction.hh"
#include "mmtActionInitialization.hh"
#include "FTFP_BERT.hh"
#include "mmtPrimaryGeneratorAction.hh"

int main(int argc,char** argv)
{
	//construct the default run manager
	G4RunManager* runManager = new G4RunManager;
	
	// set mandatory initialization classes:
	
	auto detConstruction = new mmtDetectorConstruction();
	runManager->SetUserInitialization(detConstruction);
	//runManager->SetUserInitialization(new mmtDetectorConstruction);
	
	auto physicsList = new FTFP_BERT;
	runManager->SetUserInitialization(physicsList);
	
	auto actionInitialization = new mmtActionInitialization(detConstruction);
	runManager->SetUserInitialization(actionInitialization);
	//runManager->SetUserInitialization(new mmtActionInitialization);
	//runManager->SetUserAction(new mmtPrimaryGeneratorAction);
	
	// initialize G4 kernel
	runManager->Initialize();
	
	auto visManager = new G4VisExecutive;
	visManager->Initialize();
	
	G4UImanager* UI = G4UImanager::GetUIpointer();
	G4cout<<"argc "<<argc<<G4endl;
	if ( argc == 1 )
	{
		// interactive mode : define UI session
		//#ifdef G4UI_USE
		G4cout<<"ui declaration "<<G4endl;
		G4UIExecutive* ui = new G4UIExecutive(argc, argv);
		G4cout<<"apply command "<<G4endl;		
		UI->ApplyCommand("/control/execute init_vis.mac");
		G4cout<<"session start "<<G4endl;
		ui->SessionStart();
		G4cout<<"delete ui "<<G4endl;
		delete ui;
		//#endif
	}
	else
	{
		G4cout<<"comes here"<<G4endl;
		//batch mode
		G4String command = "/control/execute ";
		G4String fileName = argv[1];
		
		G4cout<<"---> "<<command+fileName<<G4endl;
		
		UI->ApplyCommand(command+fileName);
	}
	
	delete runManager;
	delete visManager;
	
	return 0;
}
