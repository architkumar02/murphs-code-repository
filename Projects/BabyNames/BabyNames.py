#!/usr/bin/python

import os
import sys
import collections
import csv
import numpy as np
import matplotlib.pyplot as plt
import DataCursor

# Reading in the names
NameRecord = collections.namedtuple('nameRecord','name,gender,occurance')

def ReadNames(dir='names'):
	maleNames = dict()
	femaleNames = dict()
	for r,d,f in os.walk(dir):
		for files in f:
			if files.endswith(".txt"):
				year = files.strip('yob.txt')
				maleYear = dict()
				femaleYear = dict()
				for rec in map(NameRecord._make,
						csv.reader(open(os.path.join(r,files),'rb'))):
					if rec.gender == 'F':
						femaleYear[rec.name] = int(rec.occurance)
					else:
						maleYear[rec.name] = int(rec.occurance)
				maleNames[year] = maleYear
				femaleNames[year] = femaleYear
	return {'M':maleNames,'F':femaleNames}

def ReadBiblicalNames(fileName='BiblicalNames.txt'):
	f = open(fileName,'r+')
	return set(f.read().split('\n'))

def ReadPresidents(filename='PresidentList.txt'):
	pres = dict()	# Dictionary between year and first name
	vp = dict()		# Dictionary between year and first name

	with open(filename,'r') as f:
		for line in f:
			fields = line.strip('\r\n').split('\t')
			# Spitting the year
			(termStart,termEnd) = fields[1].rstrip(' ').split('-')
			# Grapping the Pres First Name
			presName = fields[2].split(' ')
			# Grapping the VP First Name
			vpName = fields[3].split(' ')
			for y in range(int(termStart),int(termEnd)):
				year = str(y)
				pres[year] = set()
				pres[year].add(presName[0])
				pres[year] = tuple(pres[year])[0]
				vp[year] = set()
				vp[year].add(vpName[0])
				vp[year] = tuple(vp[year])[0]
	return {'Pres':pres,'VP':vp}

def Popularity(nameDict,prevYears = 10):
	i = 0
	popularity = dict()
	for k in sorted(nameDict.keys()):
		i += 1
		if (i <= prevYears):
			continue
		popularity[k] = set()
		for y in range(int(k)-prevYears,int(k)):   
			popularity[k].add(nameDict[str(y)])
		popularity[k] = tuple(popularity[k])
	return popularity

def CountNamesPerYear(yearDict,nameKey):
	s = 0
	for n in nameKey:
		try:
			s += yearDict[n]
		except:
			pass
	return s


def NameTotals(names):
	data = np.zeros((len(names),2))
	i = 0
	for year in sorted(names.keys()):
		data[i,0] = int(year)
		for k in names[year]:
			data[i,1] += names[year][k]
		i = i+1
	return data


def NamesFrequency(names,searchKey):
	data = np.zeros((len(names),2))
	i = 0
	for year in sorted(names.keys()):
		data[i,0] = int(year)
		try:
			if isinstance(searchKey[year],basestring):
				key = set()
				key.add(searchKey[year])
				key = tuple(key)
				data[i,1] = CountNamesPerYear(names[year],key)
			else:
				data[i,1] = CountNamesPerYear(names[year],searchKey[year])
		except:
			data[i,1] = CountNamesPerYear(names[year],searchKey)
		i = i+1
	return data

def BiblicalRun():
	# Looking at blibical name
	biblicalNames = ReadBiblicalNames()
	names =ReadNames()

	maleCounts = NamesFrequency(names['M'],biblicalNames)
	maleTotal = NameTotals(names['M'])
	femaleTotal = NameTotals(names['F'])
	femaleCounts = NamesFrequency(names['F'],biblicalNames)

	plt.figure()
	plt.plot(maleCounts[:,0],maleCounts[:,1]/maleTotal[:,1])
	plt.plot(femaleCounts[:,0],femaleCounts[:,1]/femaleTotal[:,1])

	plt.legend(["Male","Female"])
	plt.ylabel("Fraction of Total Names")
	plt.xlabel("Year")
	plt.title('Fraction of Population Born with a Biblical Name')
	plt.savefig('BiblicalNames.png',bbox_inches=0)

def PresidentRun():
	# Looking at the Presidents
	plt.figure()
	names = ReadNames()
	presidents = ReadPresidents()
	presCounts = NamesFrequency(names['M'],presidents['Pres'])
	popusPopDict = Popularity(presidents['Pres'])
	presTotal = NamesFrequency(names['M'],popusPopDict)
	vpCounts = NamesFrequency(names['M'],presidents['VP'])
	vpPopDict = Popularity(presidents['VP'])
	vpTotal = NamesFrequency(names['M'],vpPopDict)
	l1 = plt.plot(presCounts[:,0],presCounts[:,1]/presTotal[:,1])
	l2 = plt.plot(vpCounts[:,0],vpCounts[:,1]/vpTotal[:,1])
	plt.legend(["PTUS","Vice POTUS"])
	plt.ylabel("Fraction of Previous 10 Year Names")
	plt.xlabel("Year")
	plt.title('Popularity of a presidents name, normalized by the previous 10 years')
	plt.savefig('PresidentNames.png',bbox_inches=0)
	DataCursor([l1,l2])
	plt.show()

def main():
	BiblicalRun()
	print "Running President Names"
	PresidentRun()

if __name__ == "__main__":
	sys.exit(main())
