// Dear emacs, this is -*- c++ -*-                                                                                                                   // $Id: errorcheck.h 299732 2014-03-27 17:41:34Z krasznaa $
#ifndef SKLIMMER_ERRORCHECK_H
#define SKLIMMER_ERRORCHECK_H

#define CHECK( ARG )                                     \
  do {                                                  \
  const bool result = ARG;                           \
  if( ! result ) {                                   \
  ::Error( __PRETTY_FUNCTION__, "Failed to execute: \"%s\"", \
	   #ARG );                                \
  return 1;                                       \
  }                                                  \
  } while( false )

#endif // CPANALYSISEXAMPLES_ERRORCHECK_H

