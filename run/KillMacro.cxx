#include "Riostream.h"

void KillMacro ()
{
  // Load the libraries for all packages
  gROOT->ProcessLine(".x $ROOTCOREDIR/scripts/load_packages.C");


  EL::GridDriver::stopServer(); 
  EL::GridDriver::killAll(); 
  EL::GridDriver::killAll(); 
  EL::GridDriver::reset(); 
} // end function SteeringMacro
