# Evluation of a genome for genetic programs
import os
import numpy as np
from multiprocessing import Pool
import MCNPXModel as mcnpx
import mctal

from pyevolve import G1DBinaryString
from pyevolve import G1DList
from pyevolve import GSimpleGA
from pyevolve import Statistics
from pyevolve.DBAdapters import DBFileCSV
from pyevolve.GPopulation import GPopulation

# Shared among all class instances
interactions = dict()
massLi = dict()
countRate = dict()
countRatePerMass = dict()
geoParam = dict()

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
    if not os.path.isdir(path):
        return
    for files in os.listdir(path):
        if files.endswith('.m'):
            calcResults(os.path.join(path,files),massLiPerCell,sourceStrength)

def runModel(genome,wait=False):
    """ Runs the Model """
    runName = 'INP_'+genome+'.mcnp'
    model = mcnpx.MCNPX(inp=runName)
    model.createBinaryGeometry(genome)
    model.createInputDeck()
    print 'Running the model'
    model.runModel()
    if wait:
        print 'Waiting for results'
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

def printData(fname='RunSummary.txt'): 
    with open(fname,'w') as f:
        f.write('Genome,Mass Li-6,Count Rate, Count Rate per Mass\n')
        for g in countRate:
            f.write(g+',{},{},{}\n'.format(massLi[g],
            countRate[g][0],countRatePerMass[g][0]))
 
def evolve_callback(ga_engine):
    generation = ga_engine.getCurrentGeneration()
    pop = ga_engine.getPopulation()
    # Getting a list of indvidviduals
    indList = list()
    for ind in pop:
        indList.append(ind.getBinary())
    runPopulation(indList,wait=True)
    if generation == 0:
        pop.evaluate()
        pop.sort()
    return False

def eval_func(chromosome):
    return evaluate(chromosome.getBinary())

def runGA(numSlices,lpa):
    outputName = str(numSlices)+'Slices'
    outputDir = str(numSlices)+'SlicesOutput'
    # Setting up Geometry
    mcnpx.MCNPX.geoParam={'GenomeLength':numSlices,'LayersPerAssembly':lpa,
        'RPM8Size':12.7,'DetectorThickness':0.01}
    # Reading Previous Runs
    readOutput(outputDir)
    printData(outputName+'Summary.txt')
    # Setting up GA output
    csv_adapter = DBFileCSV(identify=str(numSlices)+'_slices',
                    filename='stats_'+str(numSlices)+'.csv')
    # Setting up Genome
    genome = G1DBinaryString.G1DBinaryString(numSlices)
    genome.evaluator.set(eval_func)
    # Setting up GA
    ga = GSimpleGA.GSimpleGA(genome)
    ga.setDBAdapter(csv_adapter)
    ga.stepCallback.set(evolve_callback)
    ga.setGenerations(2)
    ga.setPopulationSize(4)
    # Evolve and print
    ga.evolve(freq_stats=10)
    print ga.bestIndividual()
    printData(outputName+'Summary.txt')

def main():
    #runGA(13,2)
    runGA(3,4)

if __name__ == "__main__":
    main()
