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

#include "Sklimmer/RazorTriggerAnalysis.h"
#include "Sklimmer/errorcheck.h"

RazorTriggerAnalysis::RazorTriggerAnalysis(xAOD::TStore * store) : // todo probably add a version which sets the collection names
  m_store(store)
{

}

//todo refactor this code to separate out common code
std::string RazorTriggerAnalysis::run(xAOD::EventInfo * eventInfo){
	// Inspired by https://cds.cern.ch/record/1508045/files/ATL-COM-PHYS-2013-072.pdf
  //  m_store->print();

  if(m_store == nullptr){
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    std::cout << "event store null, returning without doing event selection" << std::endl;
    return std::string("");
  }


	xAOD::JetContainer* jets_copy(0);
	//todo change these to checks
	assert( m_store->retrieve( jets_copy, jetCalibCollectionName ) );

	xAOD::MuonContainer* muons_copy(0);
	assert( m_store->retrieve( muons_copy, muonCalibCollectionName ) );
	xAOD::ElectronContainer* electrons_copy(0);
	assert( m_store->retrieve( electrons_copy, electronCalibCollectionName ) );

	/////////////// Lepton Veto //////////////////////////////

	// int Nel=0;
	// xAOD::ElectronContainer::iterator el_itr = electrons_copy->begin();
	// xAOD::ElectronContainer::iterator el_end = electrons_copy->end();
	// for( ; el_itr != el_end; ++el_itr ) {
	// 	if( ( *el_itr )->auxdata<char>("passOR") ) Nel++;
	// }

	// int Nmu=0;
	// xAOD::MuonContainer::iterator mu_itr = muons_copy->begin();
	// xAOD::MuonContainer::iterator mu_end = muons_copy->end();
	// for( ; mu_itr != mu_end; ++mu_itr ) {
	// 	if( ( *mu_itr )->auxdata<char>("passOR") ) Nmu++;
	// }

	// if(Nel || Nmu) return "";

	///////////////////////////////////////////////////////////


	xAOD::JetContainer* goodJets = new xAOD::JetContainer(SG::VIEW_ELEMENTS);
	// assert( m_store->record(goodJets, "MySelJets") );

	xAOD::JetContainer::iterator jet_itr = (jets_copy)->begin();
	xAOD::JetContainer::iterator jet_end = (jets_copy)->end();

	for( ; jet_itr != jet_end; ++jet_itr ) {

		if( (*jet_itr)->auxdata<char >("baseline")==1 &&
		    (*jet_itr)->auxdata<char >("passOR")  ==1 &&
		    (*jet_itr)->pt() > 30000.                  &&
		    ( fabs( (*jet_itr)->eta()) < 2.8)
		    ) {
			goodJets->push_back (*jet_itr);
		}

		(*jet_itr)->auxdecor<float>("MV1") =  ((*jet_itr)->btagging())->MV1_discriminant() ;

    }

	//need two jets
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

	//	std::cout << "Is consistent tree topology? " << LAB.InitializeTree() << std::endl;


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
	//todo make this check an assert probably
	//	std::cout << "Is consistent analysis tree? : " << LAB.InitializeAnalysis() << std::endl;

	LAB.ClearEvent();


	//////////////////////////////////////////////////////////////
	// Now, we make a 'background'-like, transverse momentum only, tree
	//////////////////////////////////////////////////////////////
	// RestFrames::RLabFrame LAB_alt("LAB","lab");
	// RestFrames::RSelfAssemblingFrame S_alt("CM","CM");
	// RestFrames::RVisibleFrame V_alt("V_alt","Vis");
	// RestFrames::RInvisibleFrame I_alt("I_alt","Iinv");

	// RestFrames::InvisibleGroup INV_alt("INV_alt","Invisible State Jigsaws");
	// INV_alt.AddFrame(I_alt);

	// RestFrames::CombinatoricGroup VIS_alt("VIS_alt","Visible Object Jigsaws");
	// VIS_alt.AddFrame(V_alt);
	// VIS_alt.SetNElementsForFrame(V_alt,1,false);

	// LAB_alt.SetChildFrame(S_alt);
	// S_alt.AddChildFrame(V_alt);
	// S_alt.AddChildFrame(I_alt);

	// LAB_alt.InitializeTree();

	// // Will just set invisible mass to zero
	// RestFrames::InvisibleMassJigsaw MinMass_alt("MINMASS_JIGSAW_ALT", "Invisible system mass Jigsaw");
	// INV_alt.AddJigsaw(MinMass_alt);

	// // will set rapidity to zero
	// RestFrames::InvisibleRapidityJigsaw Rapidity_alt("RAPIDITY_JIGSAW_ALT", "Invisible system rapidity Jigsaw");
	// INV_alt.AddJigsaw(Rapidity_alt);
	// Rapidity_alt.AddVisibleFrame((LAB_alt.GetListVisibleFrames()));

	// LAB_alt.InitializeAnalysis();

    // 	TLorentzVector jet;

    // 	jet_itr = (jets_copy)->begin();
    // 	for( ; jet_itr != jet_end; ++jet_itr ) {

    // 		if( (*jet_itr)->auxdata<char >("baseline")==1  &&
    // 			(*jet_itr)->auxdata<char >("passOR")==1  &&
    // 			(*jet_itr)->pt() > 30000.  && ( fabs( (*jet_itr)->eta()) < 2.8) ) {
    // 			VIS.AddLabFrameFourVector( (*jet_itr)->p4()  );

    // 			jet.SetPtEtaPhiM( (*jet_itr)->pt(), 0., (*jet_itr)->phi(), (*jet_itr)->m()  );
    // 			VIS_alt.AddLabFrameFourVector(jet);
    // 		}

    // }


	// Get MET Collection to hand to Rest Frames////////////////////////////////////////////////////

    // 	xAOD::MissingETContainer* MET = new xAOD::MissingETContainer;
    // 	assert( m_store->retrieve( MET, metCalibCollectionName ) );

    // 	TVector3 MET_TV3;

    // 	xAOD::MissingETContainer::const_iterator met_it = MET->find("Final");//todo?
    // 	if (met_it == MET->end()) {
    // 		Error( __PRETTY_FUNCTION__, "No RefFinal inside MET container" );
    // 	} else {
    // 		INV.SetLabFrameThreeVector(  TVector3( (*met_it)->mpx(), (*met_it)->mpy(), 0 ) );
    // 		MET_TV3.SetZ(0.);
    // 		MET_TV3.SetX((*met_it)->mpx() );
    // 		MET_TV3.SetY((*met_it)->mpy() );
    // 	}

    // 	LAB.AnalyzeEvent();


    // INV_alt.SetLabFrameThreeVector(MET_TV3);
    // LAB_alt.AnalyzeEvent();

	//std::cout << "RestFrames shatR is: " << SS.GetMass() << std::endl;


	//todo reinclude other variables
	eventInfo->auxdecor<float>("SS_Mass"           ) = SS.GetMass();
	eventInfo->auxdecor<float>("SS_InvGamma"       ) = 1./SS.GetGammaInParentFrame();
	//	eventInfo->auxdecor<float>("SS_dPhiBetaR"      ) = SS.GetDeltaPhiBoostVisible();
	//	eventInfo->auxdecor<float>("SS_dPhiVis"        ) = SS.GetDeltaPhiVisible();
	//	eventInfo->auxdecor<float>("SS_CosTheta"       ) = SS.GetCosDecayAngle();
	//	eventInfo->auxdecor<float>("SS_dPhiDecayAngle" ) = SS.GetDeltaPhiDecayAngle();
	eventInfo->auxdecor<float>("SS_VisShape"       ) = SS.GetVisibleShape();
	eventInfo->auxdecor<float>("SS_MDeltaR"        ) = SS.GetVisibleShape() * SS.GetMass() ;
	// eventInfo->auxdecor<float>("S1_Mass"           ) = S1.GetMass();
	// eventInfo->auxdecor<float>("S1_CosTheta"       ) = S1.GetCosDecayAngle();
	// eventInfo->auxdecor<float>("S2_Mass"           ) = S2.GetMass();
	// eventInfo->auxdecor<float>("S2_CosTheta"       ) = S2.GetCosDecayAngle();
	// eventInfo->auxdecor<float>("I1_Depth"          ) = S1.GetFrameDepth(I1);
	// eventInfo->auxdecor<float>("I2_Depth"          ) = S2.GetFrameDepth(I2);
	// eventInfo->auxdecor<float>("V1_N"              ) = VIS.GetNElementsInFrame(V1);
	// eventInfo->auxdecor<float>("V2_N"              ) = VIS.GetNElementsInFrame(V2);

    // // dphiR and Rptshat (formerly cosPT)
    // // for QCD rejection
    // double dphiR = SS.GetDeltaPhiBoostVisible();
    // double PTCM = SS.GetFourVector(LAB).Pt();
    // double Rptshat = PTCM / (PTCM + SS.GetMass()/4.);

    // // QCD rejection using the 'background tree'
    // // MET 'sibling' in background tree auxillary calculations
    // TLorentzVector Psib = I_alt.GetSiblingFrame()->GetFourVector(LAB_alt);
    // TLorentzVector Pmet = I_alt.GetFourVector(LAB_alt);
    // double Psib_dot_METhat = max(0., Psib.Vect().Dot(MET_TV3.Unit()));
    // double Mpar2 = Psib.E()*Psib.E()-Psib.Vect().Dot(MET_TV3.Unit())*Psib.Vect().Dot(MET_TV3.Unit());
    // double Msib2 = Psib.M2();
    // double MB2 = 2.*(Pmet.E()*Psib.E()-MET_TV3.Dot(Psib.Vect()));
    // TVector3 boostPsibM = (Pmet+Psib).BoostVector();


    // // QCD rejection variables from 'background tree'
    // double DepthBKG = S_alt.GetFrameDepth(I_alt);
    // int Nsib = I_alt.GetSiblingFrame()->GetNDescendants();
    // double cosBKG = I_alt.GetParentFrame()->GetCosDecayAngle();
    // double dphiMsib = fabs(MET_TV3.DeltaPhi(Psib.Vect()));
    // double RpsibM = Psib_dot_METhat / (Psib_dot_METhat + MET_TV3.Mag());
    // double RmsibM = 1. / ( MB2/(Mpar2-Msib2) + 1.);
    // Psib.Boost(-boostPsibM);
    // double cosPsibM = -1.*Psib.Vect().Unit().Dot(boostPsibM.Unit());
    // cosPsibM = (1.-cosPsibM)/2.;
    // double DeltaQCD1 = (cosPsibM-RpsibM)/(cosPsibM+RpsibM);
    // double DeltaQCD2 = (cosPsibM-RmsibM)/(cosPsibM+RmsibM);

    // eventInfo->auxdecor<float>("QCD_dPhiR"              ) = dphiR;
    // eventInfo->auxdecor<float>("QCD_Rpt"                ) = Rptshat;
    // eventInfo->auxdecor<float>("QCD_Rmsib"              ) = RmsibM;
    // eventInfo->auxdecor<float>("QCD_Delta"              ) = DeltaQCD2;

	// Info( __PRETTY_FUNCTION__,"RJigsaw Variables from RestFrames: sHatR %f gammainv_Rp1 %f",
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
