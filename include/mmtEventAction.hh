#ifndef mmtEventAction_h
#define mmtEventAction_h 1

#include "G4UserEventAction.hh"
#include "mmtRunAction.hh"
#include "globals.hh"
class mmtEventAction : public G4UserEventAction
{
	public:
	mmtEventAction();
       ~mmtEventAction();
	
	virtual void BeginOfEventAction(const G4Event* event);
	virtual void EndOfEventAction(const G4Event* event);
	
	void AddAbs(G4double de, G4double dl);
	void AddGap(G4double de, G4double dl);
	
	int new_event;
	
	private:
	G4int	 fEvent;

	G4int	 fInParticleID;
	G4double fKE_in;
	G4double fXX_in;
	G4double fYY_in;
	G4double fZZ_in;
	G4double fTX_in;
	G4double fTY_in;
	G4double fTZ_in;
	
	G4int fOutParticleID;
	G4double fKEout;
	G4double fXXout;
	G4double fYYout;
	G4double fZZout;
	G4double fTXout;
	G4double fTYout;
	G4double fTZout;
};

#endif
