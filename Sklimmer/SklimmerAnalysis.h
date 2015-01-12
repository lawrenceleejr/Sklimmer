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

#include <RJigsaw/TRJigsaw.h>

#include "SUSYTools/SUSYObjDef_xAOD.h"


class SklimmerAnalysis : public EL::Algorithm
{
	// put your configuration variables here as public variables.
	// that way they can be set directly from CINT and python.
public:
	// float cutValue;

	TH1 *h_nevents; //!

	Float_t weight; //!
	Int_t nEvents; //!



	// variables that don't get filled at submission time should be
	// protected from being send from the submission node to the worker
	// node (done by the //!)
public:

	xAOD::TEvent *m_event;  //!

	SUSYObjDef_xAOD* m_susy_obj = new SUSYObjDef_xAOD(); //!

	Root::TRJigsaw* RJTool = new Root::TRJigsaw(); //!

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

	void SetSyst(SystErr::Syste );
	Int_t GetSyst();

	bool isData; //!
	bool isAtlfast; //!
	bool isMC12b; //!
	bool useLeptonTrigger; //!

	// Systematic Names
	// TString whichsystname; //! 




	// this is needed to distribute the algorithm to the workers
	ClassDef(SklimmerAnalysis, 1);
};


#endif
