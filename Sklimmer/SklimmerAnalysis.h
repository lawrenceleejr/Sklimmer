#ifndef Sklimmer_SklimmerAnalysis_H
#define Sklimmer_SklimmerAnalysis_H

#include <TH1.h>
#include <map>
#include <string>
#include <TString.h>

#include <EventLoop/Algorithm.h>

#include <D3PDReader/Event.h>
#include <EventLoopAlgs/NTupleSvc.h>


#include <RJigsaw/TRJigsaw.h>

#include "SUSYTools/SUSYObjDef.h"


class SklimmerAnalysis : public EL::Algorithm
{
	// put your configuration variables here as public variables.
	// that way they can be set directly from CINT and python.
public:
	// float cutValue;

	TH1 *h_nevents; //!

	// New Branches

	// Float_t RJVars_M_0_0_0;  //!
	// Float_t RJVars_M_0_0_1;  //!
	// Float_t RJVars_M_1_0_0;  //!
	// Float_t RJVars_M_1_0_1;  //!
	// Float_t RJVars_M_1_1_0;  //!
	// Float_t RJVars_M_1_1_1;  //!
	// Float_t RJVars_M_2_0_0;  //!
	// Float_t RJVars_M_2_0_1;  //!
	// Float_t RJVars_M_2_1_0;  //!
	// Float_t RJVars_M_2_1_1;  //!
	// Float_t RJVars_MDecay_1_1_0;  //!
	// Float_t RJVars_MDecay_1_1_1;  //!
	// Float_t RJVars_MDecay_2_1_0;  //!
	// Float_t RJVars_MDecay_2_1_1;  //!
	// Float_t RJVars_dPhi_0_0_0;  //!
	// Float_t RJVars_dPhi_0_0_1;  //!
	// Float_t RJVars_dPhi_1_0_0;  //!
	// Float_t RJVars_dPhi_1_0_1;  //!
	// Float_t RJVars_dPhi_2_0_0;  //!
	// Float_t RJVars_dPhi_2_0_1;  //!
	// Float_t RJVars_dPhi_1_1_0;  //!
	// Float_t RJVars_dPhi_1_1_1;  //!
	// Float_t RJVars_dPhi_2_1_0;  //!
	// Float_t RJVars_dPhi_2_1_1;  //!
	// Float_t RJVars_dPhiVis_0_0_0;  //!
	// Float_t RJVars_dPhiVis_0_0_1;  //!
	// Float_t RJVars_dPhiVis_1_0_0;  //!
	// Float_t RJVars_dPhiVis_1_0_1;  //!
	// Float_t RJVars_dPhiVis_2_0_0;  //!
	// Float_t RJVars_dPhiVis_2_0_1;  //!
	// Float_t RJVars_dPhiVis_1_1_0;  //!
	// Float_t RJVars_dPhiVis_1_1_1;  //!
	// Float_t RJVars_dPhiVis_2_1_0;  //!
	// Float_t RJVars_dPhiVis_2_1_1;  //!
	// Float_t RJVars_cosTheta_0_0_0;  //!
	// Float_t RJVars_cosTheta_0_0_1;  //!
	// Float_t RJVars_cosTheta_1_0_0;  //!
	// Float_t RJVars_cosTheta_1_0_1;  //!
	// Float_t RJVars_cosTheta_2_0_0;  //!
	// Float_t RJVars_cosTheta_2_0_1;  //!
	// Float_t RJVars_cosTheta_1_1_0;  //!
	// Float_t RJVars_cosTheta_1_1_1;  //!
	// Float_t RJVars_cosTheta_2_1_0;  //!
	// Float_t RJVars_cosTheta_2_1_1;  //!
	// Float_t RJVars_dPhiDecay_0_0_0;  //!
	// Float_t RJVars_dPhiDecay_0_0_1;  //!
	// Float_t RJVars_dPhiDecay_1_0_0;  //!
	// Float_t RJVars_dPhiDecay_1_0_1;  //!
	// Float_t RJVars_dPhiDecay_2_0_0;  //!
	// Float_t RJVars_dPhiDecay_2_0_1;  //!
	// Float_t RJVars_cosThetaDecay_0_0_0;  //!
	// Float_t RJVars_cosThetaDecay_0_0_1;  //!
	// Float_t RJVars_cosThetaDecay_1_0_0;  //!
	// Float_t RJVars_cosThetaDecay_1_0_1;  //!
	// Float_t RJVars_cosThetaDecay_2_0_0;  //!
	// Float_t RJVars_cosThetaDecay_2_0_1;  //!
	// Float_t RJVars_gamma_0_0_0;  //!
	// Float_t RJVars_gamma_0_0_1;  //!


