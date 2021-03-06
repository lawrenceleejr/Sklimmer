
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



int SklimmerAnalysis :: putStuffInStore (){

	//this is written this way so that you don't need to have run SUSYTools in order to have access to the calibrated objects

	const char* APP_NAME = "SklimmerAnalysis";


	// Event Info /////////////////////////////////

	xAOD::EventInfo* eventInfo = 0;
	xAOD::EventAuxInfo* eventInfoAux = 0;

	CHECK( m_event->retrieve( eventInfo, "EventInfo") );
	CHECK( m_event->retrieve( eventInfoAux, "EventInfoAux.") );

	eventInfo->setStore(eventInfoAux);

	CHECK( m_store->record( eventInfo, "EventInfo" ) );


	// Objects ////////////////////////////////////

	xAOD::MuonContainer* muons_copy(0);
	xAOD::ShallowAuxContainer* muons_copyaux(0);

	xAOD::ElectronContainer* electrons_copy(0);
	xAOD::ShallowAuxContainer* electrons_copyaux(0);

	xAOD::JetContainer* jets_copy(0);
	xAOD::ShallowAuxContainer* jets_copyaux(0);

	CHECK( m_event->retrieve( jets_copy, "CalibJets" ) );
	CHECK( m_event->retrieve( muons_copy, "CalibMuons" ) );
	CHECK( m_event->retrieve( electrons_copy, "CalibElectrons" ) );

	CHECK( m_event->retrieve( jets_copyaux, "CalibJetsAux." ) );
	CHECK( m_event->retrieve( muons_copyaux, "CalibMuonsAux." ) );
	CHECK( m_event->retrieve( electrons_copyaux, "CalibElectronsAux." ) );

	jets_copy->setStore(jets_copyaux);
	muons_copy->setStore(muons_copyaux);
	electrons_copy->setStore(electrons_copyaux);

	CHECK( m_store->record( jets_copy, "CalibJets" ) );
	CHECK( m_store->record( muons_copy, "CalibMuons" ) );
	CHECK( m_store->record( electrons_copy, "CalibElectrons" ) );


	m_store->print();


	return 0;

}
