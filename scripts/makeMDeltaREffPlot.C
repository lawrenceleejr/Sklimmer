#include <TChain.h>

int makeMDeltaREffPlot(){

  //  TFile * file = TFile::Open("/afs/cern.ch/user/r/rsmith/testarea/razor/Sklimmer/Run/output.15.04.15.23.55/data-treeOutput/mc14_13TeV.110351.PowhegPythia_P2012_ttbar_allhad.recon.AOD.e3232_s2127_s2132_r6561_tid05274300_00.root");
  //std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;
  std::vector<std::string> triggerStrings;
  triggerStrings.push_back("HLT_j30_xe10_razor170");

  //  map<TString, TEfficiency*> effPlots;
  std::vector<std::string> sampleName = {
"/a/home/kolya/rsmith/testarea/razor_r20/Sklimmer/scripts/submit_dir/data-treeOutput/data15_13TeV.00276731.physics_Main.merge.DAOD_SUSY1.f620_m1480_p2411.root"
};


  for(int nsample = 0; nsample < sampleName.size(); ++nsample){
    TFile * outfile = new TFile(("smallroots/outputEff_"+std::to_string(nsample)+".root").c_str() , "RECREATE" );

    TChain fChain("tree");
    fChain.Add(sampleName.at(nsample).c_str());
    fChain.SetMakeClass(1);

    std::cout << __PRETTY_FUNCTION__ << " at line: " << __LINE__ << std::endl;

    // for (UInt_t iTrig = 0; iTrig < triggerStrings.size(); ++iTrig) {
    //   std::string iTrigStr = triggerStrings.at(iTrig);

    TEfficiency * effPlots170 = new TEfficiency("eff170_rj_mdeltaR", "170 m#DeltaR [GeV]efficiency", 100, 0, 2000);
    TEfficiency * effPlots185 = new TEfficiency("eff185_rj_mdeltaR", "185 m#DeltaR [GeV]efficiency", 100, 0, 2000);
    TEfficiency * effPlots195 = new TEfficiency("eff195_rj_mdeltaR", "195 m#DeltaR [GeV]efficiency", 100, 0, 2000);

    TEfficiency * effPlots_xe60_170 = new TEfficiency("eff170_xe60_rj_mdeltaR", "170 m#DeltaR [GeV]efficiency", 100, 0, 2000);
    TEfficiency * effPlots_xe60_185 = new TEfficiency("eff185_xe60_rj_mdeltaR", "185 m#DeltaR [GeV]efficiency", 100, 0, 2000);
    TEfficiency * effPlots_xe60_195 = new TEfficiency("eff195_xe60_rj_mdeltaR", "195 m#DeltaR [GeV]efficiency", 100, 0, 2000);

    TEfficiency * effPlots170_xe100 = new TEfficiency("eff170_xe100_rj_mdeltaR", "170 m#DeltaR [GeV] OR xe100 efficiency", 100, 0, 2000);
    TEfficiency * effPlots185_xe100 = new TEfficiency("eff185_xe100_rj_mdeltaR", "185 m#DeltaR [GeV] OR xe100 efficiency", 100, 0, 2000);
    TEfficiency * effPlots195_xe100 = new TEfficiency("eff195_xe100_rj_mdeltaR", "195 m#DeltaR [GeV] OR xe100 efficiency", 100, 0, 2000);

    TEfficiency * effPlots_xe100 = new TEfficiency("effxe100_rj_mdeltaR", " m#DeltaR [GeV] xe100 efficiency", 100, 0, 2000);
    // TEfficiency * effPlots_xe100 = new TEfficiency("effxe100_rj_mdeltaR", "185 m#DeltaR [GeV] OR xe100 efficiency", 100, 0, 2000);
    // TEfficiency * effPlots_xe100 = new TEfficiency("effxe100_rj_mdeltaR", "195 m#DeltaR [GeV] OR xe100 efficiency", 100, 0, 2000);

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


      TBranch *b_met_x;
      Float_t met_x;
      fChain.SetBranchAddress("MET_x", &met_x, &b_met_x);


      TBranch *b_met_y;
      Float_t met_y;
      fChain.SetBranchAddress("MET_y", &met_y, &b_met_y);

      TBranch *b_trigger;
      Int_t trigger;
      fChain.SetBranchAddress("RJVars_HLT_TriggerBits", &trigger, &b_trigger);

      std::bitset<32> bset(trigger);

      //      const Bool_t was_triggered_razor100 = (bset[28] == 1);//from this line in SklimmerAnalysis:
      //      const Bool_t was_triggered_L1seed_xe100 = (bset[1] == 1);
      const Bool_t was_triggered_L1seed       = (bset[10] == 1);
      const Bool_t was_triggered_L1seed_xe70  = (bset[1] == 1 );

      const Bool_t was_triggered_xe60_razor100 = (bset[15] == 1);//from this line in SklimmerAnalysis:
      const Bool_t was_triggered_xe60_razor170 = (bset[16] == 1);//from this line in SklimmerAnalysis:
      const Bool_t was_triggered_xe60_razor185 = (bset[17] == 1);//from this line in SklimmerAnalysis:
      const Bool_t was_triggered_xe60_razor195 = (bset[18] == 1);//from this line in SklimmerAnalysis:

      const Bool_t was_triggered_razor100 = (bset[28] == 1);//from this line in SklimmerAnalysis:
      const Bool_t was_triggered_razor170 = (bset[29] == 1);//from this line in SklimmerAnalysis:
      const Bool_t was_triggered_razor185 = (bset[30] == 1);//from this line in SklimmerAnalysis:
      const Bool_t was_triggered_razor195 = (bset[31] == 1);//from this line in SklimmerAnalysis:
      const Bool_t was_triggered_xe100    = (bset[4]  == 1);//from this line in SklimmerAnalysis:
      const Bool_t was_triggered_L1_xe70  = (bset[1]  == 1);


      if(was_triggered_L1seed_xe70){
	const float met = TMath::Sqrt( met_x*met_x  + met_y * met_y ) ;
	effPlots_xe100->Fill(was_triggered_xe100, met);
      }

      if(was_triggered_L1seed){

	effPlots170->Fill(was_triggered_razor170 , rj_mdeltaR/1000.);
	effPlots185->Fill(was_triggered_razor185 , rj_mdeltaR/1000.);
	effPlots195->Fill(was_triggered_razor195 , rj_mdeltaR/1000.);

	effPlots_xe60_170->Fill(was_triggered_xe60_razor170 , rj_mdeltaR/1000.);
	effPlots_xe60_185->Fill(was_triggered_xe60_razor185 , rj_mdeltaR/1000.);
	effPlots_xe60_195->Fill(was_triggered_xe60_razor195 , rj_mdeltaR/1000.);

	effPlots170_xe100->Fill(was_triggered_razor170  , rj_mdeltaR/1000.);
	effPlots185_xe100->Fill(was_triggered_razor185  , rj_mdeltaR/1000.);
	effPlots195_xe100->Fill(was_triggered_razor195  , rj_mdeltaR/1000.);

	// effPlots170->Fill(was_triggered_razor170 , rj_mdeltaR/1000.);
	// effPlots185->Fill(was_triggered_razor185 , rj_mdeltaR/1000.);
	// effPlots195->Fill(was_triggered_razor195 , rj_mdeltaR/1000.);

	// effPlots170_xe100->Fill(was_triggered_razor170 | was_triggered_xe100 , rj_mdeltaR/1000.);
	// effPlots185_xe100->Fill(was_triggered_razor185 | was_triggered_xe100 , rj_mdeltaR/1000.);
	// effPlots195_xe100->Fill(was_triggered_razor195 | was_triggered_xe100 , rj_mdeltaR/1000.);
      }
      // if(was_triggered_L1_xe70){

      // 	effPlots_xe100->Fill(was_triggered_xe100, rj_mdeltaR);
      // }
    }



    effPlots170->Write();
    effPlots185->Write();
    effPlots195->Write();

    effPlots_xe60_170->Write();
    effPlots_xe60_185->Write();
    effPlots_xe60_195->Write();

    effPlots170_xe100->Write();
    effPlots185_xe100->Write();
    effPlots195_xe100->Write();

    effPlots_xe100->Write();

    outfile->Close();
  }//tfile


  return 0;
}
