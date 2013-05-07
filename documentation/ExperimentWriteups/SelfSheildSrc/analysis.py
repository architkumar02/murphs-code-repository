#!/usr/bin/env python
# Matthew J. Urffer (matthew.urffer@gmail.com)
#
# Module for analyzing the results of self sheilding in GS20
#
# CHANGELOGE:
#   4/25/2013 - Created File

import numpy as np
import mctal
import glob
import xlwt
        
def main():
    # Reading the data
    files = glob.glob('*.m')
    data = dict()
    for f in files:
      mtNum = f.split('_')[1].split('.m')[0]
      m = mctal.MCTAL(f)
      tallyData = dict()
      # Getting data for lead and cadmium wells
      for tally in m.tallies.keys():
        d = np.array(m.tallies[tally].data)
        e = np.array(m.tallies[tally].errors)*d
        tallyData[tally] = [d, e]
      # Creating Net Data
      dFlux = tallyData[104][0] - tallyData[204][0]
      eFlux = np.sqrt(np.power(tallyData[104][1],2)+np.power(tallyData[204][1],2))
      tallyData['fluxNet'] = [dFlux,eFlux]
      dRXN = tallyData[154][0] - tallyData[254][0]
      eRXN = np.sqrt(np.power(tallyData[154][1],2)+np.power(tallyData[254][1],2))
      tallyData['rxnNet'] = [dRXN,eRXN]
      # Adding to dictionary
      data[mtNum] = tallyData

    # Saving the data to xls
    wb = xlwt.Workbook()
    cellNames = (700,701,702,703,704,705,706,707,708,709,'total')
    mtNum = data.keys()
    keys = ('fluxNet','rxnNet')
    for key in keys:
        ws = wb.add_sheet(key)
        for r in range(len(cellNames)):
            ws.write(r+1,0,cellNames[r])
        for c in range(len(mtNum)):
            ws.write(0,2*c+1,mtNum[c])
        for c in range(len(mtNum)):
            for r in range(len(cellNames)):
                ws.write(r+1,2*c+1,data[mtNum[c]][key][0][r])
                ws.write(r+1,2*c+1+1,data[mtNum[c]][key][1][r])
            
    fileName = 'GS20SelfSheildData.xls'
    wb.save(fileName)
    print "Wrote output: ",fileName
if __name__ == "__main__":
    main()
