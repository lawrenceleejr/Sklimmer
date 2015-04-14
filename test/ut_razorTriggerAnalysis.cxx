#ifdef ROOTCORE
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"
#endif

// FrameWork includes
#include "AsgTools/ToolHandle.h"
#include "AsgTools/AsgTool.h"
#include "xAODBase/IParticleContainer.h"
#include "xAODMissingET/MissingETAuxContainer.h"

#include "TFile.h"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE TEST_SKLIMMER
#include <boost/test/unit_test.hpp>

#define private public //ugly hacking way to do this...
#define protected public
#include "Sklimmer/SklimmerAnalysis.h"
#undef private
#undef protected

static std::string const toolname = "tool";
static TFile * ifile;
static xAOD::TEvent * event;
static xAOD::TStore *  store;

struct globalxAODSetup
{

  globalxAODSetup() {

    xAOD::Init() ;
    // CP::CorrectionCode::enableFailure();
    // StatusCode::enableFailure();                                                                                                                      // CP::SystematicCode::enableFailure();
    // xAOD::TReturnCode::enableFailure();
    //   TString const fileName = "AOD.pool.root";

    TString const fileName = "/afs/cern.ch/work/r/rsmith/public/METUtilities_testfiles/valid1.110401.PowhegPythia_P2012_ttbar_nonallhad.recon.AOD.e3099_s1982_s1964_r6006_tid04628718_00/AOD.04628718._000158.pool.root.1";

    ifile = new TFile( fileName, "READ" ) ;
    event = new xAOD::TEvent( ifile,  xAOD::TEvent::kClassAccess );
    store = new xAOD::TStore;
  }

  ~globalxAODSetup(){
    delete event;
    delete ifile;
  }
};

struct perTestSetup
{
  SklimmerAnalysis analysis;

  perTestSetup() //: analysis
  {
    BOOST_TEST_MESSAGE("starting test" );
    //    analysis.msg().setLevel(MSG::VERBOSE);//if you are failing tests, this is helpful
    //analysis.msg().setLevel(MSG::WARNING);

    Long64_t ievent = 1;//just check with one event
    BOOST_REQUIRE(event->getEntry(ievent) >= 0 );
  }

  ~perTestSetup()
  {
    BOOST_TEST_MESSAGE("ending test");
  }
};

BOOST_GLOBAL_FIXTURE( globalxAODSetup )

BOOST_FIXTURE_TEST_SUITE(Test_Sklimmer , perTestSetup)

//todo if this is possible
// BOOST_AUTO_TEST_CASE(initialization)
// {
//   BOOST_REQUIRE(analysis.m_Analysis =  "bbmet");
//   BOOST_REQUIRE(analysis.initialize() == EL::StatusCode::SUCCESS);

//   BOOST_REQUIRE(analysis.m_grl);
//   BOOST_REQUIRE(analysis.m_pileupReweightingTool);
//   BOOST_REQUIRE(analysis.m_susy_obj);

//   BOOST_REQUIRE(analysis.RJTool);
//   BOOST_REQUIRE(analysis.m_trigDecisionTool);
//   BOOST_REQUIRE(analysis.m_trigConfigTool);

// }

BOOST_AUTO_TEST_CASE(trigInitialization){

  BOOST_REQUIRE(analysis.initializeTrigDecisionTool() == EL::StatusCode::SUCCESS);

  BOOST_REQUIRE(analysis.m_trigDecisionTool);
  BOOST_REQUIRE(analysis.m_trigConfigTool);

}


BOOST_AUTO_TEST_SUITE_END() //Test_Sklimmer
