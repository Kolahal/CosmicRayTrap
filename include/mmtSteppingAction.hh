#ifndef mmtSteppingAction_h
#define mmtSteppingAction_h 1

#include "G4UserSteppingAction.hh"

class mmtDetectorConstruction;
class mmtEventAction;

/// Stepping action class.
//In UserSteppingAction() there are collected the energy deposit and track 
//lengths of charged particles in Absober and Gap layers and
//updated in mmtEventAction.

class mmtSteppingAction : public G4UserSteppingAction
{
	public:
		mmtSteppingAction(const mmtDetectorConstruction* detectorConstruction, mmtEventAction* eventAction);
	       ~mmtSteppingAction();

		virtual void UserSteppingAction(const G4Step* step);
		int test_int;
	private:
		const mmtDetectorConstruction* fDetConstruction;
		mmtEventAction*  fEventAction;
		int total_no_of_particles;
		int n_final_state;
};

#endif
