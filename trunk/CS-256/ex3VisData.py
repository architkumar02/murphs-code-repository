#!/user/bin/python
# 
# CS 526, Fall 2012, Ex3
# Group H
#   Nicole Pennington (npennin2@utk.edu)
#   Matthew Urffer    (murffer@utk.edu)

# Loading the data
import ex3ImportData
movieDict = ex3ImportData.movieDict
userDict = ex3ImportData.userDict

# Looking at the distrubutions
import numpy as np
# import matplotlib.pyplot as plt

# Distribution of ZIP codes - expect to be flat
zipCodes = list()
for key in userDict:
    zipCodes.append(userDict[key].zipCode)

# Creating a histogram of 25 bins
hist,bins = np.histogram(zipCodes,25)
print bins
print hist
# plt.bar(bins[:-1],hist,width=1)
# plt.xlim(min(bins),max(bins))
# plt.show()

