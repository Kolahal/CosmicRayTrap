#ifndef mmtMagField_HH
#define mmtMagField_HH

#include "G4UniformMagField.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include "G4MagneticField.hh"

class G4FieldManager;

class mmtMagneticField : public G4MagneticField
{
	public:
		mmtMagneticField();
	       ~mmtMagneticField();
		
		virtual void GetFieldValue(const G4double point[4], double* bField ) const;

		void SetField(G4double val) { fBy = val; }
		G4double GetField() const { return fBy; }
	
//	protected:
//              PyObject *pName, *pModule, *pFunc;
//              PyObject *pArgs, *pValue;
	
	private:
		//void DefineCommands();
		//PyObject *pName, *pModule, *pFunc;
		//PyObject *pArgs, *pValue;
		
	private:
		//void DefineCommands();
		G4double fBx;
		G4double fBy;
		G4double fBz;
};
#endif
