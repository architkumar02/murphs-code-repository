#!/usr/bin/env python
import os, os.path
import xlwt

def SetupData(dirName):
  # Setting up the data storage
  data = dict()
  for fname in os.listdir(dirName):
    [root,ext] = os.path.splitext(fname)
    if ext == '.o':
      [runType,radiusString,_, thicknessString,_] = root.split('_')
      if radiusString not in data:
        data[radiusString] = dict()
      data[radiusString][thicknessString] = list()
  return data

def GammaAnalysis():
  print 'Starting Gamma Analysis'
  # Writing the size in the comment and setting up the worksheets
  dirName = 'GammaOutputs'
  longName = ['Thickness','In Size','Out Side','In Top','Out Top','In Bottom','Out Bottom','In Union','Out Union']

  # Setting up the data storage
  data = SetupData(dirName)
  
  # Getting the actual data
  for fname in os.listdir(dirName):
    [root,ext] = os.path.splitext(fname)
    if ext == '.o':
      # Parsing the file
      print 'Processing File: ',fname
      [runType,radiusString,_, thicknessString,_] = root.split('_')

      with open(dirName+'/'+fname,'r') as f:
          printTokens = False
          for line in f:
            # Setting the to print boolean
            if line.startswith('1tally   1'):
              printTokens = True
              continue
            if not line.strip():
              continue
            # Unsetting the to print boolean
            if printTokens and line.startswith('1analysis'):
              printTokens = False
            # Printing the value
            if (printTokens):
              line = line.strip()
              if line[0].isdigit():
                tokens = line.split()
                data[radiusString][thicknessString].append(float(tokens[0])) 

  # Writing the output
  WriteOutput('GammaAnalysis.xls',data,longName)

def WriteOutput(outName,data,longName):
  # Generating Units
  units = ['cm']
  for i in range(len(longName)-1):
    units.append('Number per Source Particle')
  
  wb = xlwt.Workbook()
  for key in data:
    radius = data[key]
    ws = wb.add_sheet(key[2:])
    
    # Writting the Headers
    row = 0
    for col,s in enumerate(longName):
      ws.write(0,col,s)
    for col,s in enumerate(units):
      ws.write(1,col,s)
      if col > 0:
        ws.write(2,col,key[2:])
    row = 3

    # Writing the data
    for thickness in radius:
      ws.write(row,0,float(thickness[2:]))
      tallies = radius[thickness]
      for i,value in enumerate(tallies):
        ws.write(row,1+i,value)
      row += 1
  wb.save(outName)

def ProccessFile(fname,tallyKey):
  data = list()
  with open(fname,'r') as f:
    printTokens = False
    surfaceUnion = False
    for line in f:
      if not line.strip():
        line = line.strip()
        continue
      # Setting the to print boolean
      if line.startswith(tallyKey):
        printTokens = True
        continue
      # Setting up the surface union token
      if printTokens and line.strip() == 'surface union total':
        surfaceUnion = True
        continue
      # Unsetting the to print boolean
      if printTokens and line.startswith('1analysis'):
        printTokens = False
      # Printing the value
      if (printTokens and surfaceUnion):
        line = line.strip()
        if line.startswith('total'):
          tokens = line.split()
          data.append(float(tokens[1]))
          surfaceUnion = False
  return data

def NeutronAnalysis():
  print 'Starting Neturon Analysis'
  dirName = 'NeutronOutputs'
  data = SetupData(dirName)
  longName = ['Thickness','Pb In Union','Pb Out Union','Cd In Union','Cd Out Union','Net In','Net Out']
  for fname in os.listdir(dirName):
    [root,ext] = os.path.splitext(fname)
    if ext == '.o':
      [runType,radiusString,_, thicknessString,_] = root.split('_')
      # Parsing the file
      print 'Processing File: ',fname
      data[radiusString][thicknessString].extend(ProccessFile(dirName+'/'+fname,'1tally  11'))
      data[radiusString][thicknessString].extend(ProccessFile(dirName+'/'+fname,'1tally  21'))
      netIn = data[radiusString][thicknessString][0] - data[radiusString][thicknessString][2]
      netOut = data[radiusString][thicknessString][1] - data[radiusString][thicknessString][3]
      data[radiusString][thicknessString].append(netIn)
      data[radiusString][thicknessString].append(netOut)
  
  WriteOutput('NeutronAnalysis.xls',data,longName)

def main():
  GammaAnalysis()
  NeutronAnalysis()

if __name__ == '__main__':
  main()
