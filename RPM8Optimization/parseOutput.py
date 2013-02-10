#!/usr/bin/python
# coding: utf-8
# Let's do some python analysis
import sys
import os
import math
import networkx as nx

"""
ReadTally
"""
def ReadTally(f):
    data = dict()
    tallies = list()
    # Need to extract a tally - looking for the vals card
    line = f.readline()
    while not line.startswith('vals'):
        line=f.readline()
        if line.startswith('f'):
            cells=f.readline().strip().split()
    # Next stop card is the tfc
    line = f.readline()
    while not line.startswith('tfc'):
        # These are the ones we want!
        tallies.append([float(t) for t in line.strip().split()])
        line = f.readline()
    # Creating a mapping between the cells and the tallies
    i = 0;
    tallies=sum(tallies,[])
    for c in cells:
        if c in data or int(c) == 0:
            data['total'] = [tallies[2*i],tallies[2*i+1]]
        else:
            data[c] = [tallies[2*i],tallies[2*i+1]]
        i=i+1
    return data

"""
ReadFile
"""
def ReadFile(filename):
    # Creating the return value
    data = dict()
    # Parsing the filename
    tokens = filename.split('_')
    data['HDPE']=float(tokens[2])
    data['NumLayers']=int(tokens[4])
    data['AssemblySpace']=float(tokens[6])

    # Sum up each in data
    tally = 0;
    error = 0;
    # Finding the interaction rates
    with open(filename,'r') as f:
        while True:
            line = f.readline()
            if line is None or not line: break
            # Checking if we need to extract a tally
            if line.startswith('tally'):
                tokens = line.strip().split()
                if tokens[1].isdigit() and int(tokens[1]) %10 == 4:
                    key = 'tally'+tokens[1]
                    data[key] = ReadTally(f)
                    tally = tally + data[key]['total'][0]
                    error = error + math.pow(data[key]['total'][0]*data[key]['total'][1],2)
    data['TallyTotal']=[tally,math.sqrt(error)]
    return data

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
        data.append(ReadFile(filename))
   return data

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
    summaryHeaders = ['HDPE','Num Assemblies','Space Between Assemblies','Reactions','Reaction Errors']
    for col, value in enumerate(summaryHeaders):
        summary.write(0,col,value)
    row = 1
    for d in data:
        summary.write(row,0,d['HDPE'])
        summary.write(row,1,d['NumLayers'])
        summary.write(row,2,d['AssemblySpace'])
        summary.write(row,3,d['TallyTotal'][0])
        summary.write(row,4,d['TallyTotal'][1])
        row += 1

    # Writing the data 
    for d in data:
        book.add_sheet(str.format('Mod_{0}_NumAssm_{1}_AssmSpace_{2}',d['HDPE'],d['NumLayers'],d['AssemblySpace']))
    
    # Saving before exit
    book.save(str(date.today())+"Data.xls")

##########################################################################
""" 
main
Parses the files in directory Output, and then writes output to .csv files
"""
def main():
    data = ParseOutput('Output')
    Write(data)

if __name__ == "__main__":
    main()
