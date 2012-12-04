# Trains an artficial neural net for this problem
from pybrain.datasets import ClassificationDataSet
from pybrain.utilities import percentError
from pybrain.tools.shortcuts import buildNetwork
from pybrain.supervised.trainers import BackpropTrainer
from pybrain.structure.modules import SoftmaxLayer
from pylab import ion, ioff, figure, draw, contourf, clf, show, hold, plot
from scipy import diag, arange, meshgrid, where
import numpy as np
from datetime import datetime
import GraphUtility as GU
import networkx as nx
def readData(filename='../LinkAnalyticsData/UTK_problem/Moria_1.graph'):
    """ Creates a dataset for ANN training of the formated data supplied by filename """
    """ Currently based on 4x2 inputs of days, calls, call duration, and texts """
    """ Two 'classes' are implemented, either there or not """
    numInputs = 2+4+2
    alldata = ClassificationDataSet(numInputs,1,nb_classes=2)
    MG = GU.readData(filename)
    closeness = nx.closeness_centrality(MG)
    degree = nx.degree(MG)
   
    startTime = datetime.now()
    # Computing the data
    data = [[closeness[u],degree[u],\
            edata['calls'],edata['secs'],edata['texts'],edata['days'],\
            degree[v],closeness[v]] \
            for u,v,edata in MG.edges(data=True)]
    for d in data:
        alldata.addSample(d,[1])

    print "Converted to data in ",(datetime.now()-startTime)
    return alldata

def trainNetwork(alldata):
    """ Creates and trains an ANN on the data set provided """
    # Splitting into testing and training
    tstdata,trndata = alldata.splitWithProportion(0.25)
    trndata._convertToOneOfMany()
    tstdata._convertToOneOfMany()
    print "Number of training patterns: ",len(trndata)
    print "Input and output dimensions: ",trndata.indim, trndata.outdim
    print "First sample (input, target, class):"
    print trndata['input'][0],trndata['target'][0],trndata['class'][0]
    
    # Building the network
    fnn = buildNetwork(trndata.indim, 5, trndata.outdim, outclass=SoftmaxLayer)
    trainer = BackpropTrainer(fnn,dataset=trndata,momentum=0.1,verbose=True,weightdecay=0.01)
    for i in range(100):
        trainer.trainEpochs(5)
        trnresult = percentError(trainer.testOnClassData(),trndata['class'])
        tstresult = percentError(trainer.testOnClassData(dataset=tstdata),tstdata['class'])
        print "epoch: %4d" % trainer.totalepochs,\
                "train error: %5.2f%%"%trnresult,\
                " test error: %5.2f%%" %tstresult

    # Returning the network
    return fnn
