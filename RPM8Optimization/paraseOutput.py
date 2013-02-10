#!/usr/bin/python
# coding: utf-8
# Let's do some python analysis
import os
import math
# Looping through all of the files in the directory
for files in os.listdir('Output'):
    # Looking through the files
    tokens = files.split('_')
    print tokens[2]+" "+tokens[4]+" "+tokens[6]
    with open(os.path.join('Output',files),'r') as f:
        while True:
            line = f.readline()
            if line is None or not line: break
            # Checking if we need to extract a tally
            if line.startswith('tally'):
                tokens = line.strip().split()
                if tokens[1].isdigit() and int(tokens[1]) %10 == 4:
                    # Need to extract a tally - looking for the vals card
                    while not line.startswith('vals'):
                        line=f.readline()
                        if line.startswith('f'):
                            cells=f.readline().strip().split()
                    # Next stop card is the tfc
                    line = f.readline()
                    print cells
                    while not line.startswith('tfc'):
                        # These are the ones we want!
                        print line
                        line = f.readline()
