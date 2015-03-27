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


namespace Root{
  class TRJigsaw;
}
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

	Float_t weight; //!
	Int_t nEvents; //!



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

  TString eventSelectionBBMet();
public:

	RestFrames::RLabFrame * LAB_alt; //!
	RestFrames::RSelfAssemblingFrame * S_alt; //!
	RestFrames::RVisibleFrame * V_alt; //!
	RestFrames::RInvisibleFrame * I_alt; //!
	RestFrames::InvisibleGroup * INV_alt; //!
	RestFrames::CombinatoricGroup * VIS_alt; //!


	RestFrames::InvisibleMassJigsaw * MinMass_alt; //!
	RestFrames::InvisibleRapidityJigsaw * Rapidity_alt; //!


	RestFrames::RLabFrame * LAB; //!
	RestFrames::RDecayFrame * SS; //!
	RestFrames::RSelfAssemblingFrame * S1; //!
	RestFrames::RSelfAssemblingFrame * S2; //!
	RestFrames::RVisibleFrame * V1; //!
	RestFrames::RVisibleFrame * V2; //!
	RestFrames::RInvisibleFrame * I1; //!
	RestFrames::RInvisibleFrame * I2; //!
	RestFrames::InvisibleGroup * INV; //!
	RestFrames::CombinatoricGroup * VIS; //!

	RestFrames::InvisibleMassJigsaw * MinMassJigsaw; //!
	RestFrames::InvisibleRapidityJigsaw * RapidityJigsaw; //!
	RestFrames::ContraBoostInvariantJigsaw * ContraBoostJigsaw; //!
	RestFrames::MinimizeMassesCombinatoricJigsaw * HemiJigsaw; //!


	RestFrames::RLabFrame * LAB_R; //!
	RestFrames::RDecayFrame * GG_R; //!
	RestFrames::RDecayFrame * Ga_R; //!
	RestFrames::RDecayFrame * Gb_R; //!
	RestFrames::RDecayFrame * Ca_R; //!
	RestFrames::RDecayFrame * Cb_R; //!
	RestFrames::RVisibleFrame * V1a_R; //!
	RestFrames::RVisibleFrame * V2a_R; //!
	RestFrames::RInvisibleFrame * Xa_R; //!
	RestFrames::RVisibleFrame * V1b_R; //!
	RestFrames::RVisibleFrame * V2b_R; //!
	RestFrames::RInvisibleFrame * Xb_R; //!
	RestFrames::InvisibleGroup * INV_R; //!
	RestFrames::CombinatoricGroup * VIS_R; //!
	RestFrames::InvisibleMassJigsaw * MinMassJigsaw_R; //!
	RestFrames::InvisibleRapidityJigsaw * RapidityJigsaw_R; //!
	RestFrames::ContraBoostInvariantJigsaw * ContraBoostJigsaw_R; //!
	RestFrames::MinimizeMassesCombinatoricJigsaw * HemiJigsaw_R; //!
	RestFrames::MinimizeMassesCombinatoricJigsaw * CaHemiJigsaw_R; //!
	RestFrames::MinimizeMassesCombinatoricJigsaw * CbHemiJigsaw_R; //!




	xAOD::TEvent *m_event;  //!
  //	xAOD::TEvent *m_event;  //!

	xAOD::TStore *m_store;  //!

	// xAOD::MissingETContainer* m_MET; //!
	// xAOD::MissingETAuxContainer* m_METAux; //!

	#ifndef __CINT__
		GoodRunsListSelectionTool *m_grl; //!
		PileupReweightingTool *m_pileupReweightingTool; //!
		SUSYObjDef_xAOD *m_susy_obj; //!
	#endif // not __CINT__


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
