#ifndef mmtTrajectory_h
#define mmtTrajectory_h 1

#include <vector>
#include <stdlib.h>

#include "globals.hh"

#include "G4ios.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Allocator.hh"
#include "G4VTrajectory.hh"
#include "G4ParticleDefinition.hh"

#include "G4TrajectoryPoint.hh"

//#include "mmtDetectorConstruction.hh"
class G4LogicalVolume;

typedef std::vector<G4VTrajectoryPoint*> TrajectoryPointContainer;

class mmtTrajectory : public G4VTrajectory
{

//--------
   public: // without description
//--------

// Constructor/Destructor

     mmtTrajectory();
     mmtTrajectory(const G4Track* aTrack);
     mmtTrajectory(mmtTrajectory&);
     virtual ~mmtTrajectory();

// Operators

     inline void* operator new(size_t);
     inline void  operator delete(void*);
     inline int operator == (const mmtTrajectory& right) const
     { return (this==&right); }

// Get/Set functions

     inline G4int GetTrackID() const { return fTrackID; }
     inline G4int GetParentID() const { return fParentID; }
     inline G4String GetParticleName() const { return fParticleName; }
     inline G4double GetCharge() const { return fPDGCharge; }
     inline G4int GetPDGEncoding() const { return fPDGEncoding; }
     inline G4ThreeVector GetInitialMomentum() const {return fInitialMomentum;};
     inline G4double GetInitialKineticEnergy() {return fInitialKineticEnergy;}

     inline G4double GetFinalKineticEnergy() const {return fFinalKineticEnergy;}
     inline G4ThreeVector GetFinalMomentum() const {return fFinalMomentum;}
     inline G4ThreeVector GetFinalDirection() const {return fFinalDirection;}
     inline G4double GetFinalTime() const {return fFinalTime;}
     inline G4int GetState() const {return fState;}

// Other member functions

     virtual void ShowTrajectory(std::ostream& os=G4cout) const;
     virtual void AppendStep(const G4Step* aStep);
     virtual void MergeTrajectory(G4VTrajectory* secondTrajectory);

     G4ParticleDefinition* GetParticleDefinition();

     virtual int GetPointEntries() const
     { return fpPointsContainer->size(); }
     virtual G4VTrajectoryPoint* GetPoint(G4int i) const
     { return (*fpPointsContainer)[i]; }

    virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
    virtual std::vector<G4AttValue>* CreateAttValues() const;


//---------
   private:
//---------

// Member data

     TrajectoryPointContainer* fpPointsContainer;

     G4int         fTrackID;
     G4int         fParentID;
     G4double      fPDGCharge;
     G4int         fPDGEncoding;
     G4String      fParticleName;
     G4ThreeVector fInitialMomentum;
     G4double      fInitialKineticEnergy;

     G4double	   fFinalKineticEnergy;
     G4ThreeVector fFinalMomentum;
     G4ThreeVector fFinalDirection;
     G4double      fFinalTime;
     G4double	   fState;

     G4LogicalVolume* fTargetVolume;
};

extern G4ThreadLocal G4Allocator<mmtTrajectory>* mmtTrajectoryAllocator;

inline void* mmtTrajectory::operator new(size_t) {
    if(!mmtTrajectoryAllocator)
      mmtTrajectoryAllocator = new G4Allocator<mmtTrajectory>;
    return (void*) mmtTrajectoryAllocator->MallocSingle();
}

inline void mmtTrajectory::operator delete(void* aTrajectory) {
    mmtTrajectoryAllocator->FreeSingle((mmtTrajectory*)aTrajectory);
}

#endif
