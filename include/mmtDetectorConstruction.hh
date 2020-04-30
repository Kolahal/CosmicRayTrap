#ifndef mmtDetectorConstruction_h
#define mmtDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4Cache.hh"
#include "G4MagneticField.hh"
#include "G4EqMagElectricField.hh"
class G4VPhysicalVolume;
class G4LogicalVolume;
class G4GlobalMagFieldMessenger;
class G4UniformMagField;
class mmtMagField;
//class G4EqMagElectricField;
class G4MagInt_Driver;
class G4MagIntegratorStepper;
class G4ChordFinder;

class mmtDetectorConstruction : public G4VUserDetectorConstruction
{
	public:
		mmtDetectorConstruction();
	       ~mmtDetectorConstruction();
		
	public:
		virtual G4VPhysicalVolume* Construct();
		virtual void ConstructSDandField();
		
		//Get methods
		const G4VPhysicalVolume* GetMMTAirPV() const;
		const G4VPhysicalVolume* GetPPS1PV() const;
		const G4VPhysicalVolume* GetPPS2PV() const;
		
		//G4Cache<G4MagneticField*> fField;
		
	private:
		void DefineMaterials();
		G4VPhysicalVolume* DefineVolumes();
		
		static G4ThreadLocal G4GlobalMagFieldMessenger*  fMagFieldMessenger;
		G4bool  fCheckOverlaps;
		
		G4VPhysicalVolume* mmtAirPV;
		G4VPhysicalVolume* ppsPlate1PV;
		G4VPhysicalVolume* ppsPlate2PV;
		
		//G4EqMagElectricField*	fEquation;
		//G4ChordFinder*		fChordFinder;
};

inline const G4VPhysicalVolume* mmtDetectorConstruction::GetMMTAirPV() const 
{
  return mmtAirPV;
}
inline const G4VPhysicalVolume* mmtDetectorConstruction::GetPPS1PV() const {
  return ppsPlate1PV;
}
inline const G4VPhysicalVolume* mmtDetectorConstruction::GetPPS2PV() const {
  return ppsPlate2PV;
}

#endif
