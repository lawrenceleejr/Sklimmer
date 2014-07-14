#include "Riostream.h"

void StatusMacro ()
{
  // Load the libraries for all packages
  gROOT->ProcessLine(".x $ROOTCOREDIR/scripts/load_packages.C");


  EL::GridDriver::status(); 
} // end function SteeringMacro
