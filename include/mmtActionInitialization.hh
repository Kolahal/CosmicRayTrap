#ifndef mmtActionInitialization_h
#define mmtActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

class mmtDetectorConstruction;
class mmtRunAction;

class mmtActionInitialization : public G4VUserActionInitialization
{
	public:
	mmtActionInitialization(mmtDetectorConstruction*);
       ~mmtActionInitialization();
	
	virtual void BuildForMaster() const;
	virtual void Build() const;
	
	private:
	mmtDetectorConstruction* fDetConstruction;
	mmtRunAction* runAction;
};

#endif
