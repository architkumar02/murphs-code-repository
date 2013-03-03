""" MCNPX Model for RPM8

Shared Data
detMaterial -- dictionary of detector material mt and density
lightGuideMaterial -- dictionary of light guide material mt and density
modMaterial -- dictionary of moderator material mt and density
geoParam -- geometry parameters
"""
import subprocess
import mctal
class MCNPX:
    # Material Dictionaries
    detMaterial = {'name':'Detector','mt': 3, 'rho': 1.1}       # detector
    lightGuideMaterial = {'name': 'PMMA','mt':10, 'rho':0.93}   # PMMA
    modMaterial = {'name':'HDPE','mt':456, 'rho': 0.93}         # HPDE
    cellForStr = '{:5d} {:d} -{:4.3f} {:d} -{:d} 502 -503 504 -505\n'
    surForStr = '{:5d} px {:5.3f}\n'
    geoParam={'GenomeLength':13,'LayersPerAssembly':2,
            'RPM8Size':12.7,'DetectorThickness':0.01}
    
    def __init__(self,inp='INP.mcnp',name=None):
        """ MCNPX Model of RPM8

        Keywords:
        inp -- desired name of the input deck
        name -- desired name of the output deck
        """
       # Material dictionary for the moderator, light guide, and detector
        self.material =  {'Moderator':self.modMaterial,'Detector':self.detMaterial,
                            'LightGuide':self.lightGuideMaterial}
        # Cell and Surface Inital Numbering
        self.CellStartNum = 6000
        self.SurfaceStartNum = 6000
        self.ZeroSurfaceNum = 500
        self.geo = None
        self.INP = inp
        if name:
            self.NAME = name
        else:
            self.NAME = self.INP.strip('.mcnp').replace('INP','OUT')
  
    def getInteractionRate(self):
        """ Returns the interaction rate """
        m = mctal.MCTAL(self.NAME+'.m')
        t = m.tallies[4]
        # Returing the total
        return t.data[-1],t.errors[-1]

    def printGeo(self):
        """ Print the geometry """
        keyList = sorted(self.geo.keys(), key = lambda x : float(x))
        for key in keyList:
            print 'x: {:5.3f} {}'.format(key,self.geo[key])
    
    def runModel(self):
        """ Runs the Model """
        runcmd='mpirun mcnpx inp='+self.INP+' name='+self.NAME+'.'
        qsub='/usr/torque/torque-4.2.0/bin/qsub'
        genome = self.INP.strip('INP_').strip('.mcnp')
        name='QSUB_'+genome+'.qsub'
        with open('queueRunScript.sh','r') as f, open(name,'w') as o:
            for line in f:
                if line.startswith('RUNCOMMAND'):
                    o.write(runcmd+'\n')
                else:
                    o.write(line)
        process = subprocess.Popen(qsub+' '+name,shell=True)
        process.wait()
    
    def createBinaryGeometry(self,genome):
        """Create a dictionary of the geometry 

        Keyword arguments:
        genome -- A bit string of the genome

        Return arguments:
        num of detector cells in the geometry
        """
        # Computing parameters
        slices = len(genome)
        if slices != self.geoParam['GenomeLength']:
            raise ValueError()

        sT = self.geoParam['RPM8Size']/slices
        modT = sT
        lGT = (sT/self.geoParam['LayersPerAssembly']) - self.geoParam['DetectorThickness']
        dT = self.geoParam['DetectorThickness'] 
        
        # Creating the geometry dictionary
        geo = dict()
        numDetector = 0
        x = 0
        for s in genome:
            if s is '1':
                # Detector
                for layer in range(self.geoParam['LayersPerAssembly']):
                    x += dT
                    geo[x] = 'Detector'
                    numDetector += 1
                    x += lGT
                    geo[x] = 'LightGuide'
            else:
                # Moderator
                x += modT 
                geo[x] = 'Moderator'
        
        # Checking that we ended correctly
        if (abs(x-self.geoParam['RPM8Size']) >= 0.0001):
            errStr = 'x ({:5.3f}) is not within a micron'
            errstr += 'of the  detector size ({:5.3f})'
            errStr.format(x,self.geoParam['RPM8Size'])
            raise ValueError(errStr)
        
        # Returning the geometry dictionary
        self.geo = geo
        return numDetector
    
    def createInputDeck(self,baseFile='SCRIPT.mcnp',oFile=None):
        """ createInputDeck 

        Creates an input deck of the given geometry
        Keyword Arguments:
        oFile -- output file name (default is INP.mcnp)
        baseFile -- base input deck (default is SCRIPT.mcnp')
        """
        if not oFile:
            oFile = self.INP
        numCells = 0
        detectorCells = list()
        keyList = sorted(self.geo.keys(), key = lambda x: float(x))
        with open('SCRIPT.mcnp','r') as i,open(oFile,'w') as o:
            for line in i:
                if line.startswith('c CELLPREPENDTOKEN'):
                    # Write the cells
                    cNum = self.CellStartNum
                    sNum = self.SurfaceStartNum
                    sPrev = self.ZeroSurfaceNum
                    for key in keyList:
                        # Creating the cell
                        m = self.material[self.geo[key]] 
                        line = self.cellForStr.format(cNum,m['mt'],m['rho'],
                                sPrev,sNum)
                        o.write(line)
                        # List of cells for the detector
                        if self.geo[key] is 'Detector':
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

