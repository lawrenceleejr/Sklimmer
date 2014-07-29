#ifndef Sklimmer_SklimmerAnalysis_H
#define Sklimmer_SklimmerAnalysis_H

#include <TH1.h>

#include <EventLoop/Algorithm.h>

#include <D3PDReader/Event.h>
#include <EventLoopAlgs/NTupleSvc.h>


#include <RJigsaw/TRJigsaw.h>

#include "SUSYTools/SUSYObjDef.h"


class SklimmerAnalysis : public EL::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:
  // float cutValue;

  TH1 *h_nevents; //!

  // New Branches

  Float_t RJVars_M_0_0_0;  //!
  Float_t RJVars_M_0_0_1;  //!
  Float_t RJVars_M_1_0_0;  //!
  Float_t RJVars_M_1_0_1;  //!
  Float_t RJVars_M_1_1_0;  //!
  Float_t RJVars_M_1_1_1;  //!
  Float_t RJVars_M_2_0_0;  //!
  Float_t RJVars_M_2_0_1;  //!
  Float_t RJVars_M_2_1_0;  //!
  Float_t RJVars_M_2_1_1;  //!
  Float_t RJVars_MDecay_1_1_0;  //!
  Float_t RJVars_MDecay_1_1_1;  //!
  Float_t RJVars_MDecay_2_1_0;  //!
  Float_t RJVars_MDecay_2_1_1;  //!
  Float_t RJVars_dPhi_0_0_0;  //!
  Float_t RJVars_dPhi_0_0_1;  //!
  Float_t RJVars_dPhi_1_0_0;  //!
  Float_t RJVars_dPhi_1_0_1;  //!
  Float_t RJVars_dPhi_2_0_0;  //!
  Float_t RJVars_dPhi_2_0_1;  //!
  Float_t RJVars_dPhi_1_1_0;  //!
  Float_t RJVars_dPhi_1_1_1;  //!
  Float_t RJVars_dPhi_2_1_0;  //!
  Float_t RJVars_dPhi_2_1_1;  //!
  Float_t RJVars_dPhiVis_0_0_0;  //!
  Float_t RJVars_dPhiVis_0_0_1;  //!
  Float_t RJVars_dPhiVis_1_0_0;  //!
  Float_t RJVars_dPhiVis_1_0_1;  //!
  Float_t RJVars_dPhiVis_2_0_0;  //!
  Float_t RJVars_dPhiVis_2_0_1;  //!
  Float_t RJVars_dPhiVis_1_1_0;  //!
  Float_t RJVars_dPhiVis_1_1_1;  //!
  Float_t RJVars_dPhiVis_2_1_0;  //!
  Float_t RJVars_dPhiVis_2_1_1;  //!
  Float_t RJVars_cosTheta_0_0_0;  //!
  Float_t RJVars_cosTheta_0_0_1;  //!
  Float_t RJVars_cosTheta_1_0_0;  //!
  Float_t RJVars_cosTheta_1_0_1;  //!
  Float_t RJVars_cosTheta_2_0_0;  //!
  Float_t RJVars_cosTheta_2_0_1;  //!
  Float_t RJVars_cosTheta_1_1_0;  //!
  Float_t RJVars_cosTheta_1_1_1;  //!
  Float_t RJVars_cosTheta_2_1_0;  //!
  Float_t RJVars_cosTheta_2_1_1;  //!
  Float_t RJVars_dPhiDecay_0_0_0;  //!
  Float_t RJVars_dPhiDecay_0_0_1;  //!
  Float_t RJVars_dPhiDecay_1_0_0;  //!
  Float_t RJVars_dPhiDecay_1_0_1;  //!
  Float_t RJVars_dPhiDecay_2_0_0;  //!
  Float_t RJVars_dPhiDecay_2_0_1;  //!
  Float_t RJVars_cosThetaDecay_0_0_0;  //!
  Float_t RJVars_cosThetaDecay_0_0_1;  //!
  Float_t RJVars_cosThetaDecay_1_0_0;  //!
  Float_t RJVars_cosThetaDecay_1_0_1;  //!
  Float_t RJVars_cosThetaDecay_2_0_0;  //!
  Float_t RJVars_cosThetaDecay_2_0_1;  //!
  Float_t RJVars_gamma_0_0_0;  //!
  Float_t RJVars_gamma_0_0_1;  //!


  Float_t RJVars_M_Tot_Mean;  //!
  Float_t RJVars_M_0_Mean;  //!
  Float_t RJVars_M_1_Mean;  //!
  Float_t RJVars_dPhi_Tot_Mean;  //!
  Float_t RJVars_dPhi_0_Mean;  //!
  Float_t RJVars_dPhi_1_Mean;  //!
  Float_t RJVars_dPhiVis_Tot_Mean;  //!
  Float_t RJVars_dPhiVis_0_Mean;  //!
  Float_t RJVars_dPhiVis_1_Mean;  //!
  Float_t RJVars_cosTheta_Tot_Mean;  //!
  Float_t RJVars_cosTheta_0_Mean;  //!
  Float_t RJVars_cosTheta_1_Mean;  //!
  Float_t RJVars_dPhiDecay_Tot_Mean;  //!
  Float_t RJVars_gamma_Tot_Mean;  //!

  Float_t RJVars_M_Tot_Var;  //!
  Float_t RJVars_M_0_Var;  //!
  Float_t RJVars_M_1_Var;  //!
  Float_t RJVars_dPhi_Tot_Var;  //!
  Float_t RJVars_dPhi_0_Var;  //!
  Float_t RJVars_dPhi_1_Var;  //!
  Float_t RJVars_dPhiVis_Tot_Var;  //!
  Float_t RJVars_dPhiVis_0_Var;  //!
  Float_t RJVars_dPhiVis_1_Var;  //!
  Float_t RJVars_cosTheta_Tot_Var;  //!
  Float_t RJVars_cosTheta_0_Var;  //!
  Float_t RJVars_cosTheta_1_Var;  //!
  Float_t RJVars_dPhiDecay_Tot_Var;  //!
  Float_t RJVars_gamma_Tot_Var;  //!



  Int_t isEE; //!
  Int_t isMuMu; //!
  Int_t isEMu; //!

  vector<int>   *el_selected; //!
  vector<int>   *mu_selected; //!
  vector<int>   *jet_selected; //!

  TBranch        *b_el_selected;   //!
  TBranch        *b_mu_selected;   //!
  TBranch        *b_jet_selected;   //!

  Float_t weight; //!
  Int_t nEvents; //!



  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:
  // Tree *myTree; //!
  // TH1 *myHist; //!

  D3PDReader::Event *event;  //!
  EL::NTupleSvc *output; //!

  SUSYObjDef* m_susy_obj = new SUSYObjDef(); //!

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

  Float_t getMean(Float_t a, Float_t b, Float_t c, Float_t d);
  Float_t getMean(Float_t a, Float_t b);
  Float_t getVar(Float_t a, Float_t b, Float_t c, Float_t d);
  Float_t getVar(Float_t a, Float_t b);

  // this is needed to distribute the algorithm to the workers
  ClassDef(SklimmerAnalysis, 1);
};

#endif
