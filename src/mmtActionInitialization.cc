#include "mmtActionInitialization.hh"
#include "mmtPrimaryGeneratorAction.hh"
#include "mmtRunAction.hh"
#include "mmtEventAction.hh"
#include "mmtSteppingAction.hh"
#include "mmtDetectorConstruction.hh"

mmtActionInitialization::mmtActionInitialization(mmtDetectorConstruction* detConstruction): G4VUserActionInitialization(),fDetConstruction(detConstruction)
{
}
mmtActionInitialization::~mmtActionInitialization()
{
}
void mmtActionInitialization::BuildForMaster() const
{
	SetUserAction(new mmtRunAction);
}

void mmtActionInitialization::Build() const
{
	const char* infile="/home/kolahalb/Works/Computation/Projects/MMT/MMTSim/MMT/setup.file";
	SetUserAction(new mmtPrimaryGeneratorAction(infile));
	//SetUserAction(new mmtPrimaryGeneratorAction);
	SetUserAction(new mmtRunAction);
	auto eventAction = new mmtEventAction(fDetConstruction);
	SetUserAction(eventAction);
	SetUserAction(new mmtSteppingAction(fDetConstruction,eventAction));
}
