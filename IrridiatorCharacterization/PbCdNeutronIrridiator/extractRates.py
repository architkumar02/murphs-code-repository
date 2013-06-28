#!/usr/bin/env python

import os
import numpy
import mctal as m

def getTallyUnionTotal(filename,tally):
  # Getting all the lines
  f = open(filename)
  lines = f.readlines()
  f.close()

  # Looking for the key
  foundTally = False
  surfaceUnion = 0
  lookForInTally = False
  lookForOutTally = False
  for line in lines:
    if line.startswith('1tally') and line.split('nps')[0].strip().endswith(tally):
      foundTally = True
    # Looking for the unions only
    if foundTally and line.strip().startswith('surface union total'):
      if lookForInTally:
        lookForInTally = False
        lookForOutTally = True
      else:
        lookForInTally = True
    # Extracting the tallies
    if lookForInTally and line.strip().startswith('total'):
      print 'Found in union total of tally: ',tally
      print line
      tokens = line.split()
      inTally = (tokens[1],tokens[2])
    if lookForOutTally and line.strip().startswith('total'):
      print 'Found out union total of tally: ',tally
      print line
      tokens = line.split()
      outTally = (tokens[1],tokens[2])
      break
  return (inTally,outTally)      

def main():
  filename = 'MillerConfig_GS20_PbCdGamma.o'
  getTallyUnionTotal(filename,'131')
  getTallyUnionTotal(filename,'141')
  getTallyUnionTotal(filename,'231')
  getTallyUnionTotal(filename,'241')
  getTallyUnionTotal(filename,'121')
  getTallyUnionTotal(filename,'221')

  # Saving the tally structure
  tallies = m.MCTAL('MillerConfig_GS20_PbCdGamma.m')
  tallies.WriteTally('PbCdGamma.xls')

if __name__ == "__main__":
  main()
