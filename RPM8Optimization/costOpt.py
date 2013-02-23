# costOpt.py
# Matthew J. Urffer (matthew.urffer@gmail.com)
#
# Module for preforming cost optimization of the RPM8
#
# CHANGELOGE:
#   2/23/2013 - Implemented createGeometry, working on interaction rate
#               models (MCNPX and 1D transport)

import numpy as np

def createGeometry(p=np.array([2.5,2]),n=np.array([15]),dThickness=0.01,
                geoConstraints={'RPM8Size':12.7,'MinLightGuide':0.5}):
    """Create a dictionary of the geometry 

    Keyword arguments:
    p -- numpy array of moderator thickness
    n -- numpy array of detectory layers per assembly
    dThickness -- thickness of the detector in cm
    geoConstraints -- dictionary of RPM8 based geometry constraints

    Return arguments:
    dictionary between a position and a material
    """
    # Check for valid inputs
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
        geo[x] = 'Moderator'
        
        # Detector Assembly
        for layer in range(assembly):
            x += dThickness
            geo[x] = 'Detector'
            x += lightGuideThickness
            geo[x] = 'LightGuide'

    # Checking that we are ended correctly
    x += p[-1]
    if (abs(x-geoConstraints['RPM8Size']) >= 0.0001):
        errStr = 'x ({:5.3f}) is not within a micron of the  detector size ({:5.3f})'.format(x,geoConstraints['RPM8Size'])
        raise ValueError(errStr)
    
    # Adding on the last element
    geo[x] = 'Moderator'
    
    # Returning the geometry dictionary
    return geo

def calculateInteractionRates(geo=createGeometry(),model='MCNPX'):
    """ calculateInteractionRates

    Calculates the simulated interaction rate of the specified detector

    Keyword arguments:
    geo -- geometry dictionary with a mapping between an x position and the
            material to the left of the position
    model -- model on which to calculate the interaction rate. Can be either
            MCNPX or a 1D transport

    Return:
    The calcualted interaction rate
    """
    
    if model is '1DTransport':
        raise NotImplementedError('1DTransport is currently not implemented')
    elif model is 'MCNPX':
        print 'Selected MCNPX model'
        import MCNPXModel as mcnpx
        model = mcnpx.MCNPX()
        model.createInputDeck(geo)
    else:
        raise ValueError('Model '+model+' is not reconized')


def main():
    createGeometry()
    calculateInteractionRates()
if __name__ == "__main__":
    main()
