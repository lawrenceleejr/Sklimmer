#ifndef Sklimmer_PlantATree_H
#define Sklimmer_PlantATree_H

#include <EventLoop/Algorithm.h>

// Infrastructure include(s):
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"

#include <TTree.h>

// Plant a tree and watch it grow

class PlantATree : public EL::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:
  // float cutValue;



  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:
  // Tree *myTree; //!
  // TH1 *myHist; //!



  std::string outputName;
  TTree* tree; //!


  // Setup variables for branches
  int RunNumber; //!
  int EventNumber; //!
  int LumiBlock; //!
  int BCID; //!
  int MCChannelNumber; //!
  int MCEventNumber; //!
  float MCEventWeight; //!
  float ActualInteractionsPerCrossing; //!
  float AverageInteractionsPerCrossing; //!


  float RJVars_SS_Mass           ; //!
  float RJVars_SS_InvGamma       ; //!
  float RJVars_SS_dPhiBetaR      ; //!
  float RJVars_SS_dPhiVis        ; //!
  float RJVars_SS_CosTheta       ; //!
  float RJVars_SS_dPhiDecayAngle ; //!
  float RJVars_SS_VisShape       ; //!
  float RJVars_SS_MDeltaR        ; //!
  float RJVars_S1_Mass           ; //!
  float RJVars_S1_CosTheta       ; //!
  float RJVars_S2_Mass           ; //!
  float RJVars_S2_CosTheta       ; //!
  float RJVars_I1_Depth          ; //!
  float RJVars_I2_Depth          ; //!
  float RJVars_V1_N              ; //!
  float RJVars_V2_N              ; //!    

  //QCD Variables
  float RJVars_QCD_dPhiR         ; //! 
  float RJVars_QCD_Rpt           ; //! 
  float RJVars_QCD_Rmsib         ; //! 
  float RJVars_QCD_Rpsib         ; //! 
  float RJVars_QCD_Delta1         ; //! 
  float RJVars_QCD_Delta2         ; //! 

  std::vector<float> * Jet_pT;    //!
  std::vector<float> * Jet_eta;   //!
  std::vector<float> * Jet_phi;   //!
  std::vector<float> * Jet_E;     //!
  std::vector<float> * Jet_m;     //!

  std::vector<float> * Jet_MV1;     //!

  std::vector<float> * Muon_pT;    //!
  std::vector<float> * Muon_eta;   //!
  std::vector<float> * Muon_phi;   //!
  std::vector<float> * Muon_E;     //!
  std::vector<float> * Muon_m;     //!

  std::vector<float> * Electron_pT;    //!
  std::vector<float> * Electron_eta;   //!
  std::vector<float> * Electron_phi;   //!
  std::vector<float> * Electron_E;     //!
  std::vector<float> * Electron_m;     //!

  float MET_x; //!
  float MET_y; //!

  xAOD::TEvent *m_event;  //!
  xAOD::TStore *m_store;  //!


  // this is a standard constructor
  PlantATree ();

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
  ClassDef(PlantATree, 1);
};

#endif
