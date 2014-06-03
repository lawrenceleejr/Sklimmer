Sklimmer
========

Skimmer/Slimmer for RJigsaw Implementation

Works in a RootCore setup. Get it into $ROOTCOREBIN and 

rc find_packages
rc compile

This will give you the actually sklimming tool which can be steered by the files in run/.

For simplicity, you can test by doing

cd run/
source runSklimmer

which will create a run directory that includes the output file. This is already built to use the grid for when we're at full steam.

Root/SklimmerAnalysis.cxx contains the use of the TRJigsaw tool and is where we can output whatever variables we need in the final sklims. Many RJigsaw variables are already being written out to the sklim.
