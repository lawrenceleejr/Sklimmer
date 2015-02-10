Sklimmer
========

Skimmer/Slimmer for RJigsaw Implementation

Uses SUSYTools+EventLoop setup.

Input an xAOD. Flag configurable to

Sklim
Run SUSYTools for calibration, overlap removal, etc
Write out xAOD
Write out ntuple
Do event selection for string-configurable analysis - e.g. analysis-specific trigger skims


### How to get set up

Works in a RootCore setup. Requires SUSYTools and Ext_RestFrames. Current working system can be setup:

```
setupATLAS
rcSetup Base,2.0.22
rc checkout packages.txt
cd EventShapeTools && svn patch -p0 -i EventShapeTools.diff #convenient patch from diff - Giordon
#Then you'll also need Sklimmer and Ext_RestFrames - Will be moved to CERN svn at some point
svn co https://github.com/lawrenceleejr/Sklimmer/trunk Sklimmer
svn co https://github.com/lawrenceleejr/Ext_RestFrames/trunk Ext_RestFrames
rc clean
rc find_packages
rc compile
```

You can test on the xAOD sample from the tutorials:

export ALRB_TutorialData=/afs/cern.ch/atlas/project/PAT/tutorial/cern-nov2014/
cd Sklimmer/Run
root -l 'ATestRun.cxx ("submitDir")' # Make sure ATestRun.cxx is using the xAOD from the above location

In practice, I think we'll use the scripts/Run.py steering script. See file for details.


### Run it with python steering code

Stolen from the SUSY group example and modified. Used to run job directly, use FAX, or submit to the grid. (grid is default). The option inputDS can be left blank to scan CWD, given a single DQ2 search string, or be given a text file e.g. "inputDS.txt". Simple running over a bunch of datasets can be done with

```
python Run.py  --inputDS "mc14_13TeV.202266.MadGraphPythia_AUET2BCTEQ6L1_SM_BB_direct_800_1_MET50.merge.AOD.e3064_s1982_s2008_r5787_r5853/" --runTag="021015a"
python Run.py --inputDS "inputDS.txt" --runTag="021015b"
```

Results can be pulled down from the grid using the Wait.py and Retrieve.py scripts which take the --submitDir option to point them to the SampleHandler structure used in the submission.

```
python Wait.py --submitDir "submit_dir"
python Retrieve.py --submitDir "submit_dir"
```


### Configuration Flags in Run Script

You can turn each step on and off with the following flags that are public members of the algo class (See Run/ATestRun.cxx for example usage.)

```
	// To turn on the copying of branches to an output xAOD
	bool m_doSklimming; //!

	// To run the SUSYTools object definition/calibration and write shallow-copies to the TStore/TEvent
	bool m_doSUSYObjDef; //!

	// To run a selection function of your own writing. Which function is configurable via m_Analysis (e.g. "bbmet")
	bool m_doEventSelection; //!

	// Output xAOD switch
	bool m_writexAOD; //!

	// Should just be true for now - if we implement some thinning, then this will be relevant
	bool m_writeFullCollectionsToxAOD; //!
 
	// Name of event selection for steering m_doEventSelection
	std::string m_Analysis; //!
```


e.g. Event Selection in TString SklimmerAnalysis::eventSelectionBBMet() in execute() like:

```
	if( m_doEventSelection && m_Analysis=="bbmet" ){
		TString result = eventSelectionBBMet();
		if(result=="") return EL::StatusCode::SUCCESS;
		else {
			eventInfo->auxdecor< char >("selection") = *result.Data();
		}
	}
```

And does whatever you want that's analysis-specific that you don't want to do at the ntuple level, e.g. trigger skimming. Can return a string with a region label that gets written out as an EventInfo decoration in case you wanted to, say, define CR/VR/SRs here. Or whatever you want. 



### NTuple Output - **NEW**

The package contains an algorithm class called Sklimmer::PlantATree that can be added to the job (after SklimmerAnalysis) which reads out the calibrated objects from the TStore and writes out important information to a small flat NTuple which can be used for plotting, HistFitter, or whatever you want.

See PlantATree.h/cxx for information, and Run/ATestRun.cxx for example usage. 

Currently writes out event info and calibrated physics objects.

