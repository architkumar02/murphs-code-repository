#! /usr/bin/python -W all
#
# Page numbers refer to MCNPX 2.7.0 manual

import sys, re, string
from array import array

class Header:
    """mctal header container"""
    def __init__(self):
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

    def Print(self):
        """Prints the class members"""
        print ("code:\t\t%s" % self.kod)
        print ("version:\t%s" % self.ver)
        print ("date and time:\t%s" % self.probid)
        print ("dump number:\t%s" % self.knod)
        print ("number of histories:\t%s" % self.nps)
        print ("number of pseudorandom numbers used:\t%s" % self.rnr)
        print ("title: %s" % self.title)

        if self.ntal>1: print self.ntal, 'tallies:', self.ntals
        else: print self.ntal, 'tally:', self.ntals

        if self.npert != 0: print("number of perturbations: %s" % self.npert)

class Tally:
    number = None
    title = ""
    particle = None
    type = None     # tally type: 0=nondetector, 1=point detector; 2=ring; 3=pinhole radiograph; 4=transmitted image radiograph (rectangular grid); 5=transmitted image radiograph (cylindrical grid) - see page 320 of MCNPX 2.7.0 Manual
    f  = None       # number of cell, surface or detector bins
    d  = None       # number of total / direct or flagged bins
# u is the number of user bins, including the total bin if there is one.
#   If there is a total bin, then 'ut' is used.
#   If there is cumulative binning, then 'uc' is used
# The same rules apply for the s, m, c, e, and t - lines
    u  = None
    s  = None       # segment or radiography s-axis bin
    m  = None       # multiplier bin
    c  = None       # cosine or radiography t-axis bin
    e  = None       # energy bin
    t  = None       # time bin
    binlabels = []

    axes = {}

    data = []
    errors = [] # errors are relative

    tfc_n   = None  # number of sets of tally fluctuation data
    tfc_jtf = []    # list of 8 numbers, the bin indexes of tally fluctuation chart bin
    tfc_data = []   # list of 4 numbers for each set of tally fluctuation chart data: nps, tally, error, figure of merit
    
    def __init__(self, number):
        self.axes['f'] = None
        self.axes['d'] = None
        self.axes['u'] = None
        self.axes['s'] = None
        self.axes['m'] = None
        self.axes['c'] = None
        self.axes['e'] = None
        self.axes['t'] = None

        self.number = number
        del self.data[:]
        del self.errors[:]

    def Print(self, option=''):
        """Tally printer"""
        print "\nprinting tally:"
        types = ['nondetector', 'point detector', 'ring', 'FIP', 'FIR', 'FIC']
        print "tally #%d:\t%s" % (self.number, self.title)
        if option == 'title': return
        print "\tparticles:", self.particle
        print "\ttype: %s (%s)" % (self.type, types[self.type])

        if self.d == 1:
            print '\tthis is a cell or surface tally unless there is CF or SF card'
        elif self.d == 2:
            print '\tthis is a detector tally (unless thre is an ND card on the F5 tally)'

        print "\taxes:"
        for b in self.axes.keys():
            self.axes[b].Print()

        print "\tdata:"
        print self.data
        print "\terrors:"
        print self.errors

    def GetParticleNames(a):
        """Convert the array of 1 and 0 to the list of particles
        according to the Table 4-1 on page 4-10 (48) of the MCNP 2.7.0 Manual
        """
        # '!' stands for 'anti'
        names = ['neutron', '-neutron', 'photon', 'electron', 'positron',
             'mu-', '!mu-', 'tau-', 'nue', '!nue', 'num', 'nut',
             'proton', '!proton', 'lambda0', 'sigma+', 'sigma-', 'cascade0', 'cascade-', 'omega-', 'lambdac+', 'cascadec+', 'cascadec0', 'lambdab0',
             'pion+', 'pion-', 'pion0', 'kaon+', 'kaon-', 'K0short', 'K0long', 'D+', 'D0', 'Ds+', 'B+', 'B0', 'Bs0',
             'deuteron', 'triton', 'He3', 'He4', 'heavy ions']
        vals = []
        for i in range(len(a)):
            if a[i] == 1: vals.append(names[i])
            elif a[i] != 0:
                print 'strange values (not 0 or 1) found in the list of particles:', a
        return vals

    def getName(self):
        """
        Return the name of the tally
        """
        return "f%s" % self.number
    

