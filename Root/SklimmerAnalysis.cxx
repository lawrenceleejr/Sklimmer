#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <Sklimmer/SklimmerAnalysis.h>


#include <TLorentzVector.h>
#include <TVector3.h>

#include <cstdlib>
#include <string>

#include "EventLoop/OutputStream.h"


#include "RestFrames/RestFrame.hh"
#include "RestFrames/RFrame.hh"
#include "RestFrames/RLabFrame.hh"
#include "RestFrames/RDecayFrame.hh"
#include "RestFrames/RVisibleFrame.hh"
#include "RestFrames/RInvisibleFrame.hh"
#include "RestFrames/RSelfAssemblingFrame.hh"
#include "RestFrames/InvisibleMassJigsaw.hh"
#include "RestFrames/InvisibleRapidityJigsaw.hh"
#include "RestFrames/ContraBoostInvariantJigsaw.hh"
#include "RestFrames/MinimizeMassesCombinatoricJigsaw.hh"
#include "RestFrames/InvisibleGroup.hh"
#include "RestFrames/CombinatoricGroup.hh"
#include "RestFrames/FramePlot.hh"



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

#include "CPAnalysisExamples/errorcheck.h"
#include "SUSYTools/SUSYObjDef_xAOD.h"


//Still really need to implement a systematics framework

// this is needed to distribute the algorithm to the workers
ClassImp(SklimmerAnalysis)



SklimmerAnalysis :: SklimmerAnalysis()
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
	EL::OutputStream out ("outputxAOD");
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

	const char* APP_NAME = "SklimmerAnalysis";



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

    m_event = wk()->xaodEvent();
    m_store = wk()->xaodStore();

	// as a check, let's see the number of events in our xAOD
	Info("initialize()", "Number of events = %lli", m_event->getEntries() ); // print long long int

	// Output xAOD ///////////////////////////////////////////////////////////////////

	if(m_writexAOD){
		TFile *file = wk()->getOutputFile ("outputxAOD");
		CHECK(m_event->writeTo(file));
	}

	// SUSYTools ///////////////////////////////////////////////////////////////////

	if(m_doSUSYObjDef){
		m_susy_obj = new ST::SUSYObjDef_xAOD( "SUSYObjDef_xAOD" );

		CHECK( m_susy_obj->setProperty("IsData",isData) );
		CHECK( m_susy_obj->setProperty("IsAtlfast",isAtlfast) );
		CHECK( m_susy_obj->setProperty("EleId","Tight") );

		if( m_susy_obj->SUSYToolsInit().isFailure() ) {
		  Error( APP_NAME, "Failed to initialise tools in SUSYToolsInit()..." );
		  Error( APP_NAME, "Exiting..." );
		  return EL::StatusCode::FAILURE;   
		}
		else{ Info(APP_NAME,"SUSYToolsInit with success!!... " );}

		if( m_susy_obj->initialize() != StatusCode::SUCCESS){
		  Error(APP_NAME, "Cannot intialize SUSYObjDef_xAOD..." );
		  Error(APP_NAME, "Exiting... " );
		  return EL::StatusCode::FAILURE;
		}else{
		  Info(APP_NAME,"SUSYObjDef_xAOD initialized... " );
		}
	}

	// GRL ///////////////////////////////////////////////////////////////////

	m_grl = new GoodRunsListSelectionTool("GoodRunsListSelectionTool");
	std::vector<std::string> vecStringGRL;
	vecStringGRL.push_back(gSystem->ExpandPathName("$ROOTCOREBIN/data/SUSYTools/GRL/Summer2013/data12_8TeV.periodAllYear_DetStatus-v61-pro14-02_DQDefects-00-01-00_PHYS_StandardGRL_All_Good.xml"));
	CHECK( m_grl->setProperty( "GoodRunsListVec", vecStringGRL) );
	CHECK( m_grl->setProperty("PassThrough", false) ); // if true (default) will ignore result of GRL and will just pass all events
	if (!m_grl->initialize().isSuccess()) { // check this isSuccess
		Error(APP_NAME, "Failed to properly initialize the GRL. Exiting." );
		return EL::StatusCode::FAILURE;
	}

	// Pile Up Reweighting ///////////////////////////////////////////////////////////////////

	m_pileupReweightingTool= new PileupReweightingTool("PileupReweightingTool");   
	CHECK( m_pileupReweightingTool->setProperty("Input","EventInfo") );
	std::vector<std::string> prwFiles;   
	prwFiles.push_back("PileupReweighting/mc14v1_defaults.prw.root");   
	CHECK( m_pileupReweightingTool->setProperty("ConfigFiles",prwFiles) );   
	std::vector<std::string> lumicalcFiles; 
	lumicalcFiles.push_back("SUSYTools/susy_data12_avgintperbx.root");
	CHECK( m_pileupReweightingTool->setProperty("LumiCalcFiles",lumicalcFiles) );
	if(!m_pileupReweightingTool->initialize().isSuccess()){
		Error(APP_NAME, "Failed to properly initialize the Pile Up Reweighting. Exiting." );
		return EL::StatusCode::FAILURE;
	}


	// std::cout << "Leaving SklimmerAnalysis :: initialize ()"  << std::endl;

	return EL::StatusCode::SUCCESS;
}


