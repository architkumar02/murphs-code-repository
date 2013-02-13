#!/usr/bin/python
# createSurfaceCells.py
# @author matther.uffer@gmail.com
# CHANGELOG:
#   2013-02-13 Started the file.  Intent is to have regular layers for
#       determing the flux profile, and then impose the cell and tallies on
#       top of that layer structure.

import sys
import argparse

# Parsing command line arguments
parser = argparse.ArgumentParser(description='Write surface and cells for MCNPX input deck for layered detectors in the RPM8')
parser.add_argument('--modSpace',help="Thickness of the front moderator",action="store",type=float,default=2.5)
parser.add_argument('--filmLayers',help="Number of film layers in an assembly",action="store",type=int,default=1)
parser.add_argument('--assemblySpace',help="Space between film assemblies",action="store",type=float,default=1.0)
parser.add_argument('--minSpace',help="mininum space between film assemblies",action="store",type=float,default=0.5)
parser.add_argument('--filmThickness',help="thickness of the films",action="store",type=float,default=0.01)
args = parser.parse_args()

# Printing a description of the geometry
print "Creating a geometry of"
print "\t"+str(args.modSpace)+" cm front moderator thickness"
print "\t"+str(args.filmLayers)+" film layers per assembly"
print "\t"+str(args.assemblySpace)+" cm between film assemblies"
print "\t"+str(args.minSpace)+" cm spacing light guide between film layers"

# Computing and printing variable stepping
x = 0.0
dx = args.minSpace      # Spacing of surfaces
xMax = 12.7             # Maximum surface
print "\t"+str.format("Stepping by {0} cm to {1} cm",dx,xMax)

cell=600
sNum=600
sPrev=500

# Material Dictionaries
detMaterial = {'mt': 3, 'rho': 1.1}         # detector
lightGuideMaterial = {'mt':10, 'rho':0.93}  # PMMA
modMaterial = {'mt':456, 'rho': 0.93}       # HPDE

# Creating a mapping between the distance and the material
material = dict()
x = args.modSpace
material[str(x)] = modMaterial
while (x < xMax):
    x += dx
    

# Writing the surfaces and cells
with open('cells.txt','w') as c, open('surfaces.txt','w') as s:
    
    # Need to write the initial moderator
    x = args.modSpace
    s.write(str.format('{:5d} px {:5.3f}\n',sNum, x))
    m = material[str(x)]
    c.write(str.format('{:5d} {:d} -{:4.3f} {:d} -{:d} 502 -503 504 -505',cNum,m['mt'],m['rho'],sPrev,sNum))
    sNum += 1
    cNum += 1
    sPrev = sNum
    
    # Repeated Detector Assemblies
    while (x < xMax):
        s.write(str.format('{:5d} px {:5.3f}\n',sNum, x))
        m = material[str(x)]
        c.write(str.format('{:5d} {:d} -{:4.3f} {:d} -{:d} 502 -503 504 -505',cNum,m['mt'],m['rho'],sPrev,sNum))
        x += dx
        sNum += 1
        cNum += 1
        sPrev = sNum
