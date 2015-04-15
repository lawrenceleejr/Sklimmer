std::string const currentDateTime() {
  time_t     now = time(0);
  struct tm * tstruct = localtime(&now);
  char       buf[80];
  strftime(buf, sizeof(buf), ".%y.%m.%d.%H.%M", tstruct);
  return buf;
}

void runTrigSklimmer (const std::string& submitDir)
{
  //===========================================
  // FOR ROOT6 WE DO NOT PUT THIS LINE
  // (ROOT6 uses Cling instead of CINT)
  // Load the libraries for all packages
  // gROOT->Macro("$ROOTCOREDIR/scripts/load_packages.C");
  // Instead on command line do:
  // > root -l '$ROOTCOREDIR/scripts/load_packages.C' 'ATestRun.cxx ("submitDir")'
  // The above works for ROOT6 and ROOT5
  //==========================================


  // Set up the job for xAOD access:
  xAOD::Init().ignore();

  // create a new sample handler to describe the data files we use
  SH::SampleHandler sh;

  // scan for datasets in the given directory
  // this works if you are on lxplus, otherwise you'd want to copy over files
  // to your local machine and use a local path.  if you do so, make sure
  // that you copy all subdirectories and point this to the directory
  // containing all the files, not the subdirectories.

  // MC single file:
  //const char* inputFilePath = gSystem->ExpandPathName ("$ALRB_TutorialData/r5591"); // using $ALRB_TutorialData previously defined
  std::string  inputFilePath = "/afs/cern.ch/work/r/rsmith/";
  SH::DiskListLocal list (inputFilePath);
  SH::scanDir (sh, list, "AOD.05274300._000018.pool.root.1"); // specifying one particular file for testing

  // or for data, finding and running over all data "datasets" within the $ALRB_TutorialData/r5597 directory:
  // const char* inputFilePath = gSystem->ExpandPathName ("$ALRB_TutorialData/r5597");
  //SH::scanDir (sh, inputFilePath);

  // set the name of the tree in our files
  // in the xAOD the TTree containing the EDM containers is "CollectionTree"
  sh.setMetaString ("nc_tree", "CollectionTree");

  // further sample handler configuration may go here

  // print out the samples we found
  sh.print ();

  // this is the basic description of our job
  EL::Job job;
  job.sampleHandler (sh);

  // add our algorithm to the job
  SklimmerAnalysis *alg = new SklimmerAnalysis;

  alg->m_doSklimming = true;
  alg->m_doSUSYObjDef = true;
  alg->m_doEventSelection = true;
  alg->m_writexAOD = true;
  alg->m_writeFullCollectionsToxAOD = false;
  alg->m_doPileupReweighting = false;

  //  alg->m_Analysis = "triggerTurnOnCurves";
  alg->m_Analysis = "triggerTurnOnCurves";


  // define an output and an ntuple associated to that output
  EL::OutputStream output  ("treeOutput");
  job.outputAdd (output);
  EL::NTupleSvc *ntuple = new EL::NTupleSvc ("treeOutput");
  job.algsAdd (ntuple);

  PlantATree *treeWriter = new PlantATree;

  // later on we'll add some configuration options for our algorithm that go here

  job.algsAdd (alg);
  job.algsAdd (treeWriter);

  treeWriter->outputName = "treeOutput";
  job.options()->setString (EL::Job::optXaodAccessMode, EL::Job::optXaodAccessMode_branch);

  // make the driver we want to use:
  // this one works by running the algorithm directly:
  EL::DirectDriver driver;
  // we can use other drivers to run things on the Grid, with PROOF, etc.

  // process the job using the driver
  driver.submit (job, submitDir + currentDateTime());

}
