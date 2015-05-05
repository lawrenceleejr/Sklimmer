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
#include <EventLoop/StatusCode.h>

//xAOD Includes
#include "xAODEventInfo/EventInfo.h"
#include "xAODJet/JetContainer.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODEgamma/PhotonContainer.h"
#include "xAODTau/TauJetContainer.h"
#include "xAODMissingET/MissingETContainer.h"
#include "xAODBTaggingEfficiency/BTaggingEfficiencyTool.h"

#include "TRandom.h"

#include "Sklimmer/EventSelectionBBMet.h"
#include "Sklimmer/errorcheck.h"


EventSelectionBBMet::EventSelectionBBMet(xAOD::TStore * store) :  // todo probably add a version which sets the collection names
  LAB_alt(RestFrames::RLabFrame("LAB","lab")),
  S_alt(RestFrames::RSelfAssemblingFrame("CM","CM")),
  V_alt(RestFrames::RVisibleFrame("V_alt","Vis")),
  I_alt(RestFrames::RInvisibleFrame("I_alt","Iinv")),
  INV_alt(RestFrames::InvisibleGroup ("INV_alt","Invisible State Jigsaws")),
  VIS_alt(RestFrames::CombinatoricGroup("VIS_alt","Visible Object Jigsaws")),
  MinMass_alt(RestFrames::InvisibleMassJigsaw("MINMASS_JIGSAW_ALT", "Invisible system mass Jigsaw")),
  Rapidity_alt(RestFrames::InvisibleRapidityJigsaw("RAPIDITY_JIGSAW_ALT", "Invisible system rapidity Jigsaw")),
  LAB(RestFrames::RLabFrame("LAB","lab")),
  SS(RestFrames::RDecayFrame("SS","SS")),
  S1(RestFrames::RSelfAssemblingFrame("S1","#tilde{S}_{a}")),
  S2(RestFrames::RSelfAssemblingFrame("S2","#tilde{S}_{b}")),
  V1(RestFrames::RVisibleFrame("V1","V_{a}")),
  V2(RestFrames::RVisibleFrame("V2","V_{b}")),
  I1(RestFrames::RInvisibleFrame("I1","I_{a}")),
  I2(RestFrames::RInvisibleFrame("I2","I_{b}")),
  INV(RestFrames::InvisibleGroup("INV","Invisible State Jigsaws")),
  VIS(RestFrames::CombinatoricGroup("VIS","Visible Object Jigsaws")),
  MinMassJigsaw(RestFrames::InvisibleMassJigsaw("MINMASS_JIGSAW", "Invisible system mass Jigsaw")),
  RapidityJigsaw(RestFrames::InvisibleRapidityJigsaw("RAPIDITY_JIGSAW", "Invisible system rapidity Jigsaw")),
  ContraBoostJigsaw(RestFrames::ContraBoostInvariantJigsaw("CB_JIGSAW","Contraboost invariant Jigsaw")),
  HemiJigsaw(RestFrames::MinimizeMassesCombinatoricJigsaw("HEM_JIGSAW","Minimize m _{V_{a,b}} Jigsaw")),
  LAB_R(RestFrames::RLabFrame("LAB_R","LAB")),
  GG_R(RestFrames::RDecayFrame("GG_R","#tilde{g}#tilde{g}")),
  Ga_R(RestFrames::RDecayFrame("Ga_R","#tilde{g}_{a}")),
  Gb_R(RestFrames::RDecayFrame("Gb_R","#tilde{g}_{b}")),
  Ca_R(RestFrames::RDecayFrame("Ca_R","C_{a}")),
  Cb_R(RestFrames::RDecayFrame("Cb_R","C_{b}")),
  V1a_R(RestFrames::RVisibleFrame("V1a_R","j_{1a}")),
  V2a_R(RestFrames::RVisibleFrame("V2a_R","j_{2a}")),
  Xa_R(RestFrames::RInvisibleFrame("Xa_R","#tilde{#chi}_{a}")),
  V1b_R(RestFrames::RVisibleFrame("V1b_R","j_{1b}")),
  V2b_R(RestFrames::RVisibleFrame("V2b_R","j_{2b}")),
  Xb_R(RestFrames::RInvisibleFrame("Xb_R","#tilde{#chi}_{b}")),
  INV_R(RestFrames::InvisibleGroup ("INV_R","WIMP Jigsaws")),
  VIS_R(RestFrames::CombinatoricGroup("VIS","Visible Object Jigsaws")),
  MinMassJigsaw_R(RestFrames::InvisibleMassJigsaw("MINMASS_R", "Invisible system mass Jigsaw")),
  RapidityJigsaw_R(RestFrames::InvisibleRapidityJigsaw("RAPIDITY_R", "Invisible system rapidity Jigsaw")),
  ContraBoostJigsaw_R(RestFrames::ContraBoostInvariantJigsaw("CONTRA_R","Contraboost invariant Jigsaw")),
  HemiJigsaw_R(RestFrames::MinimizeMassesCombinatoricJigsaw ("HEM_JIGSAW_R","Minimize m _{V_{a,b}} Jigsaw")),
  CaHemiJigsaw_R(RestFrames::MinimizeMassesCombinatoricJigsaw("CbHEM_JIGSAW_R","Minimize m _{C_{a}} Jigsaw")),
  CbHemiJigsaw_R(RestFrames::MinimizeMassesCombinatoricJigsaw("CaHEM_JIGSAW_R","Minimize m _{C_{b}} Jigsaw")),
  m_store(store)
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  //todo do this better?
  //  m_store = store;
}

