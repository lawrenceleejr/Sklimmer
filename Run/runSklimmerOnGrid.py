#!/usr/bin/env python

import time
ts = time.time()

import datetime
st = datetime.datetime.fromtimestamp(ts).strftime('%Y%m%d_%H%M%S')

import os

import commands

f = open('inputDatasets.txt')
lines = f.readlines()


# os.system(""" root -l -b -q 'KillMacro.cxx () ' """);


for DS in lines:
	DS = DS[:-1]

	if len(DS)==0:
		continue
	if DS[0]=="#":
		continue

	# print """ root -l -b -q 'SteeringMacro.cxx ("jobs/job_%s",false,"%s")' """ %(DS,DS)  
	print DS
	os.system(""" rm -rf jobs/job_%s_%s """ %(st,DS)  );
	# os.system(""" root -l -b -q 'KillMacro.cxx () ' """);

	os.system(""" root -l -b -q 'SteeringMacro.cxx ("jobs/job_%s_%s",false,"%s","%s")' """ %(st,DS,DS, st)  )

	os.system(""" root -l -b -q 'StatusMacro.cxx ()' """ );

	while(1):
		status, output = commands.getstatusoutput("root -l -b -q 'StatusMacro.cxx ()'")
		print output
		if(output.find("hold") == -1):
			break;
		time.sleep(10)


		# var = raw_input("enter 'n' to move on or 's' to get a status update: ")
		# if var == 'n':
		# 	break
		# elif var == 's':
		# 	os.system(""" root -l -b -q 'StatusMacro.cxx ()' """ );


	print "moving on..."
	time.sleep(1)

#os.system(""" rm -rf jobs/job """   )
# os.system(""" root -l -b -q 'SteeringMacro.cxx ("jobs/job_%s",false,"%s","%s")' """ %(st,'inputDatasets.txt', st)  )
