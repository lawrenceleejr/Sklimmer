#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <Sklimmer/SklimmerAnalysis.h>


#include <TLorentzVector.h>
#include <TVector3.h>

#include <cstdlib>
#include <string>

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

#include "xAODTrigger/MuonRoIContainer.h"
#include "xAODTrigger/EmTauRoIContainer.h"
//#include "xAODTrigger/EnergySumRoIContainer.h"
//#include "xAODTrigger/JetEtRoIContainer.h"
#include "xAODTrigger/JetEtRoI.h"
#include "xAODTrigger/JetRoI.h"
#include "xAODTrigger/EnergySumRoI.h"
//#include "xAODTrigger/JetRoIContainer.h"
#include "xAODTrigMissingET/TrigMissingET.h"
#include "xAODTrigMissingET/TrigMissingETContainer.h"

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

// BACKGROUND TREE FOR QCD VARIABLES //////////////////////////
///////////////////////////////////////////////////////////////
//
//

// 	LAB_alt = new RestFrames::RLabFrame("LAB","lab");

// 	S_alt = new RestFrames::RSelfAssemblingFrame("CM","CM");
// 	V_alt = new RestFrames::RVisibleFrame("V_alt","Vis");
// 	I_alt = new RestFrames::RInvisibleFrame("I_alt","Iinv");
// 	INV_alt = new RestFrames::InvisibleGroup ("INV_alt","Invisible State Jigsaws");
// 	VIS_alt = new RestFrames::CombinatoricGroup("VIS_alt","Visible Object Jigsaws");


// 	MinMass_alt = new RestFrames::InvisibleMassJigsaw("MINMASS_JIGSAW_ALT", "Invisible system mass Jigsaw");
// 	Rapidity_alt = new RestFrames::InvisibleRapidityJigsaw("RAPIDITY_JIGSAW_ALT", "Invisible system rapidity Jigsaw");


// 	LAB = new RestFrames::RLabFrame("LAB","lab");
// 	SS = new RestFrames::RDecayFrame("SS","SS");
// 	S1 = new RestFrames::RSelfAssemblingFrame("S1","#tilde{S}_{a}");
// 	S2 = new RestFrames::RSelfAssemblingFrame("S2","#tilde{S}_{b}");
// 	V1 = new RestFrames::RVisibleFrame("V1","V_{a}");
// 	V2 = new RestFrames::RVisibleFrame("V2","V_{b}");
// 	I1 = new RestFrames::RInvisibleFrame("I1","I_{a}");
// 	I2 = new RestFrames::RInvisibleFrame("I2","I_{b}");
// 	INV = new RestFrames::InvisibleGroup("INV","Invisible State Jigsaws");
// 	VIS = new RestFrames::CombinatoricGroup("VIS","Visible Object Jigsaws");

// 	MinMassJigsaw = new RestFrames::InvisibleMassJigsaw("MINMASS_JIGSAW", "Invisible system mass Jigsaw");
// 	RapidityJigsaw = new RestFrames::InvisibleRapidityJigsaw("RAPIDITY_JIGSAW", "Invisible system rapidity Jigsaw");
// 	ContraBoostJigsaw = new RestFrames::ContraBoostInvariantJigsaw("CB_JIGSAW","Contraboost invariant Jigsaw");
// 	HemiJigsaw = new RestFrames::MinimizeMassesCombinatoricJigsaw("HEM_JIGSAW","Minimize m _{V_{a,b}} Jigsaw");

// 	LAB_R = new RestFrames::RLabFrame("LAB_R","LAB");
// 	GG_R = new RestFrames::RDecayFrame("GG_R","#tilde{g}#tilde{g}");
// 	Ga_R = new RestFrames::RDecayFrame("Ga_R","#tilde{g}_{a}");
// 	Gb_R = new RestFrames::RDecayFrame("Gb_R","#tilde{g}_{b}");
// 	Ca_R = new RestFrames::RDecayFrame("Ca_R","C_{a}");
// 	Cb_R = new RestFrames::RDecayFrame("Cb_R","C_{b}");
// 	V1a_R = new RestFrames::RVisibleFrame("V1a_R","j_{1a}");
// 	V2a_R = new RestFrames::RVisibleFrame("V2a_R","j_{2a}");
// 	Xa_R = new RestFrames::RInvisibleFrame("Xa_R","#tilde{#chi}_{a}");
// 	V1b_R = new RestFrames::RVisibleFrame("V1b_R","j_{1b}");
// 	V2b_R = new RestFrames::RVisibleFrame("V2b_R","j_{2b}");
// 	Xb_R = new RestFrames::RInvisibleFrame("Xb_R","#tilde{#chi}_{b}");
// 	INV_R = new RestFrames::InvisibleGroup ("INV_R","WIMP Jigsaws");
// 	VIS_R = new RestFrames::CombinatoricGroup("VIS","Visible Object Jigsaws");
// 	MinMassJigsaw_R = new RestFrames::InvisibleMassJigsaw("MINMASS_R", "Invisible system mass Jigsaw");
// 	RapidityJigsaw_R = new RestFrames::InvisibleRapidityJigsaw("RAPIDITY_R", "Invisible system rapidity Jigsaw");
// 	ContraBoostJigsaw_R = new RestFrames::ContraBoostInvariantJigsaw("CONTRA_R","Contraboost invariant Jigsaw");
// 	HemiJigsaw_R = new RestFrames::MinimizeMassesCombinatoricJigsaw ("HEM_JIGSAW_R","Minimize m _{V_{a,b}} Jigsaw");
// 	CaHemiJigsaw_R = new RestFrames::MinimizeMassesCombinatoricJigsaw("CbHEM_JIGSAW_R","Minimize m _{C_{a}} Jigsaw");
// 	CbHemiJigsaw_R = new RestFrames::MinimizeMassesCombinatoricJigsaw("CaHEM_JIGSAW_R","Minimize m _{C_{b}} Jigsaw");





