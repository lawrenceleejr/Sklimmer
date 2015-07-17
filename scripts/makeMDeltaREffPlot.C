#include <TChain.h>

int makeMDeltaREffPlot(){

  //  TFile * file = TFile::Open("/afs/cern.ch/user/r/rsmith/testarea/razor/Sklimmer/Run/output.15.04.15.23.55/data-treeOutput/mc14_13TeV.110351.PowhegPythia_P2012_ttbar_allhad.recon.AOD.e3232_s2127_s2132_r6561_tid05274300_00.root");
  //std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;
  std::vector<std::string> triggerStrings;
  triggerStrings.push_back("HLT_j30_xe10_razor170");

  //  map<TString, TEfficiency*> effPlots;
  std::vector<std::string> sampleName = {
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270448physics_Main_treeOutput.root.35001321/user.rsmith.5960933._000001.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270949physics_Main_treeOutput.root.35010446/user.rsmith.5960935._000003.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270949physics_Main_treeOutput.root.35010446/user.rsmith.5960935._000002.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270448physics_Main_treeOutput.root.35021282/user.rsmith.5960933._000006.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270448physics_Main_treeOutput.root.35021282/user.rsmith.5960933._000012.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270448physics_Main_treeOutput.root.35021282/user.rsmith.5960933._000007.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270448physics_Main_treeOutput.root.35021282/user.rsmith.5960933._000008.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270953physics_Main_treeOutput.root.34999978/user.rsmith.5960918._000005.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270953physics_Main_treeOutput.root.34999978/user.rsmith.5960918._000010.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270953physics_Main_treeOutput.root.34999978/user.rsmith.5960918._000006.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270953physics_Main_treeOutput.root.34999978/user.rsmith.5960918._000011.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270953physics_Main_treeOutput.root.34999978/user.rsmith.5960918._000009.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270953physics_Main_treeOutput.root.34999978/user.rsmith.5960918._000001.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270806physics_Main_treeOutput.root.34995161/user.rsmith.5960913._000002.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270806physics_Main_treeOutput.root.34995161/user.rsmith.5960913._000001.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00271388physics_Main_treeOutput.root.35008649/user.rsmith.5960937._000002.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270953physics_Main_treeOutput.root.35005751/user.rsmith.5960918._000003.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270953physics_Main_treeOutput.root.35005751/user.rsmith.5960918._000002.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270953physics_Main_treeOutput.root.35005751/user.rsmith.5960918._000004.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270816physics_Main_treeOutput.root.34997307/user.rsmith.5960929._000001.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270448physics_Main_treeOutput.root.35013923/user.rsmith.5960933._000005.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270448physics_Main_treeOutput.root.35013923/user.rsmith.5960933._000004.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270806physics_Main_treeOutput.root.35012565/user.rsmith.5960913._000004.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270806physics_Main_treeOutput.root.35012565/user.rsmith.5960913._000006.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270806physics_Main_treeOutput.root.35012565/user.rsmith.5960913._000003.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270806physics_Main_treeOutput.root.35014588/user.rsmith.5960913._000005.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270588physics_Main_treeOutput.root.34998685/user.rsmith.5960908._000002.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270588physics_Main_treeOutput.root.34998685/user.rsmith.5960908._000001.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270949physics_Main_treeOutput.root.34994923/user.rsmith.5960935._000001.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270448physics_Main_treeOutput.root.35012323/user.rsmith.5960933._000002.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270448physics_Main_treeOutput.root.35012323/user.rsmith.5960933._000011.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270448physics_Main_treeOutput.root.35012323/user.rsmith.5960933._000014.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270448physics_Main_treeOutput.root.35012323/user.rsmith.5960933._000015.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270448physics_Main_treeOutput.root.35012323/user.rsmith.5960933._000017.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270448physics_Main_treeOutput.root.35012323/user.rsmith.5960933._000003.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270448physics_Main_treeOutput.root.35012323/user.rsmith.5960933._000013.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270448physics_Main_treeOutput.root.35012323/user.rsmith.5960933._000009.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270448physics_Main_treeOutput.root.35012323/user.rsmith.5960933._000016.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270448physics_Main_treeOutput.root.35012323/user.rsmith.5960933._000010.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270953physics_Main_treeOutput.root.35013605/user.rsmith.5960918._000007.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270953physics_Main_treeOutput.root.35013605/user.rsmith.5960918._000008.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270441physics_Main_treeOutput.root.35017202/user.rsmith.5960923._000002.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270441physics_Main_treeOutput.root.35017202/user.rsmith.5960923._000001.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00271044physics_Main_treeOutput.root.34998661/user.rsmith.5960902._000001.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00271388physics_Main_treeOutput.root.34995992/user.rsmith.5960937._000001.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270588physics_Main_treeOutput.root.35021869/user.rsmith.5960908._000005.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270588physics_Main_treeOutput.root.35021869/user.rsmith.5960908._000006.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270588physics_Main_treeOutput.root.35021869/user.rsmith.5960908._000003.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270588physics_Main_treeOutput.root.35021869/user.rsmith.5960908._000004.treeOutput.root",
"/data/users/rsmith/user.rsmith.grid.razor_triggerinputDS_razor_trig.txt00270816physics_Main_treeOutput.root.35011373/user.rsmith.5960929._000002.treeOutput.root"};





  for(int nsample = 0; nsample < sampleName.size(); ++nsample){
    TFile * outfile = new TFile(("smallroots/"+"outputEff_"+std::to_string(nsample)+".root").c_str() , "RECREATE" );

    TChain fChain("tree");
    fChain.Add(sampleName.at(nsample).c_str());
    fChain.SetMakeClass(1);

    std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;

    // for (UInt_t iTrig = 0; iTrig < triggerStrings.size(); ++iTrig) {
    //   std::string iTrigStr = triggerStrings.at(iTrig);

    TEfficiency * effPlots170 = new TEfficiency("eff170_rj_mdeltaR", "170 m#DeltaR [GeV]efficiency", 100, 0, 2000);
    TEfficiency * effPlots185 = new TEfficiency("eff185_rj_mdeltaR", "185 m#DeltaR [GeV]efficiency", 100, 0, 2000);
    TEfficiency * effPlots195 = new TEfficiency("eff195_rj_mdeltaR", "195 m#DeltaR [GeV]efficiency", 100, 0, 2000);

    std::cout << "nentries : " << fChain.GetEntries() << std::endl;

    for(Long64_t ientry = 0; ientry < fChain.GetEntries(); ++ientry){
      //for(Long64_t ievent = 0;  ievent < std::min(int(event->getEntries()), 100); ++ievent){
      //std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;
      if( ientry % 1000 == 0) std::cout << "Entry number " << ientry << std::endl;
      //std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;
      fChain.GetTree()->GetEntry(ientry);

      TBranch *b_rj_mdeltaR;
      Float_t rj_mdeltaR;
      fChain.SetBranchAddress("RJVars_SS_MDeltaR", &rj_mdeltaR, &b_rj_mdeltaR);

      TBranch *b_trigger;
      Int_t trigger;
      fChain.SetBranchAddress("RJVars_TriggerBits", &trigger, &b_trigger);

      std::bitset<32> bset(trigger);

      //      const Bool_t was_triggered_razor100 = (bset[28] == 1);//from this line in SklimmerAnalysis:
      const Bool_t was_triggered_razor100 = (bset[28] == 1);//from this line in SklimmerAnalysis:
      const Bool_t was_triggered_razor170 = (bset[29] == 1);//from this line in SklimmerAnalysis:
      const Bool_t was_triggered_razor185 = (bset[30] == 1);//from this line in SklimmerAnalysis:
      const Bool_t was_triggered_razor195 = (bset[31] == 1);//from this line in SklimmerAnalysis:
      //   triggers[10] = m_susy_obj->isTrigPassed("HLT_j30_xe10_razor170");

      if(was_triggered_razor100){
	effPlots170->Fill(was_triggered_razor170 && was_triggered_razor100, rj_mdeltaR/1000.);
	effPlots185->Fill(was_triggered_razor185 && was_triggered_razor100, rj_mdeltaR/1000.);
	effPlots195->Fill(was_triggered_razor195 && was_triggered_razor100, rj_mdeltaR/1000.);
      }
    }

    effPlots170->Write();
    effPlots185->Write();
    effPlots195->Write();

    outfile->Close();
  }//tfile


  return 0;
}
