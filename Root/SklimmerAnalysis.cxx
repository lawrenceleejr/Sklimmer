#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <Sklimmer/errorcheck.h>
#include <Sklimmer/SklimmerAnalysis.h>
#include <Sklimmer/EventSelectionBBMet.h>
#include <Sklimmer/RazorTriggerAnalysis.h>



#include <TLorentzVector.h>
#include <TVector3.h>

#include <cstdlib>
#include <string>
#include <bitset>

#include "EventLoop/OutputStream.h"

// EDM includes:
#include "xAODEventInfo/EventInfo.h"
#include "xAODEventInfo/EventAuxInfo.h"
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

// GRL
#include "GoodRunsLists/GoodRunsListSelectionTool.h"
//PU Reweighting
#include "PileupReweighting/PileupReweightingTool.h"

#include "SUSYTools/SUSYObjDef_xAOD.h"

//RJigsaw
#include <RJigsaw/TRJigsaw.h>

//Still really need to implement a systematics framework

// this is needed to distribute the algorithm to the workers
ClassImp(SklimmerAnalysis)



SklimmerAnalysis :: SklimmerAnalysis() : h_nevents(nullptr),
  m_store(nullptr),
#ifndef __CINT__
  m_grl(nullptr),
  m_pileupReweightingTool(nullptr),
  m_susy_obj(nullptr),
#endif // not __CINT__
  RJTool(nullptr),
  m_trigDecisionTool(nullptr),
  m_trigConfigTool(nullptr)
{
	// Here you put any code for the base initialization of variables,
	// e.g. initialize all pointers to 0.  Note that you should only put
	// the most basic initialization here, since this method will be
	// called on both the submission and the worker node.  Most of your
	// initialization code will go into histInitialize() and
	// initialize().

}



EL::StatusCode SklimmerAnalysis :: setupJob (EL::Job& job)
{
	// Here you put code that sets up the job on the submission object
	// so that it is ready to work with your algorithm, e.g. you can
	// request the D3PDReader service or add output files.  Any code you
	// put here could instead also go into the submission script.  The
	// sole advantage of putting it here is that it gets automatically
	// activated/deactivated when you add/remove the algorithm from your
	// job, which may or may not be of value to you.


	// let's initialize the algorithm to use the xAODRootAccess package
	job.useXAOD ();
	xAOD::Init( "SklimmerAnalysis" ).ignore(); // call before opening first file

	// tell EventLoop about our output xAOD:
	if(outputxAODName.empty()){
	  outputxAODName = "outputxAOD";
	}

	EL::OutputStream out (outputxAODName);
	job.outputAdd (out);

	return EL::StatusCode::SUCCESS;
}



EL::StatusCode SklimmerAnalysis :: histInitialize ()
{
	// Here you do everything that needs to be done at the very
	// beginning on each worker node, e.g. create histograms and output
	// trees.  This method gets called before any input files are
	// connected.

	h_nevents = new TH1F("h_nevents", "h_nevents", 10, 0, 10);
	h_nevents_weighted = new TH1F("h_nevents_weighted", "h_nevents_weighted", 10, 0, 10);
	wk()->addOutput (h_nevents);
	wk()->addOutput (h_nevents_weighted);

	return EL::StatusCode::SUCCESS;
}



EL::StatusCode SklimmerAnalysis :: fileExecute ()
{
	// Here you do everything that needs to be done exactly once for every
	// single file, e.g. collect a list of all lumi-blocks processed
	return EL::StatusCode::SUCCESS;
}



EL::StatusCode SklimmerAnalysis :: changeInput (bool firstFile)
{
	// Here you do everything you need to do when we change input files,
	// e.g. resetting branch addresses on trees.  If you are using
	// D3PDReader or a similar service this method is not needed.
	return EL::StatusCode::SUCCESS;
}



