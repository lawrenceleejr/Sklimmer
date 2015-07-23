#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <Sklimmer/PlantATree.h>

#include "CPAnalysisExamples/errorcheck.h"


#include "EventLoop/OutputStream.h"

// EDM includes:
#include "xAODEventInfo/EventInfo.h"
#include "xAODJet/JetContainer.h"
#include "xAODJet/JetAuxContainer.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODEgamma/PhotonContainer.h"
#include "xAODTau/TauJetContainer.h"
#include "xAODCaloEvent/CaloCluster.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODTruth/TruthEventContainer.h"
#include "xAODTruth/TruthEvent.h"
#include "xAODCore/ShallowCopy.h"
#include "xAODMissingET/MissingETContainer.h"
#include "xAODMissingET/MissingETAuxContainer.h"
#include "xAODBTaggingEfficiency/BTaggingEfficiencyTool.h"
//#include "xAODBTagging/BTagging.h"
#include "GoodRunsLists/GoodRunsListSelectionTool.h"

#include <TSystem.h>
#include <TFile.h>

// this is needed to distribute the algorithm to the workers
ClassImp(PlantATree)



PlantATree :: PlantATree ()
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().
}



EL::StatusCode PlantATree :: setupJob (EL::Job& job)
{
  // Here you put code that sets up the job on the submission object
  // so that it is ready to work with your algorithm, e.g. you can
  // request the D3PDReader service or add output files.  Any code you
  // put here could instead also go into the submission script.  The
  // sole advantage of putting it here is that it gets automatically
  // activated/deactivated when you add/remove the algorithm from your
  // job, which may or may not be of value to you.

  job.useXAOD ();
  xAOD::Init( "PlantATree" ).ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode PlantATree :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.

// get the output file, create a new TTree and connect it to that output
  // define what braches will go in that tree
  TFile *outputFile = wk()->getOutputFile (outputName);
  tree = new TTree ("tree", "tree");
  tree->SetDirectory (outputFile);


  Jet_pT = new std::vector<float>       ;
  Jet_eta = new std::vector<float>       ;
  Jet_phi = new std::vector<float>       ;
  Jet_E = new std::vector<float>       ;
  Jet_m = new std::vector<float>       ;
  Jet_MV1 = new std::vector<float>       ;
  Muon_pT = new std::vector<float>       ;
  Muon_eta = new std::vector<float>       ;
  Muon_phi = new std::vector<float>       ;
  Muon_E = new std::vector<float>       ;
  Muon_m = new std::vector<float>       ;
  Electron_pT = new std::vector<float>       ;
  Electron_eta = new std::vector<float>       ;
  Electron_phi = new std::vector<float>       ;
  Electron_E = new std::vector<float>       ;
  Electron_m = new std::vector<float>       ;


  HLT_Jet_pT = new std::vector<float>       ;
  HLT_Jet_eta = new std::vector<float>       ;
  HLT_Jet_phi = new std::vector<float>       ;
  HLT_Jet_E = new std::vector<float>       ;
  HLT_Jet_m = new std::vector<float>       ;
  HLT_Jet_MV1 = new std::vector<float>       ;
  //  HLT_Jet_goodJet = new std::vector<float>       ;
  HLT_Muon_pT = new std::vector<float>       ;
  HLT_Muon_eta = new std::vector<float>       ;
  HLT_Muon_phi = new std::vector<float>       ;
  HLT_Muon_E = new std::vector<float>       ;
  HLT_Muon_m = new std::vector<float>       ;
  HLT_Electron_pT = new std::vector<float>       ;
  HLT_Electron_eta = new std::vector<float>       ;
  HLT_Electron_phi = new std::vector<float>       ;
  HLT_Electron_E = new std::vector<float>       ;
  HLT_Electron_m = new std::vector<float>       ;


  //Set up branches here


  tree->Branch("RunNumber"                     , &RunNumber                      );
  tree->Branch("EventNumber"                   , &EventNumber                    );
  tree->Branch("LumiBlock"                     , &LumiBlock                      );
  tree->Branch("BCID"                          , &BCID                           );
  // tree->Branch("MCChannelNumber"               , &MCChannelNumber                );
  // tree->Branch("MCEventNumber"                 , &MCEventNumber                  );
  // tree->Branch("MCEventWeight"                 , &MCEventWeight                  );
  tree->Branch("ActualInteractionsPerCrossing" , &ActualInteractionsPerCrossing  );
  tree->Branch("AverageInteractionsPerCrossing", &AverageInteractionsPerCrossing );


  tree->Branch("RJVars_SS_Mass"           , &RJVars_SS_Mass           );
  tree->Branch("RJVars_SS_InvGamma"       , &RJVars_SS_InvGamma       );
  tree->Branch("RJVars_SS_dPhiBetaR"      , &RJVars_SS_dPhiBetaR      );
  tree->Branch("RJVars_SS_dPhiVis"        , &RJVars_SS_dPhiVis        );
  tree->Branch("RJVars_SS_CosTheta"       , &RJVars_SS_CosTheta       );
  tree->Branch("RJVars_SS_dPhiDecayAngle" , &RJVars_SS_dPhiDecayAngle );
  tree->Branch("RJVars_SS_VisShape"       , &RJVars_SS_VisShape       );
  tree->Branch("RJVars_SS_MDeltaR"        , &RJVars_SS_MDeltaR        );
  tree->Branch("RJVars_S1_Mass"           , &RJVars_S1_Mass           );
  tree->Branch("RJVars_S1_CosTheta"       , &RJVars_S1_CosTheta       );
  tree->Branch("RJVars_S2_Mass"           , &RJVars_S2_Mass           );
  tree->Branch("RJVars_S2_CosTheta"       , &RJVars_S2_CosTheta       );
  tree->Branch("RJVars_I1_Depth"          , &RJVars_I1_Depth          );
  tree->Branch("RJVars_I2_Depth"          , &RJVars_I2_Depth          );
  tree->Branch("RJVars_V1_N"              , &RJVars_V1_N              );
  tree->Branch("RJVars_V2_N"              , &RJVars_V2_N              );
  tree->Branch("RJVars_G_0_CosTheta"    , &RJVars_G_0_CosTheta    );
  tree->Branch("RJVars_C_0_CosTheta"    , &RJVars_C_0_CosTheta    );
  tree->Branch("RJVars_G_0_dPhiGC"      , &RJVars_G_0_dPhiGC      );
  tree->Branch("RJVars_G_0_MassRatioGC" , &RJVars_G_0_MassRatioGC );
  tree->Branch("RJVars_G_0_Jet1_pT"     , &RJVars_G_0_Jet1_pT     );
  tree->Branch("RJVars_G_0_Jet2_pT"     , &RJVars_G_0_Jet2_pT     );
  tree->Branch("RJVars_G_1_CosTheta"    , &RJVars_G_1_CosTheta    );
  tree->Branch("RJVars_C_1_CosTheta"    , &RJVars_C_1_CosTheta    );
  tree->Branch("RJVars_G_1_dPhiGC"      , &RJVars_G_1_dPhiGC      );
  tree->Branch("RJVars_G_1_MassRatioGC" , &RJVars_G_1_MassRatioGC );
  tree->Branch("RJVars_G_1_Jet1_pT"     , &RJVars_G_1_Jet1_pT     );
  tree->Branch("RJVars_G_1_Jet2_pT"     , &RJVars_G_1_Jet2_pT     );
  tree->Branch("RJVars_QCD_dPhiR" ,  &RJVars_QCD_dPhiR  );
  tree->Branch("RJVars_QCD_Rpt"   ,  &RJVars_QCD_Rpt    );
  tree->Branch("RJVars_QCD_Rmsib" ,  &RJVars_QCD_Rmsib  );
  tree->Branch("RJVars_QCD_Rpsib" ,  &RJVars_QCD_Rpsib  );
  tree->Branch("RJVars_QCD_Delta1" ,  &RJVars_QCD_Delta1  );
  tree->Branch("RJVars_QCD_Delta2" ,  &RJVars_QCD_Delta2  );

  tree->Branch("RJVars_HLT_SS_Mass"           , &RJVars_HLT_SS_Mass           );
  tree->Branch("RJVars_HLT_SS_InvGamma"       , &RJVars_HLT_SS_InvGamma       );
  tree->Branch("RJVars_HLT_SS_dPhiBetaR"      , &RJVars_HLT_SS_dPhiBetaR      );
  tree->Branch("RJVars_HLT_SS_dPhiVis"        , &RJVars_HLT_SS_dPhiVis        );
  tree->Branch("RJVars_HLT_SS_CosTheta"       , &RJVars_HLT_SS_CosTheta       );
  tree->Branch("RJVars_HLT_SS_dPhiDecayAngle" , &RJVars_HLT_SS_dPhiDecayAngle );
  tree->Branch("RJVars_HLT_SS_VisShape"       , &RJVars_HLT_SS_VisShape       );
  tree->Branch("RJVars_HLT_SS_MDeltaR"        , &RJVars_HLT_SS_MDeltaR        );
  tree->Branch("RJVars_HLT_S1_Mass"           , &RJVars_HLT_S1_Mass           );
  tree->Branch("RJVars_HLT_S1_CosTheta"       , &RJVars_HLT_S1_CosTheta       );
  tree->Branch("RJVars_HLT_S2_Mass"           , &RJVars_HLT_S2_Mass           );
  tree->Branch("RJVars_HLT_S2_CosTheta"       , &RJVars_HLT_S2_CosTheta       );
  tree->Branch("RJVars_HLT_I1_Depth"          , &RJVars_HLT_I1_Depth          );
  tree->Branch("RJVars_HLT_I2_Depth"          , &RJVars_HLT_I2_Depth          );
  tree->Branch("RJVars_HLT_V1_N"              , &RJVars_HLT_V1_N              );
  tree->Branch("RJVars_HLT_V2_N"              , &RJVars_HLT_V2_N              );
  tree->Branch("RJVars_HLT_G_0_CosTheta"    , &RJVars_HLT_G_0_CosTheta    );
  tree->Branch("RJVars_HLT_C_0_CosTheta"    , &RJVars_HLT_C_0_CosTheta    );
  tree->Branch("RJVars_HLT_G_0_dPhiGC"      , &RJVars_HLT_G_0_dPhiGC      );
  tree->Branch("RJVars_HLT_G_0_MassRatioGC" , &RJVars_HLT_G_0_MassRatioGC );
  tree->Branch("RJVars_HLT_G_0_Jet1_pT"     , &RJVars_HLT_G_0_Jet1_pT     );
  tree->Branch("RJVars_HLT_G_0_Jet2_pT"     , &RJVars_HLT_G_0_Jet2_pT     );
  tree->Branch("RJVars_HLT_G_1_CosTheta"    , &RJVars_HLT_G_1_CosTheta    );
  tree->Branch("RJVars_HLT_C_1_CosTheta"    , &RJVars_HLT_C_1_CosTheta    );
  tree->Branch("RJVars_HLT_G_1_dPhiGC"      , &RJVars_HLT_G_1_dPhiGC      );
  tree->Branch("RJVars_HLT_G_1_MassRatioGC" , &RJVars_HLT_G_1_MassRatioGC );
  tree->Branch("RJVars_HLT_G_1_Jet1_pT"     , &RJVars_HLT_G_1_Jet1_pT     );
  tree->Branch("RJVars_HLT_G_1_Jet2_pT"     , &RJVars_HLT_G_1_Jet2_pT     );
  tree->Branch("RJVars_HLT_QCD_dPhiR" ,  &RJVars_HLT_QCD_dPhiR  );
  tree->Branch("RJVars_HLT_QCD_Rpt"   ,  &RJVars_HLT_QCD_Rpt    );
  tree->Branch("RJVars_HLT_QCD_Rmsib" ,  &RJVars_HLT_QCD_Rmsib  );
  tree->Branch("RJVars_HLT_QCD_Rpsib" ,  &RJVars_HLT_QCD_Rpsib  );
  tree->Branch("RJVars_HLT_QCD_Delta1" ,  &RJVars_HLT_QCD_Delta1  );
  tree->Branch("RJVars_HLT_QCD_Delta2" ,  &RJVars_HLT_QCD_Delta2  );


  //trigger
  tree->Branch("RJVars_HLT_TriggerBits" ,  &RJVars_TriggerBits  );


  tree->Branch("Jet_pT"       , &Jet_pT        );
  tree->Branch("Jet_eta"      , &Jet_eta       );
  tree->Branch("Jet_phi"      , &Jet_phi       );
  tree->Branch("Jet_E"        , &Jet_E         );
  tree->Branch("Jet_m"        , &Jet_m         );
  tree->Branch("Jet_MV1"      , &Jet_MV1       );
  tree->Branch("Muon_pT"      , &Muon_pT       );
  tree->Branch("Muon_eta"     , &Muon_eta      );
  tree->Branch("Muon_phi"     , &Muon_phi      );
  tree->Branch("Muon_E"       , &Muon_E        );
  tree->Branch("Muon_m"       , &Muon_m        );
  tree->Branch("Electron_pT"  , &Electron_pT   );
  tree->Branch("Electron_eta" , &Electron_eta  );
  tree->Branch("Electron_phi" , &Electron_phi  );
  tree->Branch("Electron_E"   , &Electron_E    );
  tree->Branch("Electron_m"   , &Electron_m    );
  tree->Branch("MET_x"   , &MET_x    );
  tree->Branch("MET_y"   , &MET_y    );

  tree->Branch("HLT_Jet_pT"       , &HLT_Jet_pT        );
  tree->Branch("HLT_Jet_eta"      , &HLT_Jet_eta       );
  tree->Branch("HLT_Jet_phi"      , &HLT_Jet_phi       );
  tree->Branch("HLT_Jet_E"        , &HLT_Jet_E         );
  tree->Branch("HLT_Jet_m"        , &HLT_Jet_m         );
  tree->Branch("HLT_Jet_MV1"      , &HLT_Jet_MV1       );
  tree->Branch("HLT_Muon_pT"      , &HLT_Muon_pT       );
  tree->Branch("HLT_Muon_eta"     , &HLT_Muon_eta      );
  tree->Branch("HLT_Muon_phi"     , &HLT_Muon_phi      );
  tree->Branch("HLT_Muon_E"       , &HLT_Muon_E        );
  tree->Branch("HLT_Muon_m"       , &HLT_Muon_m        );
  tree->Branch("HLT_Electron_pT"  , &HLT_Electron_pT   );
  tree->Branch("HLT_Electron_eta" , &HLT_Electron_eta  );
  tree->Branch("HLT_Electron_phi" , &HLT_Electron_phi  );
  tree->Branch("HLT_Electron_E"   , &HLT_Electron_E    );
  tree->Branch("HLT_Electron_m"   , &HLT_Electron_m    );
  tree->Branch("HLT_MET_x"   , &HLT_MET_x    );
  tree->Branch("HLT_MET_y"   , &HLT_MET_y    );



  return EL::StatusCode::SUCCESS;
}



