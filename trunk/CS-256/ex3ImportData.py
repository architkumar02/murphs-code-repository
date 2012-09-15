#!/user/bin/python
# 
# CS 526, Fall 2012, Ex3
# Group H
#   Nicole Pennington (npennin2@utk.edu)
#   Matthew Urffer    (murffer@utk.edu)

# Class for a Movie
class Movie(object):
    """Class for Movies"""
    def __init__(self,movieID,title,releaseDate,videoReleaseDate,IMDbURL,genre):
        self.movieID = movieID                      # Movie ID
        self.title = title                          # Movie Title
        self.releaseDate = releaseDate              # Movie Release Date
        self.videoReleaseDate = videoReleaseDate    # Date released on video
        self.IMDbURL = IMDbURL                      # IMDbURL
        self.genre = map(int,genre)                 # Genre of the movie
        self.genreKey = ('unknown','Action', 'Adventure', 'Animation',
              'Children', 'Comedy','Crime','Documentary','Drama','Fantasy',
              'Film-Noir','Horror','Musical','Mystery',' Romance','Sci-Fi',
              'Thriller','War','Western')
    
    def getGenreList(self):
        genreList = []
        """Getting a list of Genres"""        
        for index in range(len(self.genre)):
            if self.genre[index] == 1:
                genreList.append(self.genreKey[index])
        return genreList

    def __str__(self):
        """Prints a formated output of a movie"""
        genreList = self.getGenreList()
        s = 'MovieID: %s - %s' %(self.movieID,self.title)
        s += '\n\t Genres:'
        s += ",".join(genreList)
        s += '\n'
        return s
    
    def __repr__(self):
        return self.__str__()


class User(object):
    """Class for Users"""
    def __init__(self,userID,age,gender,occupation,zipCode):
        self.userID = userID
        self.age = age
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


# Reading in the Movie Dictionary
movieFile = open('MovieLens/u.item','r')
movieDict = dict()                                       # Dictionary of movies (HashSet)
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
    print u
    userDict[u.userID] = u
userFile.close()

