#!/user/bin/python
# 
# CS 526, Fall 2012, Ex3
# Group H
#   Nicole Pennington (npennin2@utk.edu)
#   Matthew Urffer    (murffer@utk.edu)

# Loading the data
import ex3ImportData as data
movieDict = data.movieDict
userDict = data.userDict

# Looking at the distrubutions
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

# Distribution of ZIP codes - expect to be flat
zipCodes = list()
for key in userDict:
    zipCodes.append(userDict[key].zipCode)

# www.fema.gov/sites/default/files/orig/fraphics/fhm/regions1.jpg
# Bins are broken down into regions defined above / on Wikipedia
# binLabels = ['New England','North East','Mid Atlantic','South East',
#             'Great Lakes','North Plains','Mid Plains','South','Rockies',
#             'West Coast']
binLabels = ['I','II','III','IV','V','VI','VII','VIII','IX','X']
bins = [00000,10000,20000,30000,40000,50000,60000,70000,80000,90000]
hist,bins = np.histogram(zipCodes,bins)
print bins
print hist
fig = plt.figure(1)
ax = fig.add_subplot(111)
ax.bar(bins[:-1],hist,10,align='center')
ax.xaxis.set_major_locator(ticker.MaxNLocator(len(bins)+2))
ax.xaxis.set_ticklabels(binLabels)
ax.set_xlabel('Region')
ax.set_ylabel('Frequency')

# Heat Map of the average rating of a genre based on a user category
#   Attributes:
#       - Age (0-20, 20 to 35, 35 to 50, 50 and up)
#       - Zip Code (by region, 10 categories)
#       - Gender (Male / Female)
#       - Occupation (Probably divide into sub groups)
def betweenNumValues(r,val):
    return (r >= val).any()

def equals(r,val):
    return r == val
ageGroups = np.array([[0,20],[20,35],[35,50],[50,100]])
ageCluster = data.clusterGenreRating(data.dataTable,ageGroups,'age',betweenNumValues)

zipGroups = np.array([[0,10000],[10000,20000],[20000,40000],[40000,50000],[60000,70000],
                      [80000,90000],[90000,100000]])
zipCluster = data.clusterGenreRating(data.dataTable,zipGroups,'zip',betweenNumValues)

genderGroups = ['M','F']
genderCluster = data.clusterGenreRating(data.dataTable,genderGroups,'gender',equals)

print 'Average Rating per Genre (by age group)'
print ageGroups
print ageCluster
print 'Average Rating per Genre (by zip code)'
print zipGroups
print zipCluster
print 'Average Rating per Genre (by gender)'
print genderGroups
print genderCluster

# Concanting all of the clustered data together
d = np.concatenate((ageCluster,zipCluster,genderCluster))
X = np.arange(len(data.Movie.getGenreList()))
Y = np.arange(len(ageGroups)+len(zipGroups)+len(genderGroups))
# Y = range(np.sum(len(ageGroups)+len(zipGroups)+len(genderGroups)))
fig = plt.figure(2)
ax = fig.add_subplot(111)
print d
print X
print Y
ax.pcolormesh(X,Y,d)
ax.grid('on','both')     # major and minor gridlines
ax.set_xlabel('Genre')
ax.xaxis.set_major_locator(ticker.LinearLocator(len(data.Movie.getGenreList())))
ax.xaxis.set_ticklabels(data.Movie.getGenreList())
for tick in ax.xaxis.get_major_ticks():
    tick.label.set_fontsize('x-small')
    tick.label.set_rotation('vertical')
ylabels = ['0-20','20-25','35-50','50-100']+binLabels+genderGroups
ax.yaxis.set_major_locator(ticker.LinearLocator(len(ylabels)))
ax.yaxis.set_ticklabels(ylabels)
for tick in ax.yaxis.get_major_ticks():
    tick.label.set_fontsize('x-small')
ax.set_ylabel('Cluster Group')
plt.savefig('RankingClusters.png')

