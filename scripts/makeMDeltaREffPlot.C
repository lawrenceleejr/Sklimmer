#include <TChain.h>

int makeMDeltaREffPlot(){

  //  TFile * file = TFile::Open("/afs/cern.ch/user/r/rsmith/testarea/razor/Sklimmer/Run/output.15.04.15.23.55/data-treeOutput/mc14_13TeV.110351.PowhegPythia_P2012_ttbar_allhad.recon.AOD.e3232_s2127_s2132_r6561_tid05274300_00.root");
  //std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;
  std::vector<std::string> triggerStrings;
  triggerStrings.push_back("HLT_j30_xe10_razor170");

  //  map<TString, TEfficiency*> effPlots;
  std::vector<std::string> sampleName = {"119994.Pythia8_A2MSTW2008LO_minbias",
					 "110401.PowhegPythia_P2012_ttbar",
					 "162317.MadGraphPythia_AUET2BCTEQ6L1_SM_SS_direct",
					 "410000.PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5"
  };




  for(int nsample = 0; nsample < sampleName.size(); ++nsample){
    TFile * outfile = new TFile(("outputEff_" +sampleName.at(nsample)+".root").c_str() , "RECREATE" );

    TChain fChain("tree");
    fChain.Add(("/afs/cern.ch/user/r/rsmith/testarea/razor_r20/Sklimmer/scripts/razor_trigger_5_8_15/*"+sampleName.at(nsample)+"*.root").c_str());
    fChain.SetMakeClass(1);

    std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;

    // for (UInt_t iTrig = 0; iTrig < triggerStrings.size(); ++iTrig) {
    //   std::string iTrigStr = triggerStrings.at(iTrig);

    TEfficiency * effPlots = new TEfficiency("eff_rj_mdeltaR", " m#DeltaR [GeV]efficiency", 100, 0, 2000);

    for(Long64_t ientry = 0; ientry < fChain.GetEntries(); ++ientry){
      //for(Long64_t ievent = 0;  ievent < std::min(int(event->getEntries()), 100); ++ievent){
      //std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;
      //std::cout << "Entry number " << ientry << std::endl;
      //std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;
      fChain.GetTree()->GetEntry(ientry);

      TBranch *b_rj_mdeltaR;
      Float_t rj_mdeltaR;
      fChain.SetBranchAddress("RJVars_SS_MDeltaR", &rj_mdeltaR, &b_rj_mdeltaR);

      TBranch *b_trigger;
      Int_t trigger;
      fChain.SetBranchAddress("RJVars_TriggerBits", &trigger, &b_trigger);

      std::bitset<16> bset(trigger);
      const Bool_t was_triggered_razor = (bset[10] == 1);//from this line in SklimmerAnalysis:
      //   triggers[10] = m_susy_obj->isTrigPassed("HLT_j30_xe10_razor170");

      effPlots->Fill(was_triggered_razor, rj_mdeltaR/1000.);
    }

    effPlots->Write();

    outfile->Close();
  }//tfile


  return 0;
}
