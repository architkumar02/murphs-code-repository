# Evluation of a genome for genetic programs
import os
import time
import random
import numpy as np
from multiprocessing import Pool
import MCNPXModel as mcnpx
import mctal

from pyevolve import G1DBinaryString
from pyevolve import G1DList
from pyevolve import GSimpleGA
from pyevolve import Statistics
from pyevolve import Selectors
from pyevolve.DBAdapters import DBFileCSV
from pyevolve.GPopulation import GPopulation

# Shared among all class instances
interactions = dict()
massLi = dict()
countRate = dict()
countRatePerMass = dict()
geoParam = dict()

def calcResults(fname,massLiPerCell=7.30382E1*5.73475E-2,
    sourceStrength = 2.3E3):
    """ calcResults

    Parses all of the output in a directory for the count per mg Li and count rate

    Keywords:
    fname -- filename
    massLiPerCell -- mass of 6Li per cell # Print table 60 and 40 (2nd part)
    sourceStrength -- strength of the source (n/s)
    """
    global interactions
    global massLi
    global countRate
    global countRateperMass
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

def readOutput(path='Output',massLiPerCell=7.30382E1*5.73475E-2,
    sourceStrength = 2.3E3):
    """ readOutput

    Parses all of the output in a directory for the count per mg Li and count rate

    Keywords:
    path -- directory of output files
    massLiPerCell -- mass of 6Li per cell # Print table 60 and 40 (2nd part)
    sourceStrength -- strength of the source (n/s)

    """
    if not os.path.isdir(path):
        os.makedirs(path)
    else:
        for files in os.listdir(path):
            if files.endswith('.m'):
                fname = os.path.join(path,files)
                calcResults(fname,massLiPerCell,sourceStrength)

def runModel(genome):
    """ Runs the Model """
    runName = 'INP_'+genome+'.mcnp'
    model = mcnpx.MCNPX(inp=runName)
    model.createBinaryGeometry(genome)
    model.createInputDeck()
    model.runModel()

def runPopulation(pop):
    """ Runs MCNPX on the population """
    # What indicies to we have to run?
    toRun = list()
    for ind in pop:
        if not ind in interactions:
            toRun.append(ind)
    # Running the pool
    if len(toRun) > 0:
        pool = Pool(processes=len(toRun))
        for ind in toRun:
            pool.apply_async(runModel,args=[ind,])
        pool.close()
        pool.join()
        # Waiting to collect all of the output
        while len(toRun) > 0:
            for ind in toRun:
                fname = 'OUT_'+ind+'.m'
                if os.path.exists(fname):
                    toRun.remove(ind)
                    calcResults(fname)
            time.sleep(10)

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
    indList = set()
    for ind in pop:
        if len(ind) > 3:
            ind[0] = 0
            ind[-1] = 0
        if ind.getDecimal() == 0:
            ind[random.randint(0,len(ind)-1)] = 1
        indList.add(ind.getBinary())
    runPopulation(indList)
    if generation == 0:
        pop.evaluate()
        pop.sort()
    return False

def eval_func(chromosome):
    return evaluate(chromosome.getBinary())

def summerizeData(numSlices,lpa):
    global interactions
    global massLi 
    global countRate 
    global countRatePerMass 
    global geoParam
    interactions.clear()
    massLi.clear()
    countRate.clear()
    countRatePerMass.clear()
    geoParam = {'GenomeLength':numSlices,'LayersPerAssembly':lpa,
        'RPM8Size':12.7,'DetectorThickness':0.01}
    outputName = str(numSlices)+'Slices'
    outputDir = str(numSlices)+'SlicesOutput'
    readOutput(outputDir)
    printData(outputName+'Summary.txt')

def runGA(numSlices,lpa):
    global geoParam
    global interactions
    global massLi 
    global countRate 
    global countRatePerMass 
    interactions.clear()
    massLi.clear()
    countRate.clear()
    countRatePerMass.clear()
    outputName = str(numSlices)+'Slices'
    outputDir = str(numSlices)+'SlicesOutput'
    # Setting up Geometry
    geoParam = {'GenomeLength':numSlices,'LayersPerAssembly':lpa,
        'RPM8Size':12.7,'DetectorThickness':0.01}
    print '=================================================================='
    print geoParam
    print 'Ouput File: ',outputName
    print 'Ouput Directory: ',outputDir
    print '=================================================================='
    mcnpx.MCNPX.geoParam = geoParam
    # Reading Previous Runs
    readOutput(outputDir)
    printData(outputName+'Summary.txt')
    # Setting up GA output
    csv_adapter = DBFileCSV(identify=str(numSlices)+'_slices',
                    filename=outputName+'.csv')
    # Setting up Genome
    genome = G1DBinaryString.G1DBinaryString(numSlices)
    genome.evaluator.set(eval_func)
    # Setting up GA
    ga = GSimpleGA.GSimpleGA(genome)
    ga.setDBAdapter(csv_adapter)
    ga.stepCallback.set(evolve_callback)
    ga.setGenerations(100)
    ga.setPopulationSize(2*numSlices)
    ga.selector.set(Selectors.GTournamentSelector)
    ga.terminationCriteria.set(GSimpleGA.ConvergenceCriteria)
    # Evolve and print
    ga.evolve(freq_stats=10)
    felix = ga.bestIndividual()
    print '------------------------------------------------------------------'
    print 'Best Individual:             ',felix.getBinary()
    print 'Number slices:               ',numSlices
    print 'Slice Thickness:      {:5.2f}'.format(geoParam['RPM8Size']/numSlices)
    print 'Interactions:                ',str(interactions[felix.getBinary()])
    print 'Mass 6-Li:                   ',massLi[felix.getBinary()]
    print 'Count Rate per ng Cf-252:    ',str(countRate[felix.getBinary()])
    print 'Count Rater per ng per mass: ',str(countRatePerMass[felix.getBinary()])
    print '------------------------------------------------------------------'
    printData(outputName+'Summary.txt')
    return felix

def runs():
    runGA(3,4)
    runGA(4,4)
    runGA(5,3)
    runGA(6,3)
    runGA(7,3)
    runGA(10,3)
    runGA(13,2)
    #runGA(26,1)

def summaries():
    summerizeData(3,4)
    summerizeData(4,4)
    summerizeData(5,3)
    summerizeData(6,3)
    summerizeData(7,3)
    summerizeData(10,3)
    summerizeData(13,2)
    summerizeData(26,1)

def main():
    summaries()
    runs()


if __name__ == "__main__":
    main()