EL::StatusCode PlantATree :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode PlantATree :: changeInput (bool firstFile)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode PlantATree :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

  const char * APP_NAME = "SklimmerAnalysis";

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();


  m_grl = new GoodRunsListSelectionTool("GoodRunsListSelectionTool");
  std::vector<std::string> vecStringGRL;
  vecStringGRL.push_back(gSystem->ExpandPathName("$ROOTCOREBIN/data/Sklimmer/data15_13TeV.periodAllYear_DetStatus-v63-pro18-01_DQDefects-00-01-\
02_PHYS_StandardGRL_All_Good.xml") );
  CHECK( m_grl->setProperty( "GoodRunsListVec", vecStringGRL) );
  CHECK( m_grl->setProperty("PassThrough", false) ); // if true (default) will ignore result of GRL and will just pass all events
  if (!m_grl->initialize().isSuccess()) { // check this isSuccess
    return EL::StatusCode::FAILURE;
  }



  // as a check, let's see the number of events in our xAOD
  Info("initialize()", "Number of events = %lli", m_event->getEntries() ); // print long long int


  return EL::StatusCode::SUCCESS;
}



EL::StatusCode PlantATree :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  //////////////////////////////////////////////////////////////////////////////////////////

  Jet_pT        ->clear();
  Jet_eta       ->clear();
  Jet_phi       ->clear();
  Jet_E         ->clear();
  Jet_m         ->clear();
  Jet_MV1       ->clear();
  Muon_pT       ->clear();
  Muon_eta      ->clear();
  Muon_phi      ->clear();
  Muon_E        ->clear();
  Muon_m        ->clear();
  Electron_pT   ->clear();
  Electron_eta  ->clear();
  Electron_phi  ->clear();
  Electron_E    ->clear();
  Electron_m    ->clear();

  const xAOD::EventInfo* quickInfo = 0;
  if( ! m_event->retrieve( quickInfo, "EventInfo").isSuccess() ){
    return EL::StatusCode::FAILURE;
  }

  if(!m_grl->passRunLB(*quickInfo)){
    return EL::StatusCode::SUCCESS; // go to next event
  }
  if(  (quickInfo->errorState(xAOD::EventInfo::LAr)==xAOD::EventInfo::Error ) ||
       (quickInfo->errorState(xAOD::EventInfo::Tile)==xAOD::EventInfo::Error ) ||
       (quickInfo->isEventFlagBitSet(xAOD::EventInfo::Core, 18) )  )
    {
      return EL::StatusCode::SUCCESS; // go to the next event
    } // end if event flags check


  //  std::cout << "PlantATree :: execute ()" << std::endl;
  // std::cout << "PlantATree - Contains event info? " <<std::endl; m_store->print();
  // m_store->print();

  xAOD::EventInfo* eventinfo = 0;
  m_store->retrieve(eventinfo, "myEventInfo") ;

  if( fillHLTVariables() != EL::StatusCode::SUCCESS){
    return EL::StatusCode::FAILURE;
  }
  if( fillRecoVariables() != EL::StatusCode::SUCCESS){
    return EL::StatusCode::FAILURE;
  }



  // if( eventinfo->auxdata<char>("selection") == (char) 0 ){
    //     m_store->clear();
    //     return EL::StatusCode::SUCCESS;
    // }

  RunNumber                       = eventinfo->runNumber();
  EventNumber                     = eventinfo->eventNumber();
  LumiBlock                       = eventinfo->lumiBlock();
  BCID                            = eventinfo->bcid();
  // MCChannelNumber                 = eventinfo->mcChannelNumber();
  // MCEventNumber                   = eventinfo->mcEventNumber();
  // MCEventWeight                   = eventinfo->mcEvent
  //    Weight();
  ActualInteractionsPerCrossing   = eventinfo->actualInteractionsPerCrossing();
  AverageInteractionsPerCrossing  = eventinfo->averageInteractionsPerCrossing();

  RJVars_SS_Mass            = eventinfo->auxdata<float>("SS_Mass");
  RJVars_SS_InvGamma        = eventinfo->auxdata<float>("SS_InvGamma");
  RJVars_SS_dPhiBetaR       = eventinfo->auxdata<float>("SS_dPhiBetaR");
  RJVars_SS_dPhiVis         = eventinfo->auxdata<float>("SS_dPhiVis");
  RJVars_SS_CosTheta        = eventinfo->auxdata<float>("SS_CosTheta");
  RJVars_SS_dPhiDecayAngle  = eventinfo->auxdata<float>("SS_dPhiDecayAngle");
  RJVars_SS_VisShape        = eventinfo->auxdata<float>("SS_VisShape");
  RJVars_SS_MDeltaR         = eventinfo->auxdata<float>("SS_MDeltaR");
  RJVars_S1_Mass            = eventinfo->auxdata<float>("S1_Mass");
  RJVars_S1_CosTheta        = eventinfo->auxdata<float>("S1_CosTheta");
  RJVars_S2_Mass            = eventinfo->auxdata<float>("S2_Mass");
  RJVars_S2_CosTheta        = eventinfo->auxdata<float>("S2_CosTheta");
  RJVars_I1_Depth           = eventinfo->auxdata<float>("I1_Depth");
  RJVars_I2_Depth           = eventinfo->auxdata<float>("I2_Depth");
  RJVars_V1_N               = eventinfo->auxdata<float>("V1_N");
  RJVars_V2_N               = eventinfo->auxdata<float>("V2_N");
  RJVars_G_0_CosTheta       = eventinfo->auxdata<float>("G_0_CosTheta");
  RJVars_C_0_CosTheta       = eventinfo->auxdata<float>("C_0_CosTheta");
  RJVars_G_0_dPhiGC         = eventinfo->auxdata<float>("G_0_dPhiGC");
  RJVars_G_0_MassRatioGC    = eventinfo->auxdata<float>("G_0_MassRatioGC");
  RJVars_G_1_CosTheta       = eventinfo->auxdata<float>("G_1_CosTheta");
  RJVars_C_1_CosTheta       = eventinfo->auxdata<float>("C_1_CosTheta");
  RJVars_G_1_dPhiGC         = eventinfo->auxdata<float>("G_1_dPhiGC");
  RJVars_G_1_MassRatioGC    = eventinfo->auxdata<float>("G_1_MassRatioGC");
  RJVars_G_0_Jet1_pT        = eventinfo->auxdata<float>("G_0_Jet1_pT");
  RJVars_G_0_Jet2_pT        = eventinfo->auxdata<float>("G_0_Jet2_pT");
  RJVars_G_1_Jet1_pT        = eventinfo->auxdata<float>("G_1_Jet1_pT");
  RJVars_G_1_Jet2_pT        = eventinfo->auxdata<float>("G_1_Jet2_pT");
  RJVars_QCD_dPhiR          = eventinfo->auxdata<float>("QCD_dPhiR");
  RJVars_QCD_Rpt            = eventinfo->auxdata<float>("QCD_Rpt"  );
  RJVars_QCD_Rmsib          = eventinfo->auxdata<float>("QCD_Rmsib");
  RJVars_QCD_Rpsib          = eventinfo->auxdata<float>("QCD_Rpsib");
  RJVars_QCD_Delta1          = eventinfo->auxdata<float>("QCD_Delta1");
  RJVars_QCD_Delta2          = eventinfo->auxdata<float>("QCD_Delta2");

  RJVars_HLT_SS_Mass            = eventinfo->auxdata<float>("SS_HLT_Mass");
  RJVars_HLT_SS_InvGamma        = eventinfo->auxdata<float>("SS_HLT_InvGamma");
  RJVars_HLT_SS_dPhiBetaR       = eventinfo->auxdata<float>("SS_HLT_dPhiBetaR");
  RJVars_HLT_SS_dPhiVis         = eventinfo->auxdata<float>("SS_HLT_dPhiVis");
  RJVars_HLT_SS_CosTheta        = eventinfo->auxdata<float>("SS_HLT_CosTheta");
  RJVars_HLT_SS_dPhiDecayAngle  = eventinfo->auxdata<float>("SS_HLT_dPhiDecayAngle");
  RJVars_HLT_SS_VisShape        = eventinfo->auxdata<float>("SS_HLT_VisShape");
  RJVars_HLT_SS_MDeltaR         = eventinfo->auxdata<float>("SS_HLT_MDeltaR");
  RJVars_HLT_S1_Mass            = eventinfo->auxdata<float>("S1_HLT_Mass");
  RJVars_HLT_S1_CosTheta        = eventinfo->auxdata<float>("S1_HLT_CosTheta");
  RJVars_HLT_S2_Mass            = eventinfo->auxdata<float>("S2_HLT_Mass");
  RJVars_HLT_S2_CosTheta        = eventinfo->auxdata<float>("S2_HLT_CosTheta");
  RJVars_HLT_I1_Depth           = eventinfo->auxdata<float>("I1_HLT_Depth");
  RJVars_HLT_I2_Depth           = eventinfo->auxdata<float>("I2_HLT_Depth");
  RJVars_HLT_V1_N               = eventinfo->auxdata<float>("V1_HLT_N");
  RJVars_HLT_V2_N               = eventinfo->auxdata<float>("V2_HLT_N");
  RJVars_HLT_G_0_CosTheta       = eventinfo->auxdata<float>("G_0_HLT_CosTheta");
  RJVars_HLT_C_0_CosTheta       = eventinfo->auxdata<float>("C_0_HLT_CosTheta");
  RJVars_HLT_G_0_dPhiGC         = eventinfo->auxdata<float>("G_0_HLT_dPhiGC");
  RJVars_HLT_G_0_MassRatioGC    = eventinfo->auxdata<float>("G_0_HLT_MassRatioGC");
  RJVars_HLT_G_1_CosTheta       = eventinfo->auxdata<float>("G_1_HLT_CosTheta");
  RJVars_HLT_C_1_CosTheta       = eventinfo->auxdata<float>("C_1_HLT_CosTheta");
  RJVars_HLT_G_1_dPhiGC         = eventinfo->auxdata<float>("G_1_HLT_dPhiGC");
  RJVars_HLT_G_1_MassRatioGC    = eventinfo->auxdata<float>("G_1_HLT_MassRatioGC");
  RJVars_HLT_G_0_Jet1_pT        = eventinfo->auxdata<float>("G_0_HLT_Jet1_pT");
  RJVars_HLT_G_0_Jet2_pT        = eventinfo->auxdata<float>("G_0_HLT_Jet2_pT");
  RJVars_HLT_G_1_Jet1_pT        = eventinfo->auxdata<float>("G_1_HLT_Jet1_pT");
  RJVars_HLT_G_1_Jet2_pT        = eventinfo->auxdata<float>("G_1_HLT_Jet2_pT");
  RJVars_HLT_QCD_dPhiR          = eventinfo->auxdata<float>("QCD_HLT_dPhiR");
  RJVars_HLT_QCD_Rpt            = eventinfo->auxdata<float>("QCD_HLT_Rpt"  );
  RJVars_HLT_QCD_Rmsib          = eventinfo->auxdata<float>("QCD_HLT_Rmsib");
  RJVars_HLT_QCD_Rpsib          = eventinfo->auxdata<float>("QCD_HLT_Rpsib");
  RJVars_HLT_QCD_Delta1          = eventinfo->auxdata<float>("QCD_HLT_Delta1");
  RJVars_HLT_QCD_Delta2          = eventinfo->auxdata<float>("QCD_HLT_Delta2");


  //trig
  RJVars_TriggerBits = eventinfo->auxdecor<int>("triggerBitset");



  /////////////////////////////////////////////////////////////////////////////////////
 // EventNumber = 111;

  tree->Fill();
  m_store->clear();


  return EL::StatusCode::SUCCESS;
}


