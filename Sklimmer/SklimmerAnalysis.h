#ifndef Sklimmer_SklimmerAnalysis_H
#define Sklimmer_SklimmerAnalysis_H

#include <TH1.h>
#include <map>
#include <string>
#include <TString.h>

#include <EventLoop/Algorithm.h>

// Infrastructure include(s):
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"


#include "RestFrames/RestFrame.hh"
#include "RestFrames/RFrame.hh"
#include "RestFrames/RLabFrame.hh"
#include "RestFrames/RDecayFrame.hh"
#include "RestFrames/RVisibleFrame.hh"
#include "RestFrames/RInvisibleFrame.hh"
#include "RestFrames/RSelfAssemblingFrame.hh"
#include "RestFrames/InvisibleMassJigsaw.hh"
#include "RestFrames/InvisibleRapidityJigsaw.hh"
#include "RestFrames/ContraBoostInvariantJigsaw.hh"
#include "RestFrames/MinimizeMassesCombinatoricJigsaw.hh"
#include "RestFrames/InvisibleGroup.hh"
#include "RestFrames/CombinatoricGroup.hh"
#include "RestFrames/FramePlot.hh"


#ifndef __CINT__
#include "SUSYTools/SUSYObjDef_xAOD.h"
#endif



// GRL
class GoodRunsListSelectionTool;
namespace CP{
	class PileupReweightingTool;
}
using namespace CP;

class JetCleaningTool;

namespace ST{
	class SUSYObjDef_xAOD;
}
using namespace ST;

class EventSelectionBBMet;


// struct sortByPt
// {
//     bool operator()(xAOD::Jet const & a, xAOD::Jet const & b) const
//     {
//         return a.pt() > b.pt();
//     }
// };

class SklimmerAnalysis : public EL::Algorithm
{
	// put your configuration variables here as public variables.
	// that way they can be set directly from CINT and python.

public:
	// float cutValue;
	TH1 *h_nevents; //!
	TH1 *h_nevents_weighted; //!

	// variables that don't get filled at submission time should be
	// protected from being send from the submission node to the worker
	// node (done by the //!)
private:


	int copyFullxAODContainers ();
	int applySUSYObjectDefinitions ();
	int putStuffInStore ();
  EL::StatusCode fillEmptyCollectionNames ();
  EL::StatusCode initializeSUSYTools();
  EL::StatusCode initializeGRLTool();
  EL::StatusCode initializePileupReweightingTool();

  EL::StatusCode initializeEventSelectionBBMet();

  TString doEventSelectionBBMet(xAOD::EventInfo * eventInfo );
public:
	// xAOD::MissingETContainer* m_MET; //!
	// xAOD::MissingETAuxContainer* m_METAux; //!

	#ifndef __CINT__
		GoodRunsListSelectionTool *m_grl; //!
		PileupReweightingTool *m_pileupReweightingTool; //!
		SUSYObjDef_xAOD *m_susy_obj; //!
	#endif // not __CINT__

  EventSelectionBBMet * eventSelectionBBMet;//!

	// this is a standard constructor
	SklimmerAnalysis ();

	// these are the functions inherited from Algorithm
	virtual EL::StatusCode setupJob (EL::Job& job);
	virtual EL::StatusCode fileExecute ();
	virtual EL::StatusCode histInitialize ();
	virtual EL::StatusCode changeInput (bool firstFile);
	virtual EL::StatusCode initialize ();
	virtual EL::StatusCode execute ();
	virtual EL::StatusCode postExecute ();
	virtual EL::StatusCode finalize ();
	virtual EL::StatusCode histFinalize ();


	// void SetSyst(SystErr::Syste );
	// Int_t GetSyst();

	// Systematic Names
	// TString whichsystname; //!


	int isData;
	int isAtlfast;
	int mc12b;
	int useLeptonTrigger;
	int doSyst;

  //collection names. All of these are set automatically if the user doesn't give a value

  std::string outputxAODName;

  std::string muonCollectionName;
  std::string electronCollectionName;
  std::string photonCollectionName;
  std::string jetCollectionName;
  std::string metCollectionName;
  std::string tauCollectionName;

  std::string muonCalibCollectionName;
  std::string electronCalibCollectionName;
  std::string photonCalibCollectionName;
  std::string jetCalibCollectionName;
  std::string metCalibCollectionName;
  std::string tauCalibCollectionName;

  std::string eventInfoName;
  std::string myEventInfoName;
  std::string primaryVerticesName;

  std::string truthEventName;
  std::string truthParticleName;
  std::string truthPrimaryVerticesName;
  // std::string truthMuonCollectionName;
  // std::string truthElectronCollectionName;
  // std::string truthPhotonCollectionName;
  std::string truthJetCollectionName;
  std::string truthMetCollectionName;
  // std::string truthTauCollectionName;

	// These are the switches to steer the analysis code, depending on what step you want to do

	bool m_doSklimming;
	bool m_doSUSYObjDef;
	bool m_doEventSelection;
	// bool m_writeNtuple;
	bool m_writexAOD;

	bool m_writeFullCollectionsToxAOD;

	TString m_Analysis;


	// this is needed to distribute the algorithm to the workers
	ClassDef(SklimmerAnalysis, 1);
};


#endif
