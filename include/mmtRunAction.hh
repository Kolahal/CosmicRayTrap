#ifndef mmtRunAction_h
#define mmtRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "TFile.h"
#include "TTree.h"

class G4Run;

/// Run action class
///
/// It accumulates statistic and computes dispersion of the energy deposit 
/// and track lengths of charged particles with use of analysis tools:
/// H1D histograms are created in BeginOfRunAction() for the following 
/// physics quantities:
/// - Edep in absorber
/// - Edep in gap
/// - Track length in absorber
/// - Track length in gap
/// The same values are also saved in the ntuple.
/// The histograms and ntuple are saved in the output file in a format
/// accoring to a selected technology in mmtAnalysis.hh.
///
/// In EndOfRunAction(), the accumulated statistic and computed 
/// dispersion is printed.

class mmtRunAction : public G4UserRunAction
{
	public:
	mmtRunAction();
       ~mmtRunAction();
	
	virtual void BeginOfRunAction(const G4Run*);
	virtual void EndOfRunAction(const G4Run*);
	
	static mmtRunAction* AnPointer;
	
	static const unsigned int nmax=100;
	unsigned ievt;
	G4int mcpid_in[nmax];
	G4double ke_in[nmax];
	G4double xx_in[nmax];
	G4double yy_in[nmax];
	G4double zz_in[nmax];
	
	G4double tx_in[nmax];
	G4double ty_in[nmax];
	G4double tz_in[nmax];
	
	G4int finalpid[nmax];
	G4double keEnd[nmax];
	G4double xxEnd[nmax];
	G4double yyEnd[nmax];
	G4double zzEnd[nmax];
	
	G4double txEnd[nmax];
	G4double tyEnd[nmax];
	G4double tzEnd[nmax];
	public:
	TFile* outputFile;
	TTree* outputTree;
};

#endif