	// Float_t RJVars_M_Tot_Mean;  //!
	// Float_t RJVars_M_0_Mean;  //!
	// Float_t RJVars_M_1_Mean;  //!
	// Float_t RJVars_dPhi_Tot_Mean;  //!
	// Float_t RJVars_dPhi_0_Mean;  //!
	// Float_t RJVars_dPhi_1_Mean;  //!
	// Float_t RJVars_dPhiVis_Tot_Mean;  //!
	// Float_t RJVars_dPhiVis_0_Mean;  //!
	// Float_t RJVars_dPhiVis_1_Mean;  //!
	// Float_t RJVars_cosTheta_Tot_Mean;  //!
	// Float_t RJVars_cosTheta_0_Mean;  //!
	// Float_t RJVars_cosTheta_1_Mean;  //!
	// Float_t RJVars_dPhiDecay_Tot_Mean;  //!
	// Float_t RJVars_gamma_Tot_Mean;  //!

	// Float_t RJVars_M_Tot_Var;  //!
	// Float_t RJVars_M_0_Var;  //!
	// Float_t RJVars_M_1_Var;  //!
	// Float_t RJVars_dPhi_Tot_Var;  //!
	// Float_t RJVars_dPhi_0_Var;  //!
	// Float_t RJVars_dPhi_1_Var;  //!
	// Float_t RJVars_dPhiVis_Tot_Var;  //!
	// Float_t RJVars_dPhiVis_0_Var;  //!
	// Float_t RJVars_dPhiVis_1_Var;  //!
	// Float_t RJVars_cosTheta_Tot_Var;  //!
	// Float_t RJVars_cosTheta_0_Var;  //!
	// Float_t RJVars_cosTheta_1_Var;  //!
	// Float_t RJVars_dPhiDecay_Tot_Var;  //!
	// Float_t RJVars_gamma_Tot_Var;  //!


	// Float_t RJVars_sqrtsHat_0_0_0; //!
	// Float_t RJVars_invgamma_0_0_0; //!
	// Float_t RJVars_mdelta_0_0_0; //!
	// Float_t RJVars_cospt_0_0_0; //!



	Float_t RJVars_sHatR_0_0_0;   //!
	Float_t RJVars_E12_0_0_0;   //!
	Float_t RJVars_gamma_R_0_0_0;   //!
	Float_t RJVars_dphi_Beta_R_0_0_0;   //!
	Float_t RJVars_dphi_leg1_leg2_0_0_0;   //!
	Float_t RJVars_gamma_Rp1_0_0_0;   //!
	Float_t RJVars_dphi_Beta_Rp1_Beta_R_0_0_0;   //!
	Float_t RJVars_MDeltaR_0_0_0;   //!
	Float_t RJVars_Eleg1_0_0_0;   //!
	Float_t RJVars_Eleg2_0_0_0;   //!
	Float_t RJVars_costhetaRp1_0_0_0;   //!
	Float_t RJVars_costhetaR_0_0_0;   //!


	Int_t isEE; //!
	Int_t isMuMu; //!
	Int_t isEMu; //!

	vector<int>   *el_selected; //!
	vector<int>   *mu_selected; //!
	vector<int>   *jet_selected; //!
	vector<int>   *bjet_selected; //!

	Float_t   MET; //!
	Float_t   MET_phi; //!
	vector<float>   *LEP_pt; //!
	vector<float>   *LEP_eta; //!
	vector<float>   *LEP_phi; //!
	vector<float>   *LEP_E; //!
	vector<float>   *LEP_charge; //!

	vector<float>   *JET_pt; //!
	vector<float>   *JET_eta; //!
	vector<float>   *JET_phi; //!
	vector<float>   *JET_mass; //!
	vector<float>   *JET_btag; //!

	Float_t   MET_TRUTH; //!
	Float_t   MET_TRUTH_phi; //!
	vector<float>   *LEP_TRUTH_pt; //!
	vector<float>   *LEP_TRUTH_eta; //!
	vector<float>   *LEP_TRUTH_phi; //!
	vector<float>   *LEP_TRUTH_E; //!
	vector<float>   *LEP_TRUTH_charge; //!

