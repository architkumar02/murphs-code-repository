#!/user/bin/python
# 
# CS 526, Fall 2012, Ex3
# Group H
#   Nicole Pennington (npennin2@utk.edu)
#   Matthew Urffer    (murffer@utk.edu)
import numpy as np
import collections
import time

# Class for a Movie
class Movie(object):
    """Class for Movies"""
    def __init__(self,movieID,title,releaseDate,videoReleaseDate,IMDbURL,genre):
        self.movieID = movieID                      # Movie ID
        self.title = title                          # Movie Title
        self.releaseDate = releaseDate              # Movie Release Date
        self.videoReleaseDate = videoReleaseDate    # Date released on video
        self.IMDbURL = IMDbURL                      # IMDbURL
        self.genre = [map(int,x) for x in genre]    # Genre of the movie
    
    @staticmethod 
    def getGenreList():
        """Getting a list of Genres"""        
        genreList = ('unknown','Action', 'Adventure', 'Animation',
              'Children', 'Comedy','Crime','Documentary','Drama','Fantasy',
              'Film-Noir','Horror','Musical','Mystery',' Romance','Sci-Fi',
              'Thriller','War','Western')
        return genreList

    def __str__(self):
        """Prints a formated output of a movie"""
        genreList = Movie.getGenreList()
        s = 'MovieID: %s - %s (%s)' %(self.movieID,self.title,time.asctime(self.releaseDate))
        s += '\n\t Genres:'
        s += ",".join(genreList)
        s += '\n'
        return s
    
    def __repr__(self):
        return self.__str__()


class User(object):
    """Class for Users"""
    def __init__(self,userID=-1,age=-1,gender='',occupation='',zipCode=-1):
        self.userID = userID
        self.age = int(age)
        self.gender = gender
        self.occupation = occupation
        try:
            self.zipCode = int(zipCode)
        except:
            self.zipCode = 000              # Zip code not in use for junk
    def isMale(self):
        """True is the user is male, false otherwise"""
        return (self.gender == 'M')
        
    def getPrimaryZipZone(self):
        """Genereates the Primary Index for a Zip Code"""
        # See wiki zip code to see regions 
        return int(str(self.zipCode)[0])

    def __str__(self):
        """Prints a formated output of a user"""    
        s = 'User %s - Gender: %s Age: %s\n' %(self.userID,self.gender,self.age)
        s += '\tOccupation: %s Zip: %s\n'    %(self.occupation,self.zipCode)
        return s

    def __repr__(self):
        return self.__str__()

class Rating(object):
    """Class for a Movie Rating"""
    def __init__(self,itemID,userID,ranking,timeStamp):
        self.itemID = itemID
        self.userID = userID
        self.ranking = int(ranking)
        self.timeStamp = timeStamp

    def __str__(self):
        """Prints  formated output of Rating"""
        s = 'ItemID: %s UserID: %s '%(self.itemID,self.userID)
        s += 'Ranking: %s TimeStamp: %s\n'%(self.ranking,self.timeStamp)
        return s
    def __repr__(self):
        return self.__str__()

def getRatings(fileName):
    """Reads in the ratings of a particular file, returning a list"""
    try:
        f = open(fileName,'r')
        ratings = []
    except:
        print 'Could not open file %s\n' % fileName
        return None
    for r in f:
        fields = r.strip('\n').split('\t')      # Some reason these are '\t' not '|'
        rating = Rating(fields[0],fields[1],fields[2],fields[3])
        ratings.append(rating)
    return ratings

# Reading in the Movie Dictionary
movieFile = open('MovieLens/u.item','r')
movieDict = dict()                                       # Dictionary of movies
for r in movieFile:
    fields = r.rstrip('\n').split('|')
    m = Movie(fields[0],fields[1],fields[2],fields[3],fields[4],fields[5:24])
    movieDict[m.movieID] = m
movieFile.close()

# Reading in all of the users
userFile = open('MovieLens/u.user','r')
userDict = dict()
for r in userFile:
    fields = r.strip('\n').split('|')
    u = User(fields[0],fields[1],fields[2],fields[3],fields[4])
    userDict[u.userID] = u
userFile.close()

# Getting all of the ratings
ratings = getRatings('MovieLens/u.data')

#
def mergeData(movieDict=movieDict,userDict=userDict,ratings=ratings):
    """Combines the Movies, User, and Ratings into a numpy recordList"""
    numElem = len(ratings)
    data = np.zeros(numElem,dtype=[('releaseDate','a16'),('genre','i1',(19,1)),
            ('age','i2'),('gender','a1'),('zip','i4'),('occupation','a16'),
            ('rating','i2')])
    for index in range(numElem):
        rating = ratings[index]
        try:
            u = userDict[rating.userID]
        except:
            u = User()
        m = movieDict[rating.itemID]
        data[index] =  (m.releaseDate,m.genre,
                u.age,u.gender,u.zipCode,u.occupation,rating.ranking)
    return data 

dataTable = mergeData(movieDict, userDict,ratings)

# 
def clusterGenreRating(dataTable,groups,key,inGroupFun):
    """Clusters the rating of genres by groups define by key"""
    """clusterGenreRating
        
        Clusters the data contained as a numpy record in data table
        groups - defination of the groups that key should be clustered in
        inGroupFun - function to return true if the record entry by key is in that group
    """
    genreList = Movie.getGenreList()
    clusterRatings = np.zeros((len(groups),len(genreList)))
    for entry in dataTable:
        for i in range(len(groups)):
            if inGroupFun(groups[i],entry[key]):
                clusterRatings[i] += np.transpose(entry['genre']*entry['rating'])[0]
                continue
    clusterRatings = clusterRatings/len(dataTable)
    return clusterRatings

def aggregate(dataTable,key,value,func):
    """Aggregates the data of a particular column"""
    dataPerKey = collections.defaultdict(list)
    for k,v in zip(dataTable[key],dataTable[value]):
        dataPerKey[k].append(v)
    return [(k,func(dataPerKey[k])) for k,v in dataPerKey.items()]