EL::StatusCode PlantATree :: fillHLTVariables() {
  const char* APP_NAME = "PlantATree";
  const  xAOD::JetContainer* jets = 0;
  CHECK( m_store->retrieve( jets, "HLT_Jets" ) );

  xAOD::JetContainer::const_iterator jet_itr = (jets)->begin();
  xAOD::JetContainer::const_iterator jet_end = (jets)->end();
 for( ; jet_itr != jet_end; ++jet_itr ) {
   // if( (*jet_itr)->auxdata< char >("baseline")==1  &&
   //     (*jet_itr)->auxdata< char >("passOR")==1  &&
   //     (*jet_itr)->pt() > 30000.  && ( fabs( (*jet_itr)->eta()) < 2.8)  ) {
   //   Jet_goodJet ->push_back( 1.);
   // }
   // else{
   //   Jet_goodJet ->push_back( 0.);
   // }

  HLT_Jet_pT  ->push_back( (*jet_itr)->pt()  );
  HLT_Jet_eta ->push_back( (*jet_itr)->eta()  );
  HLT_Jet_phi ->push_back( (*jet_itr)->phi()  );
  HLT_Jet_E   ->push_back( (*jet_itr)->e()  );
  HLT_Jet_m   ->push_back( (*jet_itr)->m()  );
  //   HLT_Jet_MV1 ->push_back( (*jet_itr)->auxdata< float >("MV1")   );
 }
  //   }
  // }

  /////////////////////////////////////////////////////////////////////////////////////


  xAOD::MuonContainer* muons = 0;
  CHECK( m_store->retrieve( muons, "HLT_Muons" ) );

  xAOD::MuonContainer::iterator muon_itr = (muons)->begin();
  xAOD::MuonContainer::iterator muon_end = (muons)->end();
  for( ; muon_itr != muon_end; ++muon_itr ) {
    // if( (*muon_itr)->auxdata< char >("baseline")==1  &&
    //     (*muon_itr)->auxdata< char >("passOR")==1   ) {

        HLT_Muon_pT  ->push_back( (*muon_itr)->pt()  );
        HLT_Muon_eta ->push_back( (*muon_itr)->eta()  );
        HLT_Muon_phi ->push_back( (*muon_itr)->phi()  );
        HLT_Muon_E   ->push_back( (*muon_itr)->e()  );
        HLT_Muon_m   ->push_back( (*muon_itr)->m()  );

	//    }
  }

  /////////////////////////////////////////////////////////////////////////////////////

  xAOD::ElectronContainer* electrons = 0;
  CHECK( m_store->retrieve( electrons, "HLT_Electrons" ) );

  xAOD::ElectronContainer::iterator electron_itr = (electrons)->begin();
  xAOD::ElectronContainer::iterator electron_end = (electrons)->end();
  for( ; electron_itr != electron_end; ++electron_itr ) {
    // if( (*electron_itr)->auxdata< char >("baseline")==1  &&
    //     (*electron_itr)->auxdata< char >("passOR")==1   ) {

        HLT_Electron_pT  ->push_back( (*electron_itr)->pt()  );
        HLT_Electron_eta ->push_back( (*electron_itr)->eta()  );
        HLT_Electron_phi ->push_back( (*electron_itr)->phi()  );
        HLT_Electron_E   ->push_back( (*electron_itr)->e()  );
        HLT_Electron_m   ->push_back( (*electron_itr)->m()  );

	//    }
  }

  /////////////////////////////////////////////////////////////////////////////////////



  xAOD::MissingETContainer* MET = nullptr;//new xAOD::MissingETContainer;
  CHECK( m_store->retrieve( MET, "HLT_MET_RefFinalCont" ) );

    xAOD::MissingETContainer::const_iterator met_it = MET->find("HLT_MET_Final");
  if (met_it == MET->end()) {
  } else {
    HLT_MET_x = (*met_it)->mpx();
    HLT_MET_y = (*met_it)->mpy();
  }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode PlantATree :: fillRecoVariables() {
  const char* APP_NAME = "PlantATree";
  const   xAOD::JetContainer* jets = 0;
  CHECK( m_store->retrieve( jets, "CalibJets" ) );

  xAOD::JetContainer::const_iterator jet_itr = (jets)->begin();
  xAOD::JetContainer::const_iterator jet_end = (jets)->end();
 for( ; jet_itr != jet_end; ++jet_itr ) {
   // if( (*jet_itr)->auxdata< char >("baseline")==1  &&
   //     //       (*jet_itr)->auxdata< char >("passOR")==1  &&
   //     (*jet_itr)->pt() > 30000.  && ( fabs( (*jet_itr)->eta()) < 2.8)  ) {
   //   //     Jet_goodJet ->push_back( 1.);
   // }
   // else{
   //   Jet_goodJet ->push_back( 0.);
   // }

  Jet_pT  ->push_back( (*jet_itr)->pt()  );
  Jet_eta ->push_back( (*jet_itr)->eta()  );
  Jet_phi ->push_back( (*jet_itr)->phi()  );
  Jet_E   ->push_back( (*jet_itr)->e()  );
  Jet_m   ->push_back( (*jet_itr)->m()  );
  //  Jet_MV1 ->push_back( (*jet_itr)->auxdata< float >("MV1")   );
 }
  // }

  /////////////////////////////////////////////////////////////////////////////////////


  xAOD::MuonContainer* muons = 0;
  CHECK( m_store->retrieve( muons, "CalibMuons" ) );

  xAOD::MuonContainer::iterator muon_itr = (muons)->begin();
  xAOD::MuonContainer::iterator muon_end = (muons)->end();
  for( ; muon_itr != muon_end; ++muon_itr ) {
    // if( (*muon_itr)->auxdata< char >("baseline")==1  &&
    //     (*muon_itr)->auxdata< char >("passOR")==1   ) {

        Muon_pT  ->push_back( (*muon_itr)->pt()  );
        Muon_eta ->push_back( (*muon_itr)->eta()  );
        Muon_phi ->push_back( (*muon_itr)->phi()  );
        Muon_E   ->push_back( (*muon_itr)->e()  );
        Muon_m   ->push_back( (*muon_itr)->m()  );

	//    }
  }

  /////////////////////////////////////////////////////////////////////////////////////

  xAOD::ElectronContainer* electrons = 0;
  CHECK( m_store->retrieve( electrons, "CalibElectrons" ) );

  xAOD::ElectronContainer::iterator electron_itr = (electrons)->begin();
  xAOD::ElectronContainer::iterator electron_end = (electrons)->end();
  for( ; electron_itr != electron_end; ++electron_itr ) {
    // if( (*electron_itr)->auxdata< char >("baseline")==1  &&
    //     (*electron_itr)->auxdata< char >("passOR")==1   ) {

        Electron_pT  ->push_back( (*electron_itr)->pt()  );
        Electron_eta ->push_back( (*electron_itr)->eta()  );
        Electron_phi ->push_back( (*electron_itr)->phi()  );
        Electron_E   ->push_back( (*electron_itr)->e()  );
        Electron_m   ->push_back( (*electron_itr)->m()  );

	//    }
  }

  /////////////////////////////////////////////////////////////////////////////////////



  xAOD::MissingETContainer* MET = nullptr;//new xAOD::MissingETContainer;
  CHECK( m_store->retrieve( MET, "CalibMET_Reference_AntiKt4EMTopo" ) );

    xAOD::MissingETContainer::const_iterator met_it = MET->find("Final");
  if (met_it == MET->end()) {
    //    Error( APP_NAME, "No RefFinal inside MET container" );
  } else {
    MET_x = (*met_it)->mpx();
    MET_y = (*met_it)->mpy();
  }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode PlantATree :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode PlantATree :: finalize ()
{

		if( m_grl ) {
			delete m_grl;
			m_grl = 0;
		}

  // This method is the mirror image of initialize(), meaning it gets
  // called after the last event has been processed on the worker node
  // and allows you to finish up any objects you created in
  // initialize() before they are written to disk.  This is actually
  // fairly rare, since this happens separately for each worker node.
  // Most of the time you want to do your post-processing on the
  // submission node after all your histogram outputs have been
  // merged.  This is different from histFinalize() in that it only
  // gets called on worker nodes that processed input events.



  return EL::StatusCode::SUCCESS;
}



EL::StatusCode PlantATree :: histFinalize ()
{
  // This method is the mirror image of histInitialize(), meaning it
  // gets called after the last event has been processed on the worker
  // node and allows you to finish up any objects you created in
  // histInitialize() before they are written to disk.  This is
  // actually fairly rare, since this happens separately for each
  // worker node.  Most of the time you want to do your
  // post-processing on the submission node after all your histogram
  // outputs have been merged.  This is different from finalize() in
  // that it gets called on all worker nodes regardless of whether
  // they processed input events.

  // HLT_Jet_pT        ->clear();
  // HLT_Jet_eta       ->clear();
  // HLT_Jet_phi       ->clear();
  // HLT_Jet_E         ->clear();
  // HLT_Jet_m         ->clear();
  // HLT_Jet_MV1       ->clear();
  // HLT_Muon_pT       ->clear();
  // HLT_Muon_eta      ->clear();
  // HLT_Muon_phi      ->clear();
  // HLT_Muon_E        ->clear();
  // HLT_Muon_m        ->clear();
  // HLT_Electron_pT   ->clear();
  // HLT_Electron_eta  ->clear();
  // HLT_Electron_phi  ->clear();
  // HLT_Electron_E    ->clear();
  // HLT_Electron_m    ->clear();

  return EL::StatusCode::SUCCESS;
}
