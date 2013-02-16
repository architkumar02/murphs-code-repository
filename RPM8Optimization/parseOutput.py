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
    command +=' --modSpace '+t['HDPE']
    command +=' --filmLayers '+t['NumLayers']
    command +=' --assemblySpace '+t['AssemblySpace']
    os.system(command)
    s = 0
    for k in csc.material:
        if csc.material[k]['name'] is 'Detector':
            s += 1
    return s

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
    tallyKey = '4'
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
    row = 0
    for d in data:
        t = ParseFileName(d.fname) 
        sheet = book.add_sheet(str.format('Mod_{0}_NumAssm_{1}_AssmSpace_{2}',t['HDPE'],t['NumLayers'],t['AssemblySpace']))
        # Writing
        header=['Surface','Distance','Thermal Fraction','Error']
        for col, value in enumerate(header):
            sheet.write(row,col,value)
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
    print data[0]
    #print data
#    Write(data)

if __name__ == "__main__":
    main()
