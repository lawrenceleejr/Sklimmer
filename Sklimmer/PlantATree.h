#ifndef Sklimmer_PlantATree_H
#define Sklimmer_PlantATree_H

#include <EventLoop/Algorithm.h>

// Infrastructure include(s):
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"

#include <TTree.h>

// Plant a tree and watch it grow

class GoodRunsListSelectionTool;

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

  bool writeHLTObjects; //tends to be pretty big with HLT objects

  GoodRunsListSelectionTool *m_grl; //!

  std::string outputName;
  TTree* tree; //!

  int isMC;
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

  // Gluino Variables
  float RJVars_G_0_CosTheta      ; //!
  float RJVars_C_0_CosTheta      ; //!
  float RJVars_G_0_dPhiGC        ; //!
  float RJVars_G_0_MassRatioGC   ; //!
  float RJVars_G_0_Jet1_pT       ; //!
  float RJVars_G_0_Jet2_pT       ; //!
  float RJVars_G_1_CosTheta      ; //!
  float RJVars_C_1_CosTheta      ; //!
  float RJVars_G_1_dPhiGC        ; //!
  float RJVars_G_1_MassRatioGC   ; //!
  float RJVars_G_1_Jet1_pT       ; //!
  float RJVars_G_1_Jet2_pT       ; //!

  //QCD Variables
  float RJVars_QCD_dPhiR         ; //!
  float RJVars_QCD_Rpt           ; //!
  float RJVars_QCD_Rmsib         ; //!
  float RJVars_QCD_Rpsib         ; //!
  float RJVars_QCD_Delta1         ; //!
  float RJVars_QCD_Delta2         ; //!


  float RJVars_HLT_SS_Mass           ; //!
  float RJVars_HLT_SS_InvGamma       ; //!
  float RJVars_HLT_SS_dPhiBetaR      ; //!
  float RJVars_HLT_SS_dPhiVis        ; //!
  float RJVars_HLT_SS_CosTheta       ; //!
  float RJVars_HLT_SS_dPhiDecayAngle ; //!
  float RJVars_HLT_SS_VisShape       ; //!
  float RJVars_HLT_SS_MDeltaR        ; //!
  float RJVars_HLT_S1_Mass           ; //!
  float RJVars_HLT_S1_CosTheta       ; //!
  float RJVars_HLT_S2_Mass           ; //!
  float RJVars_HLT_S2_CosTheta       ; //!
  float RJVars_HLT_I1_Depth          ; //!
  float RJVars_HLT_I2_Depth          ; //!
  float RJVars_HLT_V1_N              ; //!
  float RJVars_HLT_V2_N              ; //!

  // Gluino Variables
  float RJVars_HLT_G_0_CosTheta      ; //!
  float RJVars_HLT_C_0_CosTheta      ; //!
  float RJVars_HLT_G_0_dPhiGC        ; //!
  float RJVars_HLT_G_0_MassRatioGC   ; //!
  float RJVars_HLT_G_0_Jet1_pT       ; //!
  float RJVars_HLT_G_0_Jet2_pT       ; //!
  float RJVars_HLT_G_1_CosTheta      ; //!
  float RJVars_HLT_C_1_CosTheta      ; //!
  float RJVars_HLT_G_1_dPhiGC        ; //!
  float RJVars_HLT_G_1_MassRatioGC   ; //!
  float RJVars_HLT_G_1_Jet1_pT       ; //!
  float RJVars_HLT_G_1_Jet2_pT       ; //!

  //QCD Variables
  float RJVars_HLT_QCD_dPhiR         ; //!
  float RJVars_HLT_QCD_Rpt           ; //!
  float RJVars_HLT_QCD_Rmsib         ; //!
  float RJVars_HLT_QCD_Rpsib         ; //!
  float RJVars_HLT_QCD_Delta1         ; //!
  float RJVars_HLT_QCD_Delta2         ; //!

  int RJVars_TriggerBits          ; //!


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

  std::vector<float> * HLT_Jet_pT;    //!
  std::vector<float> * HLT_Jet_eta;   //!
  std::vector<float> * HLT_Jet_phi;   //!
  std::vector<float> * HLT_Jet_E;     //!
  std::vector<float> * HLT_Jet_m;     //!
  std::vector<float> * HLT_Jet_MV1;     //!

  std::vector<float> * HLT_Muon_pT;    //!
  std::vector<float> * HLT_Muon_eta;   //!
  std::vector<float> * HLT_Muon_phi;   //!
  std::vector<float> * HLT_Muon_E;     //!
  std::vector<float> * HLT_Muon_m;     //!

  std::vector<float> * HLT_Electron_pT;    //!
  std::vector<float> * HLT_Electron_eta;   //!
  std::vector<float> * HLT_Electron_phi;   //!
  std::vector<float> * HLT_Electron_E;     //!
  std::vector<float> * HLT_Electron_m;     //!

  float HLT_MET_x; //!
  float HLT_MET_y; //!


  xAOD::TEvent *m_event;  //!
  xAOD::TStore *m_store;  //!


  // this is a standard constructor
  PlantATree ();

  EL::StatusCode  fillRecoVariables() ;
  EL::StatusCode  fillHLTVariables() ;

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
