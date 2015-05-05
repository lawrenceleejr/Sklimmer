#include <TChain.h>

int makeEffPlots(){

  //  TFile * file = TFile::Open("/afs/cern.ch/user/r/rsmith/testarea/razor/Sklimmer/Run/output.15.04.15.23.55/data-treeOutput/mc14_13TeV.110351.PowhegPythia_P2012_ttbar_allhad.recon.AOD.e3232_s2127_s2132_r6561_tid05274300_00.root");
  std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;
  std::vector<std::string> triggerStrings;
  triggerStrings.push_back("HLT_j30_xe10_razor170");

  //  map<TString, TEfficiency*> effPlots;
  TChain fChain("tree");
  fChain.Add("/afs/cern.ch/user/r/rsmith/testarea/razor_r20/Sklimmer/scripts/submit_dir/data-treeOutput/valid1.110401.PowhegPythia_P2012_ttbar_nonallhad.recon.AOD.e3099_s2578_r6628_tid05346318_00.root");
  fChain.SetMakeClass(1);

  std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;

  TFile * outfile = new TFile("outputEff.root" , "RECREATE" );

  // for (UInt_t iTrig = 0; iTrig < triggerStrings.size(); ++iTrig) {
  //   std::string iTrigStr = triggerStrings.at(iTrig);

    TEfficiency * effPlots = new TEfficiency(("eff_rj_mdeltaR").c_str(), (" m#DeltaR [GeV]effici\
ency").c_str(), 100, 0, 2000);

  for(Long64_t ientry = 0; ientry < fChain.GetEntries(); ++ientry){
    //for(Long64_t ievent = 0;  ievent < std::min(int(event->getEntries()), 100); ++ievent){
    std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;
    std::cout << "Entry number " << ientry << std::endl;
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

  effPlots->write();

  outfile->Close();

  return 0;
}
