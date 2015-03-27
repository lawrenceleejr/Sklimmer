#ifndef Sklimmer_EventSelectionBBMet_H
#define Sklimmer_EventSelectionBBMet_H


#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"

//todo probably make this a real tool?
class EventSelectionBBMet {

public:
  EventSelectionBBMet(xAOD::TStore * store);

  std::string run(xAOD::EventInfo * eventInfo);

  std::string jetCalibCollectionName;
  std::string electronCalibCollectionName;
  std::string muonCalibCollectionName;
  std::string metCalibCollectionName;


private:
  EventSelectionBBMet();//need a store to work

  xAOD::TStore *m_store;
};
#endif