class Axis:
    """Axis of a tally"""
    def __init__(self, name, numbers):
        """Axis Constructor"""
        self.name = name
        self.numbers = numbers # we keep all array numbers, but not the first one only (in the case of mesh tally there are > than 1 number)
        self.number = int(self.numbers[0]) # see page 320
        self.arraycsn = [] # array of cell or surface numbers (written for non-detector tallies only)
        
        # ni, nj and nk make sense for mesh tallies only (see e.g. tally 4 in figs/cold-neutron-map/2/case001/small-stat/mctal)
        # these are number of bins in i,j,k directions
        self.ni = None
        self.nj = None
        self.nk = None
        if len(self.numbers)>1:
            self.ni, self.nj, self.nk = self.numbers[2:]
            print self.ni, self.nj, self.nk

    def Print(self):
        """Axis printer"""
        print "\t Axis %s" % self.name
        print "\t\tcsn %s" % self.arraycsn


class MCTAL:
    """mctal container"""
    good_tallies = [5] # list of 'good' tally types

    def __init__(self, fname):
        """Constructor"""
        self.fname = fname # mctal file name
        self.tallies = []  # list of tallies
        self.read()

    def read(self):
        """method to parse the mctal file"""

        probid_date = None       # date from probid
        probid_time = None       # time from probid
        tally = None             # current tally
        is_vals = False          # True in the data/errors section
        is_list_of_particles = False # True if the line with the list of particles follows the ^tally line
        h = Header()

        file_in = open(self.fname)
        for line in file_in.readlines():
            if h.kod == 0:
                h.kod, h.ver, probid_date, probid_time, h.knod, h.nps, h.rnr = line.split()
                h.probid.append(probid_date)
                h.probid.append(probid_time)
                continue
            else:
                if h.title is None and line[0] == " ":
                    h.title = line.strip()
                    continue

            words = line.split()

            if not h.ntal and words[0] == 'ntal':
                h.ntal = int(words[1])
                if len(words) == 4 and words[2] == 'npert':
                    h.npert = int(words[3])
                continue

            if h.ntal and not tally and words[0] != 'tally':
                for w in words:
                    h.ntals.append(int(w))

            if words[0] == 'tally':
                if tally: 
                    if tally.number:
                        self.tallies.append(tally)
                    del tally
                tally = Tally(int(words[1]))
                tally.particle = int(words[2])
                if tally.particle < 0: # then tally.particle is number of particle types and the next line lists them
                    is_list_of_particles = True
                tally.type = int(words[3])
                if tally.number not in h.ntals:
                    print 'tally %d is not in ntals' % tally.number
                    print h.ntals
                    return 1
                continue

            if is_list_of_particles:
                tally.particle = map(int, words)
                is_list_of_particles = False
            
            if not tally: continue

            if tally.axes['f'] and tally.axes['d'] is None and line[0] == ' ':
                for w in words:
                    tally.axes['f'].arraycsn.append(str(w))

            if tally.axes['t'] and is_vals == False and len(tally.data) == 0 and line[0] == ' ':
                for w in words: tally.axes['t'].arraycsn.append(float(w))

            if tally.axes['e'] and tally.axes['t'] is None and line[0] == ' ':
                for w in words: tally.axes['e'].arraycsn.append(float(w))

            if tally.axes['u'] and tally.axes['s']is None and line[0] == ' ':
                for w in words: tally.axes['u'].arraycsn.append(float(w))

            if   not tally.axes['f'] and re.search('^f', line[0]):        tally.axes['f'] = Axis(words[0], map(int, words[1:]))
            elif not tally.axes['d'] and re.search('^d', line[0]):        tally.axes['d'] = Axis(words[0], map(int, words[1:]))
            elif not tally.axes['u'] and re.search ("u[tc]?", line[0:1]): tally.axes['u'] = Axis(words[0], map(int, words[1:]))
            elif not tally.axes['s'] and re.search('^s[tc]?', line[0:1]): tally.axes['s'] = Axis(words[0], map(int, words[1:]))
            elif not tally.axes['m'] and re.search('^m[tc]?', line[0:1]): tally.axes['m'] = Axis(words[0], map(int, words[1:]))
            elif not tally.axes['c'] and re.search('^c[tc]?', line[0:1]): tally.axes['c'] = Axis(words[0], map(int, words[1:]))
            elif not tally.axes['e'] and re.search("^e[tc]?",  line[0:1]):tally.axes['e'] = Axis(words[0], map(int, words[1:]))
            elif not tally.axes['t'] and re.search("^t[tc]?", line[0:1]): tally.axes['t'] = Axis(words[0], map(int, words[1:]))
            elif line[0:2] == 'tfc':
                tally.tfc_n = words[1]
                tally.tfc_jtf = words[2:]

            if tally.tfc_n and line[0] == ' ':
                tally.tfc_data = words

            if words[0] == 'vals':
                is_vals = True
                continue

            if is_vals:
                if line[0] == ' ':
                    for iw, w in enumerate(words):
                        if not iw % 2: tally.data.append(float(w))
                        else: tally.errors.append(float(w))
                else:
                    is_vals = False

        file_in.close()
