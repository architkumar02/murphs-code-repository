# costOpt.py
# Matthew J. Urffer (matthew.urffer@gmail.com)
#
# Module for preforming cost optimization of the RPM8
#
# CHANGELOGE:
#   2/23/2013 - Implemented createGeometry, working on interaction rate
#               models (MCNPX and 1D transport)

import numpy as np

def isValidParameters(p,n,dThickness,geoConstraints):
    """ Checks if the goemetry is valid 

    Keyword arguments:
    p -- numpy array of moderator thickness
    n -- numpy array of detectory layers per assembly
    dThickness -- thickness of the detector in cm
    geoConstraints -- dictionary of RPM8 based geometry constraints

    Return arguments:
    True for valid geometry, false otherwise
    """
    # Inputs
    if (len(n) + 1 is not len(p)):
        return False
    
    # Spacing
    detectorSpace = (geoConstraints['RPM8Size']-sum(p))/sum(n)
    layerSize = geoConstraints['MinLightGuide']+dThickness
    if detectorSpace < layerSize:
        return False
    return True

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
    if not isValidParameters(p,n,dThickness,geoConstraints):
        print 'p: ',str(p)
        print 'n: ',str(n)
        raise ValueError('Geometry inputs are not valid')

    # Computing parameters
    detectorSpace = (geoConstraints['RPM8Size']-sum(p))/sum(n)
    layerSize = geoConstraints['MinLightGuide']+dThickness
    lightGuideThickness = detectorSpace-dThickness
    
    # Creating the geometry dictionary
    geo = dict()
    numDetector = 0
    x = 0
    for t,assembly in zip(p,n):
        x += t
        # Moderator
        geo[x] = 'Moderator'
        
        # Detector Assembly
        for layer in range(assembly):
            x += dThickness
            geo[x] = 'Detector'
            numDetector += 1
            x += lightGuideThickness
            geo[x] = 'LightGuide'

    # Checking that we are ended correctly
    x += p[-1]
    if (abs(x-geoConstraints['RPM8Size']) >= 0.0001):
        errStr = 'x ({:5.3f}) is not within a micron'
        errstr += 'of the  detector size ({:5.3f})'
        errStr.format(x,geoConstraints['RPM8Size'])
        raise ValueError(errStr)
    
    # Adding on the last element
    geo[x] = 'Moderator'
    
    # Returning the geometry dictionary
    return geo,numDetector


def calculateInteractionRates(p=np.array([2.5,2]),n=np.array([15]),
    geo=createGeometry(),model='MCNPX'):
    """ calculateInteractionRates

    Calculates the simulated interaction rate of the specified detector

    Keyword arguments:
    geo -- geometry dictionary with a mapping between an x position and the
            material to the left of the position
    model -- model on which to calculate the interaction rate. Can be either
            MCNPX or a 1D transport

    Return:
    The calculated interaction rate
    """
    if model is '1DTransport':
        raise NotImplementedError('1DTransport is not implemented')
    elif model is 'MCNPX':
        print 'Selected MCNPX model'
        import MCNPXModel as mcnpx
        
        model = mcnpx.MCNPX(p,n,geo)
        model.createInputDeck(geo)
        model.runModel()
        
        import os,time
        while not os.path.isfile(model.NAME+'.m'):
            time.sleep(10)

        return model.getInteractionRate()
    else:
        raise ValueError('Model '+model+' is not reconized')