// 	INV_alt->AddFrame(I_alt);
// 	VIS_alt->AddFrame(V_alt);
// 	VIS_alt->SetNElementsForFrame(V_alt,1,false);

// 	LAB_alt->SetChildFrame(S_alt);
// 	S_alt->AddChildFrame(V_alt);
// 	S_alt->AddChildFrame(I_alt);

// 	LAB_alt->InitializeTree();

// // Will just set invisible mass to zero
// 	INV_alt->AddJigsaw(MinMass_alt);

// // will set rapidity to zero
// 	INV_alt->AddJigsaw(Rapidity_alt);
// 	Rapidity_alt->AddVisibleFrame((LAB_alt->GetListVisibleFrames()));

// 	LAB_alt->InitializeAnalysis();

//
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
// SQUARK TREE /////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
//

// The invisible group is all of the weakly interacting particles
// 	INV->AddFrame(I1);
// 	INV->AddFrame(I2);

// // the combinatoric group is the list of visible objects
// // that go into our hemispheres
// 	VIS->AddFrame(V1);
// 	VIS->SetNElementsForFrame(V1,1,false);
// 	VIS->AddFrame(V2);
// 	VIS->SetNElementsForFrame(V2,1,false);

// 	LAB->SetChildFrame(SS);

// 	SS->AddChildFrame(S1);
// 	SS->AddChildFrame(S2);

// 	S1->AddChildFrame(V1);
// 	S1->AddChildFrame(I1);
// 	S2->AddChildFrame(V2);
// 	S2->AddChildFrame(I2);

// 	std::cout << "Is consistent tree topology? " << LAB->InitializeTree() << std::endl;

// 	INV->AddJigsaw(MinMassJigsaw);

// 	INV->AddJigsaw(RapidityJigsaw);
// 	RapidityJigsaw->AddVisibleFrame((LAB->GetListVisibleFrames()));

// 	INV->AddJigsaw(ContraBoostJigsaw);
// 	ContraBoostJigsaw->AddVisibleFrame((S1->GetListVisibleFrames()), 0);
// 	ContraBoostJigsaw->AddVisibleFrame((S2->GetListVisibleFrames()), 1);
// 	ContraBoostJigsaw->AddInvisibleFrame((S1->GetListInvisibleFrames()), 0);
// 	ContraBoostJigsaw->AddInvisibleFrame((S2->GetListInvisibleFrames()), 1);

// 	VIS->AddJigsaw(HemiJigsaw);
// 	HemiJigsaw->AddFrame(V1,0);
// 	HemiJigsaw->AddFrame(V2,1);

// 	std::cout << "Is consistent analysis tree? : " << LAB->InitializeAnalysis() << std::endl;

//
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
// GLUINO TREE /////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
//


// Set up 'signal-like' analysis tree
// 	LAB_R->SetChildFrame(GG_R);
// 	GG_R->AddChildFrame(Ga_R);
// 	GG_R->AddChildFrame(Gb_R);
// 	Ga_R->AddChildFrame(V1a_R);
// 	Ga_R->AddChildFrame(Ca_R);
// 	Ca_R->AddChildFrame(V2a_R);
// 	Ca_R->AddChildFrame(Xa_R);
// 	Gb_R->AddChildFrame(V1b_R);
// 	Gb_R->AddChildFrame(Cb_R);
// 	Cb_R->AddChildFrame(V2b_R);
// 	Cb_R->AddChildFrame(Xb_R);


// 	if(!LAB_R->InitializeTree()) cout << "Problem with signal-like reconstruction tree" << endl;


// 	INV_R->AddFrame(Xa_R);
// 	INV_R->AddFrame(Xb_R);
// // visible frames in first decay step must always have at least one element
// 	VIS_R->AddFrame(V1a_R);
// 	VIS_R->AddFrame(V1b_R);
// 	VIS_R->SetNElementsForFrame(V1a_R,1,false);
// 	VIS_R->SetNElementsForFrame(V1b_R,1,false);
// // visible frames in second decay step can have zero elements
// 	VIS_R->AddFrame(V2a_R);
// 	VIS_R->AddFrame(V2b_R);
// 	VIS_R->SetNElementsForFrame(V2a_R,0,false);
// 	VIS_R->SetNElementsForFrame(V2b_R,0,false);

