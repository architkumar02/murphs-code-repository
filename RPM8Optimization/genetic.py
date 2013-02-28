# A genetic algorthim approach to optimization of the RPM8
import os
import numpy as np
from pyevolve import G1DBinaryString
from pyevolve import G1DList
from pyevolve import GSimpleGA
from pyevolve.DBAdapters import DBFileCSV
from pyevolve.GPopulation import GPopulation
import geneticGeo as geometry
import geneticEval as evaluator

global geoParam
def ParameterExplore(numSlices):
    g = G1DBinaryString.G1DBinaryString(numSlices)
    
    # Two extremes
    allMod = '0000000000000'
    RunModel(allMod)
    allDet = '1111111111111'
    RunModel(allDet)

    # Setting the first and last layer
    for i in range(100):
        g.initialize()
        g[0] = 0
        g[-1] = 0
        RunModel(g.getBinary())

    # Setting the first two and last two layers
    for i in range(100):
        g.initialize()
        g[0] = 0
        g[1] = 0
        g[-1] = 0
        g[-2] = 0
        RunModel(g.getBinary())

def evolve_callback(ga_engine):
    generation = ga_engine.getCurrentGeneration()
    pop = ga_engine.getPopulation()
    # Getting a list of indvidviduals
    indList = list()
    for ind in pop:
        indList.append(ind.getBinary())
    evaluator.runPopulation(indList,wait=True)
    if generation == 0:
        pop.evaluate()
        pop.sort()
    return False

def eval_func(chromosome):
    return evaluator.evaluate(chromosome.getBinary())

def runGA(numSlices,lpa):
    global GeoParam
    # Setting up Geometry
    geoParam={'GenomeLength':numSlices,'LayersPerAssembly':lpa,
        'RPM8Size':12.7,'DetectorThickness':0.01}
    # Reading Previous Runs
    evaluator.readOutput(str(numSlices)+'SlicesOutput')
    evaluator.printData()
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
    # Evolve and print
    ga.evolve(freq_stats=10)
    print ga.bestIndividual()
    evaluator.printData()

def main():
    #ParameterExplore()
    #runGA(13,2)
    runGA(5,4)

if __name__ == "__main__":
    main()
