#!/usr/bin/python
#
# Script to demonstrate the CART-like DT classifier from
# Chapter 7 of "Programming Collective Intelligence" by
# T. Segaran, O'Reilly, (c) 2007
#
import treepredict
results=treepredict.divideset(treepredict.my_data,2,'yes')
# results is now a list of lists

# See if records are divided according to  FAQ field (column) ...

print "\nDivision on Read FAQ field...\n"
for list in results:
    for item in list:
      print "%15s %15s %5s %10d %15s" % tuple(item)


# Let's see the difference between gini- and entropy-based impurities
# of the current data (no splitting)
print "\nParent node...\n"
gini=treepredict.giniimpurity(treepredict.my_data)
entr=treepredict.entropy(treepredict.my_data)
print "Gini: %8f    Entropy: %8f" % (gini,entr)

# Let's now split on the Read FAQ field and assess impurity 
node1,node2=treepredict.divideset(treepredict.my_data,2,'yes')
print "\nRead FAQ =  Yes leaf node...\n"
gini=treepredict.giniimpurity(node1)
entr=treepredict.entropy(node1)
print "Gini: %8f    Entropy: %8f" % (gini,entr)

# Build the DT recursively using the buildtree function; assumes
# last column/field is the classification attribute.

tree=treepredict.buildtree(treepredict.my_data)

# Let's see what it looks like...
print "\nFinal tree...\n"
treepredict.printtree(tree)

# Produce a png of the tree
treepredict.drawtree(tree,jpeg="sample_tree.jpg")
print "\npng of tree generated using PIL (Python Imaging Library) modules.\n"

# Let's classify an incoming record of '(direct), USA, yes, 5' ...
incoming = ['(direct)','USA','yes',5]
print "Prediction of new record: ",treepredict.classify(incoming,tree)

# Let's see how the missing data classification via
# the "mdclassify" function performs on our sample data.

# Suppose the page field is mssing...
reload(treepredict)
missing1 = ['google','France',None,None];
treepredict.mdclassify(missing1,tree)
print "Prediction when missing pages: ", treepredict.mdclassify(missing1,tree)

# Finally, what does pruning do with say a mingain = 0.9 ?
print "\nPruned tree...\n"
treepredict.prune(tree,0.9)
treepredict.printtree(tree)

# For group homework, modify "buildtree" function so that it stops
# when a threshold value on entropy is no longer satisfied. It should
# accept a minimum gain parameter and stop dividing the branch if
# this condition is not met.  Pruning the tree will not be used in
# this cas.