// 	INV_R->AddJigsaw(MinMassJigsaw_R);
// 	INV_R->AddJigsaw(RapidityJigsaw_R);
// 	RapidityJigsaw_R->AddVisibleFrame((LAB_R->GetListVisibleFrames()));
// 	INV_R->AddJigsaw(ContraBoostJigsaw_R);
// 	ContraBoostJigsaw_R->AddVisibleFrame((Ga_R->GetListVisibleFrames()), 0);
// 	ContraBoostJigsaw_R->AddVisibleFrame((Gb_R->GetListVisibleFrames()), 1);
// 	ContraBoostJigsaw_R->AddInvisibleFrame((Ga_R->GetListInvisibleFrames()), 0);
// 	ContraBoostJigsaw_R->AddInvisibleFrame((Gb_R->GetListInvisibleFrames()), 1);
// 	VIS_R->AddJigsaw(HemiJigsaw_R);
// 	HemiJigsaw_R->AddFrame(V1a_R,0);
// 	HemiJigsaw_R->AddFrame(V1b_R,1);
// 	HemiJigsaw_R->AddFrame(V2a_R,0);
// 	HemiJigsaw_R->AddFrame(V2b_R,1);
// 	VIS_R->AddJigsaw(CaHemiJigsaw_R);
// 	CaHemiJigsaw_R->AddFrame(V1a_R,0);
// 	CaHemiJigsaw_R->AddFrame(V2a_R,1);
// 	CaHemiJigsaw_R->AddFrame(Xa_R,1);
// 	VIS_R->AddJigsaw(CbHemiJigsaw_R);
// 	CbHemiJigsaw_R->AddFrame(V1b_R,0);
// 	CbHemiJigsaw_R->AddFrame(V2b_R,1);
// 	CbHemiJigsaw_R->AddFrame(Xb_R,1);

// 	if(!LAB_R->InitializeAnalysis()) cout << "Problem with signal-tree jigsaws" << endl;



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

		ST::SettingDataSource datasource = isData ? ST::Data : (isAtlfast ? ST::AtlfastII : ST::FullSim);

		CHECK( m_susy_obj->setProperty("DataSource",datasource) );
		//		CHECK( m_susy_obj->setProperty("IsAtlfast",isAtlfast) );
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
	std::string const GRLFilePath = "$ROOTCOREBIN/data/Sklimmer/data15_13TeV.periodA1_HEAD_DQDefects-00-01-02_PHYS_StandardGRL_Atlas_Ready.xml";//todo change this
	const char* fullGRLFilePath = gSystem->ExpandPathName (GRLFilePath.c_str());
	std::vector<std::string> vecStringGRL = {fullGRLFilePath} ;
	//  vecStringGRL.push_back(fullGRLFilePath);
	if(m_grl->setProperty( "GoodRunsListVec", vecStringGRL).isFailure()){ return EL::StatusCode::FAILURE;}

	m_grl->setProperty("PassThrough", true); // if true (default) will ignore result of GRL and will just pass all events
	if(  m_grl->initialize().isFailure()){return EL::StatusCode::FAILURE;}


	//	m_gr// l = new GoodRunsListSelectionTool("GoodRunsListSelectionTool");
	// std::vector<std::string> vecStringGRL;
	// vecStringGRL.push_back(gSystem->ExpandPathName("$ROOTCOREBIN/data/SUSYTools/GRL/Summer2013/data12_8TeV.periodAllYear_DetStatus-v61-pro14-02_DQDefects-00-01-00_PHYS_StandardGRL_All_Good.xml"));
	// CHECK( m_grl->setProperty( "GoodRunsListVec", vecStringGRL) );
	// CHECK( m_grl->setProperty("PassThrough", false) ); // if true (default) will ignore result of GRL and will just pass all events
	// if (!m_grl->initialize().isSuccess()) { // check this isSuccess
	// 	Error(APP_NAME, "Failed to properly initialize the GRL. Exiting." );
	// 	return EL::StatusCode::FAILURE;
	// }

	// Pile Up Reweighting ///////////////////////////////////////////////////////////////////

	// m_pileupReweightingTool= new PileupReweightingTool("PileupReweightingTool");
// CHECK( m_pileupReweightingTool->setProperty("Input","EventInfo") );
	// std::vector<std::string> prwFiles;
	// prwFiles.push_back("PileupReweighting/mc14v1_defaults.prw.root");
	// CHECK( m_pileupReweightingTool->setProperty("ConfigFiles",prwFiles) );
	// std::vector<std::string> lumicalcFiles;
	// lumicalcFiles.push_back("SUSYTools/susy_data12_avgintperbx.root");
	// CHECK( m_pileupReweightingTool->setProperty("LumiCalcFiles",lumicalcFiles) );
	// if(!m_pileupReweightingTool->initialize().isSuccess()){
	// 	Error(APP_NAME, "Failed to properly initialize the Pile Up Reweighting. Exiting." );
	// 	return EL::StatusCode::FAILURE;
	// }


	// std::cout << "Leaving SklimmerAnalysis :: initialize ()"  << std::endl;

	return EL::StatusCode::SUCCESS;
}


