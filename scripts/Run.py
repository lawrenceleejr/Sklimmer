import ROOT
import logging

logging.basicConfig(level=logging.WARNING)
from optparse import OptionParser

parser = OptionParser()
parser.add_option("--submitDir", help="dir to store the output", default="submit_dir")
parser.add_option("--inputDS", help="input DS from DQ2", default="none")
parser.add_option("--driver", help="select where to run", choices=("direct", "prooflite", "grid"), default="grid")
parser.add_option("--nevents", type=int, help="number of events to process for all the datasets")
parser.add_option("--skip-events", type=int, help="skip the first n events")
parser.add_option("-w", "--overwrite", action='store_true', default=True, help="overwrite previous submitDir")
parser.add_option("--isMC", help="Data flag [0/1]", default=0)
parser.add_option("--isAtlfast", help="ATLFAST flag [0/1]", default=0)
parser.add_option("--mc12b", help="mc12b flag [0/1]", default=1)
parser.add_option("--useLeptonTrigger", help="Lepton Trigger flag [0/1]", default=0)
parser.add_option("--doSyst", help="Create Trees with systemtic variations [0/1]", default=0)

parser.add_option("--runTag", help="", default="Test_XXYYZZa")

(options, args) = parser.parse_args()

import atexit
@atexit.register
def quite_exit():
    ROOT.gSystem.Exit(0)

logging.info("loading packages")
ROOT.gROOT.Macro("$ROOTCOREDIR/scripts/load_packages.C")

if options.overwrite:
    import shutil
    shutil.rmtree(options.submitDir, True)


#Set up the job for xAOD access:
ROOT.xAOD.Init().ignore();

# create a new sample handler to describe the data files we use
logging.info("creating new sample handler")
sh_all = ROOT.SH.SampleHandler()


if ".txt" in options.inputDS:
  with open(options.inputDS, "r") as inputDSs:
    for line in inputDSs:
      print line[:-1]
      ROOT.SH.scanDQ2 (sh_all, line[:-1])
elif options.inputDS == "test":
    list = ROOT.SH.DiskListLocal("/data/users/rsmith/")
    ROOT.SH.scanDir(sh_all,list,"DAOD_SUSY1.05970104._000001.pool.root.1");
#    ROOT.SH.scanDir(sh_all,list,"DAOD_SUSY1.06381986._000008.pool.root.1");
elif options.inputDS != "none":
  ROOT.SH.scanDQ2 (sh_all, options.inputDS);
else :
  search_directories = []
  search_directories = ("/afs/cern.ch/atlas/project/PAT/xAODs/r5591/",)

  # scan for datasets in the given directories
  for directory in search_directories:
    ROOT.SH.scanDir(sh_all, directory)

# print out the samples we found
logging.info("%d different datasets found scanning all directories", len(sh_all))

# set the name of the tree in our files
sh_all.setMetaString("nc_tree", "CollectionTree")
sh_all.setMetaString    ("nc_grid_filter", "*");
sh_all.printContent()

# this is the basic description of our job
logging.info("creating new job")
job = ROOT.EL.Job()
job.sampleHandler(sh_all)

if options.nevents:
    logging.info("processing only %d events", options.nevents)
    job.options().setDouble(ROOT.EL.Job.optMaxEvents, options.nevents)

if options.skip_events:
    logging.info("skipping first %d events", options.skip_events)
    job.options().setDouble(ROOT.EL.Job.optSkipEvents, options.skip_events)

job.options().setDouble(ROOT.EL.Job.optCacheSize, 50*1024*1024)
job.options().setDouble(ROOT.EL.Job.optCacheLearnEntries, 50)
job.options().setString(ROOT.EL.Job.optXaodAccessMode, "branch")

#job.options().setString (ROOT.EL.Job.optXaodAccessMode, EL.Job.optXaodAccessMode_class);

# add our algorithm to the job
logging.info("creating algorithms")
alg = ROOT.SklimmerAnalysis()

alg.m_doSklimming = True;
alg.m_doSUSYObjDef = True;
alg.m_doEventSelection = True;
alg.m_writexAOD = True;
alg.m_writeFullCollectionsToxAOD = True;

print "isMC : " + options.isMC

if(options.isMC) :
    alg.isMC = 1
else :
    alg.isMC = 0

alg.m_Analysis = "bbmet";

output = ROOT.EL.OutputStream("treeOutput")
job.outputAdd(output)
ntuple = ROOT.EL.NTupleSvc("treeOutput")

treeWriter = ROOT.PlantATree();
if(options.isMC) :
    treeWriter.isMC = 1
else :
    treeWriter.isMC = 0

treeWriter.outputName = "treeOutput";

logging.info("adding algorithms")

job.algsAdd(ntuple)
job.algsAdd(alg)
job.algsAdd(treeWriter)


# make the driver we want to use:
# this one works by running the algorithm directly
logging.info("creating driver")
driver = None
if (options.driver == "direct"):
    print "direct driver"
    logging.info("running on direct")
    driver = ROOT.EL.DirectDriver()
    logging.info("submit job")
    driver.submit(job, options.submitDir)
elif (options.driver == "prooflite"):
    print "prooflite"
    logging.info("running on prooflite")
    driver = ROOT.EL.ProofDriver()
    logging.info("submit job")
    driver.submit(job, "submit_dir_prooflite")
elif (options.driver == "grid"):
    print "grid driver"
    logging.info("running on Grid")
    driver = ROOT.EL.PrunDriver()
#    outSampleName = "user.rsmith.grid.v8."+ options.inputDS + "%in:name[2]%" + "%in:name[3]%"
#    driver.options().setDouble(ROOT.EL.Job.optGridNFilesPerJob,  1);
#    driver.options().setString("nc_outputSampleName", outSampleName.replace("mc15_13TeV:","mc15_13TeV"));
    driver.options().setString("nc_outputSampleName", "user.rsmith.v2.fixBits.%in:name[2]%.%in:name[3]%.end")

#    driver.options().setString("nc_outputSampleName", "user.rsmith.testoutput.withslash")

    driver.options().setDouble(ROOT.EL.Job.optGridMergeOutput, 1);

    logging.info("submit job")
    driver.submitOnly(job,"submit_dir_grid")

