import nxExprTree as exprTree
import numpy as np
import sys
import time

def print_timing(func):
    def wrapper(*arg):
        t1 = time.time()
        res = func(*arg)
        t2 = time.time()
        print '%s took %5.3f s'%(func.func_name,(t2-t1))
        return res
    return wrapper


def createForest(initPop=500,treeDepth=10):
    """
    Creates a forest of expression trees
    """
    forest = list()
    for i in range(initPop):
        forest.append(exprTree.RandomExprTree(treeDepth))
    return forest

def evaluateForest(forest):
    """
    Evalautes a forest of expression trees
    """
    result = np.zeros(len(forest))
    i = 0
    for tree in forest:
        result[i] = tree.evalExprTree()
    return result

def performance(popSize,depths)
    for pop in popSize:
        for depth in depths:
            # Creating the forest
            t1 = time.time()
            forest = createForest(pop,depth)
            t2 = time.time()
            print '%5.3f s to create a forest of %d trees %d deep'%((t2-t1),pop,depth)
            # Evaluating the forest
            t1 = time.time()
            values = evaluateForest(forest)
            t2 = time.time()
            print '%5.3f s to evalaute %d trees %d deep.  Mean:  Std: '%((t2-t1),pop,depth,np.mean(values),np.std(values))
def main():
    # Creating a forest of trees
    initPop = 50
    depth = 10
    t1 = time.time()
    forest = createForest(initPop,depth)
    t2 = time.time()
    print '%5.3f s to create a forest of %d trees %d deep'%((t2-t1),initPop,depth)

if __name__ == "__main__":
    sys.exit(main())
