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

    def __init__(self,p=None,n=None,geo=None):
        # Material dictionary for the moderator, light guide, and detector
        self.material =  {'Moderator':self.modMaterial,'Detector':self.detMaterial,
                            'LightGuide':self.lightGuideMaterial}
        # Cell and Surface Inital Numbering
        self.CellStartNum = 6000
        self.SurfaceStartNum = 6000
        self.ZeroSurfaceNum = 500
        self.geo = geo
        self.p = p
        self.n = n
        self.INP = self.createINPName()
        self.NAME = self.createOutputName()
   
    def createOutputName(self,p=None,n=None):
        """ Create an output file name 

        Keyword Arguments:
        p -- list of HDPE moderators
        n -- list of assemblies
        
        Returns the filename string in addition to assigning it to the class
        """
        if p is None:
            p = self.p
        if n is None:
            n = self.n
        self.NAME = 'OUT_'
        for x,a in zip(p,n): 
            self.NAME+='{}_cm_{}'.format(x,a)
        self.NAME+='_{}_cm.mcnp'.format(p[-1])
        return self.NAME

    def createINPName(self,p=None,n=None):
        """ Create an INPUT file name 

        Keyword Arguments:
        p -- list of HDPE moderators
        n -- list of assemblies
        
        Returns the filename string in addition to assigning it to the class
        """
        if p is None:
            p = self.p
        if n is None:
            n = self.n
        self.INP = 'INP_'
        for x,a in zip(p,n): 
            self.INP+='{}_cm_{}'.format(x,a)
        self.INP+='_{}_cm.mcnp'.format(p[-1])
        return self.INP

    def runModel(self,queue='gen3',attropl=None):
        import pbs
        # Setting up the submission if it doesn't exists
        server = pbs.pbs_default()
        c = pbs.pbs_connect(server)
        if not attropl:
            queue='gen3'
            attropl = pbs.new_attropl(2)
            attropl[0].name = pbs.ATTR_N
            attropl[0].value = 'MCNPX Model '+self.INP
            attropl[1].name = pbs.ATTR_l
            attropl[1].resource = 'nodes'
            attropl[1].value = '4:ppn=4'
       
        runcmd='mpirun mcnpx inp='+self.INP+' name='+self.NAME+'.'
        print runcmd
        job_id = pbs.pbs_submit(c,attropl,runcmd,queue,'NULL')
        e, e_txt = pbs.error()
        if e:
            print e,e_txt
        return job_id

    def createInputDeck(self,geo,baseFile='SCRIPT.mcnp',oFile=None):
        """ createInputDeck 

        Creates an input deck of the given geometry
        Keyword Arguments:
        geo -- geometry description
        oFile -- output file name (default is INP.mcnp)
        baseFile -- base input deck (default is SCRIPT.mcnp')

        """
        if not oFile:
            oFile = self.INP
        self.geo = geo
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


