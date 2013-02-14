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
parser.add_argument('--verbose',help='Verbosity and printing',action="store",type=bool,default=False)
args = parser.parse_args()

# Printing a description of the geometry
if args.verbose:
    print "Creating a geometry of"
    print "\t"+str(args.modSpace)+" cm front moderator thickness"
    print "\t"+str(args.filmLayers)+" film layers per assembly"
    print "\t"+str(args.assemblySpace)+" cm between film assemblies"
    print "\t"+str(args.minSpace)+" cm spacing light guide between film layers"
    print "\t"+str(args.filmThickness)+" cm thick film layers"

# Material Dictionaries
detMaterial = {'name':'Detector','mt': 3, 'rho': 1.1}       # detector
lightGuideMaterial = {'name': 'PMMA','mt':10, 'rho':0.93}   # PMMA
modMaterial = {'name':'HDPE','mt':456, 'rho': 0.93}         # HPDE

# Creating a mapping between the distance and the material
material = dict()
dx = args.minSpace      # Spacing of surfaces
xMax = 12.7             # Maximum surface
x = args.modSpace       # Starting at the moderator
material[str(x)] = modMaterial
while (x < xMax):

    # Film assemblies
    layer = 0
    while layer < args.filmLayers and x < xMax:
        # Detector
        x += args.filmThickness
        if x < xMax:
            material[str(x)] = detMaterial
        
        # Light Guide (spacer)
        xLightGuide = 0
        while xLightGuide <= args.minSpace and x < xMax:
            x += dx
            xLightGuide += dx
            if x < xMax:
                material[str(x)] = lightGuideMaterial
        layer += 1

    # Assembly spacers / moderator
    xSpace = 0
    while xSpace <= args.assemblySpace and x < xMax:
        x += dx
        xSpace += dx
        if x < xMax:
            material[str(x)] = modMaterial

# Printing out mapping
keyList = sorted(material.keys(), key = lambda x : float(x))

cNum=600
sNum=600
sPrev=500

F4Tallies = list()
F2Tallies = list()
# Writing the surfaces,cells and tallies
with open('cells.txt','w') as c, open('surfaces.txt','w') as s:
    
    for key in keyList:
        # Surfaces and cells
        s.write(str.format('{:5d} px {:5.3f}\n',sNum, float(key)))
        c.write(str.format('{:5d} {:d} -{:4.3f} {:d} -{:d} 502 -503 504 -505\n',
                cNum,material[key]['mt'],material[key]['rho'],sPrev,sNum))
        # Tallies
        if material[key]['name'] is 'Detector':
            F4Tallies.append(str(cNum))
        F2Tallies.append(str(sNum))
        sPrev = sNum
        sNum += 1
        cNum += 1
    
with open('tallies.txt','w') as t:
    # Write the tallies
    F4String = 'F4:n'
    SDString = 'SD4 '
    i = 0
    for tally in F4Tallies:
        F4String += ' '+tally
        SDString += ' 1'
        if i % 6 is 5:
            F4String += '\n      '
        i += 1
    t.write(F4String+' T'+'\n')
    t.write(SDString+' 1'+'\n')
    t.write('FM4 -1 3 105\n')
    F2String = 'F2:n'
    i = 0
    for tally in F2Tallies:
        F2String += ' '+tally
        if i % 6 is 5:
            F2String += '\n      '
        i += 1
    t.write(F2String+'\n')
    t.write('E2 0 5E-7 10\n')

