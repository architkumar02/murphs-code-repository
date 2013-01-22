# coding: utf-8
import numpy as np
import matplotlib.pyplot as plt
# Reading in the data
filename = 'AtomicWeight.txt'
with open(filename,'r') as f:
    Z = list()
    res = list()
    for line in f:
        tokens = line.strip().split()
        if tokens[1].endswith('*') or tokens[0].startswith('#'):
            pass # Ignoring comments and elements with unkown weight
        else:
            z = int(tokens[0])      # Atomic number
            temp = float(tokens[3].split('(')[0])   # Atomic weight
            Z.append(z)
            res.append(temp-z)

# Plotting
fig = plt.figure()
ax = fig.add_subplot(111)
ax.plot(Z,res)
ax.grid(True)
ax.set_ylabel('Avg. Atomic Wt. - Z')
ax.set_xlabel('Atomic Number (Z)')
fig.show()
fig.savefig('StabilityBand.png')
