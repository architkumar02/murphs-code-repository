# Expression Tree # # Matthew J. Urffer (matthew.urffer@gmail.com)
#
# Recycled code from 15-111 or 15-211. Updated to python
#

class UnaryNode(Tree):
    """ Unary Node """
    """ Left and Right are mapped to each other """
    def __init__(self,key):
        #super(UnaryNode,self).__init__(key)
        self.key = key
        self.left = None

    def insertRight(self,node):
        """ Inserts a node """
        insertLeft(node)

    def isLeaf(self):
        """ True if node is a leaf node """
        return (not self.left)

from collections import deque
class Tree:
    """ 
    Tree
    Generic Binary Tree implemenation
    """
    def __init__(self,key):
        self.key = key
        self.right = None
        self.left = None

    def insertLeft(self,node):
        """
        Inserts a node to the left
        """
        if self.left = None:
            self.left = node
        else:
            t = ExprTree(node)
            t.left = self.left
            self.left = t



    def insertRight(self,node):
        """
        Inserts a node to the right
        """
        if self.right == None:
            self.right = Tree(node)
        else:
            t = ExprTree(node)
            t.right = self.right
            self.right = t


    def isLeaf(self):
        """
        true if it is a leaf node, false otherwise
        """
        return ((not self.left) and (not self.right))

    def getLeft(self):
        """
        returns the left child
        """
        return self.left

    def getRight(self):
        """
        returns right child
        """
        return self.right

    def writeTree(self,filename="tree.dot"):
        """
        Writes a tree in the DOT format:
            - http://en.wikipedia.org/wiki/DOT_language
            - http://www.graphviz.org/
        """
        nodeList = bfs(self)

    def bfs(self):
        """
        BFS - Breadth first traversal of the tree
        """
        nodeList = list()
        q = deque()
        if self:
            q.append(self.key)
        while len(q) > 0:
            p = q.pop()
            list.append(p)
            if p.left:
                q.append(p.left)
            if p.right
                q.append(p.right)
        return nodeList