def coarseSearchRuns():
    import MCNPXModel as mcnpx
    
    # Geometry Parameters
    dThickness=0.01
    geoConstraints={'RPM8Size':12.7,'MinLightGuide':0.5}
    
    # Format is max of P, min is assumed to be zero
    pRangeMax={1:np.array([5,5]),2:np.array([3,3,3]),3:np.array([3,3,3,3])}
    pStep = {1:1,2:1,3:1}
    nRangeMin = {1:np.array([5]),2:np.array([5,5]),3:np.array([2,2,2])}
    nStep = {1:2,2:2,3:2}

    # Looping through the setup
    for nA in range(1,4):
        print 'Number Assemblies: ',str(nA)
        print '\tPMax  ',str(pRangeMax[nA])
        print '\tpStep ',str(pStep[nA])
        print '\tnMin  ',str(nRangeMin[nA])
        print '\tnStep ',str(nStep[nA]) 
        # Looping through the number of films
        if nA == 1:
            numRuns = 0
            p1 = np.arange(1,pRangeMax[nA][0],pStep[nA])
            p2 = np.arange(1,pRangeMax[nA][1],pStep[nA])
            for i in  range(len(p1)):
                for j in range(len(p2)):
                    p = [p1[i],p2[j]]
                    nMax = (geoConstraints['RPM8Size']-np.sum(p))/(
                            dThickness+geoConstraints['MinLightGuide'])
                    for k in range(nRangeMin[nA],int(np.floor(nMax)),nStep[nA]):
                        n = np.array([k])
                        if isValidParameters(p,n,dThickness,geoConstraints):
                            geo,num = createGeometry(p,n,dThickness,geoConstraints)
                            model = mcnpx.MCNPX(p,n,geo)
                            model.createInputDeck(geo)
                            model.runModel()
                            numRuns += 1
            print '\tPossible geometries: ',str(numRuns)
        elif nA == 2:
            numRuns = 0
            p1 = np.arange(1,pRangeMax[nA][0],pStep[nA])
            p2 = np.arange(1,pRangeMax[nA][1],pStep[nA])
            p3 = np.arange(1,pRangeMax[nA][2],pStep[nA])
            for i in  range(len(p1)):
                for j in range(len(p2)):
                    for k in range(len(p3)):
                        p = [p1[i],p2[j],p3[k]]
                        nMax = (geoConstraints['RPM8Size']-np.sum(p))/(
                            dThickness+geoConstraints['MinLightGuide'])
                        for w in range(nRangeMin[nA][0],int(np.floor(nMax)),nStep[nA]):
                            for x in range(nRangeMin[nA][1],int(np.floor(nMax)),nStep[nA]):
                                n = np.array([w,x])
                                if isValidParameters(p,n,dThickness,geoConstraints):
                                    geo,num = createGeometry(p,n,dThickness,geoConstraints)
                                    model = mcnpx.MCNPX(p,n,geo)
                                    model.createInputDeck(geo)
                                    model.runModel()
                                    numRuns += 1
            print '\tPossible geometries: ',str(numRuns)
        elif nA == 3:
            numRuns = 0
            p1 = np.arange(1,pRangeMax[nA][0],pStep[nA])
            p2 = np.arange(1,pRangeMax[nA][1],pStep[nA])
            p3 = np.arange(1,pRangeMax[nA][2],pStep[nA])
            p4 = np.arange(1,pRangeMax[nA][3],pStep[nA])
            for i in  range(len(p1)):
                for j in range(len(p2)):
                    for k in range(len(p3)):
                        for l in range(len(p4)):
                            p = [p1[i],p2[j],p3[k],p4[l]]
                            nMax = (geoConstraints['RPM8Size']-np.sum(p))/(
                                dThickness+geoConstraints['MinLightGuide'])
                            for w in range(nRangeMin[nA][0],int(np.floor(nMax)),nStep[nA]):
                                for x in range(nRangeMin[nA][1],int(np.floor(nMax)),nStep[nA]):
                                    for y in range(nRangeMin[nA][2],int(np.floor(nMax)),nStep[nA]):
                                        n = np.array([w,x,y])
                                        if isValidParameters(p,n,dThickness,geoConstraints):
                                            geo,num = createGeometry(p,n,dThickness,geoConstraints)
                                            model = mcnpx.MCNPX(p,n,geo)
                                            model.createInputDeck(geo)
                                            model.runModel()
                                            numRuns += 1
            print '\tPossible geometries: ',str(numRuns)
        
def main():
    # Testing coarseSearchRuns
    coarseSearchRuns()
    
    # Testing geometry and runs
    #createGeometry()
    p=np.array([2,1,3])
    n=np.array([5,7])
    #geo,numDetector = createGeometry(p,n)
    #v,e = calculateInteractionRates(p,n,geo,'MCNPX')
    #v,e = calculateInteractionRates()
    #print v,e
if __name__ == "__main__":
    main()
