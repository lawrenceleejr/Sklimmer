#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <Sklimmer/SklimmerAnalysis.h>

#include <TLorentzVector.h>
#include <TVector3.h>

#include <cstdlib>
#include <string>


//Still really need to implement a systematics framework

// this is needed to distribute the algorithm to the workers
ClassImp(SklimmerAnalysis)



SklimmerAnalysis :: SklimmerAnalysis ()
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

	job.useD3PDReader ();

	return EL::StatusCode::SUCCESS;
}



EL::StatusCode SklimmerAnalysis :: histInitialize ()
{
	// Here you do everything that needs to be done at the very
	// beginning on each worker node, e.g. create histograms and output
	// trees.  This method gets called before any input files are
	// connected.

  AHist = new TH1F("AHist", "AHist", 100, 0, 1.e5);
  wk()->addOutput (AHist);


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



	if (!RJTool){
		throw std::string ("No RJTool configured");
	}



	std::cout << std::getenv("ROOTCOREBIN") << std::endl;

	string rootcorebinpath(std::getenv("ROOTCOREBIN") ) ;

	bool isData = false;
	bool isAtlfast = false;
	bool isMC12b = true;
	bool useLeptonTrigger = true;

	m_susy_obj->initialize(isData, isAtlfast, isMC12b, useLeptonTrigger);



	RJTool->initialize( rootcorebinpath + "/data/RJigsaw/RJigsawConfig/hemisphere1",
						rootcorebinpath + "/data/RJigsaw/RJigsawConfig/hemisphere2");
	// RJTool->initialize("./RJigsawConfig/hemisphere1","./RJigsawConfig/hemisphere2");
	RJTool->resetHists();


	event = wk()->d3pdreader();
	output = EL::getNTupleSvc (wk(), "output");

	//-- connect to the input TTree
	TTree *inputTree = wk()->tree(); // this is the input tree


	if (output){

		output->tree()->Branch( "RJVars_M_0_0_0", &RJVars_M_0_0_0             , "RJVars_M_0_0_0/F");               
		output->tree()->Branch( "RJVars_M_0_0_1", &RJVars_M_0_0_1             , "RJVars_M_0_0_1/F");               
		output->tree()->Branch( "RJVars_M_1_0_0", &RJVars_M_1_0_0             , "RJVars_M_1_0_0/F");               
		output->tree()->Branch( "RJVars_M_1_0_1", &RJVars_M_1_0_1             , "RJVars_M_1_0_1/F");               
		output->tree()->Branch( "RJVars_M_1_1_0", &RJVars_M_1_1_0             , "RJVars_M_1_1_0/F");               
		output->tree()->Branch( "RJVars_M_1_1_1", &RJVars_M_1_1_1             , "RJVars_M_1_1_1/F");               
		output->tree()->Branch( "RJVars_M_2_0_0", &RJVars_M_2_0_0             , "RJVars_M_2_0_0/F");               
		output->tree()->Branch( "RJVars_M_2_0_1", &RJVars_M_2_0_1             , "RJVars_M_2_0_1/F");               
		output->tree()->Branch( "RJVars_M_2_1_0", &RJVars_M_2_1_0             , "RJVars_M_2_1_0/F");               
		output->tree()->Branch( "RJVars_M_2_1_1", &RJVars_M_2_1_1             , "RJVars_M_2_1_1/F");     
		output->tree()->Branch( "RJVars_MDecay_1_1_0", &RJVars_MDecay_1_1_0             , "RJVars_MDecay_1_1_0/F");               
		output->tree()->Branch( "RJVars_MDecay_1_1_1", &RJVars_MDecay_1_1_1             , "RJVars_MDecay_1_1_1/F");  
		output->tree()->Branch( "RJVars_MDecay_2_1_0", &RJVars_MDecay_2_1_0             , "RJVars_MDecay_2_1_0/F");               
		output->tree()->Branch( "RJVars_MDecay_2_1_1", &RJVars_MDecay_2_1_1             , "RJVars_MDecay_2_1_1/F");  
		output->tree()->Branch( "RJVars_dPhi_0_0_0", &RJVars_dPhi_0_0_0          , "RJVars_dPhi_0_0_0/F");                  
		output->tree()->Branch( "RJVars_dPhi_0_0_1", &RJVars_dPhi_0_0_1          , "RJVars_dPhi_0_0_1/F");                  
		output->tree()->Branch( "RJVars_dPhi_1_0_0", &RJVars_dPhi_1_0_0          , "RJVars_dPhi_1_0_0/F");                  
		output->tree()->Branch( "RJVars_dPhi_1_0_1", &RJVars_dPhi_1_0_1          , "RJVars_dPhi_1_0_1/F");                  
		output->tree()->Branch( "RJVars_dPhi_2_0_0", &RJVars_dPhi_2_0_0          , "RJVars_dPhi_2_0_0/F");                  
		output->tree()->Branch( "RJVars_dPhi_2_0_1", &RJVars_dPhi_2_0_1,           "RJVars_dPhi_2_0_1/F");                   
		output->tree()->Branch( "RJVars_dPhi_1_1_0", &RJVars_dPhi_1_1_0          , "RJVars_dPhi_1_1_0/F");                  
		output->tree()->Branch( "RJVars_dPhi_1_1_1", &RJVars_dPhi_1_1_1          , "RJVars_dPhi_1_1_1/F");                  
		output->tree()->Branch( "RJVars_dPhi_2_1_0", &RJVars_dPhi_2_1_0          , "RJVars_dPhi_2_1_0/F");                  
		output->tree()->Branch( "RJVars_dPhi_2_1_1", &RJVars_dPhi_2_1_1,           "RJVars_dPhi_2_1_1/F");       
		output->tree()->Branch( "RJVars_dPhiVis_0_0_0", &RJVars_dPhiVis_0_0_0       , "RJVars_dPhiVis_0_0_0/F");                     
		output->tree()->Branch( "RJVars_dPhiVis_0_0_1", &RJVars_dPhiVis_0_0_1       , "RJVars_dPhiVis_0_0_1/F");                     
		output->tree()->Branch( "RJVars_dPhiVis_1_0_0", &RJVars_dPhiVis_1_0_0       , "RJVars_dPhiVis_1_0_0/F");                     
		output->tree()->Branch( "RJVars_dPhiVis_1_0_1", &RJVars_dPhiVis_1_0_1       , "RJVars_dPhiVis_1_0_1/F");                     
		output->tree()->Branch( "RJVars_dPhiVis_2_0_0", &RJVars_dPhiVis_2_0_0       , "RJVars_dPhiVis_2_0_0/F");                     
		output->tree()->Branch( "RJVars_dPhiVis_2_0_1", &RJVars_dPhiVis_2_0_1,        "RJVars_dPhiVis_2_0_1/F");                         
		output->tree()->Branch( "RJVars_dPhiVis_1_1_0", &RJVars_dPhiVis_1_1_0       , "RJVars_dPhiVis_1_1_0/F");                     
		output->tree()->Branch( "RJVars_dPhiVis_1_1_1", &RJVars_dPhiVis_1_1_1       , "RJVars_dPhiVis_1_1_1/F");                     
		output->tree()->Branch( "RJVars_dPhiVis_2_1_0", &RJVars_dPhiVis_2_1_0       , "RJVars_dPhiVis_2_1_0/F");                     
		output->tree()->Branch( "RJVars_dPhiVis_2_1_1", &RJVars_dPhiVis_2_1_1,        "RJVars_dPhiVis_2_1_1/F");              
		output->tree()->Branch( "RJVars_cosTheta_0_0_0", &RJVars_cosTheta_0_0_0      , "RJVars_cosTheta_0_0_0/F");                      
		output->tree()->Branch( "RJVars_cosTheta_0_0_1", &RJVars_cosTheta_0_0_1      , "RJVars_cosTheta_0_0_1/F");                      
		output->tree()->Branch( "RJVars_cosTheta_1_0_0", &RJVars_cosTheta_1_0_0      , "RJVars_cosTheta_1_0_0/F");                      
		output->tree()->Branch( "RJVars_cosTheta_1_0_1", &RJVars_cosTheta_1_0_1      , "RJVars_cosTheta_1_0_1/F");                      
		output->tree()->Branch( "RJVars_cosTheta_2_0_0", &RJVars_cosTheta_2_0_0      , "RJVars_cosTheta_2_0_0/F");                      
		output->tree()->Branch( "RJVars_cosTheta_2_0_1", &RJVars_cosTheta_2_0_1      , "RJVars_cosTheta_2_0_1/F");                       
		output->tree()->Branch( "RJVars_cosTheta_1_1_0", &RJVars_cosTheta_1_1_0      , "RJVars_cosTheta_1_1_0/F");                      
		output->tree()->Branch( "RJVars_cosTheta_1_1_1", &RJVars_cosTheta_1_1_1      , "RJVars_cosTheta_1_1_1/F");                      
		output->tree()->Branch( "RJVars_cosTheta_2_1_0", &RJVars_cosTheta_2_1_0      , "RJVars_cosTheta_2_1_0/F");                      
		output->tree()->Branch( "RJVars_cosTheta_2_1_1", &RJVars_cosTheta_2_1_1      , "RJVars_cosTheta_2_1_1/F");                      
		output->tree()->Branch( "RJVars_dPhiDecay_0_0_0", &RJVars_dPhiDecay_0_0_0     , "RJVars_dPhiDecay_0_0_0/F");                       
		output->tree()->Branch( "RJVars_dPhiDecay_0_0_1", &RJVars_dPhiDecay_0_0_1     , "RJVars_dPhiDecay_0_0_1/F");                       
		output->tree()->Branch( "RJVars_dPhiDecay_1_0_0", &RJVars_dPhiDecay_1_0_0     , "RJVars_dPhiDecay_1_0_0/F");                       
		output->tree()->Branch( "RJVars_dPhiDecay_1_0_1", &RJVars_dPhiDecay_1_0_1     , "RJVars_dPhiDecay_1_0_1/F");                       
		output->tree()->Branch( "RJVars_dPhiDecay_2_0_0", &RJVars_dPhiDecay_2_0_0     , "RJVars_dPhiDecay_2_0_0/F");                       
		output->tree()->Branch( "RJVars_dPhiDecay_2_0_1", &RJVars_dPhiDecay_2_0_1     , "RJVars_dPhiDecay_2_0_1/F");                       
		output->tree()->Branch( "RJVars_cosThetaDecay_0_0_0", &RJVars_cosThetaDecay_0_0_0 , "RJVars_cosThetaDecay_0_0_0 /F");                           
		output->tree()->Branch( "RJVars_cosThetaDecay_0_0_1", &RJVars_cosThetaDecay_0_0_1 , "RJVars_cosThetaDecay_0_0_1 /F");                           
		output->tree()->Branch( "RJVars_cosThetaDecay_1_0_0", &RJVars_cosThetaDecay_1_0_0 , "RJVars_cosThetaDecay_1_0_0 /F");                           
		output->tree()->Branch( "RJVars_cosThetaDecay_1_0_1", &RJVars_cosThetaDecay_1_0_1 , "RJVars_cosThetaDecay_1_0_1 /F");                           
		output->tree()->Branch( "RJVars_cosThetaDecay_2_0_0", &RJVars_cosThetaDecay_2_0_0 , "RJVars_cosThetaDecay_2_0_0 /F");                           
		output->tree()->Branch( "RJVars_cosThetaDecay_2_0_1", &RJVars_cosThetaDecay_2_0_1 , "RJVars_cosThetaDecay_2_0_1 /F");                           
		output->tree()->Branch( "RJVars_gamma_0_0_0", &RJVars_gamma_0_0_0         , "RJVars_gamma_0_0_0/F");                   
		output->tree()->Branch( "RJVars_gamma_0_0_1", &RJVars_gamma_0_0_1         , "RJVars_gamma_0_0_1/F");                   

		output->tree()->Branch( "isEE", &isEE  , "isEE/I");       
		output->tree()->Branch( "isMuMu", &isMuMu  , "isMuMu/I");       
		output->tree()->Branch( "isEMu", &isEMu  , "isEMu/I");       

		output->tree()->Branch( "isEE", &isEE  , "isEE/I");       
		output->tree()->Branch( "isMuMu", &isMuMu  , "isMuMu/I");       
		output->tree()->Branch( "isEMu", &isEMu  , "isEMu/I");       

		output->tree()->Branch("el_selected", &el_selected   );
		output->tree()->Branch("mu_selected", &mu_selected   );
		output->tree()->Branch("jet_selected", &jet_selected);

		output->tree()->Branch("weight", &weight);

	}

	//-- d3pd reader read branches from input tree
	event->ReadFrom( inputTree ); 

	// decide which branches to activate, and write to the output skimmed D3PD
	// event->SetActive( kFALSE, "*");

	event->SetActive( kTRUE, "RunNumber");
	event->SetActive( kTRUE, "EventNumber");
	event->SetActive( kTRUE, "lbn");
	event->SetActive( kTRUE, "isSimulation");
	event->SetActive( kTRUE, "^mcevt_weight$");

	event->SetActive( kTRUE, "^vx_n$"); 
	event->SetActive( kTRUE, "^vx_nTracks$"); 

	event->SetActive( kTRUE, "^el_n$");  // notation if you want the exact branch name
	event->SetActive( kTRUE, "^el_pt$");
	event->SetActive( kTRUE, "^el_eta$");
	event->SetActive( kTRUE, "^el_phi$");
	event->SetActive( kTRUE, "^mu_staco_n$");  // notation if you want the exact branch name
	event->SetActive( kTRUE, "^mu_staco_pt$");
	event->SetActive( kTRUE, "^mu_staco_eta$");
	event->SetActive( kTRUE, "^mu_staco_phi$");


	// turn on all jet branches first, then turn off some specific jet branches
	// event->SetActive( kFALSE, "jet*");
	event->SetActive( kTRUE, "^jet_AntiKt4LCTopo_pt$");
	event->SetActive( kTRUE, "^jet_AntiKt4LCTopo_eta$");
	event->SetActive( kTRUE, "^jet_AntiKt4LCTopo_phi$");
	event->SetActive( kTRUE, "^jet_AntiKt4LCTopo_E$");
	// event->SetActive( kTRUE, "jet_AntiKt4LCTopo_.*");
	// event->SetActive( kFALSE, "jet_AntiKt4LCTopo_MET.*");
	// event->SetActive( kFALSE, "jet_AntiKt4LCTopo_flavor.*");

	event->SetActive( kTRUE, "^MET_RefFinal_Egamma10NoTau_phi$");
	event->SetActive( kTRUE, "^MET_RefFinal_Egamma10NoTau_sumet$");

	// Make sure that all variables that we want to copy to the output are read into memory
	event->ReadAllActive(); 

	event->WriteTo(output->tree());

	return EL::StatusCode::SUCCESS;
}



