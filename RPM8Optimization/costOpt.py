# costOpt.py
# Matthew J. Urffer (matthew.urffer@gmail.com)
#
# Module for preforming cost optimization of the RPM8
#
# CHANGELOGE:
#   2/23/2013 - Create the file

import numpy as np

def createGeometry(p=np.array([2.5,2]),n=np.array([15]),
                m={'Moderator':'HDPE','LightGuide':'PMMA','Detector':'PEN'},
                dThickness=0.01,
                geoConstraints={'RPM8Size':12.7,'MinLightGuide':0.5}):
    """Create a dictionary of the geometry 

    Keyword arguments:
    p -- numpy array of moderator thickness
    n -- numpy array of detectory layers per assembly
    m -- dictionary of problem materials
    dThickness -- thickness of the detector in cm
    geoConstraints -- dictionary of RPM8 based geometry constraints

    Return arguments:
    dictionary between a position and a material
    """
    # Check for valid inputs
    print len(n), len(p)
    if (len(n) + 1 is not len(p)):
        raise ValueError('Invalid moderator and assemblies')
    
    # Computing parameters
    detectorSpace = (geoConstraints['RPM8Size']-sum(p))/sum(n)
    layerSize = geoConstraints['MinLightGuide']+dThickness
    lightGuideThickness = detectorSpace-dThickness

    # Check for valid geometry
    if detectorSpace < layerSize:
        raise ValueError('Not enough space for the detector')

    # Creating the geometry dictionary
    geo = dict()
    for x,assembly in zip(p,n):
        # Moderator
        geo[x] = m['Moderator']
        
        # Detector Assembly
        for layer in range(assembly):
            x += dThickness
            geo[x] = m['Detector']
            x += lightGuideThickness
            geo[x] = m['LightGuide']

    # Checking that we are ended correctly
    x += p[-1]
    if (abs(x-geoConstraints['RPM8Size']) >= 0.0001):
        errStr = 'x ({:5.3f}) is not within a micron of the  detector size ({:5.3f})'.format(x,geoConstraints['RPM8Size'])
        raise ValueError(errStr)
    
    # Adding on the last element
    geo[x] = m['Moderator']
    
    # Printing the dictionary
    keyList = sorted(geo.keys(), key = lambda x : float(x))
    for key in keyList:
        print key, geo[key]

    # Returning the geometry dictionary
    return geo

def main():
    createGeometry()
if __name__ == "__main__":
    main()
