#include "xAODRootAccess/Init.h"
#include "SampleHandler/SampleHandler.h"
#include "SampleHandler/ToolsDiscovery.h"
#include "EventLoop/Job.h"
#include "EventLoop/DirectDriver.h"
#include "SampleHandler/DiskListLocal.h"
#include <TSystem.h>

#include <EventLoopAlgs/NTupleSvc.h>
#include <EventLoop/OutputStream.h>
#include "EventLoopGrid/PrunDriver.h"

#include "Sklimmer/SklimmerAnalysis.h"
#include "Sklimmer/HelperFunctions.h"
#include "Sklimmer/PlantATree.h"

int main( int argc, char* argv[] ) {

  // Take the submit directory from the input if provided:
  std::string submitDir = "submitDir";
  if( argc > 1 ) submitDir = argv[ 1 ];

  // Set up the job for xAOD access:
  xAOD::Init().ignore();

  // Construct the samples to run on:
  SH::SampleHandler sh;

  // const char* inputFilePath = gSystem->ExpandPathName ("$ALRB_TutorialData/r5591"); // using $ALRB_TutorialData previously defined
  // SH::DiskListLocal list (inputFilePath);
  // SH::scanDir (sh, list, "AOD.01494882._113691.pool.root.1"); // specifying one particular file for testing

  SH::scanDQ2 (sh, "mc14_8TeV.117050.PowhegPythia_P2011C_ttbar.recon.AOD.e1727_s1933_s1911_r5591/");

  // Set the name of the input TTree. It's always "CollectionTree"
  // for xAOD files.
  sh.setMetaString( "nc_tree", "CollectionTree" );

  // Print what we found:
  sh.print();

  // Create an EventLoop job:
  EL::Job job;
  job.sampleHandler( sh );

  // Add our analysis to the job:
  SklimmerAnalysis *alg = new SklimmerAnalysis;

  alg->m_doSklimming = true;
  alg->m_doSUSYObjDef = true;
  alg->m_doEventSelection = true;
  alg->m_writexAOD = true;
  alg->m_writeFullCollectionsToxAOD = true;

  alg->m_Analysis = "bbmet";


  // define an output and an ntuple associated to that output
  EL::OutputStream output  ("treeOutput");
  job.outputAdd (output);
  EL::NTupleSvc *ntuple = new EL::NTupleSvc ("treeOutput");
  job.algsAdd (ntuple);

  PlantATree *treeWriter = new PlantATree;


  job.algsAdd( alg );
  job.algsAdd (treeWriter);

  treeWriter->outputName = "treeOutput";



job.options()->setDouble (EL::Job::optCacheSize, 100*1024*1024);
job.options()->setDouble (EL::Job::optCacheLearnEntries, 10);


  // Run the job using the local/direct driver:
  EL::DirectDriver driver;
  driver.submit( job, submitDir );

  return 0;
}