int SklimmerAnalysis :: copyFullxAODContainers ()
{

	const char* APP_NAME = "SklimmerAnalysis";


// copy full container(s) to new xAOD
// without modifying the contents of it:
	// CHECK(m_event->copy("EventInfo"));
	// //	CHECK(m_event->copy("TruthEvent"));

	// //CHECK(m_event->copy("TruthParticle"));

	// CHECK(m_event->copy("AntiKt4LCTopoJets"));
	// //	CHECK(m_event->copy("AntiKt4TruthJets"));

	// //	CHECK(m_event->copy("MET_RefFinal"));
	// //	CHECK(m_event->copy("MET_Truth"));

	// //	CHECK(m_event->copy("TruthVertex"));
	// CHECK(m_event->copy("PrimaryVertices"));

	// CHECK(m_event->copy("ElectronCollection"));
	// CHECK(m_event->copy("Muons"));
// CHECK(m_event->copy("PhotonCollection"));


	return 0;

}



int SklimmerAnalysis :: applySUSYObjectDefinitions (){
	//std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;
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

	// xAOD::MuonContainer::iterator mu_itr = (muons_copy)->begin();
	// xAOD::MuonContainer::iterator mu_end  = (muons_copy)->end();

	// for( ; mu_itr != mu_end; ++mu_itr ) {
	// 	// m_susy_obj->IsSignalMuonExp( **mu_itr,  ST::SignalIsoExp::TightIso ) ;
	// 	// m_susy_obj->IsCosmicMuon( **mu_itr );

	// // kill non baseline muon by setting 4-vector to small value
	// 	if ( ((*mu_itr)->muonType() != xAOD::Muon::Combined &&
	// 		(*mu_itr)->muonType() != xAOD::Muon::MuonStandAlone &&
	// 		(*mu_itr)->muonType() != xAOD::Muon::SegmentTagged) ||
	// 		!(*mu_itr)->auxdecor<char>("baseline") )
	// 	{
	// 		(*mu_itr)->setP4(1.,(*mu_itr)->eta(),(*mu_itr)->phi());
	// 	}
	// }


	//------------
	// ELECTRONS
	//------------

	// const xAOD::ElectronContainer* electrons = 0;
	// if ( !m_event->retrieve( electrons, "ElectronCollection" ).isSuccess() ){ // retrieve arguments: container type, container key
	// 	Error(APP_NAME, "Failed to retrieve Electrons container. Exiting." );
	// 	return EL::StatusCode::FAILURE;
	// }

	xAOD::ElectronContainer* electrons_copy(0);
	xAOD::ShallowAuxContainer* electrons_copyaux(0);
	CHECK( m_susy_obj->GetElectrons(electrons_copy,electrons_copyaux, true, 10000., 2.47) ); //EMULATING ZERO LEPTON PACKAGE

	// // Print their properties, using the tools:
	// xAOD::ElectronContainer::iterator el_itr = (electrons_copy)->begin();
	// xAOD::ElectronContainer::iterator el_end = (electrons_copy)->end();

	// for( ; el_itr != el_end; ++el_itr ) {
	//   //		m_susy_obj->IsSignalElectronExp( **el_itr , ST::SignalIsoExp::TightIso);
	// //Info( APP_NAME, " El passing baseline? %i signal %i",(int) (*el_itr)->auxdata< char >("baseline"), (int) (*el_itr)->auxdata< bool >("signal") );
	// }

	//------------
	// PHOTONS
	//------------

	const xAOD::PhotonContainer* photons = 0;
	if ( !m_event->retrieve( photons, "Photons" ).isSuccess() ){ // retrieve arguments: container type, container key
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
	CHECK( m_susy_obj->GetJets(jets_copy,jets_copyaux, true, 20000. , 10. ) ); //EMULATING ZEROLEPTON COLLECTION


	//------------
	// TAUS
	//------------
	const xAOD::TauJetContainer* taus = 0;
	if ( !m_event->retrieve( taus, "TauJets" ).isSuccess() ){ // retrieve arguments: container type, container key
		Error(APP_NAME, "Failed to retrieve Taus container. Exiting." );
		return EL::StatusCode::FAILURE;
	}

	xAOD::TauJetContainer* taus_copy(0);
	xAOD::ShallowAuxContainer* taus_copyaux(0);
	CHECK( m_susy_obj->GetTaus(taus_copy,taus_copyaux) );


	//------------
	// OVERLAP REMOVAL (as in susytools tester)
	//------------

	//	CHECK( m_susy_obj->OverlapRemoval(electrons_copy, muons_copy, jets_copy, false, 0.2, 0.4, 0.4) );


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
	// xAOD::MuonContainer muons_copy_met(SG::VIEW_ELEMENTS);
	// for(auto mu : *muons_copy) {
	// 	if(mu->muonType()==xAOD::Muon::Combined || mu->muonType()==xAOD::Muon::SegmentTagged || mu->muonType()==xAOD::Muon::MuonStandAlone)
	// 		muons_copy_met.push_back(mu);
	// }
	// ///// TEMPORARY CODE ONLY

	CHECK( m_susy_obj->GetMET(*MET,
		jets_copy,
		electrons_copy,
		muons_copy,
		photons_copy,
		0)
	       );



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

EL::StatusCode SklimmerAnalysis :: getHLTTriggerObjs (){
	//std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;
	const char* APP_NAME = "SklimmerAnalysis";

	const xAOD::MuonContainer* muons = 0;
	if ( !m_event->retrieve( muons, "HLT_xAOD__MuonContainer_MuTagIMO_EF" ).isSuccess() ){ // retrieve arguments: container type, container key
		Error(APP_NAME, "Failed to retrieve Muons container. Exiting." );
		return EL::StatusCode::FAILURE;

	}
	//std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;
	std::pair<xAOD::MuonContainer*, xAOD::ShallowAuxContainer*> shallowcopymuon = xAOD::shallowCopyContainer(*muons);
	xAOD::MuonContainer* muons_copy = shallowcopymuon.first;
      	xAOD::ShallowAuxContainer* muons_copyaux = shallowcopymuon.second;

	// xAOD::MuonContainer* muons_copy(0);
	// xAOD::ShallowAuxContainer* muons_copyaux(0);
	// CHECK( m_susy_obj->GetMuons(muons_copy,muons_copyaux) );

	// xAOD::MuonContainer::iterator mu_itr_copy = (muons_copy)->begin();
	// xAOD::MuonContainer::iterator mu_end _copy = (muons_copy)->end();

	// for( ; mu_itr != mu_end; ++mu_itr ) {
	// 	m_susy_obj->IsSignalMuonExp( **mu_itr,  ST::SignalIsoExp::TightIso ) ;
	// 	m_susy_obj->IsCosmicMuon( **mu_itr );

	// // kill non baseline muon by setting 4-vector to small value
	// 	if ( ((*mu_itr)->muonType() != xAOD::Muon::Combined &&
	// 		(*mu_itr)->muonType() != xAOD::Muon::MuonStandAlone &&
	// 		(*mu_itr)->muonType() != xAOD::Muon::SegmentTagged) ||
	// 		!(*mu_itr)->auxdecor<char>("baseline") )
	// 	{
	// 		(*mu_itr)->setP4(1.,(*mu_itr)->eta(),(*mu_itr)->phi());
	// 	}
	// }


	//------------
	// ELECTRONS
	//------------

	const xAOD::ElectronContainer* electrons = 0;
	if ( !m_event->retrieve( electrons, "HLT_xAOD__ElectronContainer_egamma_Electrons" ).isSuccess() ){ // retrieve arguments: container type, container key
		Error(APP_NAME, "Failed to retrieve Electrons container. Exiting." );
		return EL::StatusCode::FAILURE;
	}
	std::pair<xAOD::ElectronContainer*, xAOD::ShallowAuxContainer*> shallowcopyelectron = xAOD::shallowCopyContainer(*electrons);
	xAOD::ElectronContainer* electrons_copy = shallowcopyelectron.first;
      	xAOD::ShallowAuxContainer* electrons_copyaux = shallowcopyelectron.second;
	//std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;
	// xAOD::ElectronContainer* electrons_copy(0);
	// xAOD::ShallowAuxContainer* electrons_copyaux(0);
	// CHECK( m_susy_obj->GetElectrons(electrons_copy,electrons_copyaux, true, 10000., 2.47) ); //EMULATING ZERO LEPTON PACKAGE

	// // Print their properties, using the tools:
	// xAOD::ElectronContainer::iterator el_itr = (electrons_copy)->begin();
	// xAOD::ElectronContainer::iterator el_end = (electrons_copy)->end();

	// for( ; el_itr != el_end; ++el_itr ) {
	// 	m_susy_obj->IsSignalElectronExp( **el_itr , ST::SignalIsoExp::TightIso);
	// //Info( APP_NAME, " El passing baseline? %i signal %i",(int) (*el_itr)->auxdata< char >("baseline"), (int) (*el_itr)->auxdata< bool >("signal") );
	// }
	//------------
	// PHOTONS
	//------------

	const xAOD::PhotonContainer* photons = 0;
	if ( !m_event->retrieve( photons, "HLT_xAOD__PhotonContainer_egamma_Photons" ).isSuccess() ){ // retrieve arguments: container type, container key
		Error(APP_NAME, "Failed to retrieve Photons container. Exiting." );
		return EL::StatusCode::FAILURE;
	}
	std::pair<xAOD::PhotonContainer*, xAOD::ShallowAuxContainer*> shallowcopyphoton = xAOD::shallowCopyContainer(*photons);
	xAOD::PhotonContainer* photons_copy = shallowcopyphoton.first;
      	xAOD::ShallowAuxContainer* photons_copyaux = shallowcopyphoton.second;
	//std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;


	// xAOD::PhotonContainer* photons_copy(0);
	// xAOD::ShallowAuxContainer* photons_copyaux(0);
	// CHECK( m_susy_obj->GetPhotons(photons_copy,photons_copyaux) );
	//------------
	// JETS
	//------------

	const xAOD::JetContainer* jets = 0;
	if ( !m_event->retrieve( jets, "HLT_xAOD__JetContainer_EFJet" ).isSuccess() ){ // retrieve arguments: container type, container key
		Error(APP_NAME, "Failed to retrieve AntiKt4LCTopoJets container. Exiting." );
		return EL::StatusCode::FAILURE;
	}
	std::pair<xAOD::JetContainer*, xAOD::ShallowAuxContainer*> shallowcopyjet = xAOD::shallowCopyContainer(*jets);
	xAOD::JetContainer* jets_copy = shallowcopyjet.first;
      	xAOD::ShallowAuxContainer* jets_copyaux = shallowcopyjet.second;
	//std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;
	// xAOD::JetContainer* jets_copy(0);
	// xAOD::ShallowAuxContainer* jets_copyaux(0);
	// CHECK( m_susy_obj->GetJets(jets_copy,jets_copyaux, true, 20000. , 10. ) ); //EMULATING ZEROLEPTON COLLECTION

	//------------
	// TAUS
	//------------
	const xAOD::TauJetContainer* taus = 0;
	if ( !m_event->retrieve( taus, "HLT_xAOD__TauJetContainer_TrigTauRecMerged" ).isSuccess() ){ // retrieve arguments: container type, container key
		Error(APP_NAME, "Failed to retrieve Taus container. Exiting." );
		return EL::StatusCode::FAILURE;
	}
	std::pair<xAOD::TauJetContainer*, xAOD::ShallowAuxContainer*> shallowcopyTauJet = xAOD::shallowCopyContainer(*taus);
	xAOD::TauJetContainer* taus_copy        = shallowcopyTauJet.first;
      	xAOD::ShallowAuxContainer* taus_copyaux = shallowcopyTauJet.second;
	//std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;

	// xAOD::TauJetContainer* taus_copy(0);
	// xAOD::ShallowAuxContainer* taus_copyaux(0);
	// CHECK( m_susy_obj->GetTaus(taus_copy,taus_copyaux) );

	// //------------
	// // OVERLAP REMOVAL (as in susytools tester)
	// //------------

	// CHECK( m_susy_obj->OverlapRemoval(electrons_copy, muons_copy, jets_copy, false, 0.2, 0.4, 0.4) );


	//------------
	// GET REBUILT MET
	//------------
	const	xAOD::TrigMissingETContainer * trigmet = 0;
	  //	const	DataVector<xAOD::TrigMissingET_v1> * trigmet = 0
	  //	const xAOD::MissingETContainer* met = 0;
	if ( !m_event->retrieve(trigmet, "HLT_xAOD__TrigMissingETContainer_EFJetEtSum" ).isSuccess() ){ // retrieve arguments: container type, container key
	  Error(APP_NAME, "Failed to retrieve Met container. Exiting." );
	  return EL::StatusCode::FAILURE;
	}
	//std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;
	xAOD::MissingETContainer * met = new xAOD::MissingETContainer;
	xAOD::MissingETAuxContainer * metaux = new xAOD::MissingETAuxContainer;
	met->setStore(metaux);

	xAOD::TrigMissingETContainer::const_iterator met_itr = trigmet->begin();
        xAOD::TrigMissingETContainer::const_iterator met_end = trigmet->end();


	xAOD::MissingET * finalMet = new xAOD::MissingET(0.,
							 0.,
							 0.,
							 "HLT_MET_Final",
							 MissingETBase::Source::total()
							 );

	for( ; met_itr != met_end; ++met_itr ) {
	  xAOD::MissingET * newMet = new xAOD::MissingET((*met_itr)->ex(),
							 (*met_itr)->ey(),
							 (*met_itr)->sumEt()
							 );
	  newMet->makePrivateStore( **met_itr );
	  met->push_back(newMet);

	  std::cout << "px " << finalMet->mpx() << std::endl;
	  std::cout << "py " << finalMet->mpy() << std::endl;
	  std::cout << "sumet " << finalMet->sumet() << std::endl;

	  finalMet->setMpx  ( finalMet->mpx() + newMet->mpx()    );
	  finalMet->setMpy  ( finalMet->mpy() + newMet->mpy()    );
	  finalMet->setSumet( finalMet->sumet() + newMet->sumet());

	}
	met->push_back(finalMet);

	//std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;
	// std::pair<xAOD::MissingETContainer*, xAOD::ShallowAuxContainer*> shallowcopyMissingETContainer = xAOD::shallowCopyContainer(*met);
	// xAOD::MissingETContainer * met_copy    = shallowcopyMissingETContainer.first;
      	// xAOD::ShallowAuxContainer* met_copyaux = shallowcopyMissingETContainer.second;

	//////////////////////////////////////////////////////

	if(m_writeFullCollectionsToxAOD){

	muons_copyaux->setShallowIO( true ); // true = shallow copy, false = deep copy
	if( !m_event->record( muons_copy ,   "HLT_Muons" )){return EL::StatusCode::FAILURE;}
	if( !m_event->record( muons_copyaux, "HLT_MuonsAux." )) {return EL::StatusCode::FAILURE;}


	electrons_copyaux->setShallowIO( true ); // true = shallow copy, false = deep copy
	if( !m_event->record( electrons_copy ,   "HLT_Electrons" )){return EL::StatusCode::FAILURE;}
	if( !m_event->record( electrons_copyaux, "HLT_ElectronsAux." )) {return EL::StatusCode::FAILURE;}

	// photons_copyaux->setShallowIO( true ); // true = shallow copy, false = deep copy
	// if( !m_event->record( photons_copy ,   "HLT_Photons" )){return EL::StatusCode::FAILURE;}
	// if( !m_event->record( photons_copyaux, "HLT_PhotonsAux." )) {return EL::StatusCode::FAILURE;}

	jets_copyaux->setShallowIO( true ); // true = shallow copy, false = deep copy
	   // if true should have something like this line somewhere:
	   // m_event->copy("AntiKt4LCTopoJets");
	if( !m_event->record( jets_copy , "HLT_Jets" )){return EL::StatusCode::FAILURE;}
	if( !m_event->record( jets_copyaux, "HLT_JetsAux." )) {return EL::StatusCode::FAILURE;}

	if( !m_event->record( met,    "HLT_MET" )){return EL::StatusCode::FAILURE;}
	if( !m_event->record( metaux, "HLT_METAux." )) {return EL::StatusCode::FAILURE;}

	}
	//std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;
	muons_copy->setStore(muons_copyaux);
	CHECK( m_store->record( muons_copy, "HLT_Muons" ) );
	CHECK( m_store->record( muons_copyaux, "HLT_MuonsAux." ) );

	electrons_copy->setStore(electrons_copyaux);
	CHECK( m_store->record( electrons_copy, "HLT_Electrons" ) );
	CHECK( m_store->record( electrons_copyaux, "HLT_ElectronsAux." ) );

	photons_copy->setStore(photons_copyaux);
	CHECK( m_store->record( photons_copy, "HLT_Photons" ) );
	CHECK( m_store->record( photons_copyaux, "HLT_PhotonsAux." ) );

	jets_copy->setStore(jets_copyaux);
	CHECK( m_store->record( jets_copy, "HLT_Jets" ) );
	CHECK( m_store->record( jets_copyaux, "HLT_JetsAux." ) );

	met->setStore(metaux);
	CHECK( m_store->record( met,    "HLT_MET_RefFinalCont"     ) );
	CHECK( m_store->record( metaux, "HLT_MET_RefFinalContAux." ) );
	//std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;
	////////////////////////////////////////////////////////

  return EL::StatusCode::SUCCESS;
}
EL::StatusCode SklimmerAnalysis :: getL1TriggerObjs ()
{
	const char* APP_NAME = "SklimmerAnalysis";

	//std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;
// *Br  172 :LVL1MuonRoIs : DataVector<xAOD::MuonRoI_v1>                        *
	const DataVector<xAOD::MuonRoI> * muons = 0;
	if ( !m_event->retrieve( muons, "LVL1MuonRoIs" ).isSuccess() ){ // retrieve arguments: container type, container key
		Error(APP_NAME, "Failed to retrieve Muons container. Exiting." );
		return EL::StatusCode::FAILURE;
	}
	//std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;
// *Br  168 :LVL1EmTauRoIs : DataVector<xAOD::EmTauRoI_v2>                      *
	const xAOD::EmTauRoIContainer* emRoi = 0;
	if ( !m_event->retrieve( emRoi, "LVL1EmTauRoIs" ).isSuccess() ){ // retrieve arguments: container type, container key
	  return EL::StatusCode::FAILURE;
	}
	//std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;
// *Br  169 :LVL1EnergySumRoI : xAOD::EnergySumRoI_v1                           *
	const xAOD::EnergySumRoI * energySum = 0;
	if ( !m_event->retrieve( energySum, "LVL1EnergySumRoI" ).isSuccess() ){ // retrieve arguments: container type, container key
	  return EL::StatusCode::FAILURE;
	}
	//std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;
// *Br  171 :LVL1JetRoIs : DataVector<xAOD::JetRoI_v2>                          *
	const DataVector<xAOD::JetRoI> * jetRoi = 0;
	if ( !m_event->retrieve( jetRoi, "LVL1JetRoIs" ).isSuccess() ){ // retrieve arguments: container type, container key
	  return EL::StatusCode::FAILURE;
	}
	//std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;
// *Br  170 :LVL1JetEtRoI : xAOD::JetEtRoI_v1                                   *
//	const DataVector<xAOD::JetEtRoI> * jetEtRoi= 0;
	const xAOD::JetEtRoI * jetEtRoi= 0;
	if ( !m_event->retrieve( jetEtRoi, "LVL1JetEtRoI" ).isSuccess() ){ // retrieve arguments: container type, container key
	  return EL::StatusCode::FAILURE;
	}
	//std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;
	std::pair<DataVector<xAOD::MuonRoI> *, xAOD::ShallowAuxContainer*> shallowcopymuonroi = xAOD::shallowCopyContainer(*muons);
	DataVector<xAOD::MuonRoI> * muons_copy = shallowcopymuonroi.first;
      	xAOD::ShallowAuxContainer* muons_copyaux = shallowcopymuonroi.second;
	//std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;
	std::pair<xAOD::EmTauRoIContainer*, xAOD::ShallowAuxContainer*> shallowcopyemroi = xAOD::shallowCopyContainer(*emRoi);
	xAOD::EmTauRoIContainer*   emroi_copy = shallowcopyemroi.first;
      	xAOD::ShallowAuxContainer* emroi_copyaux = shallowcopyemroi.second;
	//std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;
	std::pair<xAOD::EnergySumRoI*, xAOD::ShallowAuxInfo*> shallowcopyenergysum = xAOD::shallowCopyObject(*energySum);
	xAOD::EnergySumRoI       * energysum_copy = shallowcopyenergysum.first;
      	xAOD::ShallowAuxInfo* energysum_copyaux = shallowcopyenergysum.second;
	//std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;
	std::pair<DataVector<xAOD::JetRoI> *, xAOD::ShallowAuxContainer*> shallowcopyjetroi = xAOD::shallowCopyContainer(*jetRoi);
	DataVector<xAOD::JetRoI> *    jetrois_copy = shallowcopyjetroi.first;
      	xAOD::ShallowAuxContainer* jetrois_copyaux = shallowcopyjetroi.second;
	//std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;
	std::pair<xAOD::JetEtRoI *, xAOD::ShallowAuxInfo*> shallowcopyjetetroi = xAOD::shallowCopyObject(*jetEtRoi);
	xAOD::JetEtRoI      * jetetrois_copy = shallowcopyjetetroi.first;
      	xAOD::ShallowAuxInfo* jetetrois_copyaux = shallowcopyjetetroi.second;
	//std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;
 	CHECK( m_store->record( muons_copy, "L1_Muons" ) );
	CHECK( m_store->record( emroi_copy, "L1_EmRoi" ) );
	CHECK( m_store->record( energysum_copy, "L1_EnergySum" ) );
	CHECK( m_store->record( jetrois_copy, "L1_JetRoi" ) );
	CHECK( m_store->record( jetetrois_copy, "L1_JetEtRoi" ) );

	CHECK( m_store->record( muons_copyaux, "L1_MuonsAux."  ) );
	CHECK( m_store->record( emroi_copyaux, "L1_EmRoiAux."  ) );
	CHECK( m_store->record( energysum_copyaux, "L1_EnergySumAux."  ) );
	CHECK( m_store->record( jetrois_copyaux, "L1_JetRoiAux."  ) );
	CHECK( m_store->record( jetetrois_copyaux, "L1_JetEtRoiAux."  ) );

	//	muons_copy->setStore(muons_copyaux);



	////////////////////////////////////////////////////////


	return 0;




  return EL::StatusCode::SUCCESS;
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
	//std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;
			const xAOD::EventInfo* eventInfo = 0;
			if( ! m_event->retrieve( eventInfo, "EventInfo").isSuccess() ){
				Error(APP_NAME, "Failed to retrieve event info collection. Exiting." );
				return EL::StatusCode::FAILURE;
			}
	//std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;

			int EventNumber = eventInfo->eventNumber();
			int RunNumber = eventInfo->runNumber();
			float EventWeight = 1;
			if(!isData) EventWeight = eventInfo->mcEventWeight();

			//			int MCChannelNumber = eventInfo->mcChannelNumber();
	//std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;



			h_nevents->Fill(0.);
			h_nevents_weighted->Fill(0.,EventWeight);



			//			if(sherpaWZInclVeto(MCChannelNumber)) return EL::StatusCode::SUCCESS;





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
		    //		    CHECK( m_pileupReweightingTool->execute() );
		    //Info( APP_NAME,"PileupReweightingTool: PileupWeight %f RandomRunNumber %i RandomLumiBlockNumber %i",eventInfo->auxdata< double >("PileupWeight"), eventInfo->auxdata< unsigned int >("RandomRunNumber"),  eventInfo->auxdata< unsigned int >("RandomLumiBlockNumber") );
		  }
		}// end if IS MC

		// Let's calibrate
	//std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;
		applySUSYObjectDefinitions();
		getHLTTriggerObjs();
		getL1TriggerObjs();


		std::pair< xAOD::EventInfo*, xAOD::ShallowAuxInfo* > eventInfo_shallowCopy = xAOD::shallowCopyObject( *eventInfo );
		if( !m_store->record( eventInfo_shallowCopy.first , "myEventInfo" )){return EL::StatusCode::FAILURE;}
		if( !m_store->record( eventInfo_shallowCopy.second, "myEventInfoAux." )) {return EL::StatusCode::FAILURE;}


		eventInfo_shallowCopy.second->setShallowIO(true);
		if(m_writeFullCollectionsToxAOD){
			if( !m_event->record( eventInfo_shallowCopy.first , "myEventInfo" )){return EL::StatusCode::FAILURE;}
			if( !m_event->record( eventInfo_shallowCopy.second, "myEventInfoAux." )) {return EL::StatusCode::FAILURE;}
		}


		// m_store->print();

		if( m_Analysis == "saveOutput"){

		}

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
		// if( m_pileupReweightingTool ) {
		// 	delete m_pileupReweightingTool;
		// 	m_pileupReweightingTool = 0;
		// }

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


int SklimmerAnalysis :: sherpaWZInclVeto(int MCChannelNumber){

	const char* APP_NAME = "SklimmerAnalysis";


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
// //std::cout << "doSherpaPtFilterCheck: Unable to find 2 leptons" << std::endl;
	return 0;
else if ( foundMore )
// //std::cout << "doSherpaPtFilterCheck: Found more than 2 leptons" << std::endl;
	return 0;
else {
	V = l1 + l2;
// m_h_sherpaPt->Fill( V.Pt() / GeV, m_eventWeight );
	if(V.Pt()>70000.) return 1;
	else return 0;
}

} else {
	return 0;
}
// stupid sherpa stuff...///////////////////////////////////////////////////////////////////


}









