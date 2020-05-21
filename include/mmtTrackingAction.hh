#ifndef mmtTrackingAction_h
#define mmtTrackingAction_h 1

#include "globals.hh"
#include "G4UserTrackingAction.hh"

//class mmtTrackingActionMessenger;

class mmtTrackingAction : public G4UserTrackingAction
{
	public:
		mmtTrackingAction();
		virtual ~mmtTrackingAction() {};

		virtual void PreUserTrackingAction(const G4Track*);
		virtual void PostUserTrackingAction(const G4Track*);
		/*
		inline void		SetOutTrackId(G4int val){iOutTrackId=val;strOutParticle="all";}; 
		inline G4int	GetOutTrackId(){return iOutTrackId;}; 
		inline void		SetNoSecondary(G4int val){iNoSecondary=val;}; 
		inline G4int	GetNoSecondary(){return iNoSecondary;};
		inline void		SetOutParticleName(G4String val){strOutParticle=val;iOutTrackId=-1;}; 
		inline G4String	GetOutParticleName(){return strOutParticle;}; 

	private:
		G4int		iOutTrackId;	//default value -1
		G4int		iNoSecondary;	//default value 1
		G4String	strOutParticle;	//default value "all"

		mmtTrackingActionMessenger *messenger;
		*/
};

#endif
