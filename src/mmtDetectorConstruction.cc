#include "mmtDetectorConstruction.hh"
#include "mmtMagField.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4EqMagElectricField.hh"
#include "G4ClassicalRK4.hh"
#include "G4SimpleRunge.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4MagIntegratorDriver.hh"
#include "G4ChordFinder.hh"
#include "G4PropagatorInField.hh"

#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4Mag_UsualEqRhs.hh"
//G4ThreadLocal G4GlobalMagFieldMessenger* mmtDetectorConstruction::fMagFieldMessenger = nullptr;

mmtDetectorConstruction::mmtDetectorConstruction():G4VUserDetectorConstruction(),fCheckOverlaps(true)
{
	//fField.Put(0);
}

mmtDetectorConstruction::~mmtDetectorConstruction()
{
}

G4VPhysicalVolume* mmtDetectorConstruction::Construct()
{
	DefineMaterials();
	return DefineVolumes();
}
void mmtDetectorConstruction::DefineMaterials()
{
	// Lead material defined using NIST Manager
	auto nistManager = G4NistManager::Instance();
	
	//G4Material* vac= new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density, kStateGas, 2.73*kelvin, 3.e-18*pascal);
	G4Material* air=nistManager->FindOrBuildMaterial("G4_AIR");
	G4Material* pps=nistManager->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
	G4cout<<"Kolahal: MMTDetectorConstruction::DefineMaterials() -> "<< *(G4Material::GetMaterialTable()) << G4endl;
}
G4VPhysicalVolume* mmtDetectorConstruction::DefineVolumes()
{
        // Geometry parameters
	G4int nofLayers = 1;
        G4double airThickness = 1000.0*cm;
        G4double ppsThickness = 10.0*cm;
        G4double calorSizeXY  = 1000.0*cm;
        //G4double airThickness = 100.0*cm;
        //G4double ppsThickness = 1.0*cm;
        //G4double calorSizeXY  = 100.0*cm;
	
        auto layerThickness = ppsThickness + airThickness + ppsThickness;
        auto calorThickness = nofLayers * layerThickness;
        auto worldSizeXY = 1.05 * calorSizeXY;
        auto worldSizeZ  = 1.05 * calorThickness;
	
	///////////////////////////////////////////////////////////////////////////
        // Get materials
	
	// Material definition
	G4NistManager* nistManager = G4NistManager::Instance();

	// Air defined using NIST Manager
	nistManager->FindOrBuildMaterial("G4_AIR");

	// Lead defined using NIST Manager
	auto defaultMaterial = nistManager->FindOrBuildMaterial("G4_Galactic");
	auto MagTrapMaterial = nistManager->FindOrBuildMaterial("G4_AIR");
	auto ScintlrMaterial = nistManager->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
	/*
        //auto defaultMaterial = G4Material::GetMaterial("Galactic");
	auto defaultMaterial = G4Material::GetMaterial("G4_AIR");//G4_AIR
	auto MagTrapMaterial = G4Material::GetMaterial("G4_Fe");//G4_AIR
	auto ScintlrMaterial = G4Material::GetMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
	*///////////////////////////////////////////////////////////////////////////
	G4RotationMatrix* Rot = new G4RotationMatrix;
	Rot->rotateX(M_PI/2.*rad);//M_PI/2.*rad
	/*
	G4RotationMatrix* Rot = new G4RotationMatrix;
	Rot->rotateX(M_PI/2.*rad);
	
	//World:
	auto worldS = new G4Box("World", worldSizeXY/2, worldSizeXY/2, worldSizeZ/2);
	auto worldLV= new G4LogicalVolume(worldS, defaultMaterial, "World");
	auto worldPV= new G4PVPlacement(0, G4ThreeVector(), worldLV, "World", 0, false, 0, fCheckOverlaps);
	
	// Calorimeter
	auto calorimeterS= new G4Box("Calorimeter",calorSizeXY/2, calorSizeXY/2, calorThickness/2);
	auto calorLV	 = new G4LogicalVolume(calorimeterS, defaultMaterial,"Calorimeter");
	auto calorPV	 = new G4PVPlacement(0, G4ThreeVector(), calorLV, "Calorimeter", worldLV, false, 0, fCheckOverlaps);
	auto calorPV	 = new G4PVPlacement(Rot, G4ThreeVector(), calorLV, "Calorimeter", worldLV, false, 0, fCheckOverlaps);
	
	// Layer
	auto layerS = new G4Box("Layer", calorSizeXY/2, calorSizeXY/2, layerThickness/2);
	auto layerLV= new G4LogicalVolume(layerS, defaultMaterial, "Layer");
	auto replica= new G4PVReplica("Layer", layerLV, calorLV, kZAxis, nofLayers, layerThickness);
	
	// Air
	auto mmtAirS 	= new G4Box("mmtAir", calorSizeXY/2, calorSizeXY/2, airThickness/2);
	auto mmtAirLV	= new G4LogicalVolume(mmtAirS, MagTrapMaterial, "mmtAir");
	mmtAirPV	= new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), mmtAirLV, "mmtAir", layerLV, false, 0, fCheckOverlaps);
	
	// Scintillator - up
	auto ppsPlate1S	= new G4Box("pps_up", calorSizeXY/2, calorSizeXY/2, ppsThickness/2);
	auto ppsPlate1LV= new G4LogicalVolume(ppsPlate1S, ScintlrMaterial, "pps_up");
	ppsPlate1PV	= new G4PVPlacement(0, G4ThreeVector(0.,0.,+(airThickness/2+ppsThickness/2)), ppsPlate1LV, "pps_up", layerLV, false, 0, fCheckOverlaps);
	ppsPlate1PV	= new G4PVPlacement(0, G4ThreeVector(0., 0., (airThickness/2+ppsThickness/2)), ppsPlate1LV, "pps_up", layerLV, false, 0, fCheckOverlaps);
	
	// Scintillator - dn
	auto ppsPlate2S = new G4Box("pps_dn", calorSizeXY/2, calorSizeXY/2, ppsThickness/2);
	auto ppsPlate2LV= new G4LogicalVolume(ppsPlate2S, ScintlrMaterial, "pps_dn");
	ppsPlate2PV     = new G4PVPlacement(0, G4ThreeVector(0.,0.,-(airThickness/2+ppsThickness/2)), ppsPlate2LV, "pps_dn", layerLV, false, 0, fCheckOverlaps);
	*/
	
	//World:
	auto worldS 	= new G4Box("World", worldSizeXY/2, worldSizeXY/2, worldSizeZ/2);
	auto worldLV	= new G4LogicalVolume(worldS, defaultMaterial, "World");
	auto worldPV	= new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), worldLV, "World", 0, false, 0, fCheckOverlaps);
	
	// Air
	auto mmtAirS	= new G4Box("mmtAir", calorSizeXY/2, calorSizeXY/2, airThickness/2);
	auto mmtAirLV	= new G4LogicalVolume(mmtAirS, MagTrapMaterial, "mmtAir");
	mmtAirPV	= new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), mmtAirLV, "mmtAir", worldLV, false, 0, fCheckOverlaps);
	
	// Scintillator - up
	auto ppsPlate1S = new G4Box("pps_up", calorSizeXY/2, calorSizeXY/2, ppsThickness/2);
	auto ppsPlate1LV= new G4LogicalVolume(ppsPlate1S, ScintlrMaterial, "pps_up");
	ppsPlate1PV     = new G4PVPlacement(Rot, G4ThreeVector(0.,+(airThickness/2+ppsThickness/2), 0.), ppsPlate1LV, "pps_up", worldLV, false, 0, fCheckOverlaps);
	
	// Scintillator - dn
	auto ppsPlate2S = new G4Box("pps_dn", calorSizeXY/2, calorSizeXY/2, ppsThickness/2);
	auto ppsPlate2LV= new G4LogicalVolume(ppsPlate2S, ScintlrMaterial, "pps_dn");
	ppsPlate2PV     = new G4PVPlacement(Rot, G4ThreeVector(0.,-(airThickness/2+ppsThickness/2), 0.), ppsPlate2LV, "pps_dn", worldLV, false, 0, fCheckOverlaps);
	//ppsPlate2PV     = new G4PVPlacement(0, G4ThreeVector(0., 0.,-(airThickness/2+ppsThickness/2)), ppsPlate2LV, "pps_dn", layerLV, false, 0, fCheckOverlaps);
	
  	G4cout
    	<< G4endl
    	<< "------------------------------------------------------------" << G4endl
    	<< "---> The Magnetic mirror trap is " << nofLayers << " layers of: [ "
	<< ppsThickness/cm << "cm of " << ScintlrMaterial->GetName()
	<< " + "
    	<< airThickness/cm << "cm of " << MagTrapMaterial->GetName()
    	<< " + "
    	<< ppsThickness/cm << "cm of " << ScintlrMaterial->GetName() << " ] " << G4endl
	<< ppsThickness/mm << "mm of " << ScintlrMaterial->GetName()
	<< " + "
    	<< airThickness/mm << "mm of " << MagTrapMaterial->GetName()
    	<< " + "
    	<< ppsThickness/mm << "mm of " << ScintlrMaterial->GetName() << " ] " << G4endl
    	<< "------------------------------------------------------------" << G4endl;
	
	// Visualization attributes
	
	//worldLV->SetVisAttributes (G4VisAttributes::GetInvisible());
	//auto simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,0.0,0.0));
	//simpleBoxVisAtt->SetVisibility(true);
	
	//mmtAirLV->SetVisAttributes(simpleBoxVisAtt);
	//auto simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
	//simpleBoxVisAtt->SetVisibility(true);
	//calorLV->SetVisAttributes(simpleBoxVisAtt);
	
	return worldPV;
}
void mmtDetectorConstruction::ConstructSDandField()
{
	/*
	auto pps_up_SD = new mmtSD("Scintillator_Up_Panel");
	G4SDManager::GetSDMpointer()->AddNewDetector(pps_up_SD);
	SetSensitiveDetector("pps_up",pps_up_SD);
	
	auto pps_dn_SD = new mmtSD("Scintillator_Dn_Panel");
	G4SDManager::GetSDMpointer()->AddNewDetector(pps_dn_SD);
	SetSensitiveDetector("pps_dn",pps_dn_SD);
	*/
	/*
	mmtMagneticField* fField        = new mmtMagneticField();
	//if(!fField)
	//fField = new mmtMagneticField();
	G4FieldManager* fFieldMgr = G4TransportationManager::GetTransportationManager()->GetFieldManager();
	fFieldMgr->SetDetectorField(fField);
	fFieldMgr->CreateChordFinder(fField);
	*/
	
	mmtMagneticField* fField	= new mmtMagneticField();
	G4FieldManager* fFieldMgr	= G4TransportationManager::GetTransportationManager()->GetFieldManager();
	fFieldMgr->SetDetectorField(fField);
	G4Mag_UsualEqRhs *fEquation = new G4Mag_UsualEqRhs(fField);
	G4MagIntegratorStepper *fStepper = new G4ClassicalRK4(fEquation);
	
	G4ChordFinder *fChordFinder = new G4ChordFinder(fField, 1.e-1*mm, fStepper);//1.e-1
	fChordFinder->SetDeltaChord(1.0e-3*mm);//1.0e-3
	fFieldMgr->SetChordFinder(fChordFinder);
	fFieldMgr->SetDeltaOneStep(1.0e-3*mm);//1.0e-3
	fFieldMgr->SetDeltaIntersection(1.0e-4*mm);//1.0e-4
	
	G4PropagatorInField* fieldPropagator = G4TransportationManager::GetTransportationManager()->GetPropagatorInField();
	fieldPropagator->SetMinimumEpsilonStep(1.e-5*mm);//1.e-5
	fieldPropagator->SetMaximumEpsilonStep(1.e-2*mm);//1.e-2
	
	/*
	fIntgrDriver = new G4MagInt_Driver(0.000001*mm,fStepper,fStepper->GetNumberOfVariables() );
	fChordFinder = new G4ChordFinder(fIntgrDriver);
	fFieldMgr->SetChordFinder(fChordFinder);
	fFieldMgr->SetDetectorField(fField);
	
	fFieldMgr->GetChordFinder()->SetDeltaChord(1e-9*m);
	fFieldMgr->SetDeltaIntersection(1e-9*m);
	fFieldMgr->SetDeltaOneStep(1e-9*m);
	
	fPropInField = G4TransportationManager::GetTransportationManager()->GetPropagatorInField();
	fPropInField->SetMinimumEpsilonStep(1e-11);
	fPropInField->SetMaximumEpsilonStep(1e-10);
	*/
}
