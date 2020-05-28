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
	//auto nistManager = G4NistManager::Instance();
	
	//G4Material* vac= new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density, kStateGas, 2.73*kelvin, 3.e-18*pascal);
	//G4Material* air=nistManager->FindOrBuildMaterial("G4_AIR");
	//G4Material* pps=nistManager->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
	G4cout<<"Kolahal: MMTDetectorConstruction::DefineMaterials() -> "<< *(G4Material::GetMaterialTable()) << G4endl;
}
G4VPhysicalVolume* mmtDetectorConstruction::DefineVolumes()
{
        // Geometry parameters
	G4int nofLayers = 1;
        airThickness = 200.0*cm;
        ppsThickness = 1.0000*cm;
        G4double calorSizeXY  = 1000.0*cm;
	
        auto layerThickness = ppsThickness + airThickness + ppsThickness;
        auto calorThickness = nofLayers * layerThickness;
	auto PD_x = 5.0*m;
	PD_y = 2.5*m;
	auto PD_z = 5.0*m;
	
	// introduce world spanning a large area
	auto world_x = 100.0*m;
	auto world_y = 1.05*(calorThickness + PD_y);
	// world z extent just enough to contain the trap
	auto world_z = 100.0*m;
        
	///////////////////////////////////////////////////////////////////////////
        // Get materials
	// Material definition
	G4NistManager* nistManager = G4NistManager::Instance();
	
	// Air defined using NIST Manager
	nistManager->FindOrBuildMaterial("G4_AIR");
	
	// Lead defined using NIST Manager
	auto defaultMaterial = nistManager->FindOrBuildMaterial("G4_Galactic");//G4_Galactic
	auto MagTrapMaterial = nistManager->FindOrBuildMaterial("G4_Galactic");//G4_CONCRETE//G4_AIR
	auto ScintlrMaterial = nistManager->FindOrBuildMaterial("G4_Galactic");
	/*
        //auto defaultMaterial = G4Material::GetMaterial("Galactic");
	auto defaultMaterial = G4Material::GetMaterial("G4_AIR");//G4_AIR
	auto MagTrapMaterial = G4Material::GetMaterial("G4_Fe");//G4_AIR
	auto ScintlrMaterial = G4Material::GetMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
	*///////////////////////////////////////////////////////////////////////////
	//G4RotationMatrix* Rot = new G4RotationMatrix;
	//Rot->rotateX(M_PI/2.*rad);//M_PI/2.*rad
	
	//World:
	auto worldS 	= new G4Box("World", world_x/2, world_y/2, world_z/2);
	auto worldLV	= new G4LogicalVolume(worldS, defaultMaterial, "World");
	auto worldPV	= new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), worldLV, "World", 0, false, 0, fCheckOverlaps);
	
	// Reference level
	auto RefPlaneSV  = new G4Box("ReferenceLevel", world_x/2, 1.0*cm, world_z/2);
	auto RefPlaneLV  = new G4LogicalVolume(RefPlaneSV, defaultMaterial, "RefLevel");
	RefPlanePV	 = new G4PVPlacement(0, G4ThreeVector(0., +(airThickness/2+2.0*ppsThickness+PD_y/2), 0.), RefPlaneLV, "RefLevel", worldLV, false, 0, fCheckOverlaps);
	
	// Magnetic Trap
	auto mmtAirS	= new G4Box("mmtAir", calorSizeXY/2, airThickness/2, calorSizeXY/2);
	mmtAirLV	= new G4LogicalVolume(mmtAirS, MagTrapMaterial, "mmtAir");
	mmtAirPV	= new G4PVPlacement(0, G4ThreeVector(0., 0.+PD_y/2, 0.), mmtAirLV, "mmtAir", worldLV, false, 0, fCheckOverlaps);
	
	// Scintillator - up
	auto ppsPlate1S = new G4Box("pps_up", calorSizeXY/2, ppsThickness/2, calorSizeXY/2);
	auto ppsPlate1LV= new G4LogicalVolume(ppsPlate1S, ScintlrMaterial, "pps_up");
	ppsPlate1PV     = new G4PVPlacement(0, G4ThreeVector(0.,+(airThickness/2+ppsThickness/2+PD_y/2),0.), ppsPlate1LV, "pps_up", worldLV, false, 0, fCheckOverlaps);
	
	// Scintillator - dn
	auto ppsPlate2S = new G4Box("pps_dn", calorSizeXY/2, ppsThickness/2, calorSizeXY/2);
	auto ppsPlate2LV= new G4LogicalVolume(ppsPlate2S, ScintlrMaterial, "pps_dn");
	ppsPlate2PV     = new G4PVPlacement(0, G4ThreeVector(0.,-(airThickness/2+ppsThickness/2)+PD_y/2,0.), ppsPlate2LV, "pps_dn", worldLV, false, 0, fCheckOverlaps);

	// Particle Detector (PD)
	PD_SV = new G4Box("ParticleDetector", PD_x/2, PD_y/2, PD_z/2);
	auto PD_LV = new G4LogicalVolume(PD_SV, defaultMaterial, "ParticleDetector");
	PD_PV = new G4PVPlacement(0, G4ThreeVector(0.,-(airThickness/2+ppsThickness/2),0.), PD_LV, "ParticleDetector", worldLV, false, 0, fCheckOverlaps);
	
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
	G4VisAttributes* worldVis = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
	worldLV->SetVisAttributes(worldVis);

	G4VisAttributes* RefPlaneVis = new G4VisAttributes(G4Colour(1.0,0.0,0.0));
	//RefPlaneVis->SetForceAuxEdgeVisible(true);
	RefPlaneLV->SetVisAttributes(RefPlaneVis);
	
	G4VisAttributes* MagTrapVis = new G4VisAttributes(G4Colour(0.0,1.0,0.0));
	mmtAirLV->SetVisAttributes(MagTrapVis);

	G4VisAttributes* PPS_upVis = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
	ppsPlate1LV->SetVisAttributes(PPS_upVis);

	G4VisAttributes* PPS_dnVis = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
	ppsPlate2LV->SetVisAttributes(PPS_dnVis);

	G4VisAttributes* DetVis = new G4VisAttributes(G4Colour(0.7,0.4,.0));
	PD_LV->SetVisAttributes(DetVis);

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
	//G4FieldManager* fFieldMgr	= G4TransportationManager::GetTransportationManager()->GetFieldManager();
	//fFieldMgr->SetDetectorField(fField);
	G4FieldManager* fFieldMgr	= new G4FieldManager(fField);
	G4bool alllocal = true;
	mmtAirLV->SetFieldManager(fFieldMgr, alllocal);
	
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
	//G4bool alllocal = true;
	//mmtAirLV->SetFieldManager(fFieldMgr, alllocal);
	
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
