Sklimmer
========

Skimmer/Slimmer for RJigsaw Implementation

Works in a RootCore setup. Get it into $ROOTCOREBIN and 

    rc find_packages; 
    rc compile;

This will give you the actually sklimming tool which can be steered by the files in run/.

For simplicity, you can test by doing

    cd run/;
    source runSklimmer; 

which will create a run directory that includes the output file. You'll need to edit runSklimmer to point to a folder with D3PDs that has a structure ./[DSName]/*.root* like:

    ./mc12_8TeV.105200.McAtNloJimmy_CT10_ttbar_LeptonFilter.merge.NTUP_COMMON.e1513_s1499_s1504_r3658_r3549_p1562/NTUP_COMMON.01315237._002716.root


This is already built to use the grid for when we're at full steam.

Root/SklimmerAnalysis.cxx contains the use of the TRJigsaw tool and is where we can output whatever variables we need in the final sklims. Many RJigsaw variables are already being written out to the sklim.

A note about the grid - I've only gotten grid submission to work for RootCore Base,1.5.2 and EventLoopGrid-00-00-23 and then maybe a little hack in EventLoopGrid. Let me know if you run into that same problem and I can give you a hacked file.
