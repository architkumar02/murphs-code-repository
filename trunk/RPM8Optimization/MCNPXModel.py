#!/usr/bin/python
# @author matther.uffer@gmail.com
#
# Runs MCNPX on a simulated geometry. Expects to have SCRIPT.mcnp and 
#   queueRunScript.sh
#

class MCNPX:
    
    # Material Dictionaries
    detMaterial = {'name':'Detector','mt': 3, 'rho': 1.1}       # detector
    lightGuideMaterial = {'name': 'PMMA','mt':10, 'rho':0.93}   # PMMA
    modMaterial = {'name':'HDPE','mt':456, 'rho': 0.93}         # HPDE
    cellForStr = '{:5d} {:d} -{:4.3f} {:d} -{:d} 502 -503 504 -505\n'
    surForStr = '{:5d} px {:5.3f}\n'

    def __init__(self):
        # Material dictionary for the moderator, light guide, and detector
        self.material =  {'Moderator':self.modMaterial,'Detector':self.detMaterial,
                            'LightGuide':self.lightGuideMaterial}
        # Cell and Surface Inital Numbering
        self.CellStartNum = 6000
        self.SurfaceStartNum = 6000
        self.ZeroSurfaceNum = 500
        
    def createInputDeck(self,geo,oFile='INP.mcnp',baseFile='SCRIPT.mcnp'):
        """ createInputDeck 

        Creates an input deck of the given geometry
        Keyword Arguments:
        geo -- geometry description
        oFile -- output file name (default is INP.mcnp)
        baseFile -- base input deck (default is SCRIPT.mcnp')

        """

        numCells = 0
        detectorCells = list()
        keyList = sorted(geo.keys(), key = lambda x: float(x))
        with open('SCRIPT.mcnp','r') as i,open(oFile,'w') as o:
            for line in i:
                if line.startswith('c CELLPREPENDTOKEN'):
                    # Write the cells
                    cNum = self.CellStartNum
                    sNum = self.SurfaceStartNum
                    sPrev = self.ZeroSurfaceNum
                    for key in keyList:
                        # Creating the cell
                        m = self.material[geo[key]] 
                        line = self.cellForStr.format(cNum,m['mt'],m['rho'],
                                sPrev,sNum)
                        o.write(line)
                        # List of cells for the detector
                        if geo[key] is 'Detector':
                            detectorCells.append(cNum)
                        # Incrementing counters
                        sPrev = sNum
                        sNum += 1
                        cNum += 1
                        numCells +=1
                    # Need to write the last cell!
                    sNum = 501
                    m = self.modMaterial
                    line = self.cellForStr.format(cNum,m['mt'],m['rho'],sPrev,sNum)
                    numCells +=1
                        
                elif line.startswith('c SURFACEPREPENDTOKEN'):
                    # Write the surfaces
                    sNum = self.SurfaceStartNum
                    sPrev = self.ZeroSurfaceNum
                    for key in keyList:
                        o.write(self.surForStr.format(sNum,float(key)))
                        sPrev = sNum
                        sNum += 1
                
                elif line.startswith('c TALLYPREPENDTOKEN'):
                    # Write the tallies
                    F4String = 'F4:n'
                    SDString = 'SD4 '
                    i = 0
                    for cell in detectorCells:
                        F4String += ' '+str(cell)
                        SDString += ' 1'
                        if i % 6 is 5:
                            F4String += '\n     '
                        i += 1
                    # Writing the tallies
                    o.write(F4String+' T'+'\n')
                    o.write(SDString+' 1'+'\n')
                    o.write('FM4 -1 3 105\n')
                
                elif line.startswith('IMP:N'):
                    numCells += 4 # 3 Source, 1 RPM8 Encasing
                    o.write('IMP:n 1 '+str(numCells)+'r  0\n')
                else:
                    o.write(line.rstrip()+'\n')


