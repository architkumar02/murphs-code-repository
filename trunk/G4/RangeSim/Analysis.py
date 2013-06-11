#!/usr/bin/env python
import sys
import xlwt
import os
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib.pyplot import *

def ReadNISTFile(filename):
  r = list()
  energy = list()
  with open(filename) as f:
    for i in range(8):
      next(f)
    for line in f:
      [e,csda] = line.split()
      energy.append(float(e))
      r.append(float(csda))
  return [energy,r]

def GetNISTRANGES(dirName,pType):
  data = dict()
  # Getting List of Files and parsing data
  files = [f for f in os.listdir(dirName) if pType in f]
  for f in files:
    mat = f.split('_',1)[1]
    data[mat] = ReadNISTFile(os.path.join(dirName,f))
  return data

def GetRanges(f):
  ranges = list()
  for line in f:
    line = line.strip()
    if line.startswith('The run was'):
      tokens = line.split()
      ranges.append(dict())
      ranges[-1]['particle'] = tokens[4] 
      ranges[-1]['energy'] = tokens[6]
      ranges[-1]['material'] = tokens[12]
      ranges[-1]['density'] = tokens[-3]+' '+tokens[-2]
    if line.startswith('mass true Range'):
      tokens = line.split('=')
      [r,rerr] = tokens[1].split('+/-')
      ranges[-1]['mass true range'] = float(r.strip().split()[0])
      ranges[-1]['mass true range rms'] = float(rerr.strip().split()[0])
  return ranges

def WriteData(filename,ws):
  # Reading in the Data
  ranges = None
  with open(filename,'r') as f:
    ranges = GetRanges(f)
  # Writing to XLS worksheet
  row = 0
  ws.write(0,0,'Material')
  ws.write(0,1,'Density')
  ws.write(0,2,'2.05 MeV Alpha CSDA Range')
  ws.write(0,4,'2.73 MeV Triton CSDA Range')
  ws.write(0,6,'1.78 MeV Alpha CSDA Range')
  ws.write(0,8,'1.02 MeV Li-7 CSDA Range')
  for i in range(4):
    ws.write(1,2+2*i,'Range g/cm^2')
  rowCount = 1
  mat = dict()
  for d in ranges:
    if not str(d['material']) in mat:
      rowCount += 1
      mat[str(d['material'])] = rowCount
      ws.write(rowCount,0,d['material'])
      ws.write(rowCount,1,d['density'])
    # Writing to the correct row
    row = mat[d['material']]
    p = (d['particle']).strip()
    e = (d['energy']).strip()
    den = float(d['density'].split()[0])
    r = d['mass true range']
    rerr =d['mass true range rms']
    if p == 'alpha':
      if float(e) == 2.05:
        ws.write(row,2,r)
        ws.write(row,3,rerr)
      elif float(e) == 1.78:
        ws.write(row,6,r)
        ws.write(row,7,rerr)
      else:
        print e+' is not reconized'
    elif p == 'triton':
        ws.write(row,4,r)
        ws.write(row,5,rerr)
    elif p == 'Li7[0.0]':
        ws.write(row,8,r)
        ws.write(row,9,rerr)
    else:
      print p+' is not recongized'

def WriteWaterPDTA(filename,ws):
  # Reading in the Data
  ranges = None
  with open(filename,'r') as f:
    ranges = GetRanges(f)
  # Writing to the XLS worksheet
  headers = ['Energy (MeV)','Proton Range','','Deuteron','','Triton','','Alpha Range','']
  pcol = {'proton':1,'deuteron':3,'triton':5,'alpha':7}
  erow = {10:1,5:2,1:3,0.5:4,0.1:5,0.05:6,0.01:7}
  for col in range(len(headers)):
    ws.write(0,col,headers[col])
  for eKey in erow:
    ws.write(erow[eKey],0,eKey)
  for d in ranges:
    p = (d['particle']).strip()
    e = (d['energy']).strip()
    r = d['mass true range']
    rerr =d['mass true range rms']
    ws.write(erow[float(e)],pcol[p],r) 
    ws.write(erow[float(e)],pcol[p]+1,rerr) 

def WriteVal(filename,star,ws,name):
  # Reading in the Data
  ranges = None
  with open(filename,'r') as f:
    ranges = GetRanges(f)
  # Writing to XLS worksheet
  row = 0
  ws.write(0,0,'Material')
  ws.write(0,1,'Energy')
  ws.write(0,2,'SIM CSDA')
  ws.write(0,4,'SIM/NIST')
  ws.write(0,5,'SIM/NIST Err')
  row = 1
  matDict = dict()
  for d in ranges:
    mat = d['material']
    e = float(d['energy'])
    r = d['mass true range']
    rerr = d['mass true range rms']
    nistVal = np.interp(e,star[mat][0],star[mat][1])
    if nistVal <= 0:
      pass
    if not mat in matDict:
      matDict[mat] = list()
    matDict[mat].append([e,r/nistVal])
    ws.write(row,0,d['material'])
    ws.write(row,1,e)
    ws.write(row,2,r)
    ws.write(row,3,rerr)
    ws.write(row,4,r/nistVal)
    ws.write(row,5,r/nistVal*(rerr/r))
    row += 1

  # Plotting
  col = ['bs','go','r^','c>']
  fig = plt.figure()
  ax = plt.subplot(111)
  i = 0
  for mat in matDict:
    data = np.transpose(np.array(matDict[mat]))
    ax.plot(data[:][0],data[:][1],col[i],label=mat)
    i += 1
  ax.legend(loc=3,bbox_to_anchor=(0.05,1.05),prop={'size':8},ncol=4,borderaxespad=0.)
  plt.yscale('linear')
  plt.xlabel("Energy (MeV)")
  plt.ylabel("Sim Range / NIST Range")
  if name == 'electron':
    plt.xscale('log')
  else:
    plt.xscale('linear')
  fig.savefig(name+'.png')

def main():
  # Reading in the NIST DATA
  print '\tImporting NIST Data'
  estar = GetNISTRANGES('NIST_DATA','estar')
  astar = GetNISTRANGES('NIST_DATA','astar')

  # Analayzing the Data
  wb = xlwt.Workbook()
  print '\tComparing to NIST Values'
  WriteVal('out_val_arange.txt',astar,wb.add_sheet('Alpha Validaiton'),'alpha')
  WriteVal('out_val_erange.txt',estar,wb.add_sheet('Electron Validaiton'),'electron')
  WriteWaterPDTA('out_pdta.txt',wb.add_sheet('Water PDTA'))
  print '\tGathering Computed Data'
  WriteData('out_range.txt',wb.add_sheet('Range'))
  
  wb.save('RangeSimAnalysis.xls')

if __name__ == '__main__':
  print "Running Analysis . . ."
  main()
  print "Finished Analysis"
