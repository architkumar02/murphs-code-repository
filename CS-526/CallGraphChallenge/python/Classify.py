import os
import GraphUtility as GU
import matplotlib.pyplot as pyplot
import heapq
import numpy as np
import networkx as nx
def DegreeClassify(dataPath='../LinkAnalyticsData/UTK_problem/'):
    """ Finds the distrbtion of the cummulative node and edge wights """
    filenames=('Moria_2','Standelf_2')
    attrs=('calls','texts','degree','secs')
    for f in filenames:
        # Reading in the Graph
        MG = GU.readData(os.path.join(dataPath,f)+'.graph')
        deg = nx.degree(MG)

def DegreeClassifyHelper(deg,filename):
    threshold = np.mean(deg.values())
    fout = open(filename+'.result','w')
    with open(filename+'.edge','r') as f:
        for line in f:
            token = line.strip().split(' ')
            u = long(token[0])
            v = long(token[1])
            if (deg[u]+deg[v]) > threshold:
                fout.write('%l %l %i'%u,v,1)
            else:
                fout.write('%l %l %i'%u,v,0)

def main():
    DegreeClassify()

if __name__ == "__main__":
    main()

