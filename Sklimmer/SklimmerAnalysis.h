#ifndef Sklimmer_SklimmerAnalysis_H
#define Sklimmer_SklimmerAnalysis_H

#include <EventLoop/Algorithm.h>

#include <D3PDReader/Event.h>
#include <EventLoopAlgs/NTupleSvc.h>

//#include "JetSelectorTools/TJetCleaningTool.h"

#include <RJigsaw/TRJigsaw.h>


class SklimmerAnalysis : public EL::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:
  // float cutValue;

  // New Branches
  Float_t RJVars_M_0_0_0; //!
  Float_t RJVars_M_0_0_1; //!

  Float_t RJVars_M_1_0_0; //!
  Float_t RJVars_M_1_0_1; //!
  Float_t RJVars_M_1_1_0; //!
  Float_t RJVars_M_1_1_1; //!

  Float_t RJVars_M_2_0_0; //!
  Float_t RJVars_M_2_0_1; //!
  Float_t RJVars_M_2_1_0; //!
  Float_t RJVars_M_2_1_1; //!

  Float_t RJVars_dPhi_0_0_0; //!
  Float_t RJVars_dPhi_0_0_1; //!
  Float_t RJVars_dPhi_1_0_0; //!
  Float_t RJVars_dPhi_1_0_1; //!
  Float_t RJVars_dPhi_2_0_0; //!
  Float_t RJVars_dPhi_2_0_1; //!

  Float_t RJVars_dPhiVis_0_0_0; //!
  Float_t RJVars_dPhiVis_0_0_1; //!
  Float_t RJVars_dPhiVis_1_0_0; //!
  Float_t RJVars_dPhiVis_1_0_1; //!
  Float_t RJVars_dPhiVis_2_0_0; //!
  Float_t RJVars_dPhiVis_2_0_1; //!

  Float_t RJVars_gamma_0_0_0; //!
  Float_t RJVars_gamma_0_0_1; //!


  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:
  // Tree *myTree; //!
  // TH1 *myHist; //!

  D3PDReader::Event *event;  //!
  EL::NTupleSvc *output; //!

  // Root::TJetCleaningTool *my_JetCleaningTool;

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

  // this is needed to distribute the algorithm to the workers
  ClassDef(SklimmerAnalysis, 1);
};

#endif
