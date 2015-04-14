#ifndef Sklimmer_RazorTriggerAnalysis_H
#define Sklimmer_RazorTriggerAnalysis_H


#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"

//todo probably make this a real tool?
class RazorTriggerAnalysis {

public:
  RazorTriggerAnalysis(xAOD::TStore * store);

  std::string run(xAOD::EventInfo * eventInfo);

  std::string jetCalibCollectionName;
  std::string electronCalibCollectionName;
  std::string muonCalibCollectionName;
  std::string metCalibCollectionName;


private:
  RazorTriggerAnalysis();//need a store to work

  xAOD::TStore *m_store;
};
#endif
