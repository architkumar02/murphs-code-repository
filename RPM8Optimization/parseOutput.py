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

    # Finding the interaction rates
    with open(filename,'r') as f:
        while True:
            line = f.readline()
            if line is None or not line: break
            # Checking if we need to extract a tally
            if line.startswith('tally'):
                tokens = line.strip().split()
                if tokens[1].isdigit() and int(tokens[1]) %10 == 4:
                    data['tallies'] = ReadTally(f)
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

import csv
def WriteSummary(data):
    with open('summary.csv','wb') as f:
        writer = csv.writer(f):
        writer.writerow(['HDPE','Num Assemblies','Space Between Assemblies','Reactions','Reaction Errors'])
        # Sum up each in data
        for d in data:
            tally = 0;
            error = 0;
            for c in data['tallies']:
""" 
main
Parses the files in directory Output, and then writes output to .csv files
"""
def main():
    data = ParseOutput('Output')
    WriteSummary(data)
    WriteData(data)

if __name__ == "__main__":
    main()