int SklimmerAnalysis :: copyFullxAODContainers ()
{

	const char* APP_NAME = "SklimmerAnalysis";


	// copy full container(s) to new xAOD
	// without modifying the contents of it: 
        CHECK(m_event->copy("EventInfo"));
	CHECK(m_event->copy("TruthEvent"));

	CHECK(m_event->copy("TruthParticle"));

	CHECK(m_event->copy("AntiKt4LCTopoJets"));
	CHECK(m_event->copy("AntiKt4TruthJets"));

	CHECK(m_event->copy("MET_RefFinal"));
	CHECK(m_event->copy("MET_Truth"));

	CHECK(m_event->copy("TruthVertex"));
	CHECK(m_event->copy("PrimaryVertices"));
	
	CHECK(m_event->copy("ElectronCollection"));
	CHECK(m_event->copy("Muons"));
	// CHECK(m_event->copy("PhotonCollection"));


	return 0;

}



int SklimmerAnalysis :: applySUSYObjectDefinitions (){

	const char* APP_NAME = "SklimmerAnalysis";

	//------------
	// MUONS
	//------------

	const xAOD::MuonContainer* muons = 0;
	if ( !m_event->retrieve( muons, "Muons" ).isSuccess() ){ // retrieve arguments: container type, container key
		Error(APP_NAME, "Failed to retrieve Muons container. Exiting." );
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
		//Info(APP_NAME, "  Muon passing IsBaseline? %i",(int) (*mu_itr)->auxdata< char >("baseline") );
	}


	//------------
	// ELECTRONS
	//------------

	const xAOD::ElectronContainer* electrons = 0;
	if ( !m_event->retrieve( electrons, "ElectronCollection" ).isSuccess() ){ // retrieve arguments: container type, container key
		Error(APP_NAME, "Failed to retrieve Electrons container. Exiting." );
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
		//Info( APP_NAME, " El passing baseline? %i signal %i",(int) (*el_itr)->auxdata< char >("baseline"), (int) (*el_itr)->auxdata< bool >("signal") );
	}




	//------------
	// PHOTONS
	//------------

	const xAOD::PhotonContainer* photons = 0;
	if ( !m_event->retrieve( photons, "PhotonCollection" ).isSuccess() ){ // retrieve arguments: container type, container key
		Error(APP_NAME, "Failed to retrieve Photons container. Exiting." );
		return EL::StatusCode::FAILURE;
	}
	  
	xAOD::PhotonContainer* photons_copy(0);
	xAOD::ShallowAuxContainer* photons_copyaux(0);
	CHECK( m_susy_obj->GetPhotons(photons_copy,photons_copyaux) );



	//------------
	// JETS
	//------------

	const xAOD::JetContainer* jets = 0;
	if ( !m_event->retrieve( jets, "AntiKt4LCTopoJets" ).isSuccess() ){ // retrieve arguments: container type, container key
		Error(APP_NAME, "Failed to retrieve AntiKt4LCTopoJets container. Exiting." );
		return EL::StatusCode::FAILURE;
	}

	xAOD::JetContainer* jets_copy(0);
	xAOD::ShallowAuxContainer* jets_copyaux(0);
	CHECK( m_susy_obj->GetJets(jets_copy,jets_copyaux) );


	//------------
	// TAUS
	//------------
	const xAOD::TauJetContainer* taus = 0;
	if ( !m_event->retrieve( taus, "TauRecContainer" ).isSuccess() ){ // retrieve arguments: container type, container key
		Error(APP_NAME, "Failed to retrieve Taus container. Exiting." );
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
	CHECK( m_store->record( MET, "CalibMET_RefFinal" ) );
	CHECK( m_store->record( METAux, "CalibMET_RefFinalAux." ) );

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
		if( !m_event->record( muons_copy ,   "CalibMuons" )){return EL::StatusCode::FAILURE;}
		if( !m_event->record( muons_copyaux, "CalibMuonsAux." )) {return EL::StatusCode::FAILURE;}


		electrons_copyaux->setShallowIO( true ); // true = shallow copy, false = deep copy
		if( !m_event->record( electrons_copy ,   "CalibElectrons" )){return EL::StatusCode::FAILURE;}
		if( !m_event->record( electrons_copyaux, "CalibElectronsAux." )) {return EL::StatusCode::FAILURE;}

		// photons_copyaux->setShallowIO( true ); // true = shallow copy, false = deep copy
		// if( !m_event->record( photons_copy ,   "CalibPhotons" )){return EL::StatusCode::FAILURE;}
		// if( !m_event->record( photons_copyaux, "CalibPhotonsAux." )) {return EL::StatusCode::FAILURE;}

		jets_copyaux->setShallowIO( true ); // true = shallow copy, false = deep copy
	                                       // if true should have something like this line somewhere:
	                                       // m_event->copy("AntiKt4LCTopoJets");
		if( !m_event->record( jets_copy , "CalibJets" )){return EL::StatusCode::FAILURE;}
		if( !m_event->record( jets_copyaux, "CalibJetsAux." )) {return EL::StatusCode::FAILURE;}

		if( !m_event->record( MET,    "CalibMET" )){return EL::StatusCode::FAILURE;}
		if( !m_event->record( METAux, "CalibMETAux." )) {return EL::StatusCode::FAILURE;}

	} 

	muons_copy->setStore(muons_copyaux);
	CHECK( m_store->record( muons_copy, "CalibMuons" ) );
	CHECK( m_store->record( muons_copyaux, "CalibMuonsAux." ) );

	electrons_copy->setStore(electrons_copyaux);
	CHECK( m_store->record( electrons_copy, "CalibElectrons" ) );
	CHECK( m_store->record( electrons_copyaux, "CalibElectronsAux." ) );
	
	photons_copy->setStore(photons_copyaux);
	CHECK( m_store->record( photons_copy, "CalibPhotons" ) );
	CHECK( m_store->record( photons_copyaux, "CalibPhotonsAux." ) );

	jets_copy->setStore(jets_copyaux);
	CHECK( m_store->record( jets_copy, "CalibJets" ) );
	CHECK( m_store->record( jets_copyaux, "CalibJetsAux." ) );





	////////////////////////////////////////////////////////


	return 0;

}


