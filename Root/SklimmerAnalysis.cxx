#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <Sklimmer/SklimmerAnalysis.h>

#include <TLorentzVector.h>
#include <TVector3.h>





// this is needed to distribute the algorithm to the workers
ClassImp(SklimmerAnalysis)



SklimmerAnalysis :: SklimmerAnalysis ()
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().




// my_JetCleaningTool = 0;

}



EL::StatusCode SklimmerAnalysis :: setupJob (EL::Job& job)
{
  // Here you put code that sets up the job on the submission object
  // so that it is ready to work with your algorithm, e.g. you can
  // request the D3PDReader service or add output files.  Any code you
  // put here could instead also go into the submission script.  The
  // sole advantage of putting it here is that it gets automatically
  // activated/deactivated when you add/remove the algorithm from your
  // job, which may or may not be of value to you.

  job.useD3PDReader ();

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode SklimmerAnalysis :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode SklimmerAnalysis :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode SklimmerAnalysis :: changeInput (bool firstFile)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode SklimmerAnalysis :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.



  // if (!my_JetCleaningTool){
  //   throw std::string ("No JetCleaningTool configured");
  // }
  // my_JetCleaningTool->initialize();



  if (!RJTool){
    throw std::string ("No RJTool configured");
  }



  RJTool->initialize("./RJigsawConfig/hemisphere1","./RJigsawConfig/hemisphere2");
  RJTool->resetHists();


  event = wk()->d3pdreader();
  output = EL::getNTupleSvc (wk(), "output");

  //-- connect to the input TTree
  TTree *inputTree = wk()->tree(); // this is the input tree


  if (output){

    output->tree()->Branch( "RJVars_M_0_0_0", &RJVars_M_0_0_0      , "RJVars_M_0_0_0/F");   
    output->tree()->Branch( "RJVars_M_0_0_1", &RJVars_M_0_0_1      , "RJVars_M_0_0_1/F");   
    output->tree()->Branch( "RJVars_M_1_0_0", &RJVars_M_1_0_0      , "RJVars_M_1_0_0/F");   
    output->tree()->Branch( "RJVars_M_1_0_1", &RJVars_M_1_0_1      , "RJVars_M_1_0_1/F");   
    output->tree()->Branch( "RJVars_M_1_1_0", &RJVars_M_1_1_0      , "RJVars_M_1_1_0/F");   
    output->tree()->Branch( "RJVars_M_1_1_1", &RJVars_M_1_1_1      , "RJVars_M_1_1_1/F");   
    output->tree()->Branch( "RJVars_M_2_0_0", &RJVars_M_2_0_0      , "RJVars_M_2_0_0/F");   
    output->tree()->Branch( "RJVars_M_2_0_1", &RJVars_M_2_0_1      , "RJVars_M_2_0_1/F");   
    output->tree()->Branch( "RJVars_M_2_1_0", &RJVars_M_2_1_0      , "RJVars_M_2_1_0/F");   
    output->tree()->Branch( "RJVars_M_2_1_1", &RJVars_M_2_1_1      , "RJVars_M_2_1_1/F");   
    output->tree()->Branch( "RJVars_dPhi_0_0_0", &RJVars_dPhi_0_0_0   , "RJVars_dPhi_0_0_0/F");    
    output->tree()->Branch( "RJVars_dPhi_0_0_1", &RJVars_dPhi_0_0_1   , "RJVars_dPhi_0_0_1/F");    
    output->tree()->Branch( "RJVars_dPhi_1_0_0", &RJVars_dPhi_1_0_0   , "RJVars_dPhi_1_0_0/F");    
    output->tree()->Branch( "RJVars_dPhi_1_0_1", &RJVars_dPhi_1_0_1   , "RJVars_dPhi_1_0_1/F");    
    output->tree()->Branch( "RJVars_dPhi_2_0_0", &RJVars_dPhi_2_0_0   , "RJVars_dPhi_2_0_0/F");    
    output->tree()->Branch( "RJVars_dPhi_2_0_1", &RJVars_dPhi_2_0_1   , "RJVars_dPhi_2_0_1/F");    
    output->tree()->Branch( "RJVars_dPhiVis_0_0_0", &RJVars_dPhiVis_0_0_0, "RJVars_dPhiVis_0_0_0/F");
    output->tree()->Branch( "RJVars_dPhiVis_0_0_1", &RJVars_dPhiVis_0_0_1, "RJVars_dPhiVis_0_0_1/F");
    output->tree()->Branch( "RJVars_dPhiVis_1_0_0", &RJVars_dPhiVis_1_0_0, "RJVars_dPhiVis_1_0_0/F");
    output->tree()->Branch( "RJVars_dPhiVis_1_0_1", &RJVars_dPhiVis_1_0_1, "RJVars_dPhiVis_1_0_1/F");
    output->tree()->Branch( "RJVars_dPhiVis_2_0_0", &RJVars_dPhiVis_2_0_0, "RJVars_dPhiVis_2_0_0/F");
    output->tree()->Branch( "RJVars_dPhiVis_2_0_1", &RJVars_dPhiVis_2_0_1, "RJVars_dPhiVis_2_0_1/F");
    output->tree()->Branch( "RJVars_gamma_0_0_0", &RJVars_gamma_0_0_0  , "RJVars_gamma_0_0_0/F");       
    output->tree()->Branch( "RJVars_gamma_0_0_1", &RJVars_gamma_0_0_1  , "RJVars_gamma_0_0_1/F");       

  }

  //-- d3pd reader read branches from input tree
  event->ReadFrom( inputTree ); 

  // decide which branches to activate, and write to the output skimmed D3PD
  event->SetActive( kTRUE, "RunNumber");
  event->SetActive( kTRUE, "EventNumber");
  event->SetActive( kTRUE, "lbn");
  event->SetActive( kTRUE, "isSimulation");
  event->SetActive( kTRUE, "^el_n$");  // notation if you want the exact branch name
  event->SetActive( kTRUE, "^el_pt$");
  event->SetActive( kTRUE, "^el_eta$");
  event->SetActive( kTRUE, "^el_phi$");
  event->SetActive( kTRUE, "^mu_muid_n$"); 

  // turn on all jet branches first, then turn off some specific jet branches
  event->SetActive( kTRUE, "jet_AntiKt4LCTopo_.*");
  event->SetActive( kFALSE, "jet_AntiKt4LCTopo_MET.*");
  event->SetActive( kFALSE, "jet_AntiKt4LCTopo_flavor.*");

  event->SetActive( kTRUE, "MET_RefFinal_Egamma10NoTau_.*");

  // Make sure that all variables that we want to copy to the output are read into memory
  event->ReadAllActive(); 

  event->WriteTo(output->tree());

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode SklimmerAnalysis :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  int passEvent = 1;




  // unsigned njets = 0;
  // for( Int_t i = 0; i < event->jet_AntiKt4LCTopo.n(); ++i ) {
  //   if (my_JetCleaningTool->accept(event->jet_AntiKt4LCTopo[i].eta(),
  //                                  event->jet_AntiKt4LCTopo[i].NegativeE(),
  //                                  event->jet_AntiKt4LCTopo[i].hecf(),
  //                                  event->jet_AntiKt4LCTopo[i].HECQuality(),
  //                                  event->jet_AntiKt4LCTopo[i].emfrac(),
  //                                  event->jet_AntiKt4LCTopo[i].sumPtTrk(),
  //                                  event->jet_AntiKt4LCTopo[i].LArQuality(),
  //                                  event->jet_AntiKt4LCTopo[i].Timing(),
  //                                  event->jet_AntiKt4LCTopo[i].fracSamplingMax(),
  //                                  event->jet_AntiKt4LCTopo[i].AverageLArQF() )){
  //     ++ njets;
  //   } // end if 
  // } // end for loop of jets

if( event->jet_AntiKt4LCTopo.n() < 2 ) passEvent=0;
if( event->mu.n() < 2 ) passEvent=0;

TLorentzVector Muon[3];
TLorentzVector Jet[3];

Muon[0].SetPtEtaPhiM( event->mu[0].pt(),
                      event->mu[0].eta(),
                      event->mu[0].phi(),
                      event->mu[0].m() );
Muon[1].SetPtEtaPhiM( event->mu[1].pt(),
                      event->mu[1].eta(),
                      event->mu[1].phi(),
                      event->mu[1].m() );

Jet[0].SetPtEtaPhiM(  event->jet_AntiKt4LCTopo[0].pt(),
                      event->jet_AntiKt4LCTopo[0].eta(),
                      event->jet_AntiKt4LCTopo[0].phi(),
                      event->jet_AntiKt4LCTopo[0].m() );
Jet[1].SetPtEtaPhiM(  event->jet_AntiKt4LCTopo[1].pt(),
                      event->jet_AntiKt4LCTopo[1].eta(),
                      event->jet_AntiKt4LCTopo[1].phi(),
                      event->jet_AntiKt4LCTopo[1].m() );

if( Muon[0].DeltaR(Jet[0]) > Muon[1].DeltaR(Jet[0])  ){
  Muon[3] = Muon[0];
  Muon[0] = Muon[1];
  Muon[1] = Muon[3];
}


RJTool->newEvent();

RJTool->addVisParticle("b",Jet[0],1);
RJTool->addVisParticle("b",Jet[1],2);

RJTool->addVisParticle("l",Muon[0],1);
RJTool->addVisParticle("l",Muon[1],2);

TVector3 MET;
MET.SetXYZ(  event->MET_RefFinal_Egamma10NoTau.etx(),  event->MET_RefFinal_Egamma10NoTau.ety(), 0.0 );
RJTool->addMET( MET );

RJTool->setHemisphereMode(0); //top symmetry
RJTool->guessInvParticles();
RJTool->getObservables();

// If you want access to the variables to do whatever with...
// std::map< TString, double > observables = RJTool->getObservablesMap();

  // Try again with other Hemisphere mode

  RJTool->setHemisphereMode(1); //W symmetry
  RJTool->guessInvParticles();
  RJTool->getObservables();

  std::map< TString, double > RJVars = RJTool->getObservablesMap();



  RJVars_M_0_0_0           = RJVars["M_0_0_0"];
  RJVars_M_0_0_1           = RJVars["M_0_0_1"];
  RJVars_M_1_0_0           = RJVars["M_1_0_0"];
  RJVars_M_1_0_1           = RJVars["M_1_0_1"];
  RJVars_M_1_1_0           = RJVars["M_1_1_0"];
  RJVars_M_1_1_1           = RJVars["M_1_1_1"];
  RJVars_M_2_0_0           = RJVars["M_2_0_0"];
  RJVars_M_2_0_1           = RJVars["M_2_0_1"];
  RJVars_M_2_1_0           = RJVars["M_2_1_0"];
  RJVars_M_2_1_1           = RJVars["M_2_1_1"];
  RJVars_dPhi_0_0_0        = RJVars["dPhi_0_0_0"];
  RJVars_dPhi_0_0_1        = RJVars["dPhi_0_0_1"];
  RJVars_dPhi_1_0_0        = RJVars["dPhi_1_0_0"];
  RJVars_dPhi_1_0_1        = RJVars["dPhi_1_0_1"];
  RJVars_dPhi_2_0_0        = RJVars["dPhi_2_0_0"];
  RJVars_dPhi_2_0_1        = RJVars["dPhi_2_0_1"];
  RJVars_dPhiVis_0_0_0     = RJVars["dPhiVis_0_0_0"];
  RJVars_dPhiVis_0_0_1     = RJVars["dPhiVis_0_0_1"];
  RJVars_dPhiVis_1_0_0     = RJVars["dPhiVis_1_0_0"];
  RJVars_dPhiVis_1_0_1     = RJVars["dPhiVis_1_0_1"];
  RJVars_dPhiVis_2_0_0     = RJVars["dPhiVis_2_0_0"];
  RJVars_dPhiVis_2_0_1     = RJVars["dPhiVis_2_0_1"];
  RJVars_gamma_0_0_0       = RJVars["gamma_0_0_0"];
  RJVars_gamma_0_0_1       = RJVars["gamma_0_0_1"];




  // if( (event->triggerbits.EF_mu24_tight() != 1)    && 
  //     (event->triggerbits.EF_e24vh_medium1() != 1) && 
  //     (event->triggerbits.EF_xe80_tclcw() != 1) ){
  //   passEvent = 0;
  // }

  if(passEvent){
    // You can put some criteria here for the event to be selected.
    output->setFilterPassed (); // You must have this line somewhere
  }

//  output->setFilterPassed ();
  
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode SklimmerAnalysis :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode SklimmerAnalysis :: finalize ()
{
  // This method is the mirror image of initialize(), meaning it gets
  // called after the last event has been processed on the worker node
  // and allows you to finish up any objects you created in
  // initialize() before they are written to disk.  This is actually
  // fairly rare, since this happens separately for each worker node.
  // Most of the time you want to do your post-processing on the
  // submission node after all your histogram outputs have been
  // merged.  This is different from histFinalize() in that it only
  // gets called on worker nodes that processed input events.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode SklimmerAnalysis :: histFinalize ()
{
  // This method is the mirror image of histInitialize(), meaning it
  // gets called after the last event has been processed on the worker
  // node and allows you to finish up any objects you created in
  // histInitialize() before they are written to disk.  This is
  // actually fairly rare, since this happens separately for each
  // worker node.  Most of the time you want to do your
  // post-processing on the submission node after all your histogram
  // outputs have been merged.  This is different from finalize() in
  // that it gets called on all worker nodes regardless of whether
  // they processed input events.
  return EL::StatusCode::SUCCESS;
}
