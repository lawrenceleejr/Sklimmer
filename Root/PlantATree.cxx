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
#include "xAODTruth/TruthParticleAuxContainer.h"
#include "xAODTruth/TruthEventContainer.h"
#include "xAODTruth/TruthEvent.h"
#include "xAODCore/ShallowCopy.h"
#include "xAODMissingET/MissingETContainer.h"
#include "xAODMissingET/MissingETAuxContainer.h"
#include "xAODBTaggingEfficiency/BTaggingEfficiencyTool.h"
//#include "xAODBTagging/BTagging.h"


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
  TruthJet_pT = new std::vector<float>       ;    
  TruthJet_eta = new std::vector<float>       ;   
  TruthJet_phi = new std::vector<float>       ;   
  TruthJet_E = new std::vector<float>       ;     
  TruthJet_m = new std::vector<float>       ;     
  //TruthJet_flavor = new std::vector<float>       ;     
  TruthMuon_pT = new std::vector<float>       ;    
  TruthMuon_eta = new std::vector<float>       ;   
  TruthMuon_phi = new std::vector<float>       ;   
  TruthMuon_E = new std::vector<float>       ;     
  TruthMuon_m = new std::vector<float>       ;     
  TruthElectron_pT = new std::vector<float>       ;    
  TruthElectron_eta = new std::vector<float>       ;   
  TruthElectron_phi = new std::vector<float>       ;   
  TruthElectron_E = new std::vector<float>       ;     
  TruthElectron_m = new std::vector<float>       ;     

  

  //Set up branches here

  
  tree->Branch("RunNumber"                     , &RunNumber                      );
  tree->Branch("EventNumber"                   , &EventNumber                    );
  tree->Branch("LumiBlock"                     , &LumiBlock                      );
  tree->Branch("BCID"                          , &BCID                           );
  tree->Branch("MCChannelNumber"               , &MCChannelNumber                );
  tree->Branch("MCEventNumber"                 , &MCEventNumber                  );
  tree->Branch("MCEventWeight"                 , &MCEventWeight                  );
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

  tree->Branch("TruthRJVars_SS_Mass"           , &TruthRJVars_SS_Mass           ); 
  tree->Branch("TruthRJVars_SS_InvGamma"       , &TruthRJVars_SS_InvGamma       ); 
  tree->Branch("TruthRJVars_SS_dPhiBetaR"      , &TruthRJVars_SS_dPhiBetaR      ); 
  tree->Branch("TruthRJVars_SS_dPhiVis"        , &TruthRJVars_SS_dPhiVis        ); 
  tree->Branch("TruthRJVars_SS_CosTheta"       , &TruthRJVars_SS_CosTheta       ); 
  tree->Branch("TruthRJVars_SS_dPhiDecayAngle" , &TruthRJVars_SS_dPhiDecayAngle ); 
  tree->Branch("TruthRJVars_SS_VisShape"       , &TruthRJVars_SS_VisShape       ); 
  tree->Branch("TruthRJVars_SS_MDeltaR"        , &TruthRJVars_SS_MDeltaR        ); 
  tree->Branch("TruthRJVars_S1_Mass"           , &TruthRJVars_S1_Mass           ); 
  tree->Branch("TruthRJVars_S1_CosTheta"       , &TruthRJVars_S1_CosTheta       ); 
  tree->Branch("TruthRJVars_S2_Mass"           , &TruthRJVars_S2_Mass           ); 
  tree->Branch("TruthRJVars_S2_CosTheta"       , &TruthRJVars_S2_CosTheta       ); 
  tree->Branch("TruthRJVars_I1_Depth"          , &TruthRJVars_I1_Depth          ); 
  tree->Branch("TruthRJVars_I2_Depth"          , &TruthRJVars_I2_Depth          ); 
  tree->Branch("TruthRJVars_V1_N"              , &TruthRJVars_V1_N              ); 
  tree->Branch("TruthRJVars_V2_N"              , &TruthRJVars_V2_N              ); 


  //QCD Variables
  
  tree->Branch("RJVars_QCD_dPhiR" ,  &RJVars_QCD_dPhiR  );
  tree->Branch("RJVars_QCD_Rpt"   ,  &RJVars_QCD_Rpt    );
  tree->Branch("RJVars_QCD_Rmsib" ,  &RJVars_QCD_Rmsib  );
  tree->Branch("RJVars_QCD_Rpsib" ,  &RJVars_QCD_Rpsib  );
  tree->Branch("RJVars_QCD_Delta1" ,  &RJVars_QCD_Delta1  );
  tree->Branch("RJVars_QCD_Delta2" ,  &RJVars_QCD_Delta2  );

  tree->Branch("TruthRJVars_QCD_dPhiR" ,  &TruthRJVars_QCD_dPhiR  );
  tree->Branch("TruthRJVars_QCD_Rpt"   ,  &TruthRJVars_QCD_Rpt    );
  tree->Branch("TruthRJVars_QCD_Rmsib" ,  &TruthRJVars_QCD_Rmsib  );
  tree->Branch("TruthRJVars_QCD_Rpsib" ,  &TruthRJVars_QCD_Rpsib  );
  tree->Branch("TruthRJVars_QCD_Delta1" ,  &TruthRJVars_QCD_Delta1  );
  tree->Branch("TruthRJVars_QCD_Delta2" ,  &TruthRJVars_QCD_Delta2  );

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
  tree->Branch("TruthJet_pT"       , &TruthJet_pT        );
  tree->Branch("TruthJet_eta"      , &TruthJet_eta       );
  tree->Branch("TruthJet_phi"      , &TruthJet_phi       );
  tree->Branch("TruthJet_E"        , &TruthJet_E         );
  tree->Branch("TruthJet_m"        , &TruthJet_m         );
  //tree->Branch("TruthJet_flavor"      , &TruthJet_flavor       );
  tree->Branch("TruthMuon_pT"      , &TruthMuon_pT       );
  tree->Branch("TruthMuon_eta"     , &TruthMuon_eta      );
  tree->Branch("TruthMuon_phi"     , &TruthMuon_phi      );
  tree->Branch("TruthMuon_E"       , &TruthMuon_E        );
  tree->Branch("TruthMuon_m"       , &TruthMuon_m        );
  tree->Branch("TruthElectron_pT"  , &TruthElectron_pT   );
  tree->Branch("TruthElectron_eta" , &TruthElectron_eta  );
  tree->Branch("TruthElectron_phi" , &TruthElectron_phi  );
  tree->Branch("TruthElectron_E"   , &TruthElectron_E    );
  tree->Branch("TruthElectron_m"   , &TruthElectron_m    );


  tree->Branch("MET_x"   , &MET_x    );
  tree->Branch("MET_y"   , &MET_y    );


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

  const char* APP_NAME = "PlantATree";



  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();


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

  const char* APP_NAME = "PlantATree";

  std::cout << "PlantATree :: execute ()" << std::endl;
  // std::cout << "PlantATree - Contains event info? " <<std::endl; m_store->print();

  //m_store->print();

  xAOD::EventInfo* eventinfo = 0;
  CHECK( m_store->retrieve(eventinfo, "MyEventInfo") );

  if( eventinfo->auxdata<char>("selection") == (char) 0 ){ //if not an event passing your 'selection' don't save to ttree
      m_store->clear(); 
      return EL::StatusCode::SUCCESS;
  }

  RunNumber                       = eventinfo->runNumber(); 
  EventNumber                     = eventinfo->eventNumber(); 
  LumiBlock                       = eventinfo->lumiBlock(); 
  BCID                            = eventinfo->bcid(); 
  MCChannelNumber                 = eventinfo->mcChannelNumber(); 
  MCEventNumber                   = eventinfo->mcEventNumber(); 
  MCEventWeight                   = eventinfo->mcEventWeight(); 
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

  RJVars_QCD_dPhiR          = eventinfo->auxdata<float>("QCD_dPhiR"); 
  RJVars_QCD_Rpt            = eventinfo->auxdata<float>("QCD_Rpt"  );
  RJVars_QCD_Rmsib          = eventinfo->auxdata<float>("QCD_Rmsib");
  RJVars_QCD_Rpsib          = eventinfo->auxdata<float>("QCD_Rpsib");
  RJVars_QCD_Delta1          = eventinfo->auxdata<float>("QCD_Delta1");
  RJVars_QCD_Delta2          = eventinfo->auxdata<float>("QCD_Delta2");



  TruthRJVars_SS_Mass            = eventinfo->auxdata<float>("TruthSS_Mass");
  TruthRJVars_SS_InvGamma        = eventinfo->auxdata<float>("TruthSS_InvGamma");
  TruthRJVars_SS_dPhiBetaR       = eventinfo->auxdata<float>("TruthSS_dPhiBetaR");
  TruthRJVars_SS_dPhiVis         = eventinfo->auxdata<float>("TruthSS_dPhiVis");
  TruthRJVars_SS_CosTheta        = eventinfo->auxdata<float>("TruthSS_CosTheta");
  TruthRJVars_SS_dPhiDecayAngle  = eventinfo->auxdata<float>("TruthSS_dPhiDecayAngle");
  TruthRJVars_SS_VisShape        = eventinfo->auxdata<float>("TruthSS_VisShape");
  TruthRJVars_SS_MDeltaR         = eventinfo->auxdata<float>("TruthSS_MDeltaR");
  TruthRJVars_S1_Mass            = eventinfo->auxdata<float>("TruthS1_Mass");
  TruthRJVars_S1_CosTheta        = eventinfo->auxdata<float>("TruthS1_CosTheta");
  TruthRJVars_S2_Mass            = eventinfo->auxdata<float>("TruthS2_Mass");
  TruthRJVars_S2_CosTheta        = eventinfo->auxdata<float>("TruthS2_CosTheta");
  TruthRJVars_I1_Depth           = eventinfo->auxdata<float>("TruthI1_Depth");
  TruthRJVars_I2_Depth           = eventinfo->auxdata<float>("TruthI2_Depth");
  TruthRJVars_V1_N               = eventinfo->auxdata<float>("TruthV1_N");
  TruthRJVars_V2_N               = eventinfo->auxdata<float>("TruthV2_N");

  TruthRJVars_QCD_dPhiR          = eventinfo->auxdata<float>("TruthQCD_dPhiR"); 
  TruthRJVars_QCD_Rpt            = eventinfo->auxdata<float>("TruthQCD_Rpt"  );
  TruthRJVars_QCD_Rmsib          = eventinfo->auxdata<float>("TruthQCD_Rmsib");
  TruthRJVars_QCD_Rpsib          = eventinfo->auxdata<float>("TruthQCD_Rpsib");
  TruthRJVars_QCD_Delta1          = eventinfo->auxdata<float>("TruthQCD_Delta1");
  TruthRJVars_QCD_Delta2          = eventinfo->auxdata<float>("TruthQCD_Delta2");

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
  TruthJet_pT        ->clear();
  TruthJet_eta       ->clear();
  TruthJet_phi       ->clear();
  TruthJet_E         ->clear();
  TruthJet_m         ->clear();
  //TruthJet_flavor  ->clear();
  TruthMuon_pT       ->clear();
  TruthMuon_eta      ->clear();
  TruthMuon_phi      ->clear();
  TruthMuon_E        ->clear();
  TruthMuon_m        ->clear();
  TruthElectron_pT   ->clear();
  TruthElectron_eta  ->clear();
  TruthElectron_phi  ->clear();
  TruthElectron_E    ->clear();
  TruthElectron_m    ->clear();







  /////////////////////////////////////////////////////////////////////////////////////

  xAOD::JetContainer* jets = 0;
  CHECK( m_store->retrieve( jets, "CalibJets" ) );

  xAOD::JetContainer::iterator jet_itr = (jets)->begin();
  xAOD::JetContainer::iterator jet_end = (jets)->end();
  for( ; jet_itr != jet_end; ++jet_itr ) {
    if( ((int)((*jet_itr)->auxdata< char >("baseline")))==1  &&
        ((int)((*jet_itr)->auxdata< char >("passOR")))==1  &&
        (*jet_itr)->pt() > 20000.  && ( fabs( (*jet_itr)->eta()) < 2.8)  ) { // changing from 2.5 to 2.8 to match baseline

        Jet_pT  ->push_back( (*jet_itr)->pt()  );
        Jet_eta ->push_back( (*jet_itr)->eta()  );
        Jet_phi ->push_back( (*jet_itr)->phi()  );
        Jet_E   ->push_back( (*jet_itr)->e()  );
        Jet_m   ->push_back( (*jet_itr)->m()  );
        Jet_MV1 ->push_back( (*jet_itr)->auxdata< float >("MV1")   );

    } 
  }

  /////////////////////////////////////////////////////////////////////////////////////


  xAOD::MuonContainer* muons = 0;
  CHECK( m_store->retrieve( muons, "CalibMuons" ) );

  xAOD::MuonContainer::iterator muon_itr = (muons)->begin();
  xAOD::MuonContainer::iterator muon_end = (muons)->end();
  for( ; muon_itr != muon_end; ++muon_itr ) {

    if( ((int)((*muon_itr)->auxdata< char >("baseline")))==1  &&
        ((int)((*muon_itr)->auxdata< char >("passOR")))==1   ) {

        Muon_pT  ->push_back( (*muon_itr)->pt()  );
        Muon_eta ->push_back( (*muon_itr)->eta()  );
        Muon_phi ->push_back( (*muon_itr)->phi()  );
        Muon_E   ->push_back( (*muon_itr)->e()  );
        Muon_m   ->push_back( (*muon_itr)->m()  );

    } 
  }

  /////////////////////////////////////////////////////////////////////////////////////

  xAOD::ElectronContainer* electrons = 0;
  CHECK( m_store->retrieve( electrons, "CalibElectrons" ) );

  xAOD::ElectronContainer::iterator electron_itr = (electrons)->begin();
  xAOD::ElectronContainer::iterator electron_end = (electrons)->end();
  for( ; electron_itr != electron_end; ++electron_itr ) {
  	
    if( ((int) ((*electron_itr)->auxdata< char >("baseline"))) ==1  &&
        ((int) ((*electron_itr)->auxdata< char >("passOR")))==1   ) {

        Electron_pT  ->push_back( (*electron_itr)->pt()  );
        Electron_eta ->push_back( (*electron_itr)->eta()  );
        Electron_phi ->push_back( (*electron_itr)->phi()  );
        Electron_E   ->push_back( (*electron_itr)->e()  );
        Electron_m   ->push_back( (*electron_itr)->m()  );

    } 
  }

  /////////////////////////////////////////////////////////////////////////////////////



  xAOD::MissingETContainer* MET = new xAOD::MissingETContainer;
  CHECK( m_store->retrieve( MET, "CalibMET_RefFinal" ) );

    xAOD::MissingETContainer::const_iterator met_it = MET->find("Final");
  if (met_it == MET->end()) {
    Error( APP_NAME, "No RefFinal inside MET container" );
  } else {
    MET_x = (*met_it)->mpx();
    MET_y = (*met_it)->mpy();
  }



  /////////////////////////////////////////////////////////////////////////////////////

  //----------------
  // Truth particles
  // Currently not doing any overlap removal
  //----------------
  
 if( eventinfo->auxdata<int>("IsMC") == 1 ){

	  // instance of xAOD::TEvent to access truth info from input xAOD
	  xAOD::TEvent* event = wk()->xaodEvent();
	
	  const xAOD::JetContainer* truthJets = 0;
	  CHECK( event->retrieve( truthJets, "AntiKt4TruthJets" ) );
	  
	  xAOD::JetContainer::const_iterator truthJet_itr = (truthJets)->begin();
	  xAOD::JetContainer::const_iterator truthJet_end = (truthJets)->end();
	  for( ; truthJet_itr != truthJet_end; ++truthJet_itr ) {
	    if(  (*truthJet_itr)->pt() > 20000.  && ( fabs( (*truthJet_itr)->eta()) < 2.8)  ) { // geometrical cuts matching reco. (should probably not hard-code)
	
	        TruthJet_pT  ->push_back( (*truthJet_itr)->pt()  );
	        TruthJet_eta ->push_back( (*truthJet_itr)->eta()  );
	        TruthJet_phi ->push_back( (*truthJet_itr)->phi()  );
	        TruthJet_E   ->push_back( (*truthJet_itr)->e()  );
	        TruthJet_m   ->push_back( (*truthJet_itr)->m()  );
	        //TruthJet_flavor ->push_back( (*truthJet_itr)->auxdata< float >("MV1")   ); // flavor tagging
	
	    } // end if passing kinematical cuts
	
	  
	  } // end loop over truth jets

    
   	//-------------------------------------------------
   	// Truth particles to get truth electrons and muons
   	//-------------------------------------------------
   	const xAOD::TruthParticleContainer* truthParticles = 0;
   	CHECK(event->retrieve( truthParticles, "TruthParticle" ));
           
    xAOD::TruthParticleContainer::const_iterator tp_itr = truthParticles->begin();
    xAOD::TruthParticleContainer::const_iterator tp_end = truthParticles->end();
    for( ; tp_itr != tp_end; ++tp_itr ) {
       	// status == 1 && barcode < 100,000 for stable particles from generator
       	if ( ((*tp_itr)->status() == 1) && ((*tp_itr)->barcode() < 100000) ) {
       		
       		// electrons
       		if((fabs( (*tp_itr)->pdgId() ) == 11)){
       			// kinematic cuts, To do: don't make hardcoded! these are SUSYTools baseline electrons selection
       			if( ((*tp_itr)->pt() > 10000) && (fabs((*tp_itr)->eta()) < 2.47)  ){ 
       			    TruthElectron_pT  ->push_back( (*tp_itr)->pt()  );
                    TruthElectron_eta ->push_back( (*tp_itr)->eta()  );
                    TruthElectron_phi ->push_back( (*tp_itr)->phi()  );
                    TruthElectron_E   ->push_back( (*tp_itr)->e()  );
                    TruthElectron_m   ->push_back( (*tp_itr)->m()  );
       			} // end if passing electron kinematic cuts    
       		}// end if electrons	
       		       		
       		// muons
       		if((fabs( (*tp_itr)->pdgId() ) == 13)){
       			// kinematic cuts, To do: don't make hardcoded! these are SUSYTools baseline electrons selection
       			if( ((*tp_itr)->pt() > 10000) && (fabs((*tp_itr)->eta()) < 2.4)  ){ 
       			    TruthMuon_pT  ->push_back( (*tp_itr)->pt()  );
                    TruthMuon_eta ->push_back( (*tp_itr)->eta()  );
                    TruthMuon_phi ->push_back( (*tp_itr)->phi()  );
                    TruthMuon_E   ->push_back( (*tp_itr)->e()  );
                    TruthMuon_m   ->push_back( (*tp_itr)->m()  );
       			} // end if passing electron kinematic cuts    
       		}// end if muons	
       	      		
       	}	
       	
     } // end for loop over truth particles	
     

       

  } // end if MC


  /////////////////////////////////////////////////////////////////////////////////////




// fill the branches of our trees
  // EventNumber = 111;




  tree->Fill();

  m_store->clear(); 


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
  // This method is the mirror image of initialize(), meaning it gets
  // called after the last event has been processed on the worker node
  // and allows you to finish up any objects you created in
  // initialize() before they are written to disk.  This is actually
  // fairly rare, since this happens separately for each worker node.
  // Most of the time you want to do your post-processing on the
  // submission node after all your histogram outputs have been
  // merged.  This is different from histFinalize() in that it only
  // gets called on worker nodes that processed input events.

  const char* APP_NAME = "PlantATree";

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
  return EL::StatusCode::SUCCESS;
}