EL::StatusCode SklimmerAnalysis :: initialize ()
{
	// Here you do everything that you need to do after the first input
	// file has been connected and before the first event is processed,
	// e.g. create additional histograms based on which variables are
	// available in the input files.  You can also create all of your
	// histograms and trees in here, but be aware that this method
	// doesn't get called if no events are processed.  So any objects
	// you create here won't be available in the output if you have no
	// input events.

	// This will get moved to submission at some point... //////////////////////
        if(m_writexAOD == false) m_writeFullCollectionsToxAOD = false; // logic and fail-safe

	Info("initialize()", "m_doSklimming = %i"               , m_doSklimming                 );
	Info("initialize()", "m_doSUSYObjDef = %i"              , m_doSUSYObjDef                );
	Info("initialize()", "m_doEventSelection = %i"          , m_doEventSelection            );
	// Info("initialize()", "m_writeNtuple = %i"               , m_writeNtuple                 );
	Info("initialize()", "m_writexAOD = %i"                 , m_writexAOD                   );
	Info("initialize()", "m_writeFullCollectionsToxAOD = %i", m_writeFullCollectionsToxAOD  );
	Info("initialize()", "m_Analysis = %s"                  , m_Analysis.Data()             );


	// m_doSklimming = true;
	// m_doSUSYObjDef = true;
	// m_doEventSelection = true;
	// m_writeNtuple = false;
	// m_writexAOD = true;
	// m_writeFullCollectionsToxAOD = true;

	// m_Analysis = "bbmet";


	////////////////////////////////////////////////////////////////////////////

	xAOD::TEvent *event = wk()->xaodEvent();
	m_store = wk()->xaodStore();

	if(fillEmptyCollectionNames() != StatusCode::SUCCESS){
	  Error( __PRETTY_FUNCTION__, "Failed to fill empty collection names" );
	  return EL::StatusCode::FAILURE;
	}

	// as a check, let's see the number of events in our xAOD
	Info("initialize()", "Number of events = %lli", event->getEntries() ); // print long long int

	// Output xAOD ///////////////////////////////////////////////////////////////////

	if(m_writexAOD){
		TFile *file = wk()->getOutputFile (outputxAODName);
		CHECK(event->writeTo(file));
	}

	// SUSYTools ///////////////////////////////////////////////////////////////////

	if(m_doSUSYObjDef &&
	   (initializeSUSYTools() != StatusCode::SUCCESS)){
	  Error(__PRETTY_FUNCTION__ , "Failed to initialize SUSY Tools" );
	  return EL::StatusCode::FAILURE;
	}

	// GRL ///////////////////////////////////////////////////////////////////
	if( initializeGRLTool()  != StatusCode::SUCCESS){
	  Error(__PRETTY_FUNCTION__ , "Failed to initialize GRL Tool" );
	  return EL::StatusCode::FAILURE;
	}

	// Pile Up Reweighting ///////////////////////////////////////////////////////////////////
	if( initializePileupReweightingTool() != StatusCode::SUCCESS) {
	  Error(__PRETTY_FUNCTION__ , "Failed to initialize Pileup Reweighting Tool" );
	  return EL::StatusCode::FAILURE;
	}

	// TrigDecisionTool ///////////////////////////////////////////////////////////////////

	if(initializeTrigDecisionTool() != StatusCode::SUCCESS){
	  Error(__PRETTY_FUNCTION__ , "Failed to initialize TrigDecisionTool" );
	  return EL::StatusCode::FAILURE;
	}

	// RJigsaw Tool ///////////////////////////////////////////////////////////////////

	RJTool = new Root::TRJigsaw();

	if (!RJTool){
		throw std::string ("No RJTool configured");
	}

	RJTool->initialize( gSystem->ExpandPathName("$ROOTCOREBIN/data/RJigsaw/RJigsawConfig/hemisphere1"),
						gSystem->ExpandPathName("$ROOTCOREBIN/data/RJigsaw/RJigsawConfig/hemisphere2") );

	RJTool->resetHists();

	// std::cout << "Leaving SklimmerAnalysis :: initialize ()"  << std::endl;

	return EL::StatusCode::SUCCESS;
}

