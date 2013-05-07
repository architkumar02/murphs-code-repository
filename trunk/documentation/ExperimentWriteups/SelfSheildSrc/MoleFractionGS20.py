#!/usr/bin/env python
# Script to calculate the atomic compostion of GS20 at various mass fractions
# of Li-6.  It is dubios that this is actually correct, however it produces
# differnet atomic compositions for use in MCNPX, and we can get the actual
# mass fraction from the print table 40 of MCNPX
import numpy as np

# Global Variables
# moleFrac = np.array([0.165,0.67067,0.05,0.05,0.51,0.0035]) # Martin
moleFrac = np.array([0.66,1.91,0.05,0.20,0.51,0.14]) # Matthew Urffer
zaID = ('3006.70c','8000.70c','12000.70c','13000.70c','14000.70c','58000.70c')
massFracLi6GS20 = 6.6

def ComputeMoleFrac(massFracLi6=6.6):
  val = massFracLi6/massFracLi6GS20*moleFrac
  valLi = val[0]
  val = moleFrac-val/100
  val[0] = valLi
  val = np.abs(val)
  return val/np.sum(val)

def PrintMoleFrac(mtNum,mFrac,massFracLi):
  s = 'm{0}    $ GS20, scaled to {1} % mass Li-6\n'.format(mtNum,massFracLi)
  s += '      {0} {1:.3f} {2} {3:.3f} {4} {5:.3f}\n'.format(zaID[0],mFrac[0],zaID[1],mFrac[1],zaID[2],mFrac[2])
  s += '      {0} {1:.3f} {2} {3:.3f} {4} {5:.3f}'.format(zaID[3],mFrac[3],zaID[4],mFrac[4],zaID[5],mFrac[5])
  print s

def main():
	massFrac = np.array([1, 2.5, 5, 6.6,10])
	mtBase = '30'
	for i in range(len(massFrac)):
		mFrac = ComputeMoleFrac(massFrac[i])
		PrintMoleFrac(mtBase+str(i),mFrac,massFrac[i])

if __name__ == "__main__":
  main()

