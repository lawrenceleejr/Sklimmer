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

#include <EventLoop/Worker.h>

//xAOD Includes
#include "xAODEventInfo/EventInfo.h"
#include "xAODJet/JetContainer.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODEgamma/PhotonContainer.h"
#include "xAODTau/TauJetContainer.h"
#include "xAODMissingET/MissingETContainer.h"
#include "xAODBTaggingEfficiency/BTaggingEfficiencyTool.h"

#include "Sklimmer/EventSelectionBBMet.h"
#include "Sklimmer/errorcheck.h"

//----------------------------------------------------
// Constructor
//----------------------------------------------------
EventSelectionBBMet::EventSelectionBBMet(xAOD::TStore * store) //probably add a version which sets the collection names
{
   m_store = store;
}

//----------------------------------------------------
// Calculate the RJigsaw variables for this analysis
//----------------------------------------------------
void EventSelectionBBMet::calculateRJigsaw(xAOD::JetContainer* goodJets, std::string varPrefix, xAOD::EventInfo * eventInfo, std::string metCollectionName, std::string metString){
	

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

    // loop over good jets (they pass baseline, passOR, and have some extra kinematic cuts)
	xAOD::JetContainer::iterator goodJet_itr = (goodJets)->begin();
	xAOD::JetContainer::iterator goodJet_end = (goodJets)->end();
	for( ; goodJet_itr != goodJet_end; ++goodJet_itr ) {
        const xAOD::Jet* ajet = (*goodJet_itr);
        VIS.AddLabFrameFourVector( (*goodJet_itr)->p4()  );
		jet.SetPtEtaPhiM( (*goodJet_itr)->pt(), 0., (*goodJet_itr)->phi(), (*goodJet_itr)->m()  );
		VIS_alt.AddLabFrameFourVector(jet);
    } 



	// Get MET Collection to hand to Rest Frames////////////////////////////////////////////////////

	xAOD::MissingETContainer* MET = new xAOD::MissingETContainer;
	assert( m_store->retrieve( MET, metCollectionName ) );

	TVector3 MET_TV3;

	xAOD::MissingETContainer::const_iterator met_it = MET->find(metString);//todo? "Final" for reco, "NonInt" for truth
	
	if (met_it == MET->end()) {
		Error( __PRETTY_FUNCTION__, "Could not find final met inside MET container" );
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

	eventInfo->auxdecor<float>(varPrefix+"SS_Mass"           ) = SS.GetMass();
	eventInfo->auxdecor<float>(varPrefix+"SS_InvGamma"       ) = 1./SS.GetGammaInParentFrame();
	eventInfo->auxdecor<float>(varPrefix+"SS_dPhiBetaR"      ) = SS.GetDeltaPhiBoostVisible();
	eventInfo->auxdecor<float>(varPrefix+"SS_dPhiVis"        ) = SS.GetDeltaPhiVisible();
	eventInfo->auxdecor<float>(varPrefix+"SS_CosTheta"       ) = SS.GetCosDecayAngle();
	eventInfo->auxdecor<float>(varPrefix+"SS_dPhiDecayAngle" ) = SS.GetDeltaPhiDecayAngle();
	eventInfo->auxdecor<float>(varPrefix+"SS_VisShape"       ) = SS.GetVisibleShape();
	eventInfo->auxdecor<float>(varPrefix+"SS_MDeltaR"        ) = SS.GetVisibleShape() * SS.GetMass() ;
	eventInfo->auxdecor<float>(varPrefix+"S1_Mass"           ) = S1.GetMass();
	eventInfo->auxdecor<float>(varPrefix+"S1_CosTheta"       ) = S1.GetCosDecayAngle();
	eventInfo->auxdecor<float>(varPrefix+"S2_Mass"           ) = S2.GetMass();
	eventInfo->auxdecor<float>(varPrefix+"S2_CosTheta"       ) = S2.GetCosDecayAngle();
	eventInfo->auxdecor<float>(varPrefix+"I1_Depth"          ) = S1.GetFrameDepth(I1);
	eventInfo->auxdecor<float>(varPrefix+"I2_Depth"          ) = S2.GetFrameDepth(I2);
	eventInfo->auxdecor<float>(varPrefix+"V1_N"              ) = VIS.GetNElementsInFrame(V1);
	eventInfo->auxdecor<float>(varPrefix+"V2_N"              ) = VIS.GetNElementsInFrame(V2);

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

    eventInfo->auxdecor<float>(varPrefix+"QCD_dPhiR"              ) = dphiR;
    eventInfo->auxdecor<float>(varPrefix+"QCD_Rpt"                ) = Rptshat;
    eventInfo->auxdecor<float>(varPrefix+"QCD_Rmsib"              ) = RmsibM;
    eventInfo->auxdecor<float>(varPrefix+"QCD_Delta"              ) = DeltaQCD2;

	// Info( __PRETTY_FUNCTION__,"RJigsaw Variables from RestFrames: sHatR %f gammainv_Rp1 %f",
	// 	eventInfo->auxdata< float >("sHatR"), eventInfo->auxdata< float >("gammainv_Rp1") );
	
	
} // end 	


//----------------------------------------------------
// The primary function
//----------------------------------------------------
std::string EventSelectionBBMet::run(xAOD::EventInfo * eventInfo){
	// Inspired by https://cds.cern.ch/record/1508045/files/ATL-COM-PHYS-2013-072.pdf

     
    bool truthRun = true;
    std::string varPrefix = "";
    if (truthRun) varPrefix = "Truth";


    //----------------------------
    // reco-level
    //----------------------------        
	xAOD::JetContainer* jets_copy(0);
	assert( m_store->retrieve( jets_copy, jetCalibCollectionName ) );

	xAOD::MuonContainer* muons_copy(0);
	assert( m_store->retrieve( muons_copy, muonCalibCollectionName ) );

	xAOD::ElectronContainer* electrons_copy(0);
	assert( m_store->retrieve( electrons_copy, electronCalibCollectionName ) );


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
	// assert( m_store->record(goodJets, "MySelJets") );

	xAOD::JetContainer::iterator jet_itr = (jets_copy)->begin();
	xAOD::JetContainer::iterator jet_end = (jets_copy)->end();

	for( ; jet_itr != jet_end; ++jet_itr ) {
        
		if( (*jet_itr)->auxdata< char >("baseline")==1 &&
		    (*jet_itr)->auxdata< char >("passOR")  ==1 &&
		    (*jet_itr)->pt() > 30000.                  &&
		    ( fabs( (*jet_itr)->eta()) < 2.8)
		    ) {
			goodJets->push_back (*jet_itr);
		}

		(*jet_itr)->auxdecor<float>("MV1") =  ((*jet_itr)->btagging())->MV1_discriminant() ; // LH: should this be above if statement?

    }


    //--------------------------------------------------------
	// need two jets
	//--------------------------------------------------------
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

    //---------------------------------------------------------------------
    // calculate the rjigsaw variables (with reco) for this topology
    //---------------------------------------------------------------------
    calculateRJigsaw(goodJets, "", eventInfo, metCalibCollectionName, "Final");

    //---------------------------------------------
    // truth level
    // only compute if event has no *reco* leptons, 
    // (ignore how many truth leptons there are)
    // -> note the check and return statement is above
    // compute truthGoodJets
    // note: only including true jets in truthGoodJets
    // check there are two good truth jets for rjigsaw
    // if <2 do *not* return anything (just fill in Truth variables with zeros?? )
    //---------------------------------------------
    
    // only do truth if it's MC
    if( eventInfo->auxdata<int>("IsMC") == 1 ){
    	xAOD::JetContainer* truthGoodJets = new xAOD::JetContainer(SG::VIEW_ELEMENTS);

	    xAOD::JetContainer* truthJets = 0;
	    assert(m_store->retrieve( truthJets, "MyTruthJets" )) ; 
	  
	    xAOD::JetContainer::iterator truthJet_itr = (truthJets)->begin();
	    xAOD::JetContainer::iterator truthJet_end = (truthJets)->end();

	    for( ; truthJet_itr != truthJet_end; ++truthJet_itr ) {
	    	//std::cout << "LH test: (*truthJet_itr)->pt() =" << (*truthJet_itr)->pt()  << std::endl;
            // kinematic cuts to match reco good jets
		    if( (*truthJet_itr)->pt() > 30000.   &&
		    ( fabs( (*truthJet_itr)->eta()) < 2.8)
		    ) {
		    	//std::cout << "LH test: (*truthJet_itr)->pt() =" << (*truthJet_itr)->pt()  << std::endl;
			   truthGoodJets->push_back (*truthJet_itr);
		    } // end if inside kinematical cuts

        } // end loop over truth jets


        //---------------------------------------------------------------------
        // calculate the rjigsaw variables (with truth) for this topology
        //---------------------------------------------------------------------
        std::string truthRJigVarPrefix = "Truth";
        if(truthGoodJets->size()>1) calculateRJigsaw(truthGoodJets, truthRJigVarPrefix, eventInfo, "MyTruthMET", "NonInt");
        
        else{ // need to fill var for this event (this event which is passing reco requirements)
		    eventInfo->auxdecor<float>(truthRJigVarPrefix+"SS_Mass"           ) = -999.;
			eventInfo->auxdecor<float>(truthRJigVarPrefix+"SS_InvGamma"       ) = -999.;
			eventInfo->auxdecor<float>(truthRJigVarPrefix+"SS_dPhiBetaR"      ) = -999.;
			eventInfo->auxdecor<float>(truthRJigVarPrefix+"SS_dPhiVis"        ) = -999.;
			eventInfo->auxdecor<float>(truthRJigVarPrefix+"SS_CosTheta"       ) = -999.;
			eventInfo->auxdecor<float>(truthRJigVarPrefix+"SS_dPhiDecayAngle" ) = -999.;
			eventInfo->auxdecor<float>(truthRJigVarPrefix+"SS_VisShape"       ) = -999.;
			eventInfo->auxdecor<float>(truthRJigVarPrefix+"SS_MDeltaR"        ) = -999.;
			eventInfo->auxdecor<float>(truthRJigVarPrefix+"S1_Mass"           ) = -999.;
			eventInfo->auxdecor<float>(truthRJigVarPrefix+"S1_CosTheta"       ) = -999.;
			eventInfo->auxdecor<float>(truthRJigVarPrefix+"S2_Mass"           ) = -999.;
			eventInfo->auxdecor<float>(truthRJigVarPrefix+"S2_CosTheta"       ) = -999.;
			eventInfo->auxdecor<float>(truthRJigVarPrefix+"I1_Depth"          ) = -999.;
			eventInfo->auxdecor<float>(truthRJigVarPrefix+"I2_Depth"          ) = -999.;
			eventInfo->auxdecor<float>(truthRJigVarPrefix+"V1_N"              ) = -999.;
			eventInfo->auxdecor<float>(truthRJigVarPrefix+"V2_N"              ) = -999.;
		    eventInfo->auxdecor<float>(truthRJigVarPrefix+"QCD_dPhiR"              ) = -999.;
		    eventInfo->auxdecor<float>(truthRJigVarPrefix+"QCD_Rpt"                ) = -999.;
		    eventInfo->auxdecor<float>(truthRJigVarPrefix+"QCD_Rmsib"              ) = -999.;
		    eventInfo->auxdecor<float>(truthRJigVarPrefix+"QCD_Delta"              ) = -999.;
	
        	
        } // end else (not enough jets to do calculation)	
    	
    } // end if MC	
  






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
