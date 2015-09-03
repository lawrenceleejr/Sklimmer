#!/usr/bin/python

import os
import subprocess
from optparse import OptionParser

inputfile = open("inputDS_25ns_signal.txt" , "r" )
outfile = open("inputDS_25ns_signalTIDONLY.txt", "w")

subresult = []
for line in inputfile :
    print line
    print "Checking rucio is configured"
    if(not os.getenv('RUCIO_HOME')) :
        print "You haven't configured rucio. Exiting"
        exit()

    rucioCall = 'rucio list-dids --short ' + line
    print "Running rucio to find taskId for the files in your dataset : " + line
    print rucioCall

    result = subprocess.Popen(rucioCall.split(), stdout=subprocess.PIPE).communicate()[0]

    for res in result.split() :
        print res
        if( "tid" in res) :  subresult.append(res)

    print subresult

for subres in subresult :
    outfile.write(subres+"\n")