EL::StatusCode SklimmerAnalysis :: execute ()
{
	// Here you do everything that needs to be done on every single
	// events, e.g. read input variables, apply cuts, and fill
	// histograms and trees.  This is where most of your actual analysis
	// code will go.

	const char* APP_NAME = "SklimmerAnalysis";



	int passEvent = 1;


	//if(m_doSklimming) copyFullxAODContainers();
        // LH update, should the above instead be this:
        if(m_writeFullCollectionsToxAOD) copyFullxAODContainers();

	//----------------------------
	// Event information
	//--------------------------- 
	const xAOD::EventInfo* eventInfo = 0;
	if( ! m_event->retrieve( eventInfo, "EventInfo").isSuccess() ){
		Error(APP_NAME, "Failed to retrieve event info collection. Exiting." );
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
		if ( !m_event->retrieve( truthParticles, "TruthParticle"  ).isSuccess() ){ // retrieve arguments: container type, container key
			Error(APP_NAME, "Failed to retrieve truth container. Exiting." );
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
			//Info( APP_NAME,"PileupReweightingTool: PileupWeight %f RandomRunNumber %i RandomLumiBlockNumber %i",eventInfo->auxdata< double >("PileupWeight"), eventInfo->auxdata< unsigned int >("RandomRunNumber"),  eventInfo->auxdata< unsigned int >("RandomLumiBlockNumber") );
		}
	}// end if IS MC

	// Let's calibrate 

	if(m_doSUSYObjDef) applySUSYObjectDefinitions();
	else putStuffInStore();



	std::pair< xAOD::EventInfo*, xAOD::ShallowAuxInfo* > eventInfo_shallowCopy = xAOD::shallowCopyObject( *eventInfo );
	if( !m_store->record( eventInfo_shallowCopy.first , "myEventInfo" )){return EL::StatusCode::FAILURE;}
	if( !m_store->record( eventInfo_shallowCopy.second, "myEventInfoAux." )) {return EL::StatusCode::FAILURE;}


	eventInfo_shallowCopy.second->setShallowIO(true);
        if(m_writeFullCollectionsToxAOD){
	    if( !m_event->record( eventInfo_shallowCopy.first , "myEventInfo" )){return EL::StatusCode::FAILURE;}
	    if( !m_event->record( eventInfo_shallowCopy.second, "myEventInfoAux." )) {return EL::StatusCode::FAILURE;}
        }


	// m_store->print();



	if( m_doEventSelection && m_Analysis=="bbmet" ){
		TString result = eventSelectionBBMet();
		(eventInfo_shallowCopy.first)->auxdecor< char >("selection") = *result.Data();
		//if(result=="") return EL::StatusCode::SUCCESS;
	}

	//Info( APP_NAME,"About to access eventInfo "  );

	//Info( APP_NAME,"RJigsaw Variables: sHatR %f",
	//	(eventInfo_shallowCopy.first)->auxdata< float >("sHatR")  );
	//Info( APP_NAME,"RJigsaw Variables: gammainv_Rp1 %f",
	//	(eventInfo_shallowCopy.first)->auxdata< float >("gammainv_Rp1") );

	// m_store->clear(); 

	//Info( APP_NAME,"About to write to xAOD "  );

	if(m_writexAOD){
		// Save the event:
		m_event->fill();
	}

	//Info( APP_NAME,"leaving execute "  );


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

	const char* APP_NAME = "SklimmerAnalysis";


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

	// finalize and close our output xAOD file:

	if(m_writexAOD){
		TFile *file = wk()->getOutputFile ("outputxAOD");
		CHECK(m_event->finishWritingTo( file ));
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



TString SklimmerAnalysis :: eventSelectionBBMet()
{

	const char* APP_NAME = "SklimmerAnalysis";


	// Inspired by https://cds.cern.ch/record/1508045/files/ATL-COM-PHYS-2013-072.pdf

	xAOD::JetContainer* jets_copy(0);
	CHECK( m_store->retrieve( jets_copy, "CalibJets" ) );

	xAOD::MuonContainer* muons_copy(0);
	CHECK( m_store->retrieve( muons_copy, "CalibMuons" ) );

	xAOD::ElectronContainer* electrons_copy(0);
	CHECK( m_store->retrieve( electrons_copy, "CalibElectrons" ) );


	xAOD::EventInfo* eventInfo = 0;
	CHECK(m_store->retrieve(eventInfo, "myEventInfo"));

	/////////////// Lepton Veto //////////////////////////////

	int Nel=0;
	xAOD::ElectronContainer::iterator el_itr = electrons_copy->begin();
	xAOD::ElectronContainer::iterator el_end = electrons_copy->end();
	for( ; el_itr != el_end; ++el_itr ) {
		if( ( *el_itr )->auxdata<char>("passOR") ) Nel++;
	}
	
	int Nmu=0;
	xAOD::MuonContainer::iterator mu_itr = muons_copy->begin();
	xAOD::MuonContainer::iterator mu_end = muons_copy->end();
	for( ; mu_itr != mu_end; ++mu_itr ) {
		if( ( *mu_itr )->auxdata<char>("passOR") ) Nmu++;
	}

	if(Nel || Nmu) return "";

	///////////////////////////////////////////////////////////


	xAOD::JetContainer* goodJets = new xAOD::JetContainer(SG::VIEW_ELEMENTS);
	// CHECK( m_store->record(goodJets, "MySelJets") );

	xAOD::JetContainer::iterator jet_itr = (jets_copy)->begin();
	xAOD::JetContainer::iterator jet_end = (jets_copy)->end();

	for( ; jet_itr != jet_end; ++jet_itr ) {

		if( (*jet_itr)->auxdata< char >("baseline")==1  &&
			(*jet_itr)->auxdata< char >("passOR")==1  &&
			(*jet_itr)->pt() > 30000.  && ( fabs( (*jet_itr)->eta()) < 2.8) ) {
			goodJets->push_back (*jet_itr); 
		}

		(*jet_itr)->auxdecor<float>("MV1") =  ((*jet_itr)->btagging())->MV1_discriminant() ;
    
    }

    if(goodJets->size() < 2){


		// eventInfo->auxdecor<float>("SS_Mass"         ) = 0.;
		// eventInfo->auxdecor<float>("SS_Phi"          ) = 0.;
		// eventInfo->auxdecor<float>("SS_CosTheta"     ) = 0.;
		// eventInfo->auxdecor<float>("S1_Mass"         ) = 0.;
		// eventInfo->auxdecor<float>("S1_Phi"          ) = 0.;
		// eventInfo->auxdecor<float>("S1_CosTheta"     ) = 0.;
		// eventInfo->auxdecor<float>("S2_Mass"         ) = 0.;
		// eventInfo->auxdecor<float>("S2_Phi"          ) = 0.;
		// eventInfo->auxdecor<float>("S2_CosTheta"     ) = 0.;
		// eventInfo->auxdecor<float>("I1_Depth"        ) = 0.;
		// eventInfo->auxdecor<float>("I2_Depth"        ) = 0.;
		// eventInfo->auxdecor<float>("V1_N"            ) = 0.;
		// eventInfo->auxdecor<float>("V2_N"            ) = 0.;

		return "";

    } 

	std::sort(goodJets->begin(), goodJets->end(),
    [](xAOD::Jet  *a, xAOD::Jet  *b){return a->pt() > b->pt();});


	// Set up RestFrames topology ///////////////////////////////////

	RestFrames::RLabFrame LAB("LAB","lab");
	RestFrames::RDecayFrame SS("SS","SS");
	RestFrames::RSelfAssemblingFrame S1("S1","#tilde{S}_{a}");
	RestFrames::RSelfAssemblingFrame S2("S2","#tilde{S}_{b}");
	RestFrames::RVisibleFrame V1("V1","V_{a}");
	RestFrames::RVisibleFrame V2("V2","V_{b}");
	RestFrames::RInvisibleFrame I1("I1","I_{a}");
	RestFrames::RInvisibleFrame I2("I2","I_{b}");

	// The invisible group is all of the weakly interacting particles
	RestFrames::InvisibleGroup INV("INV","Invisible State Jigsaws");
	INV.AddFrame(I1);
	INV.AddFrame(I2);

	// the combinatoric group is the list of visible objects
	// that go into our hemispheres 
	RestFrames::CombinatoricGroup VIS("VIS","Visible Object Jigsaws");
	VIS.AddFrame(V1);
	VIS.SetNElementsForFrame(V1,1,false);
	VIS.AddFrame(V2);
	VIS.SetNElementsForFrame(V2,1,false);

	LAB.SetChildFrame(SS);

	SS.AddChildFrame(S1);
	SS.AddChildFrame(S2);

	S1.AddChildFrame(V1);
	S1.AddChildFrame(I1);
	S2.AddChildFrame(V2);
	S2.AddChildFrame(I2);

	std::cout << "Is consistent tree topology? " << LAB.InitializeTree() << std::endl; 


	// MT2 etc
 
	//////////////////////////////////////////////////////////////
	// now we define 'jigsaw rules' that tell the tree
	// how to define the objects in our groups 
	//////////////////////////////////////////////////////////////
	RestFrames::InvisibleMassJigsaw MinMassJigsaw("MINMASS_JIGSAW", "Invisible system mass Jigsaw");
	INV.AddJigsaw(MinMassJigsaw);

	RestFrames::InvisibleRapidityJigsaw RapidityJigsaw("RAPIDITY_JIGSAW", "Invisible system rapidity Jigsaw");
	INV.AddJigsaw(RapidityJigsaw);
	RapidityJigsaw.AddVisibleFrame((LAB.GetListVisibleFrames()));

	RestFrames::ContraBoostInvariantJigsaw ContraBoostJigsaw("CB_JIGSAW","Contraboost invariant Jigsaw");
	INV.AddJigsaw(ContraBoostJigsaw);
	ContraBoostJigsaw.AddVisibleFrame((S1.GetListVisibleFrames()), 0);
	ContraBoostJigsaw.AddVisibleFrame((S2.GetListVisibleFrames()), 1);
	ContraBoostJigsaw.AddInvisibleFrame((S1.GetListInvisibleFrames()), 0);
	ContraBoostJigsaw.AddInvisibleFrame((S2.GetListInvisibleFrames()), 1);

	RestFrames::MinimizeMassesCombinatoricJigsaw HemiJigsaw("HEM_JIGSAW","Minimize m _{V_{a,b}} Jigsaw");
	VIS.AddJigsaw(HemiJigsaw);
	HemiJigsaw.AddFrame(V1,0);
	HemiJigsaw.AddFrame(V2,1);

	//////////////////////////////////////////////////////////////
	// check to make sure that all the jigsaws etc. are correctly connected
	//////////////////////////////////////////////////////////////
	std::cout << "Is consistent analysis tree? : " << LAB.InitializeAnalysis() << std::endl; 

	LAB.ClearEvent();


	//////////////////////////////////////////////////////////////
	// Now, we make a 'background'-like, transverse momentum only, tree
	//////////////////////////////////////////////////////////////
	RestFrames::RLabFrame LAB_alt("LAB","lab");
	RestFrames::RSelfAssemblingFrame S_alt("CM","CM");
	RestFrames::RVisibleFrame V_alt("V_alt","Vis");
	RestFrames::RInvisibleFrame I_alt("I_alt","Iinv");

	RestFrames::InvisibleGroup INV_alt("INV_alt","Invisible State Jigsaws");
	INV_alt.AddFrame(I_alt);

	RestFrames::CombinatoricGroup VIS_alt("VIS_alt","Visible Object Jigsaws");
	VIS_alt.AddFrame(V_alt);
	VIS_alt.SetNElementsForFrame(V_alt,1,false);

	LAB_alt.SetChildFrame(S_alt);
	S_alt.AddChildFrame(V_alt);
	S_alt.AddChildFrame(I_alt);

	LAB_alt.InitializeTree(); 

	// Will just set invisible mass to zero
	RestFrames::InvisibleMassJigsaw MinMass_alt("MINMASS_JIGSAW_ALT", "Invisible system mass Jigsaw");
	INV_alt.AddJigsaw(MinMass_alt);

	// will set rapidity to zero
	RestFrames::InvisibleRapidityJigsaw Rapidity_alt("RAPIDITY_JIGSAW_ALT", "Invisible system rapidity Jigsaw");
	INV_alt.AddJigsaw(Rapidity_alt);
	Rapidity_alt.AddVisibleFrame((LAB_alt.GetListVisibleFrames()));

	LAB_alt.InitializeAnalysis(); 




	TLorentzVector jet;

	jet_itr = (jets_copy)->begin();
	for( ; jet_itr != jet_end; ++jet_itr ) {

		if( (*jet_itr)->auxdata< char >("baseline")==1  &&
			(*jet_itr)->auxdata< char >("passOR")==1  &&
			(*jet_itr)->pt() > 30000.  && ( fabs( (*jet_itr)->eta()) < 2.8) ) {
			VIS.AddLabFrameFourVector( (*jet_itr)->p4()  );  
			
			jet.SetPtEtaPhiM( (*jet_itr)->pt(), 0., (*jet_itr)->phi(), (*jet_itr)->m()  );
			VIS_alt.AddLabFrameFourVector(jet);
		}
    
    }


	// Get MET Collection to hand to Rest Frames////////////////////////////////////////////////////

	xAOD::MissingETContainer* MET = new xAOD::MissingETContainer;
	CHECK( m_store->retrieve( MET, "CalibMET_RefFinal" ) );

	TVector3 MET_TV3;

    xAOD::MissingETContainer::const_iterator met_it = MET->find("Final");
	if (met_it == MET->end()) {
		Error( APP_NAME, "No RefFinal inside MET container" );
	} else {
		INV.SetLabFrameThreeVector(  TVector3( (*met_it)->mpx(), (*met_it)->mpy(), 0 ) );
		MET_TV3.SetZ(0.);
		MET_TV3.SetX((*met_it)->mpx() );
		MET_TV3.SetY((*met_it)->mpy() );
	}

	LAB.AnalyzeEvent();

	
    INV_alt.SetLabFrameThreeVector(MET_TV3);
    LAB_alt.AnalyzeEvent();

	//std::cout << "RestFrames shatR is: " << SS.GetMass() << std::endl;

	eventInfo->auxdecor<float>("SS_Mass"           ) = SS.GetMass();
	eventInfo->auxdecor<float>("SS_InvGamma"       ) = 1./SS.GetGammaInParentFrame();
	eventInfo->auxdecor<float>("SS_dPhiBetaR"      ) = SS.GetDeltaPhiBoostVisible();
	eventInfo->auxdecor<float>("SS_dPhiVis"        ) = SS.GetDeltaPhiVisible();
	eventInfo->auxdecor<float>("SS_CosTheta"       ) = SS.GetCosDecayAngle();
	eventInfo->auxdecor<float>("SS_dPhiDecayAngle" ) = SS.GetDeltaPhiDecayAngle();
	eventInfo->auxdecor<float>("SS_VisShape"       ) = SS.GetVisibleShape();
	eventInfo->auxdecor<float>("SS_MDeltaR"        ) = SS.GetVisibleShape() * SS.GetMass() ;
	eventInfo->auxdecor<float>("S1_Mass"           ) = S1.GetMass();
	eventInfo->auxdecor<float>("S1_CosTheta"       ) = S1.GetCosDecayAngle();
	eventInfo->auxdecor<float>("S2_Mass"           ) = S2.GetMass();
	eventInfo->auxdecor<float>("S2_CosTheta"       ) = S2.GetCosDecayAngle();
	eventInfo->auxdecor<float>("I1_Depth"          ) = S1.GetFrameDepth(I1);
	eventInfo->auxdecor<float>("I2_Depth"          ) = S2.GetFrameDepth(I2);
	eventInfo->auxdecor<float>("V1_N"              ) = VIS.GetNElementsInFrame(V1);
	eventInfo->auxdecor<float>("V2_N"              ) = VIS.GetNElementsInFrame(V2);



    // dphiR and Rptshat (formerly cosPT)
    // for QCD rejection
    double dphiR = SS.GetDeltaPhiBoostVisible();
    double PTCM = SS.GetFourVector(LAB).Pt();
    double Rptshat = PTCM / (PTCM + SS.GetMass()/4.);

    // QCD rejection using the 'background tree'
    // MET 'sibling' in background tree auxillary calculations
    TLorentzVector Psib = I_alt.GetSiblingFrame()->GetFourVector(LAB_alt);
    TLorentzVector Pmet = I_alt.GetFourVector(LAB_alt);
    double Psib_dot_METhat = max(0., Psib.Vect().Dot(MET_TV3.Unit()));
    double Mpar2 = Psib.E()*Psib.E()-Psib.Vect().Dot(MET_TV3.Unit())*Psib.Vect().Dot(MET_TV3.Unit());
    double Msib2 = Psib.M2();
    double MB2 = 2.*(Pmet.E()*Psib.E()-MET_TV3.Dot(Psib.Vect()));
    TVector3 boostPsibM = (Pmet+Psib).BoostVector();


    // QCD rejection variables from 'background tree'
    double DepthBKG = S_alt.GetFrameDepth(I_alt);
    int Nsib = I_alt.GetSiblingFrame()->GetNDescendants();
    double cosBKG = I_alt.GetParentFrame()->GetCosDecayAngle();
    double dphiMsib = fabs(MET_TV3.DeltaPhi(Psib.Vect()));
    double RpsibM = Psib_dot_METhat / (Psib_dot_METhat + MET_TV3.Mag());
    double RmsibM = 1. / ( MB2/(Mpar2-Msib2) + 1.);
    Psib.Boost(-boostPsibM);
    double cosPsibM = -1.*Psib.Vect().Unit().Dot(boostPsibM.Unit());
    cosPsibM = (1.-cosPsibM)/2.;
    double DeltaQCD1 = (cosPsibM-RpsibM)/(cosPsibM+RpsibM);
    double DeltaQCD2 = (cosPsibM-RmsibM)/(cosPsibM+RmsibM);

    eventInfo->auxdecor<float>("QCD_dPhiR"              ) = dphiR;
    eventInfo->auxdecor<float>("QCD_Rpt"                ) = Rptshat;
    eventInfo->auxdecor<float>("QCD_Rmsib"              ) = RmsibM;
    eventInfo->auxdecor<float>("QCD_Delta2"              ) = DeltaQCD2;
    eventInfo->auxdecor<float>("QCD_Rpsib"              ) = RpsibM;
    eventInfo->auxdecor<float>("QCD_Delta1"              ) = DeltaQCD1;

	// Info( APP_NAME,"RJigsaw Variables from RestFrames: sHatR %f gammainv_Rp1 %f",
	// 	eventInfo->auxdata< float >("sHatR"), eventInfo->auxdata< float >("gammainv_Rp1") );



	/////////////////////////////////////////////////////////////////

	if(goodJets->at(0)->pt() >  20000 &&
		goodJets->at(1)->pt() > 20000 
		// (goodJets->at(0)->btagging())->MV1_discriminant() > 0.98 &&
		// (goodJets->at(1)->btagging())->MV1_discriminant() > 0.98 
		)
	{
			return "SRA";
		
	}


	// if(goodJets->size() > 2){
	// 	if(goodJets->at(0)->pt() > 150000 &&
	// 		goodJets->at(1)->pt() > 30000 && 
	// 		goodJets->at(2)->pt() > 30000 && 
	// 		(goodJets->at(1)->btagging())->MV1_discriminant() > 0.98 &&
	// 		(goodJets->at(2)->btagging())->MV1_discriminant() > 0.98 ){
	// 		return "SRB";
	// 	}
	// }


	return "";

}