EventSelectionBBMet::~EventSelectionBBMet()
{
}

EL::StatusCode EventSelectionBBMet::initialize()
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
	// BACKGROUND TREE FOR QCD VARIABLES //////////////////////////
	///////////////////////////////////////////////////////////////
	//
	//

	INV_alt.AddFrame(I_alt);
	VIS_alt.AddFrame(V_alt);
	VIS_alt.SetNElementsForFrame(V_alt,1,false);

	LAB_alt.SetChildFrame(S_alt);
	S_alt.AddChildFrame(V_alt);
	S_alt.AddChildFrame(I_alt);


	if(!LAB_alt.InitializeTree()){//todo do we need both of these?
	  Error(__PRETTY_FUNCTION__ , "failed to initialize LAB tree");
	  return EL::StatusCode::FAILURE;
	}

	// Will just set invisible mass to zero
	INV_alt.AddJigsaw(MinMass_alt);

	// will set rapidity to zero
	INV_alt.AddJigsaw(Rapidity_alt);
	Rapidity_alt.AddVisibleFrame((LAB_alt.GetListVisibleFrames()));

	if(!LAB_alt.InitializeAnalysis()){
	  Error(__PRETTY_FUNCTION__ , "failed to initialize LAB tree");
	  return EL::StatusCode::FAILURE;
	}

	//
	//
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	// SQUARK TREE /////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	//
	//

	// The invisible group is all of the weakly interacting particles
	INV.AddFrame(I1);
	INV.AddFrame(I2);

	// the combinatoric group is the list of visible objects
	// that go into our hemispheres
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
	if(!LAB.InitializeTree()){//todo do we need both of these?
	  Error(__PRETTY_FUNCTION__ , "failed to initialize LAB tree");
	  return EL::StatusCode::FAILURE;
	}

	INV.AddJigsaw(MinMassJigsaw);

	INV.AddJigsaw(RapidityJigsaw);
	RapidityJigsaw.AddVisibleFrame((LAB.GetListVisibleFrames()));

	INV.AddJigsaw(ContraBoostJigsaw);
	ContraBoostJigsaw.AddVisibleFrame((S1.GetListVisibleFrames()), 0);
	ContraBoostJigsaw.AddVisibleFrame((S2.GetListVisibleFrames()), 1);
	ContraBoostJigsaw.AddInvisibleFrame((S1.GetListInvisibleFrames()), 0);
	ContraBoostJigsaw.AddInvisibleFrame((S2.GetListInvisibleFrames()), 1);

	VIS.AddJigsaw(HemiJigsaw);
	HemiJigsaw.AddFrame(V1,0);
	HemiJigsaw.AddFrame(V2,1);

	if(!LAB.InitializeTree()){
	  Error(__PRETTY_FUNCTION__ , "failed to initialize LAB tree");
	  return EL::StatusCode::FAILURE;
	}

	//std::cout << "Is consistent analysis tree? : " << LAB.InitializeAnalysis() << std::endl;

	//
	//
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	// GLUINO TREE /////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	//
	//


	// Set up 'signal-like' analysis tree
	LAB_R.SetChildFrame(GG_R);
	GG_R.AddChildFrame(Ga_R);
	GG_R.AddChildFrame(Gb_R);
	Ga_R.AddChildFrame(V1a_R);
	Ga_R.AddChildFrame(Ca_R);
	Ca_R.AddChildFrame(V2a_R);
	Ca_R.AddChildFrame(Xa_R);
	Gb_R.AddChildFrame(V1b_R);
	Gb_R.AddChildFrame(Cb_R);
	Cb_R.AddChildFrame(V2b_R);
	Cb_R.AddChildFrame(Xb_R);


	if(!LAB_R.InitializeTree()){
	  Error(__PRETTY_FUNCTION__ , "failed to initialize LAB_R tree");
	  return EL::StatusCode::FAILURE;
	}

	INV_R.AddFrame(Xa_R);
	INV_R.AddFrame(Xb_R);
	// visible frames in first decay step must always have at least one element
	VIS_R.AddFrame(V1a_R);
	VIS_R.AddFrame(V1b_R);
	VIS_R.SetNElementsForFrame(V1a_R,1,false);
	VIS_R.SetNElementsForFrame(V1b_R,1,false);
	// visible frames in second decay step can have zero elements
	VIS_R.AddFrame(V2a_R);
	VIS_R.AddFrame(V2b_R);
	VIS_R.SetNElementsForFrame(V2a_R,0,false);
	VIS_R.SetNElementsForFrame(V2b_R,0,false);

	INV_R.AddJigsaw(MinMassJigsaw_R);
	INV_R.AddJigsaw(RapidityJigsaw_R);
	RapidityJigsaw_R.AddVisibleFrame((LAB_R.GetListVisibleFrames()));
	INV_R.AddJigsaw(ContraBoostJigsaw_R);
	ContraBoostJigsaw_R.AddVisibleFrame((Ga_R.GetListVisibleFrames()), 0);
	ContraBoostJigsaw_R.AddVisibleFrame((Gb_R.GetListVisibleFrames()), 1);
	ContraBoostJigsaw_R.AddInvisibleFrame((Ga_R.GetListInvisibleFrames()), 0);
	ContraBoostJigsaw_R.AddInvisibleFrame((Gb_R.GetListInvisibleFrames()), 1);
	VIS_R.AddJigsaw(HemiJigsaw_R);
	HemiJigsaw_R.AddFrame(V1a_R,0);
	HemiJigsaw_R.AddFrame(V1b_R,1);
	HemiJigsaw_R.AddFrame(V2a_R,0);
	HemiJigsaw_R.AddFrame(V2b_R,1);
	VIS_R.AddJigsaw(CaHemiJigsaw_R);
	CaHemiJigsaw_R.AddFrame(V1a_R,0);
	CaHemiJigsaw_R.AddFrame(V2a_R,1);
	CaHemiJigsaw_R.AddFrame(Xa_R,1);
	VIS_R.AddJigsaw(CbHemiJigsaw_R);
	CbHemiJigsaw_R.AddFrame(V1b_R,0);
	CbHemiJigsaw_R.AddFrame(V2b_R,1);
	CbHemiJigsaw_R.AddFrame(Xb_R,1);

	if(!LAB_R.InitializeAnalysis()){
	  Error(__PRETTY_FUNCTION__, "Failred to properly initialize RJigsaw Analysis");
	  return EL::StatusCode::FAILURE;
	}

	return EL::StatusCode::SUCCESS;
}

