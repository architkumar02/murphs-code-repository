#!/user/bin/python
# Pickle Data utility
#   Matthew Urffer    (matthew.urffer@gmail.com)
import numpy as np
import collections
import time
import pickle
import os

# Reading in the Movie and and Rating, and then pickling it
pickleDir = 'PickleData'

def getPickleData(fileName):
    """Loads Pickled Data Object"""
    """
        Loads pickled data objects stored in filename, where filename  is the
            name of the unpickled data file.
        Returns the data object if found, None otherwise
    """
    (path,name) = os.path.split(fileName)
    pickleName = name + '.pickle'
    try:
        with open(os.path.join(pickleDir,pickleName),'rb') as f:
            return pickle.load(f)
        print 'Loaded pickled data %s'%(pickleName)
    except:
        return None

def pickleData(fileName,obj):
    """ Pickles a data object """
    (path,name) = os.path.split(fileName)
    pickleName = name + '.pickle'
    if not os.path.exists(pickleDir):
        os.makedirs(pickleDir)
    with open(os.path.join(pickleDir,pickleName),'wb') as f:
        pickle.dump(obj,f)
    print 'File %s pickled in %s'%(fileName,pickleName)

