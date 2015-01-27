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


Works in a RootCore setup. Requires SUSYTools and RJigsaw. Current working system can be setup:

setupATLAS
rcSetup -u
rcSetup Base,2.0.22
rc checkout packages.txt
cd EventShapeTools && svn patch -p0 -i EventShapeTools.diff
#Then you'll also need Sklimmer and RJigsaw - Will be moved to CERN svn at some point
svn co https://github.com/lawrenceleejr/Sklimmer/trunk Sklimmer
svn co https://github.com/lawrenceleejr/RJigsaw/trunk RJigsaw
rc clean
rc find_packages
rc compile


You can test on the xAOD sample from the tutorials:

export ALRB_TutorialData=/afs/cern.ch/atlas/project/PAT/tutorial/cern-nov2014/
cd Sklimmer/Run
root -l 'ATestRun.cxx ("submitDir")' # Make sure ATestRun.cxx is using the xAOD from the above location



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

	// Write out ntuples for analysis/HistFitter - Not a thing yet...
	bool m_writeNtuple; //!

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

