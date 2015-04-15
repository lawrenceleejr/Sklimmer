#include <TChain.h>

int makeEffPlots(){

  //  TFile * file = TFile::Open("/afs/cern.ch/user/r/rsmith/testarea/razor/Sklimmer/Run/output.15.04.15.23.55/data-treeOutput/mc14_13TeV.110351.PowhegPythia_P2012_ttbar_allhad.recon.AOD.e3232_s2127_s2132_r6561_tid05274300_00.root");
  std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;
  std::vector<std::string> triggerStrings;
  triggerStrings.push_back("L1_XE50");
  triggerStrings.push_back("L1_XE70");
  triggerStrings.push_back("HLT_xe70");
  triggerStrings.push_back("HLT_xe70_pueta");
  triggerStrings.push_back("HLT_xe100");
  triggerStrings.push_back("HLT_xe100_pueta");
  triggerStrings.push_back("HLT_e28_tight_iloose");
  triggerStrings.push_back("HLT_e60_medium");
  triggerStrings.push_back("HLT_mu26_imedium");
  triggerStrings.push_back("HLT_mu50");
  triggerStrings.push_back("HLT_j30_xe10_razor170");
  std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;

  map<TString, TEfficiency*> effPlots;
  TChain fChain("tree");
  fChain.Add("/afs/cern.ch/user/r/rsmith/testarea/razor/Sklimmer/Run/output.15.04.15.23.55/data-treeOutput/mc14_13TeV.110351.Powh\
egPythia_P2012_ttbar_allhad.recon.AOD.e3232_s2127_s2132_r6561_tid05274300_00.root");
  fChain.SetMakeClass(1);

  std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;

  TFile * outfile = new TFile("outputEff.root" , "RECREATE" );

  for (UInt_t iTrig = 0; iTrig < triggerStrings.size(); ++iTrig) {
    std::string iTrigStr = triggerStrings.at(iTrig);

    effPlots[iTrigStr + "_rj_mdeltaR"] = new TEfficiency(("eff_" + iTrigStr + "_rj_mdeltaR").c_str(), (iTrigStr +  "; m#DeltaR [GeV]effici\
ency").c_str(), 100, 0, 2000);
  }


  for(Long64_t ientry = 0; ientry < fChain.GetEntries(); ++ientry){
    //	for(Long64_t ievent = 0;  ievent < std::min(int(event->getEntries()), 100); ++ievent){
    std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;
    std::cout << "Entry number " << ientry << std::endl;
    fChain.GetTree()->GetEntry(ientry);

    TBranch *b_rj_mdeltaR;
    Float_t rj_mdeltaR;
    fChain.SetBranchAddress("RJVars_SS_MDeltaR", &rj_mdeltaR, &b_rj_mdeltaR);

    TBranch *b_trigger;
    Int_t trigger;
    fChain.SetBranchAddress("RJVars_TriggerBits", &trigger, &b_trigger);


    for (UInt_t iTrig = 0; iTrig < triggerStrings.size(); ++iTrig) {
      // std::string iTrigStr = std::to_string(iTrig);

      // effPlots[iTrigStr + "_rj_mdeltaR"] = new TEfficiency(("eff_" + iTrigStr + "_rj_mdeltaR").c_str(), (iTrigStr + ";MET (no muon, CST) [GeV];efficiency").c_str(), 100, 0, 2000);
      // effPlots[iTrigStr + "_met_nomuon_tst_et"] = new TEfficiency(("eff_" + iTrigStr + "_met_nomuon_tst_et").c_str(), (iTrigStr + ";MET (no muon, TST) [GeV];efficiency").c_str(), 100, 0, 2000);
      // effPlots[iTrigStr + "_met_truth_et"] = new TEfficiency(("eff_" + iTrigStr + "_met_truth_et").c_str(), (iTrigStr + ";truth MET [GeV];efficiency").c_str(), 100, 0, 2000);

      //    TString const &thisTrig = triggerStrings[iTrig];
      std::string iTrigStr = triggerStrings.at(iTrig);

      std::bitset<16> bset(trigger);
      const Bool_t was_triggered = (bset[iTrig] == 1);
      effPlots[iTrigStr + "_rj_mdeltaR"]->Fill(was_triggered, rj_mdeltaR / 1000);
      // effPlots[iTrigStr + "_met_nomuon_tst_et"]->Fill(was_triggered, met_nomuon_tst_et / 1000);
      // effPlots[iTrigStr + "_met_truth_et"]->Fill(was_triggered, met_truth_et / 1000);

      //

    }
  }

  for (UInt_t iTrig = 0; iTrig < triggerStrings.size(); ++iTrig) {
    std::string iTrigStr = triggerStrings.at(iTrig);
    effPlots[iTrigStr + "_rj_mdeltaR"]->Write();
  }

  outfile->Close();

  return 0;
}
