
int SklimmerAnalysis :: putStuffInStore (){

	//this is written this way so that you don't need to have run SUSYTools in order to have access to the calibrated objects

	const char* APP_NAME = "SklimmerAnalysis";


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



	return 0;

}
