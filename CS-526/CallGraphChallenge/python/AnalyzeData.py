import os
import GraphUtility as GU
import matplotlib.pyplot as pyplot
def GetEdgeDistributions(dataPath='../LinkAnalyticsData/UTK_problem/'):
    filenames=('Moria_1.graph','Standelf_1.graph')
    attrs=('calls','texts','days','secs')
    for f in filenames:
        # Reading in the Graph
        MG = GU.readData(os.path.join(dataPath,f))
        # Distribution of each Attribute
        for attr in attrs:
            data = GU.GetAttr(MG,attr)
            # Plotting
            pyplot.hist(data,100)
            pyplot.yscale('log')
            pyplot.grid(True)
            pyplot.ylabel("Frequency")
            pyplot.xlabel(attr)
            name = f.split('.')[0].split('_')[0]
            title = name+" "+attr+" distribution"
            pyplot.title(title)
            pyplot.savefig(name+"_"+attr+"_distribution.png")
            pyplot.clf()

import heapq
import numpy as np
def GetDataDistributions(dataPath='../LinkAnalyticsData/UTK_problem/'):
    """ Finds the distrbtion of the cummulative node and edge wights """
    filenames=('Moria_1.graph','Standelf_1.graph')
    attrs=('calls','texts','degree','secs')
    for f in filenames:
        # Reading in the Graph
        MG = GU.readData(os.path.join(dataPath,f))
        g = GU.ConvertToSingle(MG)
        for attr in attrs:
            x = [d[attr] for n,d in G.nodes_iter(data=True)]
            
            # Plotting the Data
            largest = heapq.nlargest(3,x)
            pyplot.figure()
            pyplot.hist(x,bins=np.logspace(1,np.log2(largest[2]),25,base=2))
            pyplot.ylabel("Frequency")
            pyplot.xlabel(attr)
            name = f.split('.')[0].split('_')[0]
            title = name+" "+attr+" distribution"
            pyplot.title(title)
            pyplot.savefig(name+"_"+attr+"_cum_distribution.png")
            pyplot.clf()

def main():
    GetEdgeDistributions()
    GetDataDistributions()

if __name__ == "__main__":
    main()

