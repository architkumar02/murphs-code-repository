import os
import GraphUtility as GU
import matplotlib.pyplot as pyplot

import heapq
import numpy as np
def GetDataDistributions(dataPath='../LinkAnalyticsData/UTK_problem/'):
    filenames=('Moria_1.graph','Standelf_1.graph')
    """ Finds the distrbtion of the cummulative node and edge wights """
    attrs=('calls','texts','degree','secs')
    for f in filenames:
        # Reading in the Graph
        MG = GU.readData(os.path.join(dataPath,f))
        g = GU.ConvertToSingle(MG)
        for attr in attrs:
            x = list()
            for n in g.nodes():
                x.append(g.node[n][attr])

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

def main():
    GetDataDistributions()

if __name__ == "__main__":
    main()

