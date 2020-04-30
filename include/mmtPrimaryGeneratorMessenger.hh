#ifndef mmtPrimaryGeneratorMessenger_h
#define mmtPrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class mmtPrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;
class G4UIcmdWithADoubleAndUnit;

class mmtPrimaryGeneratorMessenger: public G4UImessenger
{
	public:
		mmtPrimaryGeneratorMessenger(mmtPrimaryGeneratorAction*);
	       ~mmtPrimaryGeneratorMessenger();

		void SetNewValue(G4UIcommand*, G4String);

	private:
		mmtPrimaryGeneratorAction*   Action;
		G4UIdirectory*               CRYDir;
		G4UIcmdWithAString*          FileCmd;
		G4UIcmdWithAString*          InputCmd;
		G4UIcmdWithoutParameter*     UpdateCmd;
		std::string* MessInput;
};

#endif
