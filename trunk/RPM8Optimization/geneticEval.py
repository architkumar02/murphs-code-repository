# Evluation of a genome for genetic programs
import os
import numpy as np
from multiprocessing import Process, Queue, Pool
import MCNPXModel as mcnpx
import mctal


# Shared among all class instances
interactions = dict()
massLi = dict()
countRate = dict()
countRatePerMass = dict()
    

def calcResults(fname,massLiPerCell=7.30382E1*5.63475E-3,
    sourceStrength = 2.3E3):
    """ calcResults

    Parses all of the output in a directory for the count per mg Li and count rate

    Keywords:
    fname -- filename
    massLiPerCell -- mass of 6Li per cell # Print table 60 and 40 (2nd part)
    sourceStrength -- strength of the source (n/s)

    Return:
    genome -- bitstring representation
    """
    genome = fname.split('OUT_')[1].strip('.m')
    m = mctal.MCTAL(fname)
    t = m.tallies[4]
    inter = np.array([t.data[-1],t.errors[-1]])
    m = genome.count('1')*massLiPerCell*geoParam['LayersPerAssembly']
    cr = inter*sourceStrength
     
    interactions[genome] = inter
    massLi[genome] = m
    countRate[genome] = cr
    countRatePerMass[genome] = cr/m
    return genome

def readOutput(path='Output',massLiPerCell=7.30382E1*5.63475E-3,
    sourceStrength = 2.3E3):
    """ readOutput

    Parses all of the output in a directory for the count per mg Li and count rate

    Keywords:
    path -- directory of output files
    massLiPerCell -- mass of 6Li per cell # Print table 60 and 40 (2nd part)
    sourceStrength -- strength of the source (n/s)

    """
    for files in os.listdir(path):
        if files.endswith('.m'):
            calcResults(os.path.join(path,files),massLiPerCell,sourceStrength)

def runModel(genome,wait=False):
    """ Runs the Model """
    runName = 'INP_'+genome+'.mcnp'
    print runName
    g,num = geo.createGeometry(genome)
    print 'Created a geometry with ',str(num)
    model = mcnpx.MCNPX(g,runName)
    print 'Trying to create an input deck'
    model.createInputDeck(g)
    model.runModel()
    if wait:
        fname = model.NAME+'.m'
        while not os.path.isfile(fname):
            time.sleep(10)
        calcResults(fname)

def runPopulation(pop,wait=True):
    """ Runs MCNPX on the population """
    # What indicies to we have to run?
    toRun = list()
    for ind in pop:
        if not ind in interactions:
            toRun.append(ind)
    # Running the pool
    pool = Pool(processes=len(toRun))
    for ind in toRun:
        pool.apply_async(runModel,args=[ind,wait])
    pool.close()
    pool.join()  

def evaluate(chromosome):
    if chromosome not in countRate:
        return 0
    elif countRate[chromosome][0] < 2.5:
        return 0
    else:
        return  countRatePerMass[chromosome][0]

def printData(): 
    with open('RunSummary.txt','w') as f:
        f.write('Genome,Mass Li-6,Count Rate, Count Rate per Mass\n')
        for g in countRate:
            f.write(g+',{},{},{}\n'.format(massLi[g],
            countRate[g][0],countRatePerMass[g][0]))
           
