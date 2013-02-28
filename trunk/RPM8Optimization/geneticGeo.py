# Creates the geoemtry of the RPM8 Based on a bit string
class BinaryGeo:
    def __init__(self,param={'GenomeLength':13,'LayersPerAssembly':2,
        'RPM8Size':12.7,'DetectorThickness':0.01}):
        self.param=param
        self.genome=''.join('0' for i in range(self.param['GenomeLength'])
    
    def createGeometry(self,genome='0000000000000'):
        """Create a dictionary of the geometry 

        Keyword arguments:
        genome -- A bit string of the genome
        param -- dictionary of RPM8 based geometry constraints.

        Return arguments:
        dictionary between a position and a material
        """
        # Computing parameters
        slices = len(genome)
        if slices != self.param['GenomeLength']:
            raise ValueError()

        sT = self.param['RPM8Size']/slices
        modT = sT
        lGT = (sT/self.param['LayersPerAssembly']) - self.param['DetectorThickness']
        dT = self.param['DetectorThickness'] 
        
        # Creating the geometry dictionary
        geo = dict()
        numDetector = 0
        x = 0
        for s in genome:
            if s is '1':
                # Detector
                for layer in range(self.param['LayersPerAssembly']):
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
        if (abs(x-self.param['RPM8Size']) >= 0.0001):
            errStr = 'x ({:5.3f}) is not within a micron'
            errstr += 'of the  detector size ({:5.3f})'
            errStr.format(x,self.param['RPM8Size'])
            raise ValueError(errStr)
        
        # Returning the geometry dictionary
        return geo,numDetector

    def printGeo(self,geo):
        keyList = sorted(geo.keys(), key = lambda x : float(x))
        for key in keyList:
            print 'x: {:5.3f} {}'.format(key,geo[key])
