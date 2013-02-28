# Creates the geoemtry of the RPM8 Based on a bit string
 
def createGeometry(genome='0000000000000'):
    """Create a dictionary of the geometry 

    Keyword arguments:
    genome -- A bit string of the genome
    geoParam -- dictionary of RPM8 based geometry constraints.

    Return arguments:
    dictionary between a position and a material
    """
    # Computing parameters
    print 'Creating a geometry for '+genome
    slices = len(genome)
    if slices != geoParam['GenomeLength']:
        raise ValueError()

    sT = geoParam['RPM8Size']/slices
    modT = sT
    lGT = (sT/geoParam['LayersPerAssembly']) - geoParam['DetectorThickness']
    dT = geoParam['DetectorThickness'] 
    
    # Creating the geometry dictionary
    geo = dict()
    numDetector = 0
    x = 0
    for s in genome:
        if s is '1':
            # Detector
            for layer in range(geoParam['LayersPerAssembly']):
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
    if (abs(x-geoParam['RPM8Size']) >= 0.0001):
        errStr = 'x ({:5.3f}) is not within a micron'
        errstr += 'of the  detector size ({:5.3f})'
        errStr.format(x,geoParam['RPM8Size'])
        raise ValueError(errStr)
    
    # Returning the geometry dictionary
    return geo,numDetector

def printGeo(geo):
    keyList = sorted(geo.keys(), key = lambda x : float(x))
    for key in keyList:
        print 'x: {:5.3f} {}'.format(key,geo[key])