	vector<float>   *JET_TRUTH_pt; //!
	vector<float>   *JET_TRUTH_eta; //!
	vector<float>   *JET_TRUTH_phi; //!
	vector<float>   *JET_TRUTH_mass; //!
	vector<float>   *JET_TRUTH_btag; //!


	std::vector<float>  *TRUTH_pdgId   ; //! 
	std::vector<float>  *TRUTH_charge   ; //! 
	std::vector<float>  *TRUTH_pt   ; //! 
	std::vector<float>  *TRUTH_eta  ; //! 
	std::vector<float>  *TRUTH_phi  ; //! 
	std::vector<float>  *TRUTH_m    ; //! 


	TBranch        *b_el_selected;   //!
	TBranch        *b_mu_selected;   //!
	TBranch        *b_jet_selected;   //!
	TBranch        *b_bjet_selected;   //!



	TBranch   *b_MET; //!
	TBranch   *b_MET_phi; //!
	TBranch   *b_LEP_pt; //!
	TBranch   *b_LEP_eta; //!
	TBranch   *b_LEP_phi; //!
	TBranch   *b_LEP_E; //!
	TBranch   *b_LEP_charge; //!

	TBranch   *b_JET_pt; //!
	TBranch   *b_JET_eta; //!
	TBranch   *b_JET_phi; //!
	TBranch   *b_JET_mass; //!
	TBranch   *b_JET_btag; //!

	TBranch   *b_MET_TRUTH; //!
	TBranch   *b_MET_TRUTH_phi; //!
	TBranch   *b_LEP_TRUTH_pt; //!
	TBranch   *b_LEP_TRUTH_eta; //!
	TBranch   *b_LEP_TRUTH_phi; //!
	TBranch   *b_LEP_TRUTH_E; //!
	TBranch   *b_LEP_TRUTH_charge; //!

	TBranch   *b_JET_TRUTH_pt; //!
	TBranch   *b_JET_TRUTH_eta; //!
	TBranch   *b_JET_TRUTH_phi; //!
	TBranch   *b_JET_TRUTH_mass; //!
	TBranch   *b_JET_TRUTH_btag; //!


	Float_t weight; //!
	Int_t nEvents; //!



	// variables that don't get filled at submission time should be
	// protected from being send from the submission node to the worker
	// node (done by the //!)
public:
	// Tree *myTree; //!
	// TH1 *myHist; //!

	D3PDReader::Event *event;  //!
	EL::NTupleSvc *output; //!

	SystErr::Syste whichsyst; //!
	// int whichsyst = 0; //!

	SUSYObjDef* m_susy_obj = new SUSYObjDef(); //!

	Root::TRJigsaw* RJTool = new Root::TRJigsaw(); //!

	// Root::BTagCalib* BTagCalibTool = new Root::BTagCalib(); //!

	// this is a standard constructor
	SklimmerAnalysis ();

	// these are the functions inherited from Algorithm
	virtual EL::StatusCode setupJob (EL::Job& job);
	virtual EL::StatusCode fileExecute ();
	virtual EL::StatusCode histInitialize ();
	virtual EL::StatusCode changeInput (bool firstFile);
	virtual EL::StatusCode initialize ();
	virtual EL::StatusCode execute ();
	virtual EL::StatusCode postExecute ();
	virtual EL::StatusCode finalize ();
	virtual EL::StatusCode histFinalize ();

	void SetSyst(SystErr::Syste );
	Int_t GetSyst();

	Int_t getOriginBarcode(int iMC, int originPDGID);
	Bool_t hasAncestor(int iMC, int ancestor);

	Float_t getMean(Float_t a, Float_t b, Float_t c, Float_t d);
	Float_t getMean(Float_t a, Float_t b);
	Float_t getVar(Float_t a, Float_t b, Float_t c, Float_t d);
	Float_t getVar(Float_t a, Float_t b);

	bool isData; //!
	bool isAtlfast; //!
	bool isMC12b; //!
	bool useLeptonTrigger; //!

	// Systematic Names
	TString whichsystname; //! 




	// this is needed to distribute the algorithm to the workers
	ClassDef(SklimmerAnalysis, 1);
};


#endif
