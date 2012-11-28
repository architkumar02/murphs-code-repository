#!/usr/bin/python
import random
def createSmallGraph(filename,p=0.01):
    with open(filename,'r') as f:
        with open('SmallGraph.txt','w') as fout:
            for line in f:
                if (random.random() < p):
                    fout.write(line)
                    
