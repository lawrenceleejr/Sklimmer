#include "Riostream.h"

void SteeringMacro (const std::string& submitDir, const bool localTest, const std::string dataSet, const std::string date) 
{
  // Load the libraries for all packages
  gROOT->ProcessLine(".x $ROOTCOREDIR/scripts/load_packages.C");

  // create a new sample handler to describe the data samples we will 'handle'
  SH::SampleHandler sh;
 
  // running locally or on the grid
  if(localTest){
    SH::scanDir (sh, dataSet); // scan the given directory
  }
  else{
   //  TString dir = gSystem->UnixPathName(gInterpreter->GetCurrentMacroName());
   //  ifstream in;
   //  in.open("inputDatasets.txt");

   //  TString thisDS;

   //  while (1) {
   //    in >> thisDS;
   //    if (!in.good()) break;
   //    if( thisDS.Contains("#") == 0 ){ 
   //      std::cout << thisDS.Data() << std::endl;
   //      SH::scanDQ2 (sh, thisDS.Data() ); // use DQ2 tools to find the dataset
   //    }
   // }

    SH::scanDQ2 (sh, dataSet); // use DQ2 tools to find the dataset
  }

  // set the name of the tree in our files
  // It is important to do this after you have scanned for all your samples, or it will not be set for all samples:
  // sh.setMetaString ("nc_tree", "physics"); // TTree name of variables we want to access (it is called 'physics' for NTUP_COMMON)
  sh.setMetaString ("nc_tree", "susy"); // TTree name of variables we want to access (it is called 'physics' for NTUP_COMMON)
  sh.setMetaString ("nc_grid_filter", "*.root*"); //process only root files in a dataset (last wildcard important for the cases of root.1, etc.)

  sh.setMetaDouble (EL::Job::optEventsPerWorker, 5000);


  //SH::scanNEvents (sh);

  // print out the samples we found
  sh.print ();
  // we will use EventLoop to run over the events in the sample
  EL::Job job;
  job.sampleHandler (sh);

  // create an output for the output ntuple
  EL::OutputStream output  ("output");
  job.outputAdd (output);
  EL::NTupleSvc *ntuple = new EL::NTupleSvc ("output");
  job.algsAdd (ntuple);

  // add our algorithm to the job
  SklimmerAnalysis *alg = new SklimmerAnalysis;


  job.algsAdd (alg);

  // decide which driver we want to use (depends if local or on the Grid)
  // this one works by running the algorithm directly
  if(localTest){
    EL::DirectDriver driver;
  }
  // otherwise use the Grid driver
  else{
    EL::GridDriver driver;
    //EL::PrunDriver driver;
  }


  driver.options()->setString("nc_outputSampleName", "user.leejr.test.%in:name[2]%");
  driver.options()->setString("nc_cmtConfig", "x86_64-slc6-gcc47-opt"); 
  driver.cmtConfig       = "x86_64-slc6-gcc47-opt";

  driver.outputSampleName = "user.leejr.SklimmerTest.%in:name[2]%."+date;
  driver.nMajorRetries = 0;
  driver.nMinorRetries = 0;


  if(localTest){
    // process the job using the driver
    driver.submit (job, submitDir);
  }
  // otherwise only submit the job to the grid, we can retrieve the output later
  else{
    // driver.nFilesPerJob = 1;
    driver.submitOnly (job, submitDir);
    // driver.submit (job, submitDir);
  }
} // end function SteeringMacro