std::string EventSelectionBBMet::run(xAOD::EventInfo * eventInfo){
  // Inspired by https://cds.cern.ch/record/1508045/files/ATL-COM-PHYS-2013-072.pdf
  assert(m_store&&"you need to have a store to run the event selection");
  assert(eventInfo);

  m_store->print();

  xAOD::JetContainer* jets_copy(0);
  assert( m_store->retrieve( jets_copy, "CalibJets" ) );

  xAOD::MuonContainer* muons_copy(0);
  assert( m_store->retrieve( muons_copy, "CalibMuons" ) );

  xAOD::ElectronContainer* electrons_copy(0);
  assert( m_store->retrieve( electrons_copy, "CalibElectrons" ) );

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

    if( (*jet_itr)->auxdata< char >("baseline")==1  &&
	(*jet_itr)->auxdata< char >("passOR")==1  &&
	(*jet_itr)->pt() > 30000.  && ( fabs( (*jet_itr)->eta()) < 2.8) ) {
      goodJets->push_back (*jet_itr);
    }

    (*jet_itr)->auxdecor<float>("MV1") =  ((*jet_itr)->btagging())->MV1_discriminant() ;

  }

  if(goodJets->size() < 2){
    return "";
  }


  std::sort(goodJets->begin(), goodJets->end(),
	    [](xAOD::Jet  *a, xAOD::Jet  *b){return a->pt() > b->pt();});


  LAB.ClearEvent();
  LAB_R.ClearEvent();
  LAB_alt.ClearEvent();

  vector<RestFrames::GroupElementID> jetID_R;                    // ID for tracking jets in tree

  jet_itr = (jets_copy)->begin();

  for( ; jet_itr != jet_end; ++jet_itr ) {

    if( (*jet_itr)->auxdata< char >("baseline")==1  &&
	(*jet_itr)->auxdata< char >("passOR")==1  &&
	(*jet_itr)->pt() > 30000.  && ( fabs( (*jet_itr)->eta()) < 2.8) ) {
      VIS.AddLabFrameFourVector( (*jet_itr)->p4()  );
      jetID_R.push_back( VIS_R.AddLabFrameFourVector( (*jet_itr)->p4()  )  );
      VIS_alt.AddLabFrameFourVector( (*jet_itr)->p4()   );
    }

  }

  // Get MET Collection to hand to Rest Frames////////////////////////////////////////////////////

  xAOD::MissingETContainer* MET = new xAOD::MissingETContainer;
  assert( m_store->retrieve( MET, "CalibMET_RefFinal" ) );

  TVector3 MET_TV3;

  xAOD::MissingETContainer::const_iterator met_it = MET->find("Final");
  if (met_it == MET->end()) {
    Error( __PRETTY_FUNCTION__, "No RefFinal inside MET container" );
  } else {
    MET_TV3.SetZ(0.);
    MET_TV3.SetX((*met_it)->mpx() );
    MET_TV3.SetY((*met_it)->mpy() );
  }

  INV.SetLabFrameThreeVector(MET_TV3);
  LAB.AnalyzeEvent();

  INV_alt.SetLabFrameThreeVector(MET_TV3);
  LAB_alt.AnalyzeEvent();


  if(goodJets->size()>3){
    INV_R.SetLabFrameThreeVector(MET_TV3);
    LAB_R.AnalyzeEvent();


    RestFrames::RDecayFrame* G[2];
    RestFrames::RDecayFrame* C[2];
    RestFrames::RVisibleFrame* VS[2];
    RestFrames::RVisibleFrame* VC[2];
    RestFrames::RInvisibleFrame* X[2];
    // Randomize the two hemispheres
    int flip = (gRandom->Rndm() > 0.5);
    G[flip] = &Ga_R;
    G[(flip+1)%2] = &Gb_R;
    C[flip] = &Ca_R;
    C[(flip+1)%2] = &Cb_R;
    VS[flip] = &V1a_R;
    VS[(flip+1)%2] = &V1b_R;
    VC[flip] = &V2a_R;
    VC[(flip+1)%2] = &V2b_R;
    X[flip] = &Xa_R;
    X[(flip+1)%2] = &Xb_R;


    double NV[2];
    double jet1PT[2];
    double jet2PT[2];


    for(int i = 0; i < 2; i++){

      NV[i] =  VIS_R.GetNElementsInFrame(VS[i]);
      NV[i] += VIS_R.GetNElementsInFrame(VC[i]);

      int N = jetID_R.size();
      double pTmax[2]; pTmax[0] = -1.; pTmax[1] = -1.;
      for(int j = 0; j < N; j++){
	const RestFrames::RestFrame* frame = VIS_R.GetFrame(jetID_R[j]);
	if(VS[i]->IsSame(frame) || VC[i]->IsSame(frame)){
	  double pT = frame->GetFourVector(LAB_R).Pt();
	  if(pT > pTmax[0]){
	    pTmax[1] = pTmax[0];
	    pTmax[0] = pT;
	  } else {
	    if(pT > pTmax[1]) pTmax[1] = pT;
	  }
	}
      }
      jet1PT[i] = pTmax[0];
      jet2PT[i] = pTmax[1];


      if(NV[i] > 1){
	eventInfo->auxdecor<float>(Form("C_%d_CosTheta",i)     ) = C[i]->GetCosDecayAngle();
	eventInfo->auxdecor<float>(Form("G_%d_dPhiGC",i)     ) = G[i]->GetDeltaPhiDecayPlanes(C[i]);
	eventInfo->auxdecor<float>(Form("G_%d_MassRatioGC",i)     ) = (C[i]->GetMass()-X[i]->GetMass())/(G[i]->GetMass()-X[i]->GetMass());
      } else {
	eventInfo->auxdecor<float>(Form("C_%d_CosTheta",i)     ) = -10.;
	eventInfo->auxdecor<float>(Form("G_%d_dPhiGC",i)     ) = -10.;
	eventInfo->auxdecor<float>(Form("G_%d_MassRatioGC",i)     ) = -10.;
      }

      eventInfo->auxdecor<float>(Form("G_%d_CosTheta",i)     ) = G[i]->GetCosDecayAngle();

      eventInfo->auxdecor<float>(Form("G_%d_Jet1_pT",i)     ) = jet1PT[i];
      eventInfo->auxdecor<float>(Form("G_%d_Jet2_pT",i)     ) = jet2PT[i];

    }

  } else {

    for(int i = 0; i < 2; i++){
      eventInfo->auxdecor<float>(Form("C_%d_CosTheta",i)     ) = -10.;
      eventInfo->auxdecor<float>(Form("G_%d_dPhiGC",i)     ) = -10.;
      eventInfo->auxdecor<float>(Form("G_%d_MassRatioGC",i)     ) = -10.;
      eventInfo->auxdecor<float>(Form("G_%d_CosTheta",i)     ) = -10.;
      eventInfo->auxdecor<float>(Form("G_%d_Jet1_pT",i)     ) = -10.;
      eventInfo->auxdecor<float>(Form("G_%d_Jet2_pT",i)     ) = -10.;
    }

  }

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
  // eventInfo->auxdata< float >("sHatR"), eventInfo->auxdata< float >("gammainv_Rp1") );

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
  // if(goodJets->at(0)->pt() > 150000 &&
  // goodJets->at(1)->pt() > 30000 &&
  // goodJets->at(2)->pt() > 30000 &&
  // (goodJets->at(1)->btagging())->MV1_discriminant() > 0.98 &&
  // (goodJets->at(2)->btagging())->MV1_discriminant() > 0.98 ){
  // return "SRB";
  // }
  // }

  return "";
}
