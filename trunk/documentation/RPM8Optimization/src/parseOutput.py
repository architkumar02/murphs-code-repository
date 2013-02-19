#!/usr/bin/python
# coding: utf-8
# Let's do some python analysis
import sys
import os
import math
import mctal

# Looping through all of the files in the directory
""""
ParseOutput
Reads all of the files in the directory, create a graph of the data files
"""
def ParseOutput(dirpath):
   data = list() 
   for files in os.listdir('Output'):
        # Looking through the files
        filename = os.path.join('Output',files)
        print "Parsing file: "+filename
        data.append(mctal.MCTAL(filename))
   return data

##########################################################################
def ParseFileName(filename):
    data = dict()
    tokens = filename.split('_')
    data['HDPE']=float(tokens[2])
    data['NumLayers']=int(tokens[4])
    data['AssemblySpace']=float(tokens[6])
    return data

##########################################################################
import createSurfaceCells as csc
def CountDetectorCells(t):
    command = 'python createSurfaceCells.py'
    command +=' --modSpace '+str(t['HDPE'])
    command +=' --filmLayers '+str(t['NumLayers'])
    command +=' --assemblySpace '+str(t['AssemblySpace'])
    os.system(command)
    s = 0
    with open('cells.txt','r') as f:
        for line in f:
            tokens = line.strip().split()
            if int(tokens[1]) == 3:
                s += 1
    return s

##########################################################################
import createSurfaceCells as csc
def SurfaceDict(t):
    command = 'python createSurfaceCells.py'
    command +=' --modSpace '+str(t['HDPE'])
    command +=' --filmLayers '+str(t['NumLayers'])
    command +=' --assemblySpace '+str(t['AssemblySpace'])
    os.system(command)
    surface = dict()
    with open('surfaces.txt','r') as f:
        for line in  f:
            tokens = line.strip().split()
            surface[tokens[0]] = float(tokens[2])
    return surface

##########################################################################
import csv, xlwt
from datetime import date
"""
Write
"""
def Write(data):
    book = xlwt.Workbook(encoding="utf-8")
    summary = book.add_sheet("Summary")
    # Writting the summary
    summaryHeaders = ['HDPE','Num Assemblies','Space Between Assemblies','Total Assemblies','Reactions','Reaction Errors']
    for col, value in enumerate(summaryHeaders):
        summary.write(0,col,value)
    row = 1
    print 'Starting to Write Summary'
    tallyKey = 4
    for d in data:
        t = ParseFileName(d.fname) 
        summary.write(row,0,t['HDPE'])
        summary.write(row,1,t['NumLayers'])
        summary.write(row,2,t['AssemblySpace'])
        summary.write(row,3,CountDetectorCells(t))
        summary.write(row,4,d.tallies[tallyKey].data[-1])
        summary.write(row,5,d.tallies[tallyKey].errors[-1])
        row += 1
    print 'Finsihed writing summary'
    
    # Writing the Surface 
    tallyKey = 2
    for d in data:
        row = 0
        t = ParseFileName(d.fname) 
        sheet = book.add_sheet(str.format('Mod_{0}_NumAssm_{1}_AssmSpace_{2}',t['HDPE'],t['NumLayers'],t['AssemblySpace']))
        # Writing
        header=['Surface','Distance','Thermal Fraction','Error']
        for col, value in enumerate(header):
            sheet.write(row,col,value)
        row +=1
        tally = d.tallies[tallyKey]
        surfaces = tally.axes['f'].arraycsn
        sDict = SurfaceDict(t)
        for i in range(len(surfaces)):
            sheet.write(row,0,int(surfaces[i]))
            sheet.write(row,1,sDict[surfaces[i]])
            thermalFraction = tally.data[4*i+1]/tally.data[4*i+3]
            error = thermalFraction*math.sqrt(math.pow(tally.data[4*i+1],2)+math.pow(tally.data[4*i+3],2))
            sheet.write(row,2,thermalFraction)
            sheet.write(row,3,error)
            row +=1

    # Saving before exit
    book.save(str(date.today())+"Data.xls")

##########################################################################
""" 
main
Parses the files in directory Output, and then writes output to .csv files
"""
def main():
    data = ParseOutput('Output')
    #print data
    Write(data)

if __name__ == "__main__":
    main()