EL::StatusCode SklimmerAnalysis :: execute ()
{
	// Here you do everything that needs to be done on every single
	// events, e.g. read input variables, apply cuts, and fill
	// histograms and trees.  This is where most of your actual analysis
	// code will go.

	int passEvent = 1;



	// Important tools
	// GRL Reader
	// Pileup Reweighting Stuff
	// Event Weight

	// Get the electrons
	// containers - baseline and signal and trigmatch


	std::vector<int > el_baseline;
	std::vector<int > el_crack;
	std::vector<int > el_met;
	std::vector<int > el_signal;

	std::vector<int > mu_baseline;
	std::vector<int > mu_cosmic;
	std::vector<int > mu_bad;
	std::vector<int > mu_met;
	std::vector<int > mu_signal;

	std::vector<int > jet_signal;
	std::vector<int > jet_good;
	std::vector<int > jet_bad;
	std::vector<int > jet_LArHoleVeto;
	std::vector<int > jet_btagged;


	int iEl=0;
	for( iEl = 0; iEl < event->el.n(); iEl++  ){

		if( m_susy_obj->FillElectron(iEl,
									event->el[iEl].eta(),
									event->el[iEl].phi(),
									event->el[iEl].cl_eta(),
									event->el[iEl].cl_phi(),
									event->el[iEl].cl_E(),
									event->el[iEl].tracketa(),
									event->el[iEl].trackphi(),
									event->el[iEl].author(),
									event->el[iEl].mediumPP(),
									event->el[iEl].OQ(),
									event->el[iEl].nPixHits(),
									event->el[iEl].nSCTHits(),
									event->el_MET_Egamma10NoTau[iEl].wet().at(0),
									10000.,2.47,SystErr::NONE) ){
			el_baseline.push_back(iEl);
		}

		if( event->el_MET_Egamma10NoTau[iEl].wet().at(0) != 0. ){
			el_met.push_back(iEl);
		}

	}

	for( int iEl_baseline = 0; iEl_baseline < el_baseline.size(); iEl_baseline++){
		iEl = el_baseline.at(iEl_baseline);

		if (m_susy_obj->IsInCrack( event->el[iEl].cl_eta() ))  {
			el_crack.push_back(iEl);
		}
		if(m_susy_obj->IsSignalElectronExp(iEl, 
											event->el[iEl].tightPP(), 
											event->vxp.nTracks(), 
											event->el[iEl].ptcone30(), 
											event->el[iEl].topoEtcone30_corrected(), 
											event->el[iEl].trackIPEstimate_d0_unbiasedpvunbiased(), 
											event->el[iEl].trackIPEstimate_z0_unbiasedpvunbiased(), 
											event->el[iEl].trackIPEstimate_sigd0_unbiasedpvunbiased(), 
											SignalIsoExp::TightIso) ){
			el_signal.push_back(iEl);
		}
	}




	int iMu=0;
	for( iMu = 0; iMu < event->mu_staco.n(); iMu++  ){

		if( m_susy_obj->FillMuon(iMu,
							  event->mu_staco[iMu].pt(),
							  event->mu_staco[iMu].eta(),
							  event->mu_staco[iMu].phi(),
							  event->mu_staco[iMu].me_qoverp_exPV(),
							  event->mu_staco[iMu].id_qoverp_exPV(),
							  event->mu_staco[iMu].me_theta_exPV(),
							  event->mu_staco[iMu].id_theta_exPV(),
							  event->mu_staco[iMu].id_theta(),
							  event->mu_staco[iMu].charge(),
							  event->mu_staco[iMu].isCombinedMuon(),
							  event->mu_staco[iMu].isSegmentTaggedMuon(),
							  event->mu_staco[iMu].loose(),
							  event->mu_staco[iMu].nPixHits(),
							  event->mu_staco[iMu].nPixelDeadSensors(),
							  event->mu_staco[iMu].nPixHoles(),
							  event->mu_staco[iMu].nSCTHits(),
							  event->mu_staco[iMu].nSCTDeadSensors(),
							  event->mu_staco[iMu].nSCTHoles(),
							  event->mu_staco[iMu].nTRTHits(),
							  event->mu_staco[iMu].nTRTOutliers(),
							  10000.,2.5, SystErr::NONE) ){
			mu_met.push_back(iMu);
			mu_baseline.push_back(iMu);
		}
	}

	for( int iMu_baseline = 0; iMu_baseline < mu_baseline.size(); iMu_baseline++){
		iMu = mu_baseline.at(iMu_baseline);

		if (m_susy_obj->IsSignalMuonExp(iMu,
										event->vxp.nTracks(),
										event->mu_staco[iMu].ptcone30_trkelstyle(),
										event->mu_staco[iMu].etcone30(),
										event->mu_staco[iMu].trackIPEstimate_d0_unbiasedpvunbiased(),
										event->mu_staco[iMu].trackIPEstimate_z0_unbiasedpvunbiased(),
										event->mu_staco[iMu].trackIPEstimate_sigd0_unbiasedpvunbiased(),
										SignalIsoExp::TightIso)){
			mu_signal.push_back(iMu);
		}
		if(m_susy_obj->IsCosmicMuon(event->mu_staco[iMu].z0_exPV(),event->mu_staco[iMu].d0_exPV(),1.,0.2) ){
			mu_cosmic.push_back(iMu);
		}
		if( m_susy_obj->IsBadMuon(event->mu_staco[iMu].qoverp_exPV(),event->mu_staco[iMu].cov_qoverp_exPV() ) ){
			mu_bad.push_back(iMu);
		}

	}




	int iJet=0;
	for( iJet = 0; iJet < event->jet_AntiKt4LCTopo.n(); iJet++  ){

		m_susy_obj->FillJet(iJet,
								event->jet_AntiKt4LCTopo[iJet].pt(),
								event->jet_AntiKt4LCTopo[iJet].eta(),
								event->jet_AntiKt4LCTopo[iJet].phi(),
								event->jet_AntiKt4LCTopo[iJet].E(),
								event->jet_AntiKt4LCTopo[iJet].constscale_eta(),
								event->jet_AntiKt4LCTopo[iJet].constscale_phi(),
								event->jet_AntiKt4LCTopo[iJet].constscale_E(),
								event->jet_AntiKt4LCTopo[iJet].constscale_m(),
								event->jet_AntiKt4LCTopo[iJet].ActiveAreaPx(),
								event->jet_AntiKt4LCTopo[iJet].ActiveAreaPy(),
								event->jet_AntiKt4LCTopo[iJet].ActiveAreaPz(),
								event->jet_AntiKt4LCTopo[iJet].ActiveAreaE(),
								event->Eventshape.rhoKt4LC(),  
								event->eventinfo.averageIntPerXing(),
								event->vxp.nTracks() );

		if(m_susy_obj->GetJetTLV(iJet).Pt() <= 20000.) continue;
		
		bool isoverlap = false;
		for( int iEl_baseline = 0; iEl_baseline < el_baseline.size(); iEl_baseline++){
			iEl = el_baseline.at(iEl_baseline);
			if( m_susy_obj->GetElecTLV(iEl).DeltaR(m_susy_obj->GetJetTLV(iJet)) > 0.2 ) continue;
			isoverlap = true;
			break;
		}
		if(isoverlap) continue;

		bool isgoodjet = false;
		if( m_susy_obj->IsGoodJet( iJet,
									event->jet_AntiKt4LCTopo[iJet].constscale_eta(),
                                    event->jet_AntiKt4LCTopo[iJet].emfrac(),
                                    event->jet_AntiKt4LCTopo[iJet].hecf(),
                                    event->jet_AntiKt4LCTopo[iJet].LArQuality(),
                                    event->jet_AntiKt4LCTopo[iJet].HECQuality(),
                                    event->jet_AntiKt4LCTopo[iJet].AverageLArQF(),
                                    event->jet_AntiKt4LCTopo[iJet].Timing(),
                                    event->jet_AntiKt4LCTopo[iJet].sumPtTrk_pv0_500MeV(),
                                    event->jet_AntiKt4LCTopo[iJet].fracSamplingMax(),
                                    event->jet_AntiKt4LCTopo[iJet].SamplingMax(),
                                    event->jet_AntiKt4LCTopo[iJet].NegativeE(),
                                    event->eventinfo.RunNumber(),20000.,10.) ) isgoodjet = true;

		if(m_susy_obj->GetJetTLV(iJet).Pt() <= 20000.) continue;

		if(!isgoodjet) jet_bad.push_back(iJet);
		else if(fabs(m_susy_obj->GetJetTLV(iJet).Eta() ) < 2.8 ){
			jet_good.push_back(iJet);
		}

	}


	// Now look at all the chosen leptons and make sure they're >0.4 away from any good jet


	for( int iEl_signal = 0; iEl_signal < el_signal.size(); iEl_signal++){
		iEl = el_signal.at(iEl_signal);

		bool isoverlap = false;
		for( int iJet_good = 0; iJet_good < jet_good.size(); iJet_good++){
			iJet = jet_good[iJet_good];
			if( m_susy_obj->GetElecTLV(iEl).DeltaR(m_susy_obj->GetJetTLV(iJet)) < 0.4 ){
				isoverlap = true;
				break;
			}
		}
		if(isoverlap){
			el_signal.erase(el_signal.begin() + iEl_signal);
			iEl_signal--;
		}
	}


	for( int iMu_signal = 0; iMu_signal < mu_signal.size(); iMu_signal++){
		iMu = mu_signal.at(iMu_signal);

		bool isoverlap = false;
		for( int iJet_good = 0; iJet_good < jet_good.size(); iJet_good++){
			iJet = jet_good[iJet_good];
			if( m_susy_obj->GetElecTLV(iMu).DeltaR(m_susy_obj->GetJetTLV(iJet)) < 0.4 ){
				isoverlap = true;
				break;
			}
		}
		if(isoverlap){
			mu_signal.erase(mu_signal.begin() + iMu_signal);
			iMu_signal--;
		}
	}

	if(event->mcevt.weight()[0].size() ){
		weight = event->mcevt.weight()[0][0][0];
	} else {
		weight = 1.;
	}

	isEE = 0;
	isMuMu = 0;
	isEMu = 0;


	TLorentzVector Lepton[3];
	TLorentzVector Jet[3];

	if( jet_good.size() < 2 ) return EL::StatusCode::SUCCESS;;
	Jet[0] = m_susy_obj->GetJetTLV( jet_good[0] );
	Jet[1] = m_susy_obj->GetJetTLV( jet_good[1] );

	if( el_signal.size()==2 ){
		isEE = 1;
		Lepton[0] = m_susy_obj->GetElecTLV( el_signal[0] );
		Lepton[1] = m_susy_obj->GetElecTLV( el_signal[1] );
	} else if ( mu_signal.size()==2 ){
		isMuMu = 1;
		Lepton[0] = m_susy_obj->GetMuonTLV( mu_signal[0] );
		Lepton[1] = m_susy_obj->GetMuonTLV( mu_signal[1] );
	} else if ( mu_signal.size()==1 && el_signal.size()==1 ){
		isEMu = 1;
		Lepton[0] = m_susy_obj->GetElecTLV( el_signal[0] );
		Lepton[1] = m_susy_obj->GetMuonTLV( mu_signal[0] );
	} else return EL::StatusCode::SUCCESS;

	if( (Lepton[0]+Jet[0]).M() + (Lepton[1]+Jet[1]).M()  >  (Lepton[1]+Jet[0]).M() + (Lepton[0]+Jet[1]).M()  ){
		Lepton[3] = Lepton[0];
		Lepton[0] = Lepton[1];
		Lepton[1] = Lepton[3];
	}

	TVector2 met = m_susy_obj->GetMET(
									event->jet_AntiKt4LCTopo_MET_Egamma10NoTau.wet(),
	                                event->jet_AntiKt4LCTopo_MET_Egamma10NoTau.wpx(),
	                                event->jet_AntiKt4LCTopo_MET_Egamma10NoTau.wpy(),
	                                event->jet_AntiKt4LCTopo_MET_Egamma10NoTau.statusWord(),
	                                el_met,
	                                event->el_MET_Egamma10NoTau.wet(),
	                                event->el_MET_Egamma10NoTau.wpx(),
	                                event->el_MET_Egamma10NoTau.wpy(),
	                                event->el_MET_Egamma10NoTau.statusWord(),                             
	                                event->MET_CellOut_Egamma10NoTau.etx(), 
	                                event->MET_CellOut_Egamma10NoTau.ety(),
	                                event->MET_CellOut_Egamma10NoTau.sumet(),
	                                event->MET_CellOut_Eflow_STVF_Egamma10NoTau.etx(), 
	                                event->MET_CellOut_Eflow_STVF_Egamma10NoTau.ety(),
	                                event->MET_CellOut_Eflow_STVF_Egamma10NoTau.sumet(),                                
	                                event->MET_RefGamma_Egamma10NoTau.etx(),
	                                event->MET_RefGamma_Egamma10NoTau.ety(),
	                                event->MET_RefGamma_Egamma10NoTau.sumet(),
	                                mu_met,
	                                event->mu_staco.ms_qoverp(), 
	                                event->mu_staco.ms_theta(), 
	                                event->mu_staco.ms_phi(), 
	                                event->mu_staco.charge(),
	                                event->mu_staco.energyLossPar(),
	                                event->eventinfo.averageIntPerXing(),
	                                SUSYMet::Default,
	                                SystErr::NONE,
	                                false);


	RJTool->newEvent();

	RJTool->addVisParticle("b",Jet[0],1);
	RJTool->addVisParticle("b",Jet[1],2);

	RJTool->addVisParticle("l",Lepton[0],1);
	RJTool->addVisParticle("l",Lepton[1],2);

	if( met.X() == 0. && met.Y() == 0. ) return EL::StatusCode::SUCCESS;
	TVector3 MET;
	MET.SetXYZ(  met.X(), met.Y(), 0.0 );
	RJTool->addMET( MET );

	RJTool->setHemisphereMode(0); //top symmetry
	RJTool->guessInvParticles();
	RJTool->getObservables();

	// If you want access to the variables to do whatever with...

	// Try again with other Hemisphere mode

	RJTool->setHemisphereMode(1); //W symmetry
	RJTool->guessInvParticles();
	RJTool->getObservables();

	std::map< TString, double > RJVars = RJTool->getObservablesMap();

	RJVars_M_0_0_0           = RJVars["M_0_0_0"];
	RJVars_M_0_0_1           = RJVars["M_0_0_1"];
	RJVars_M_1_0_0           = RJVars["M_1_0_0"];
	RJVars_M_1_0_1           = RJVars["M_1_0_1"];
	RJVars_M_1_1_0           = RJVars["M_1_1_0"];
	RJVars_M_1_1_1           = RJVars["M_1_1_1"];
	RJVars_M_2_0_0           = RJVars["M_2_0_0"];
	RJVars_M_2_0_1           = RJVars["M_2_0_1"];
	RJVars_M_2_1_0           = RJVars["M_2_1_0"];
	RJVars_M_2_1_1           = RJVars["M_2_1_1"];
	RJVars_dPhi_0_0_0        = RJVars["dPhi_0_0_0"];
	RJVars_dPhi_0_0_1        = RJVars["dPhi_0_0_1"];
	RJVars_dPhi_1_0_0        = RJVars["dPhi_1_0_0"];
	RJVars_dPhi_1_0_1        = RJVars["dPhi_1_0_1"];
	RJVars_dPhi_2_0_0        = RJVars["dPhi_2_0_0"];
	RJVars_dPhi_2_0_1        = RJVars["dPhi_2_0_1"];
	RJVars_dPhiVis_0_0_0     = RJVars["dPhiVis_0_0_0"];
	RJVars_dPhiVis_0_0_1     = RJVars["dPhiVis_0_0_1"];
	RJVars_dPhiVis_1_0_0     = RJVars["dPhiVis_1_0_0"];
	RJVars_dPhiVis_1_0_1     = RJVars["dPhiVis_1_0_1"];
	RJVars_dPhiVis_2_0_0     = RJVars["dPhiVis_2_0_0"];
	RJVars_dPhiVis_2_0_1     = RJVars["dPhiVis_2_0_1"];
	RJVars_gamma_0_0_0       = RJVars["gamma_0_0_0"];
	RJVars_gamma_0_0_1       = RJVars["gamma_0_0_1"];




	RJVars_M_0_0_0              =  RJVars["M_0_0_0"];
	RJVars_M_0_0_1              =  RJVars["M_0_0_1"];
	RJVars_M_1_0_0              =  RJVars["M_1_0_0"];
	RJVars_M_1_0_1              =  RJVars["M_1_0_1"];
	RJVars_M_1_1_0              =  RJVars["M_1_1_0"];
	RJVars_M_1_1_1              =  RJVars["M_1_1_1"];
	RJVars_M_2_0_0              =  RJVars["M_2_0_0"];
	RJVars_M_2_0_1              =  RJVars["M_2_0_1"];
	RJVars_M_2_1_0              =  RJVars["M_2_1_0"];
	RJVars_M_2_1_1              =  RJVars["M_2_1_1"];
	RJVars_MDecay_1_1_0         =  RJVars["MDecay_1_1_0"];
	RJVars_MDecay_1_1_1         =  RJVars["MDecay_1_1_1"];
	RJVars_MDecay_2_1_0         =  RJVars["MDecay_2_1_0"];
	RJVars_MDecay_2_1_1         =  RJVars["MDecay_2_1_1"];
	RJVars_dPhi_0_0_0         =  RJVars["dPhi_0_0_0"];
	RJVars_dPhi_0_0_1         =  RJVars["dPhi_0_0_1"];
	RJVars_dPhi_1_0_0         =  RJVars["dPhi_1_0_0"];
	RJVars_dPhi_1_0_1         =  RJVars["dPhi_1_0_1"];
	RJVars_dPhi_2_0_0         =  RJVars["dPhi_2_0_0"];
	RJVars_dPhi_2_0_1         =  RJVars["dPhi_2_0_1"];
	RJVars_dPhi_1_1_0         =  RJVars["dPhi_1_1_0"];
	RJVars_dPhi_1_1_1         =  RJVars["dPhi_1_1_1"];
	RJVars_dPhi_2_1_0         =  RJVars["dPhi_2_1_0"];
	RJVars_dPhi_2_1_1         =  RJVars["dPhi_2_1_1"];
	RJVars_dPhiVis_0_0_0         =  RJVars["dPhiVis_0_0_0"];
	RJVars_dPhiVis_0_0_1         =  RJVars["dPhiVis_0_0_1"];
	RJVars_dPhiVis_1_0_0         =  RJVars["dPhiVis_1_0_0"];
	RJVars_dPhiVis_1_0_1         =  RJVars["dPhiVis_1_0_1"];
	RJVars_dPhiVis_2_0_0         =  RJVars["dPhiVis_2_0_0"];
	RJVars_dPhiVis_2_0_1         =  RJVars["dPhiVis_2_0_1"];
	RJVars_dPhiVis_1_1_0         =  RJVars["dPhiVis_1_1_0"];
	RJVars_dPhiVis_1_1_1         =  RJVars["dPhiVis_1_1_1"];
	RJVars_dPhiVis_2_1_0         =  RJVars["dPhiVis_2_1_0"];
	RJVars_dPhiVis_2_1_1         =  RJVars["dPhiVis_2_1_1"];
	RJVars_cosTheta_0_0_0         =  RJVars["cosTheta_0_0_0"];
	RJVars_cosTheta_0_0_1         =  RJVars["cosTheta_0_0_1"];
	RJVars_cosTheta_1_0_0         =  RJVars["cosTheta_1_0_0"];
	RJVars_cosTheta_1_0_1         =  RJVars["cosTheta_1_0_1"];
	RJVars_cosTheta_2_0_0         =  RJVars["cosTheta_2_0_0"];
	RJVars_cosTheta_2_0_1         =  RJVars["cosTheta_2_0_1"];
	RJVars_cosTheta_1_1_0         =  RJVars["cosTheta_1_1_0"];
	RJVars_cosTheta_1_1_1         =  RJVars["cosTheta_1_1_1"];
	RJVars_cosTheta_2_1_0         =  RJVars["cosTheta_2_1_0"];
	RJVars_cosTheta_2_1_1         =  RJVars["cosTheta_2_1_1"];
	RJVars_dPhiDecay_0_0_0         =  RJVars["dPhiDecay_0_0_0"];
	RJVars_dPhiDecay_0_0_1         =  RJVars["dPhiDecay_0_0_1"];
	RJVars_dPhiDecay_1_0_0         =  RJVars["dPhiDecay_1_0_0"];
	RJVars_dPhiDecay_1_0_1         =  RJVars["dPhiDecay_1_0_1"];
	RJVars_dPhiDecay_2_0_0         =  RJVars["dPhiDecay_2_0_0"];
	RJVars_dPhiDecay_2_0_1         =  RJVars["dPhiDecay_2_0_1"];
	RJVars_cosThetaDecay_0_0_0         =  RJVars["cosThetaDecay_0_0_0"];
	RJVars_cosThetaDecay_0_0_1         =  RJVars["cosThetaDecay_0_0_1"];
	RJVars_cosThetaDecay_1_0_0         =  RJVars["cosThetaDecay_1_0_0"];
	RJVars_cosThetaDecay_1_0_1         =  RJVars["cosThetaDecay_1_0_1"];
	RJVars_cosThetaDecay_2_0_0         =  RJVars["cosThetaDecay_2_0_0"];
	RJVars_cosThetaDecay_2_0_1         =  RJVars["cosThetaDecay_2_0_1"];
	RJVars_gamma_0_0_0         =  RJVars["gamma_0_0_0"];
	RJVars_gamma_0_0_1         =  RJVars["gamma_0_0_1"];

	el_selected  = new vector<int>(el_signal);
	mu_selected  = new vector<int>(mu_signal);
	jet_selected = new vector<int>(jet_good );


	// if( (event->triggerbits.EF_mu24_tight() != 1)    && 
	//     (event->triggerbits.EF_e24vh_medium1() != 1) && 
	//     (event->triggerbits.EF_xe80_tclcw() != 1) ){
	//   passEvent = 0;
	// }

	if(passEvent){
		// You can put some criteria here for the event to be selected.
		output->setFilterPassed (); // You must have this line somewhere
	}

//  output->setFilterPassed ();
	
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
