#ifndef mmtTrajectoryPoint_h
#define mmtTrajectoryPoint_h 1

#include "G4TrajectoryPoint.hh"
#include "G4TouchableHandle.hh"

#include "globals.hh"
#include "G4Allocator.hh"

#include "G4ThreeVector.hh"
#include "G4StepStatus.hh"

class G4Track;
class G4Step;
class G4VProcess;

class mmtTrajectoryPoint : public G4TrajectoryPoint {

	//--------
	public: // without description
		//--------

		// Constructor/Destructor

		mmtTrajectoryPoint();
		mmtTrajectoryPoint(const G4Track* aTrack);
		mmtTrajectoryPoint(const G4Step* aStep);
		mmtTrajectoryPoint(mmtTrajectoryPoint &right);
		virtual ~mmtTrajectoryPoint();
		
		// Operators

		inline void *operator new(size_t);
		inline void operator delete(void *aTrajectoryPoint);
		inline int operator==(const mmtTrajectoryPoint& right) const
		{ return (this==&right); };

		// Get/Set functions
		
		const std::vector<G4ThreeVector>* GetAuxiliaryPoints() const
			{ return fpAuxiliaryPointVector; }

		inline G4double GetTime() const { return fTime; };
		inline const G4ThreeVector GetMomentum() const { return fMomentum; };
		inline G4StepStatus GetStepStatus() const { return fStepStatus; };
		inline G4String GetVolumeName() const { return fVolumeName; };

		// Get method for HEPRep style attributes

		virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
		virtual std::vector<G4AttValue>* CreateAttValues() const;
		
		//G4double findTime(mmtTrajectoryPoint* p);
		//G4ThreeVector findMomentum();//mmtTrajectoryPoint* p

		//---------
	private:
		//---------
		std::vector<G4ThreeVector>* fpAuxiliaryPointVector;
		// Member data

		G4double      fTime;
		G4ThreeVector fMomentum;
		G4StepStatus  fStepStatus;
		G4String      fVolumeName;

};

extern G4ThreadLocal G4Allocator<mmtTrajectoryPoint>* mmtTrajPointAllocator;

inline void* mmtTrajectoryPoint::operator new(size_t)
{
	if(!mmtTrajPointAllocator)
		mmtTrajPointAllocator = new G4Allocator<mmtTrajectoryPoint>;
	return (void *) mmtTrajPointAllocator->MallocSingle();
}

inline void mmtTrajectoryPoint::operator delete(void *aTrajectoryPoint)
{
	mmtTrajPointAllocator->FreeSingle(
			(mmtTrajectoryPoint *) aTrajectoryPoint);
}

#endif
