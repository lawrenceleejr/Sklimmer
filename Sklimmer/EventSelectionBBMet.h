#ifndef Sklimmer_EventSelectionBBMet_H
#define Sklimmer_EventSelectionBBMet_H


#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"

//todo probably make this a real tool?
class EventSelectionBBMet {

public:
  EventSelectionBBMet(xAOD::TStore * store);
  ~EventSelectionBBMet();

  EL::StatusCode initialize();

  std::string run(xAOD::EventInfo * eventInfo);

  std::string jetCalibCollectionName;
  std::string electronCalibCollectionName;
  std::string muonCalibCollectionName;
  std::string metCalibCollectionName;


private:
  EventSelectionBBMet();//need a store to work

  RestFrames::RLabFrame * LAB_alt;
  RestFrames::RSelfAssemblingFrame * S_alt;
  RestFrames::RVisibleFrame * V_alt;
  RestFrames::RInvisibleFrame * I_alt;
  RestFrames::InvisibleGroup * INV_alt;
  RestFrames::CombinatoricGroup * VIS_alt;


  RestFrames::InvisibleMassJigsaw * MinMass_alt;
  RestFrames::InvisibleRapidityJigsaw * Rapidity_alt;


  RestFrames::RLabFrame * LAB;
  RestFrames::RDecayFrame * SS;
  RestFrames::RSelfAssemblingFrame * S1;
  RestFrames::RSelfAssemblingFrame * S2;
  RestFrames::RVisibleFrame * V1;
  RestFrames::RVisibleFrame * V2;
  RestFrames::RInvisibleFrame * I1;
  RestFrames::RInvisibleFrame * I2;
  RestFrames::InvisibleGroup * INV;
  RestFrames::CombinatoricGroup * VIS;

  RestFrames::InvisibleMassJigsaw * MinMassJigsaw;
  RestFrames::InvisibleRapidityJigsaw * RapidityJigsaw;
  RestFrames::ContraBoostInvariantJigsaw * ContraBoostJigsaw;
  RestFrames::MinimizeMassesCombinatoricJigsaw * HemiJigsaw;


  RestFrames::RLabFrame * LAB_R;
  RestFrames::RDecayFrame * GG_R;
  RestFrames::RDecayFrame * Ga_R;
  RestFrames::RDecayFrame * Gb_R;
  RestFrames::RDecayFrame * Ca_R;
  RestFrames::RDecayFrame * Cb_R;
  RestFrames::RVisibleFrame * V1a_R;
  RestFrames::RVisibleFrame * V2a_R;
  RestFrames::RInvisibleFrame * Xa_R;
  RestFrames::RVisibleFrame * V1b_R;
  RestFrames::RVisibleFrame * V2b_R;
  RestFrames::RInvisibleFrame * Xb_R;
  RestFrames::InvisibleGroup * INV_R;
  RestFrames::CombinatoricGroup * VIS_R;
  RestFrames::InvisibleMassJigsaw * MinMassJigsaw_R;
  RestFrames::InvisibleRapidityJigsaw * RapidityJigsaw_R;
  RestFrames::ContraBoostInvariantJigsaw * ContraBoostJigsaw_R;
  RestFrames::MinimizeMassesCombinatoricJigsaw * HemiJigsaw_R;
  RestFrames::MinimizeMassesCombinatoricJigsaw * CaHemiJigsaw_R;
  RestFrames::MinimizeMassesCombinatoricJigsaw * CbHemiJigsaw_R;



  xAOD::TStore *m_store;
};
#endif
