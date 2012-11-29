import os
import GraphUtility as GU
import matplotlib.pyplot as pyplot
def GetDataDistributions(dataPath='../LinkAnalyticsData/UTK_problem/'):
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



