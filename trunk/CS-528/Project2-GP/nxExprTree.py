#!/usr/bin/python

import networkx as nx
from collections import deque
import operator
import math
import numpy as np
import random
import fileinput
import sys

class NodeOpp(object):
    """
    Opperators for an expression tree
    """
    def __init__(self,binaryOpps,unaryOpps,terminals):
        self.binaryOpps = binaryOpps
        self.unaryOpps = unaryOpps
        self.terminals = list(terminals)
        self.oppSet = dict(binaryOperators.items()+unaryOperators.items())

    def setWeights(self,weights):
        """
        Sets the weights
        """
        self.weights = weights

    def randomGenerator(self,S,w):
        """
        Randomly returns an item randomly from set S according to its weight w
        """
        s = random.choice([key for key, val in S.iteritems() for x in w])
        return s

    def randomTerminal(self):
        """
        Returns a uniformly chosen terminal from the terminal set
        """
        return random.choice(list(self.terminals))
    
    def randomOpp(self):
        """
        Returns a uniformly randomly chosen opp
        """
        w = np.ones(len(self.oppSet))
        return self.randomGenerator(self.oppSet,w)
    
    def isUnaryOperator(self,opp):
        """
        True if opp is in the unary opp set
        """
        return opp in self.unaryOpps

    def isBinaryOperator(self,opp):
        """
        True if opp is in the binary set
        """
        return opp in self.binaryOpps

    def isTerminal(self,opp):
        """
        True is opp is in the terminal set
        """
        return opp in self.terminals

    def evalOpp(self,opp,arg1,arg2=None):
        """
        Evaluates an operator
        """
        if self.isUnaryOperator(opp):
            return self.unaryOpps[opp](arg1)
        elif self.isBinaryOperator(opp):
            return self.binaryOpps[opp](arg1,arg2)
        else:
            return self.terminals.get(opp)

# Function and Terminal Set
binaryOperators = {'+':operator.add, '-':operator.sub,'*':operator.mul,'/':operator.truediv}
unaryOperators = {'sin':math.sin,'cos':math.cos,'exp':math.exp}
terminals = set([1,math.pi])

oppExprNode = NodeOpp(binaryOperators,unaryOperators,terminals)
def RandomExprTree(height,nodeSet=oppExprNode):
    """
    Creates a graph of a random (full) Epxression Tree
    """
    # Creating a full balanced binary tree of height
    branching = 2			# Binary Tree
    G = nx.balanced_tree(branching,height,create_using=nx.DiGraph())
    numElements = countChildren(G)
    
    prunedNodes = set()
    H = nx.DiGraph(nodeSet=nodeSet)
    # Breadth first traversal (by node)
    for parent in nx.algorithms.bfs_successors(G,0):
        children = G.neighbors(parent)
        if not (parent in prunedNodes):
            # Choose what we operator we want to node to be
            opp = nodeSet.randomOpp()
            #H.add_node(parent,operator=opp)
            H.add_node(parent,operator=opp)
            H.node[parent]['label']=opp
            
            if nodeSet.isUnaryOperator(opp):
                prunedNodes.add(children[0])
        else:
            # Add child to pruned nodes
            prunedNodes |= set(children)
    
    # Adding the leaf nodes
    for n in G.nodes():
        rootSuccessors = [r for r in nx.algorithms.bfs_successors(G,0)]
        if (n not in rootSuccessors) and (n not in prunedNodes):
            terminal = nodeSet.randomTerminal()
            H.add_node(n,operator=terminal)
            H.node[n]['label'] = "%3.2f"%terminal

    # Adding the edges
    for e in nx.algorithms.bfs_edges(G,0):
        parent = e[0]
        child = e[1]
        if (parent in H) and (child in H):
            H.add_edge(parent,child)
    return H

def writeExprTree(G,filename='ExprTree.dot'):
    """
    Writes a tree in the DOT format:
        - http://en.wikipedia.org/wiki/DOT_language
        - http://www.graphviz.org/
    """
    nx.write_dot(G,filename)

def evalExprTree(G):
    """
    Evaluates an expression tree by a post order traversal of the graph
    """
    # Initialize to all initial leafs
    qNode = deque()
    qValue = deque()
    branchNodes = [n for n in nx.algorithms.bfs_successors(G,0)]
    for n in G.nodes_iter():
        if not n in branchNodes:
            qNode.appendleft(n)
            qValue.appendleft(float(G.node[n]['operator']))
    G = nx.Graph(G) # Undirected graphs have neighbors
    parents = nx.algorithms.bfs_predecessors(G,0)
    while len(qNode) > 1:
        child = qNode.pop()
        value = qValue.pop()

        parent = parents[child]
        opp = G.node[parent]['operator']
        if G.graph['nodeSet'].isUnaryOperator(opp):
            qValue.appendleft(G.graph['nodeSet'].evalOpp(opp,value))
            qNode.appendleft(parent)
        else:
            childTwo = qNode.pop()
            valueTwo = qValue.pop()
            qValue.appendleft(G.graph['nodeSet'].evalOpp(opp,value,valueTwo))
            qNode.appendleft(parent)

    return qValue.pop()
    
def subgraph(G,node=0):
    """
    Creates a subgraph on G, starting from node
        A deep copy of the graph is returned
    """
    H = G.subgraph(nx.algorithms.dfs_tree(G,node))
    return nx.DiGraph(H)

def deleteGraph(G,node):
    # Getting the subgraph
    subG = nx.algorthims.dfs_tree(G,node)
    G.remove_edges_from(subG.edges())
    G.remove_nodes_from(subG.nodes())
    return G

def countChildren(G,node=0):
    i = 0     
    return sum(i+1 for n in nx.algorithms.dfs_postorder_nodes(G,node))

def main():
    # Creating a Random Expression Tree
    R = RandomExprTree(2)
    writeExprTree(R)
    print evalExprTree(R)

if __name__ == "__main__":
    sys.exit(main())
