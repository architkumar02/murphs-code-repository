#! /usr/bin/python -W all
#
# Page numbers refer to MCNPX 2.7.0 manual

import sys, re, string
from array import array
import tally

class Header:
    """mctal header container"""
    def __init__(self,data=None):
        """Header initialisation"""
        self.kod = 0            # name of the code, MCNPX
        self.ver = 0            # code version
        self.probid = []        # date and time when the problem was run
        self.knod = 0           # the dump number
        self.nps = 0            # number of histories that were run
        self.rnr = 0            # number of pseudorandom numbers that were used
        self.title = None       # problem identification line
        self.ntal = 0           # number of tallies
        self.ntals = []         # array of tally numbers
        self.npert = 0          # number of perturbations
        if data:
            self.parse(data)

    def parse(self,data):
        """ Parses the header data """
        for line in data.split('\n'):
            if self.kod == 0:
                self.kod, self.ver, probid_date, probid_time, self.knod, self.nps, self.rnr = line.split()
                self.probid.append(probid_date)
                self.probid.append(probid_time)
                continue
            else:
                if self.title is None and line[0] == " ":
                    self.title = line.strip()
                    continue
            words = line.split()
            if not self.ntal and words[0] == 'ntal':
                self.ntal = int(words[1])
                if len(words) == 4 and words[2] == 'npert':
                    self.npert = int(words[3])
                continue
            if self.ntal:
                for w in words:
                    self.ntals.append(int(w))

    def __str__(self):
        """Prints the class members"""
        s ="code:\t\t{}\n".format(self.kod)
        s+="version:\t{}\n".format(self.ver)
        s+="date and time:\t{}\n".format(self.probid)
        s+="dump number:\t{}\n".format(self.knod)
        s+="number of histories:\t{}\n".format(self.nps)
        s+="number of pseudorandom numbers used:\t{}\n".format(self.rnr)
        s+="title: {}\n".format(self.title)
        if self.ntal>1: 
            s +=str(self.ntal)+' tallies: '+str(self.ntals)+'\n'
        else: 
            s +=str(self.ntal)+' tally: '+str(self.ntals)+'\n'
        if self.npert != 0: s+="number of perturbations: {}\n".format(self.npert)
        return s

class MCTAL:
    """mctal container"""
    good_tallies = []
    def __init__(self, fname='mctal'):
        """Constructor"""
        self.fname = fname      # mctal file name
        self.tallies = dict()   # list of tallies
        self.header = None      # Header
        self.read()

    def read(self):
        """method to parse the mctal file"""

        probid_date = None       # date from probid
        probid_time = None       # time from probid
        tally = None             # current tally
        self.header = Header()
        lines = open(self.fname).read()
        data = lines.split('tally')
        
        self.header = Header(data[0])
        for i in range(1,len(data)):
            self.tallies[self.header.ntals[i-1]] = Tally('tally'+data[i])

    def __str__(self):
        """ String  """
        s = 'Filename: {}\n'.format(str(self.fname))
        s += str(self.header)
        for t in self.tallies:
            s += str(self.tallies[t])
        return s
