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

  tree->Branch("RJVars_GG_Mass", &RJVars_GG_Mass);
  tree->Branch("RJVars_GG_GammaInv", &RJVars_GG_GammaInv);
  tree->Branch("RJVars_GG_MDeltaR", &RJVars_GG_MDeltaR);
  tree->Branch("RJVars_GG_CosTheta", &RJVars_GG_CosTheta);
  tree->Branch("RJVars_GG_DeltaBetaGG", &RJVars_GG_DeltaBetaGG);
  tree->Branch("RJVars_GG_dPhiVG", &RJVars_GG_dPhiVG);
  tree->Branch("RJVars_GG_dPhiVGM", &RJVars_GG_dPhiVGM);
  tree->Branch("RJVars_Ga_Mass", &RJVars_Ga_Mass);
  tree->Branch("RJVars_Ga_CosTheta", &RJVars_Ga_CosTheta);
  tree->Branch("RJVars_Gb_Mass", &RJVars_Gb_Mass);
  tree->Branch("RJVars_Gb_CosTheta", &RJVars_Gb_CosTheta);
  tree->Branch("RJVars_Ia_Depth", &RJVars_Ia_Depth);
  tree->Branch("RJVars_Ib_Depth", &RJVars_Ib_Depth);
  tree->Branch("RJVars_Va_N", &RJVars_Va_N);
  tree->Branch("RJVars_Ca_N", &RJVars_Ca_N);
  tree->Branch("RJVars_Vb_N", &RJVars_Vb_N);
  tree->Branch("RJVars_Cb_N", &RJVars_Cb_N);

  //QCD Variables

  tree->Branch("RJVars_QCD_RPT", &RJVars_QCD_RPT);
  tree->Branch("RJVars_QCD_RPZ", &RJVars_QCD_RPZ);
  tree->Branch("RJVars_QCD_RMsib", &RJVars_QCD_RMsib);
  tree->Branch("RJVars_QCD_RPsib", &RJVars_QCD_RPsib);
  tree->Branch("RJVars_QCD_CosTheta", &RJVars_QCD_CosTheta);
  tree->Branch("RJVars_QCD_dPhiR", &RJVars_QCD_dPhiR);
  tree->Branch("RJVars_QCD_Delta1", &RJVars_QCD_Delta1);
  tree->Branch("RJVars_QCD_Detla2", &RJVars_QCD_Delta2);

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

  //tree->Branch("MET_x"   , &MET_x    );
  //tree->Branch("MET_y"   , &MET_y    );

  float HT;
  float MET;
  float Meff;
  float METsig;

  tree->Branch("HT", &HT);
  tree->Branch("MET", &MET);
  tree->Branch("Meff", &Meff);
  tree->Branch("METsig", &METsig);
  
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

  // m_store->print();

  xAOD::EventInfo* eventinfo = 0;
  CHECK( m_store->retrieve(eventinfo, "myEventInfo") );

  if( eventinfo->auxdata<char>("selection") == (char) 0 ){
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


  RJVars_GG_Mass = eventinfo->auxdata<float>("GG_Mass");
  RJVars_GG_GammaInv = eventinfo->auxdata<float>("GG_GammaInv");
  RJVars_GG_MDeltaR = eventinfo->auxdata<float>("GG_MDeltaR");
  RJVars_GG_CosTheta = eventinfo->auxdata<float>("GG_CosTheta");
  RJVars_GG_DeltaBetaGG = eventinfo->auxdata<float>("GG_DeltaBetaGG");
  RJVars_GG_dPhiVG = eventinfo->auxdata<float>("GG_dPhiVG");
  RJVars_GG_dPhiVGM = eventinfo->auxdata<float>("GG_dPhiVGM");
  RJVars_Ga_Mass = eventinfo->auxdata<float>("Ga_Mass");
  RJVars_Ga_CosTheta = eventinfo->auxdata<float>("Gb_CosTheta");
  RJVars_Gb_Mass = eventinfo->auxdata<float>("Gb_Mass");
  RJVars_Gb_CosTheta = eventinfo->auxdata<float>("Gb_CosTheta");
  RJVars_Ia_Depth = eventinfo->auxdata<float>("Ia_Depth");
  RJVars_Ib_Depth = eventinfo->auxdata<float>("Ib_Depth");
  RJVars_Va_N = eventinfo->auxdata<float>("Va_N");
  RJVars_Ca_N = eventinfo->auxdata<float>("Ca_N");
  RJVars_Vb_N = eventinfo->auxdata<float>("Vb_N");
  RJVars_Cb_N = eventinfo->auxdata<float>("Cb_N");

  //QCD Variables

  RJVars_QCD_RPT = eventinfo->auxdata<float>("QCD_RPT");
  RJVars_QCD_RPZ = eventinfo->auxdata<float>("QCD_RPZ");
  RJVars_QCD_RMsib = eventinfo->auxdata<float>("QCD_RMsib");
  RJVars_QCD_RPsib = eventinfo->auxdata<float>("QCD_RPsib");
  RJVars_QCD_CosTheta = eventinfo->auxdata<float>("QCD_CosTheta");
  RJVars_QCD_dPhiR = eventinfo->auxdata<float>("QCD_dPhiR");
  RJVars_QCD_Delta1 = eventinfo->auxdata<float>("QCD_Delta1");
  RJVars_QCD_Delta2 = eventinfo->auxdata<float>("QCD_Delta2");

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

  /////////////////////////////////////////////////////////////////////////////////////

  xAOD::JetContainer* jets = 0;
  CHECK( m_store->retrieve( jets, "CalibJets" ) );

  xAOD::JetContainer::iterator jet_itr = (jets)->begin();
  xAOD::JetContainer::iterator jet_end = (jets)->end();
  HT = 0;
  for( ; jet_itr != jet_end; ++jet_itr ) {
    if( (*jet_itr)->auxdata< char >("baseline")==1  &&
        (*jet_itr)->auxdata< char >("passOR")==1  &&
        (*jet_itr)->pt() > 20000.  && ( fabs( (*jet_itr)->eta()) < 2.5)  ) {

        Jet_pT  ->push_back( (*jet_itr)->pt()  );
        Jet_eta ->push_back( (*jet_itr)->eta()  );
        Jet_phi ->push_back( (*jet_itr)->phi()  );
        Jet_E   ->push_back( (*jet_itr)->e()  );
        Jet_m   ->push_back( (*jet_itr)->m()  );
        Jet_MV1 ->push_back( (*jet_itr)->auxdata< float >("MV1")   );
	HT += (*jet_itr)->pt();
	
    } 
  }

  /////////////////////////////////////////////////////////////////////////////////////


  xAOD::MuonContainer* muons = 0;
  CHECK( m_store->retrieve( muons, "CalibMuons" ) );

  xAOD::MuonContainer::iterator muon_itr = (muons)->begin();
  xAOD::MuonContainer::iterator muon_end = (muons)->end();
  for( ; muon_itr != muon_end; ++muon_itr ) {
    if( (*muon_itr)->auxdata< char >("baseline")==1  &&
        (*muon_itr)->auxdata< char >("passOR")==1   ) {

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
    if( (*electron_itr)->auxdata< char >("baseline")==1  &&
        (*electron_itr)->auxdata< char >("passOR")==1   ) {

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


  MET = TMath::Sqrt(MET_x*MET_x+MET_y*MET_y);
  Meff = HT + MET;
  METsig = MET/TMath::Sqrt(HT);

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
