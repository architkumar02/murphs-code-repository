#! /usr/bin/python -W all
#
# Tally Class
# Matthew Urffer (matthew.urffer@gmail.com)
# Page numbers refer to MCNPX 2.7.0 manual

import sys, re, string
from array import array

class Tally:
    def __init__(self,text=None):
        self.number = None
        self.title = ""
        self.particle = None
        self.type = None    
        self.axes = dict()
        self.axes['f'] = None
        self.axes['d'] = None
        self.axes['u'] = None
        self.axes['s'] = None
        self.axes['m'] = None
        self.axes['c'] = None
        self.axes['e'] = None
        self.axes['t'] = None
        self.data = []
        self.errors = []
        self.tfc_n =  None
        self.tfc_jtf = []
        self.tfc_data = []
        if text:
            self.parse(text)
   
    def parse(tally,data):
        """ Parsing the tally """
        is_vals = False   
        is_list_of_particles = False
        for line in data.split('\n'):
            if not line:
                break
            words = line.split()
            if words[0] == 'tally':
                tally.number = int(words[1])
                tally.particle = int(words[2])
                if tally.particle < 0:
                    is_list_of_particles = True
                tally.type = int(words[3])
                continue

            if is_list_of_particles:
                tally.particle = map(int, words)
                is_list_of_particles = False

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
    
    def __str__(self):
        """Tally printer"""
        types = ['nondetector', 'point detector', 'ring', 'FIP', 'FIR', 'FIC']
        s="tally #{}:\t{}\n".format(self.number, self.title)
        s+="\tparticles: ".join(self.GetParticleNames())+'\n'
        s+="\ttype: {} ({})\n".format(self.type, types[self.type])
        if self.d == 1:
            s+='\tthis is a cell or surface tally unless there is CF or SF card\n'
        elif self.d == 2:
            s+='\tthis is a detector tally (unless thre is an ND card on the F5 tally)\n'
        for ax in self.axes:
            s += '\t'+str(self.axes[ax])+''
        s += 'Also have data and errors fields\n'
        return s

    def GetParticleNames(self):
        """Convert the array of 1 and 0 to the list of particles
        according to the Table 4-1 on page 4-10 (48) of the MCNP 2.7.0 Manual
        """
        names = ['neutron', '-neutron', 'photon', 'electron', 'positron',
             'mu-', '!mu-', 'tau-', 'nue', '!nue', 'num', 'nut',
             'proton', '!proton', 'lambda0', 'sigma+', 'sigma-', 'cascade0', 'cascade-', 'omega-', 'lambdac+', 'cascadec+', 'cascadec0', 'lambdab0',
             'pion+', 'pion-', 'pion0', 'kaon+', 'kaon-', 'K0short', 'K0long', 'D+', 'D0', 'Ds+', 'B+', 'B0', 'Bs0',
             'deuteron', 'triton', 'He3', 'He4', 'heavy ions']
        vals = []
        for i in range(len(self.particle)):
            if self.particle[i] == 1: vals.append(names[i])
            elif self.particle[i] != 0:
                print 'strange values (not 0 or 1) found in the list of particles:', a
        return vals
    
    def GetBins(self,binName):
        """ Returns the bins of binName """
        return self.axes[binName].GetBins()

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
        self.numbers = numbers
        self.number = int(self.numbers[0])
        self.arraycsn = [] 
        self.ni = None
        self.nj = None
        self.nk = None
        if len(self.numbers)>1:
            self.ni, self.nj, self.nk = self.numbers[2:]
            print self.ni, self.nj, self.nk
    
    def __str__(self):
        s = "Axis: "+self.name+' Number Bins: {}\n'.format(self.number)
        return s

    def Print(self):
        """Axis printer"""
        print "\t Axis %s" % self.name
        print "\t\tcsn %s" % self.arraycsn
