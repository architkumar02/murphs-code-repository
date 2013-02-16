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
    print 'Starting to Write Summary'
    for d in data:
        summary.write(row,0,d['HDPE'])
        summary.write(row,1,d['NumLayers'])
        summary.write(row,2,d['AssemblySpace'])
        summary.write(row,3,d['TallyTotal'][0])
        summary.write(row,4,d['TallyTotal'][1])
        row += 1
    print 'Finsihed writing summary'
    
    # Writing the data (grouping by numAssm}
    row = 0
    for d in data:
        val = list()
        val.append(d['HDPE']); del d['HDPE']
        val.append(d['NumLayers']); del d['NumLayers']
        val.append(d['AssemblySpace']); del d['AssemblySpace']
        sheet = book.add_sheet(str.format('Mod_{0}_NumAssm_{1}_AssmSpace_{2}',val[0],val[1],val[2]))
        # Writing
        header=['Moderator (cm)','Num Assemblies','Space between assemblies','Tally','Cell','Reactions','Reactions Error']
        for col, value in enumerate(header):
            sheet.write(row,col,value)
        row +=1
        del d['TallyTotal']
        for tkey in d:             # Dictionary with tallies as keys
            for col, value in enumerate(val):
                sheet.write(row,col,value)
            col = len(val) 
            sheet.write(row,col,tkey)
            col +=1
            t = d[tkey]
            for c in t.keys():          
                if c == 'total':
                    pass
                sheet.write(row,col,c)
                col += 1
                sheet.write(row,col,t[c][0])
                col += 1
                sheet.write(row,col,t[c][1])
                col += 1
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