EL::StatusCode SklimmerAnalysis :: initializeTrigDecisionTool(){
  m_trigConfigTool   = new TrigConf::xAODConfigTool("TrigConfigTool");
  m_configHandle = m_trigConfigTool;
  CHECK(  m_configHandle->initialize() );

  m_trigDecisionTool = new Trig::TrigDecisionTool("TrigDecisionTool");
  CHECK(    m_trigDecisionTool->setProperty("ConfigTool" , m_configHandle));
  //trigDecTool.setProperty("OutputLevel", MSG::VERBOSE);
  CHECK(    m_trigDecisionTool->setProperty("TrigDecisionKey","xTrigDecision"));
  CHECK(    m_trigDecisionTool->initialize() );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode SklimmerAnalysis :: initializePileupReweightingTool(){
  m_pileupReweightingTool= new PileupReweightingTool("PileupReweightingTool");
  CHECK( m_pileupReweightingTool->setProperty("Input","EventInfo") );//todo should this EventInfo be the eventInfoName member?
  std::vector<std::string> prwFiles;
  prwFiles.push_back("PileupReweighting/mc14v1_defaults.prw.root");
  CHECK( m_pileupReweightingTool->setProperty("ConfigFiles",prwFiles) );
  std::vector<std::string> lumicalcFiles;
  lumicalcFiles.push_back("SUSYTools/susy_data12_avgintperbx.root");
  CHECK( m_pileupReweightingTool->setProperty("LumiCalcFiles",lumicalcFiles) );
  if(!m_pileupReweightingTool->initialize().isSuccess()){
    Error(__PRETTY_FUNCTION__, "Failed to properly initialize the Pile Up Reweighting. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode SklimmerAnalysis :: initializeSUSYTools(){
  m_susy_obj = new ST::SUSYObjDef_xAOD( "SUSYObjDef_xAOD" );

  //todo do we need this isData property? either this or isMC seems redundant?
  CHECK( m_susy_obj->setProperty("IsData",isData) );
  CHECK( m_susy_obj->setProperty("IsAtlfast",isAtlfast) );
  CHECK( m_susy_obj->setProperty("EleId","Tight") );

  if( m_susy_obj->SUSYToolsInit().isFailure() ) {
    Error( __PRETTY_FUNCTION__, "Failed to initialise tools in SUSYToolsInit()..." );
    Error( __PRETTY_FUNCTION__, "Exiting..." );
    return EL::StatusCode::FAILURE;
  }
  else{ Info(__PRETTY_FUNCTION__,"SUSYToolsInit with success!!... " );}

  if( m_susy_obj->initialize() != StatusCode::SUCCESS){
    Error(__PRETTY_FUNCTION__, "Cannot intialize SUSYObjDef_xAOD..." );
    Error(__PRETTY_FUNCTION__, "Exiting... " );
    return EL::StatusCode::FAILURE;
  }else{
    Info(__PRETTY_FUNCTION__,"SUSYObjDef_xAOD initialized... " );
  }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode SklimmerAnalysis :: initializeGRLTool(){
	m_grl = new GoodRunsListSelectionTool("GoodRunsListSelectionTool");
	std::vector<std::string> vecStringGRL;
	vecStringGRL.push_back(gSystem->ExpandPathName("$ROOTCOREBIN/data/SUSYTools/GRL/Summer2013/data12_8TeV.periodAllYear_DetStatus-v61-pro14-02_DQDefects-00-01-00_PHYS_StandardGRL_All_Good.xml"));
	CHECK( m_grl->setProperty( "GoodRunsListVec", vecStringGRL) );
	CHECK( m_grl->setProperty("PassThrough", false) ); // if true (default) will ignore result of GRL and will just pass all events
	if (!m_grl->initialize().isSuccess()) { // check this isSuccess
		Error(__PRETTY_FUNCTION__, "Failed to properly initialize the GRL. Exiting." );
		return EL::StatusCode::FAILURE;
	}

	return EL::StatusCode::SUCCESS;
}

int SklimmerAnalysis :: copyFullxAODContainers ()
{
	// copy full container(s) to new xAOD
	// without modifying the contents of it:
	xAOD::TEvent *event = wk()->xaodEvent();

	CHECK(event->copy(eventInfoName));
	CHECK(event->copy(truthEventName));
	CHECK(event->copy(truthParticleName));

	CHECK(event->copy(jetCollectionName));
	CHECK(event->copy(truthJetCollectionName));

	CHECK(event->copy(metCollectionName));
	CHECK(event->copy(truthMetCollectionName));

	CHECK(event->copy(truthPrimaryVerticesName));
	CHECK(event->copy(primaryVerticesName));

	CHECK(event->copy(electronCollectionName));
	CHECK(event->copy(muonCollectionName));
	// CHECK(event->copy(photonCollectionName));

	return 0;

}



int SklimmerAnalysis :: applySUSYObjectDefinitions (){

	xAOD::TEvent *event = wk()->xaodEvent();

	//------------
	// MUONS
	//------------

	const xAOD::MuonContainer* muons = 0;
	if ( !event->retrieve( muons, muonCollectionName ).isSuccess() ){ // retrieve arguments: container type, container key
	  Error(__PRETTY_FUNCTION__, ("Failed to retrieve "+ muonCollectionName+ " container. Exiting.").c_str() );
	  return EL::StatusCode::FAILURE;
	}

	xAOD::MuonContainer* muons_copy(0);
	xAOD::ShallowAuxContainer* muons_copyaux(0);
	CHECK( m_susy_obj->GetMuons(muons_copy,muons_copyaux) );

	xAOD::MuonContainer::iterator mu_itr = (muons_copy)->begin();
	xAOD::MuonContainer::iterator mu_end  = (muons_copy)->end();

	for( ; mu_itr != mu_end; ++mu_itr ) {
		m_susy_obj->IsSignalMuonExp( **mu_itr,  ST::SignalIsoExp::TightIso ) ;
		m_susy_obj->IsCosmicMuon( **mu_itr );
		//Info(__PRETTY_FUNCTION__, "  Muon passing IsBaseline? %i",(int) (*mu_itr)->auxdata< bool >("baseline") );
	}


	//------------
	// ELECTRONS
	//------------

	const xAOD::ElectronContainer* electrons = 0;

	if ( !event->retrieve( electrons, electronCollectionName).isSuccess() ){ // retrieve arguments: container type, container key
	  Error(__PRETTY_FUNCTION__, ("Failed to retrieve"+ electronCollectionName + " container. Exiting.").c_str()) ;

	  return EL::StatusCode::FAILURE;
	}

	xAOD::ElectronContainer* electrons_copy(0);
	xAOD::ShallowAuxContainer* electrons_copyaux(0);
	CHECK( m_susy_obj->GetElectrons(electrons_copy,electrons_copyaux) );

	// Print their properties, using the tools:
	xAOD::ElectronContainer::iterator el_itr = (electrons_copy)->begin();
	xAOD::ElectronContainer::iterator el_end = (electrons_copy)->end();

	for( ; el_itr != el_end; ++el_itr ) {
		m_susy_obj->IsSignalElectronExp( **el_itr , ST::SignalIsoExp::TightIso);
		//Info( __PRETTY_FUNCTION__, " El passing baseline? %i signal %i",(int) (*el_itr)->auxdata< bool >("baseline"), (int) (*el_itr)->auxdata< bool >("signal") );
	}




	//------------
	// PHOTONS
	//------------

	const xAOD::PhotonContainer* photons = 0;
	if ( !event->retrieve( photons, photonCollectionName ).isSuccess() ){ // retrieve arguments: container type, container key
	  Error(__PRETTY_FUNCTION__, ("Failed to retrieve" + photonCollectionName+ ". Exiting.").c_str() );
	  return EL::StatusCode::FAILURE;
	}

	xAOD::PhotonContainer* photons_copy(0);
	xAOD::ShallowAuxContainer* photons_copyaux(0);
	CHECK( m_susy_obj->GetPhotons(photons_copy,photons_copyaux) );



	//------------
	// JETS
	//------------

	const xAOD::JetContainer* jets = 0;
	if ( !event->retrieve( jets, jetCollectionName ).isSuccess() ){ // retrieve arguments: container type, container key
	  Error(__PRETTY_FUNCTION__, ("Failed to retrieve " + jetCollectionName +  ". Exiting.").c_str());

	  return EL::StatusCode::FAILURE;
	}

	xAOD::JetContainer* jets_copy(0);
	xAOD::ShallowAuxContainer* jets_copyaux(0);
	CHECK( m_susy_obj->GetJets(jets_copy,jets_copyaux) );


	//------------
	// TAUS
	//------------
	const xAOD::TauJetContainer* taus = 0;
	if ( !event->retrieve( taus, tauCollectionName ).isSuccess() ){ // retrieve arguments: container type, container key
	  Error(__PRETTY_FUNCTION__, ("Failed to retrieve"+ tauCollectionName+ "   container. Exiting.").c_str()) ;
	  return EL::StatusCode::FAILURE;
	}

	xAOD::TauJetContainer* taus_copy(0);
	xAOD::ShallowAuxContainer* taus_copyaux(0);
	CHECK( m_susy_obj->GetTaus(taus_copy,taus_copyaux) );


	//------------
	// OVERLAP REMOVAL (as in susytools tester)
	//------------

	CHECK( m_susy_obj->OverlapRemoval(electrons_copy, muons_copy, jets_copy) );


	//------------
	// GET REBUILT MET
	//------------

	xAOD::MissingETContainer*    MET = new xAOD::MissingETContainer;
	xAOD::MissingETAuxContainer* METAux = new xAOD::MissingETAuxContainer;
	MET->setStore(METAux);
	CHECK( m_store->record( MET, metCalibCollectionName ) );
	CHECK( m_store->record( METAux, metCalibCollectionName+"Aux." ) );

	///// TEMPORARY CODE ONLY
	// Protection against bad muons (calo-tagged, si-associated forward)
	// Implemented here due to limitations preventing internal patch
	// This will be made redundant by fixes in MET
	xAOD::MuonContainer muons_copy_met(SG::VIEW_ELEMENTS);
	for(auto mu : *muons_copy) {
	if(mu->muonType()==xAOD::Muon::Combined || mu->muonType()==xAOD::Muon::SegmentTagged || mu->muonType()==xAOD::Muon::MuonStandAlone)
		muons_copy_met.push_back(mu);
	}
	///// TEMPORARY CODE ONLY

	CHECK( m_susy_obj->GetMET(*MET,
			  jets_copy,
			  electrons_copy,
			  &muons_copy_met,
			  photons_copy,
			  taus_copy) );


	//////////////////////////////////////////////////////

	if(m_writeFullCollectionsToxAOD){

		muons_copyaux->setShallowIO( true ); // true = shallow copy, false = deep copy
		if( !event->record( muons_copy ,   muonCalibCollectionName )){return EL::StatusCode::FAILURE;}
		if( !event->record( muons_copyaux, muonCalibCollectionName+"Aux." )) {return EL::StatusCode::FAILURE;}


		electrons_copyaux->setShallowIO( true ); // true = shallow copy, false = deep copy
		if( !event->record( electrons_copy ,   electronCalibCollectionName )){return EL::StatusCode::FAILURE;}
		if( !event->record( electrons_copyaux, electronCalibCollectionName+"Aux." )) {return EL::StatusCode::FAILURE;}

		// photons_copyaux->setShallowIO( true ); // true = shallow copy, false = deep copy
		// if( !event->record( photons_copy ,   "CalibPhotons" )){return EL::StatusCode::FAILURE;}
		// if( !event->record( photons_copyaux, "CalibPhotonsAux." )) {return EL::StatusCode::FAILURE;}

		jets_copyaux->setShallowIO( true ); // true = shallow copy, false = deep copy
	                                       // if true should have something like this line somewhere:
	                                       // event->copy(jetCollectionName);
		if( !event->record( jets_copy , jetCalibCollectionName )){return EL::StatusCode::FAILURE;}
		if( !event->record( jets_copyaux, jetCalibCollectionName+"Aux." )) {return EL::StatusCode::FAILURE;}

		if( !event->record( MET,    metCalibCollectionName )){return EL::StatusCode::FAILURE;}
		if( !event->record( METAux, metCalibCollectionName+"Aux." )) {return EL::StatusCode::FAILURE;}

	}

	muons_copy->setStore(muons_copyaux);
	CHECK( m_store->record( muons_copy, muonCalibCollectionName ) );
	CHECK( m_store->record( muons_copyaux, muonCalibCollectionName+"Aux." ) );

	electrons_copy->setStore(electrons_copyaux);
	CHECK( m_store->record( electrons_copy, electronCalibCollectionName ) );
	CHECK( m_store->record( electrons_copyaux, electronCalibCollectionName+"Aux." ) );

	photons_copy->setStore(photons_copyaux);
	CHECK( m_store->record( photons_copy, photonCalibCollectionName ) );
	CHECK( m_store->record( photons_copyaux, photonCalibCollectionName+"Aux." ) );

	jets_copy->setStore(jets_copyaux);
	CHECK( m_store->record( jets_copy, jetCalibCollectionName ) );
	CHECK( m_store->record( jets_copyaux, jetCalibCollectionName+"Aux." ) );





	////////////////////////////////////////////////////////


	return 0;
}


EL::StatusCode SklimmerAnalysis :: execute ()
{
	// Here you do everything that needs to be done on every single
	// events, e.g. read input variables, apply cuts, and fill
	// histograms and trees.  This is where most of your actual analysis
	// code will go.

	xAOD::TEvent *event = wk()->xaodEvent();

	int passEvent = 1;


	//if(m_doSklimming) copyFullxAODContainers();
        // LH update, should the above instead be this:
        if(m_writeFullCollectionsToxAOD) copyFullxAODContainers();

	//----------------------------
	// Event information
	//---------------------------
	const xAOD::EventInfo* eventInfo = 0;
	if( ! event->retrieve( eventInfo, eventInfoName).isSuccess() ){
	  Error(__PRETTY_FUNCTION__, ("Failed to retrieve " + eventInfoName + ". Exiting.").c_str()) ;
	  return EL::StatusCode::FAILURE;
	}


	int EventNumber = eventInfo->eventNumber();
	int RunNumber = eventInfo->runNumber();
	float EventWeight = eventInfo->mcEventWeight();

	int MCChannelNumber = eventInfo->mcChannelNumber();




	h_nevents->Fill(0.);
	h_nevents_weighted->Fill(0.,EventWeight);

	// stupid sherpa stuff...///////////////////////////////////////////////////////////////////
	if (
		MCChannelNumber == 167740 ||
		MCChannelNumber == 167741 ||
		MCChannelNumber == 167742 ||
		MCChannelNumber == 167743 ||
		MCChannelNumber == 167744 ||
		MCChannelNumber == 167745 ||
		MCChannelNumber == 167746 ||
		MCChannelNumber == 167747 ||
		MCChannelNumber == 167748 ||
		MCChannelNumber == 167749 ||
		MCChannelNumber == 167750 ||
		MCChannelNumber == 167751 ||
		MCChannelNumber == 167752 ||
		MCChannelNumber == 167753 ||
		MCChannelNumber == 167754 ||
		MCChannelNumber == 167755 ||
		MCChannelNumber == 167756 ||
		MCChannelNumber == 167757 ||
		MCChannelNumber == 167758 ||
		MCChannelNumber == 167759 ||
		MCChannelNumber == 167760 ){

		const xAOD::TruthParticleContainer* truthParticles = 0;
		if ( !event->retrieve( truthParticles, truthParticleName  ).isSuccess() ){ // retrieve arguments: container type, container key
			Error(__PRETTY_FUNCTION__, "Failed to retrieve truth container. Exiting." );
			return EL::StatusCode::FAILURE;
		}


		TLorentzVector V;
		TLorentzVector l1;
		TLorentzVector l2;

		bool foundFirst  = false;
		bool foundSecond = false;
		bool foundMore   = false;



		for (xAOD::TruthParticleContainer::const_iterator tpi = truthParticles->begin(); tpi != truthParticles->end(); ++tpi) {
		  // const TruthParticle* p = *tpi;
		  // In general for Sherpa,  you have to select particles with status==3 and barcode<100,000 to get get the Matrix element in and out
		  if ( ((*tpi)->status() == 3) && (fabs( (*tpi)->pdgId() ) >= 11) && (fabs( (*tpi)->pdgId() ) <= 16) && ((*tpi)->barcode() < 100000) ) {
		    if ( !foundFirst ) {
		      l1.SetPtEtaPhiM( (*tpi)->pt(), (*tpi)->eta(), (*tpi)->phi(), (*tpi)->m() );
		      foundFirst = true;
		    } else if ( !foundSecond ) {
		      l2.SetPtEtaPhiM( (*tpi)->pt(), (*tpi)->eta(), (*tpi)->phi(), (*tpi)->m() );
		      foundSecond = true;
		    } else {
		      foundMore = true;
		      break;
		    }
		  }
		}

		if ( !foundSecond )
		  std::cout << "doSherpaPtFilterCheck: Unable to find 2 leptons" << std::endl;
		else if ( foundMore )
		  std::cout << "doSherpaPtFilterCheck: Found more than 2 leptons" << std::endl;
		else {
		  V = l1 + l2;
		  // m_h_sherpaPt->Fill( V.Pt() / GeV, m_eventWeight );

	  		if(V.Pt()>70000.) return EL::StatusCode::SUCCESS;

		}

	}
	// stupid sherpa stuff...///////////////////////////////////////////////////////////////////




	// check if the event is data or MC
	// (many tools are applied either to data or MC)
	//todo we
	bool isMC = false;
	// check if the event is MC
	if(eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) ){
	   isMC = true; // can do something with this later
	}


	// if data, check if event passes GRL ////////////////////////////////////////////////

	if(!isMC){ // it's data!
		if(!m_grl->passRunLB(*eventInfo)){
			return EL::StatusCode::SUCCESS; // go to next event
		}
		if(  (eventInfo->errorState(xAOD::EventInfo::LAr)==xAOD::EventInfo::Error ) ||
			(eventInfo->errorState(xAOD::EventInfo::Tile)==xAOD::EventInfo::Error ) ||
			(eventInfo->isEventFlagBitSet(xAOD::EventInfo::Core, 18) )  )
		{
			return EL::StatusCode::SUCCESS; // go to the next event
		} // end if event flags check
	} // end if not MC

	if(isMC){
		// Check if input file is mc14_13TeV to skip pileup reweighting
		bool mc14_13TeV = false;
		if( RunNumber == 222222) mc14_13TeV = true;
		if (!mc14_13TeV){ // Only reweight 8 TeV MC
			CHECK( m_pileupReweightingTool->execute() );
			//Info( __PRETTY_FUNCTION__,"PileupReweightingTool: PileupWeight %f RandomRunNumber %i RandomLumiBlockNumber %i",eventInfo->auxdata< double >("PileupWeight"), eventInfo->auxdata< unsigned int >("RandomRunNumber"),  eventInfo->auxdata< unsigned int >("RandomLumiBlockNumber") );
		}
	}// end if IS MC

	// Let's calibrate

	if(m_doSUSYObjDef) applySUSYObjectDefinitions();
	else putStuffInStore();//todo this can't be used yet?



	std::pair< xAOD::EventInfo*, xAOD::ShallowAuxInfo* > eventInfo_shallowCopy = xAOD::shallowCopyObject( *eventInfo );
	if( !m_store->record( eventInfo_shallowCopy.first , myEventInfoName )){return EL::StatusCode::FAILURE;}
	if( !m_store->record( eventInfo_shallowCopy.second, myEventInfoName+"Aux." )) {return EL::StatusCode::FAILURE;}


	eventInfo_shallowCopy.second->setShallowIO(true);

	if( !event->record( eventInfo_shallowCopy.first , myEventInfoName )){return EL::StatusCode::FAILURE;}
	if( !event->record( eventInfo_shallowCopy.second, myEventInfoName+"Aux." )) {return EL::StatusCode::FAILURE;}

	// m_store->print();



	if( m_doEventSelection && m_Analysis=="bbmet" ){
		TString result = eventSelectionBBMet(eventInfo_shallowCopy.first );
		(eventInfo_shallowCopy.first)->auxdecor< char >("selection") = *result.Data();
		//if(result=="") return EL::StatusCode::SUCCESS;
	}

	if( m_Analysis=="triggerTurnOnCurves"){
	  TString result = eventSelectionRazorTrigger(eventInfo_shallowCopy.first );
	  (eventInfo_shallowCopy.first)->auxdecor< char >("selection") = *result.Data();

	  addTrigDecisionInfo(eventInfo_shallowCopy.first );
	}

	//Info( __PRETTY_FUNCTION__,"About to access eventInfo "  );

	//Info( __PRETTY_FUNCTION__,"RJigsaw Variables: sHatR %f",
	//	(eventInfo_shallowCopy.first)->auxdata< float >("sHatR")  );
	//Info( __PRETTY_FUNCTION__,"RJigsaw Variables: gammainv_Rp1 %f",
	//	(eventInfo_shallowCopy.first)->auxdata< float >("gammainv_Rp1") );

	// m_store->clear();

	//Info( __PRETTY_FUNCTION__,"About to write to xAOD "  );

	if(m_writexAOD){
		// Save the event:
		CHECK(event->fill()); // Trying to fill the output xAOD causes problems right now...
	}

	//Info( __PRETTY_FUNCTION__,"leaving execute "  );


	return EL::StatusCode::SUCCESS;
}

EL::StatusCode SklimmerAnalysis :: addTrigDecisionInfo ( xAOD::EventInfo * eventInfo)
{
  std::bitset<32> triggers;

  //todo make this list configurable
  triggers[0] = m_trigDecisionTool->isPassed("L1_XE50");
  triggers[1] = m_trigDecisionTool->isPassed("L1_XE70");
  triggers[2] = m_trigDecisionTool->isPassed("HLT_xe70");
  triggers[3] = m_trigDecisionTool->isPassed("HLT_xe70_pueta");
  triggers[4] = m_trigDecisionTool->isPassed("HLT_xe100");
  triggers[5] = m_trigDecisionTool->isPassed("HLT_xe100_pueta");
  triggers[6] = m_trigDecisionTool->isPassed("HLT_e28_tight_iloose");
  triggers[7] = m_trigDecisionTool->isPassed("HLT_e60_medium");
  triggers[8] = m_trigDecisionTool->isPassed("HLT_mu26_imedium");
  triggers[9] = m_trigDecisionTool->isPassed("HLT_mu50");
  triggers[10] = m_trigDecisionTool->isPassed("HLT_j30_xe10_razor170");
  triggers[11] = m_trigDecisionTool->isPassed("HLT_xe70_tc_em");
  triggers[12] = m_trigDecisionTool->isPassed("HLT_xe70_tc_lcw");
  triggers[13] = m_trigDecisionTool->isPassed("HLT_xe70_mht");
  triggers[14] = m_trigDecisionTool->isPassed("HLT_xe70_pufit");
  triggers[15] = m_trigDecisionTool->isPassed("HLT_xe100_tc_em");
  triggers[16] = m_trigDecisionTool->isPassed("HLT_xe100_tc_lcw");
  triggers[17] = m_trigDecisionTool->isPassed("HLT_xe100_mht");
  triggers[18] = m_trigDecisionTool->isPassed("HLT_xe100_pufit");

  int const triggerSet = triggers.to_ulong();

  eventInfo->auxdecor<int>("triggerBitset") = triggerSet;

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode SklimmerAnalysis :: postExecute ()
{
	// Here you do everything that needs to be done after the main event
	// processing.  This is typically very rare, particularly in user
	// code.  It is mainly used in implementing the NTupleSvc.
	return EL::StatusCode::SUCCESS;
}



EL::StatusCode SklimmerAnalysis :: finalize ()
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

	xAOD::TEvent *event = wk()->xaodEvent();

	// GRL
	if( m_grl ) {
		delete m_grl;
		m_grl = 0;
	}

	// Pileup_Reweighting
	if( m_pileupReweightingTool ) {
		delete m_pileupReweightingTool;
		m_pileupReweightingTool = 0;
	}


	delete m_susy_obj;
	delete RJTool;
	delete m_trigDecisionTool;
	delete m_trigConfigTool;

	// finalize and close our output xAOD file:

	if(m_writexAOD){

		TFile *file = wk()->getOutputFile (outputxAODName);
		CHECK(event->finishWritingTo( file ));
	}
	return EL::StatusCode::SUCCESS;
}



EL::StatusCode SklimmerAnalysis :: histFinalize ()
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

TString SklimmerAnalysis :: eventSelectionRazorTrigger(xAOD::EventInfo * eventInfo ) {

  if(eventInfo == nullptr ){
    Error(__PRETTY_FUNCTION__, "can't do razor trigger eventSelection without eventInfo object" ) ;
    return TString("");
  }

  RazorTriggerAnalysis evtSelection(m_store);//todo should probably be a tool owned up the analysis class? we wouldn't need this silly store pass
  evtSelection.jetCalibCollectionName      = jetCalibCollectionName;
  evtSelection.muonCalibCollectionName     = muonCalibCollectionName;
  evtSelection.electronCalibCollectionName = electronCalibCollectionName;
  evtSelection.metCalibCollectionName      = metCalibCollectionName;

  TString selectionResult(evtSelection.run(eventInfo));

  //todo get mdeltaR out and check if the event passed using the TrigDecisionTool

  return selectionResult;
}

TString SklimmerAnalysis :: eventSelectionBBMet(xAOD::EventInfo * eventInfo )
{
  if(eventInfo == nullptr ){
    Error(__PRETTY_FUNCTION__, "can't do bbMET eventSelection without eventInfo object" ) ;
    return TString("");
  }

  EventSelectionBBMet evtSelection(m_store);//todo should probably be a tool owned up the analysis class? we wouldn't need this silly store pass
  evtSelection.jetCalibCollectionName      = jetCalibCollectionName;
  evtSelection.muonCalibCollectionName     = muonCalibCollectionName;
  evtSelection.electronCalibCollectionName = electronCalibCollectionName;
  evtSelection.metCalibCollectionName      = metCalibCollectionName;

  return TString( evtSelection.run(eventInfo));//todo check this is okay.  All the RJ variables are decorators to eventInfo (I think), so it should be? but check

}

EL::StatusCode SklimmerAnalysis :: fillEmptyCollectionNames (){
  if(eventInfoName.empty())       eventInfoName       = "EventInfo";
  if(primaryVerticesName.empty()) primaryVerticesName = "PrimaryVertices";

  if(muonCollectionName.empty())     muonCollectionName     = "Muons";
  if(electronCollectionName.empty()) electronCollectionName = "ElectronCollection";
  if(photonCollectionName.empty())   photonCollectionName   = "PhotonCollection";
  if(jetCollectionName.empty())      jetCollectionName      = "AntiKt4LCTopoJets";
  if(metCollectionName.empty())      metCollectionName      = "MET_RefFinal";
  if(tauCollectionName.empty())      tauCollectionName      = "TauRecContainer";

  if(myEventInfoName.empty())  myEventInfoName = "MyEventInfo";

  if(muonCalibCollectionName.empty())     muonCalibCollectionName     = "CalibMuons";
  if(electronCalibCollectionName.empty()) electronCalibCollectionName = "CalibElectrons";
  if(photonCalibCollectionName.empty())   photonCalibCollectionName   = "CalibPhotons";
  if(jetCalibCollectionName.empty())      jetCalibCollectionName      = "CalibJets";
  if(metCalibCollectionName.empty())      metCalibCollectionName      = "CalibMET_RefFinal";
  if(tauCalibCollectionName.empty())      tauCalibCollectionName      = "CalibTaus";

  if(truthEventName.empty())           truthEventName           = "TruthEvent";
  if(truthParticleName.empty())        truthParticleName        = "TruthParticle";
  if(truthJetCollectionName.empty())   truthJetCollectionName   = "AntiKt4TruthJets";
  if(truthMetCollectionName.empty())   truthMetCollectionName   = "Truth_MET";
  if(truthPrimaryVerticesName.empty()) truthPrimaryVerticesName = "TruthVertex";

  return EL::StatusCode::SUCCESS;
}








//  LocalWords:  xaodEvent
