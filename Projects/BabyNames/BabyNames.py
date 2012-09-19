#!/usr/bin/python

import os
key = 'Lexie'
print 'Year,Female Births,Name ends with "ie",Fraction'
for r,d,f in os.walk('names'):
	for files in f:
		if files.endswith(".txt"):
			femaleBirths = 0
			namesEndInIE = 0
			# Grapping the birth year
			year = int(files.strip('yob.txt'))
			fin = open(os.path.join(r,files),'r')
			for line in fin:
				fields = line.rstrip('\n').split(',')
				if fields[1] == 'F':
					femaleBirths += int(fields[2])
					if fields[0].endswith(key):
						namesEndInIE += int(fields[2])
			print '%d,%d,%d,%f'%(year,femaleBirths,namesEndInIE,float(namesEndInIE)/float(femaleBirths))
