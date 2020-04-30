#ifndef mmtPrimaryGeneratorAction_h
#define mmtPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ParticleTable.hh"
#include "/home/kolahalb/Works/Computation/Packages/CRY/cry_v1.7/src/CRYSetup.h"
#include "/home/kolahalb/Works/Computation/Packages/CRY/cry_v1.7/src/CRYGenerator.h"
#include "/home/kolahalb/Works/Computation/Packages/CRY/cry_v1.7/src/CRYParticle.h"
#include "/home/kolahalb/Works/Computation/Packages/CRY/cry_v1.7/src/CRYUtils.h"
#include "RNGWrapper.hh"
#include "mmtPrimaryGeneratorMessenger.hh"
//#include "mmtAnalysis.hh"

class G4ParticleGun;
class G4Event;

/// The primary generator action class with particle gum.
///
/// It defines a single particle which hits the calorimeter 
/// perpendicular to the input face. The type of the particle
/// can be changed via the G4 build-in commands of G4ParticleGun class 
/// (see the macros provided with this example).

class mmtPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
	public:
	mmtPrimaryGeneratorAction(const char* infile);
       ~mmtPrimaryGeneratorAction();
	
	void InputCRY();
	void UpdateCRY(std::string* MessInput);
	void CRYFromFile(G4String newValue);
	//mmtPrimaryGeneratorAction();
	//virtual ~mmtPrimaryGeneratorAction();
	
	virtual void GeneratePrimaries(G4Event* event);
	
	// set methods
	void SetRandomFlag(G4bool value);
	
	private:
	std::vector<CRYParticle*> *vect; // vector of generated particles
	G4ParticleTable* particleTable;
	CRYGenerator* gen;
	G4int InputState;
	G4ParticleGun* particleGun;
	G4ParticleGun*  fParticleGun; // G4 particle gun
	mmtPrimaryGeneratorMessenger* gunMessenger;
	
	double (*rngdptr)(void);
	//auto analysisManager = G4AnalysisManager::Instance();
	//G4ParticleGun*  fParticleGun; // G4 particle gun
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
