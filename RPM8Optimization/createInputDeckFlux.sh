#!/bin/bash

# RPM8 SIZE
L0='12.7'
W0='15.25'
H0='217.7'

# MCNP INPUT DECK Starts
sStart=601
cStart=601
tStart=1

# Detector Constants
XMIN='0.5'      # 5 mm spacing between films
XDET='0.01'     # Films are 100 microns

# INPUT ARGUMENT CHECKING
# We expect two arguments, the number of film layers the spacing between
# detector assemblies
if [ $# -lt 3 ]; then
    echo "usage: ./createInputDeck ModSpace #filmLayers filmSpacing"
    echo "       modSpac     - the thickness of the front moderator"
    echo "       #filmLayers - the number of film layer assemblies"
    echo "       filmspacing - the spacing between the assemblies"
    exit 1
elif [ $(echo "$1 < 0" | bc -l) -eq 1 ]; then
    echo "usage: ./createInputDeck ModSpace #filmLayers filmSpacing"
    echo "       $1 modSpace must be at least zero"
    exit 1
elif [ $(echo "$3 < $XMIN" | bc -l) -eq 1 ]; then
    echo "usage: ./createInputDeck ModSpace #filmLayers filmSpacing"
    echo "       filmspacing $3 must be greater than the min spacing: $XMIN"
    exit 1
fi

# Input argument assignment
MODSPACE=$1
FILMLAYERS=$2
XSPACE=$3

###############################################################################
#                    Creation of MCNP Input Deck                              #
# Algorthim:                                                                  #
#   1) Compute the distance                                                   #
#   2) Add surface, cells, and tallies                                        #
#       - surface are simple planes                                           #
#       - cells are left and right of surface, inside an RPP                  #
#       - tallies are grouped into the detector units                         #
###############################################################################

cellCounter=0
#
# Writes the surface and cells
#
writeCell(){
    cell=$(printf "%5d %d -%4.3f %d -%d 502 -503 504 -505" $cNum $mt $rho $sPrev $sNum )
    sed -i "/CELLPREPENDTOKEN/i $cell" INP.mcnp
}
writeSurface(){
    surface=$(printf "%5d px %5.3f\n" $sNum $x)
    sed -i "/SURFACEPREPENDTOKEN/i $surface" INP.mcnp
}
#
# Write Tally
#
writeTally(){
    sed -i "/TALLYPREPENDTOKEN/i $F2 T" INP.mcnp
    sed -i "/TALLYPREPENDTOKEN/i E${tNum}2 1E-6 50ilog 2" INP.mcnp
    sed -i "/TALLYPREPENDTOKEN/i $F4 T" INP.mcnp
    sed -i "/TALLYPREPENDTOKEN/i $SD 1" INP.mcnp
    sed -i "/TALLYPREPENDTOKEN/i FM${tNum}4 -1 $mt 105" INP.mcnp
    tNum=$(echo "$tNum+1" | bc)
}
#
# Adds to the tally 
# 
incTally(){
    F2="${F2} $sNum"
    F4="${F4} $cNum"
    SD="${SD} 1"
}
# 
# Increments the surface and cell numbers
#
incrementNum(){
    sPrev=$sNum
    sNum=$(echo "$sNum+1" | bc)
    cNum=$(echo "$cNum+1" | bc)
    cellCounter=$(echo "$cellCounter+1" | bc)
}

# Copying the old input deck
cp SCRIPT.mcnp INP.mcnp

# Adding the HDPE front moderator
x=0.0
x=$(echo "scale=3; $x + $MODSPACE" | bc)
sNum=$sStart; cNum=$cStart; tNum=$tStart; sPrev='500'
mt=456; rho='0.93'
writeSurface; writeCell; incrementNum
# Adding Detector Layers
while [ $(echo "$x > $L0" | bc -l ) -eq 0 ]; do
    # Detecotor Layers
    i=0
    F2="F${tNum}2:n "
    F4="F${tNum}4:n "
    SD="SD${tNum}4 "
    while [ $i -lt $FILMLAYERS ] && [ $(echo "$x > $L0" | bc -l ) -eq 0 ]; do
        
        # Adding the Film
        x=$(echo "scale=3; $x + $XDET" | bc)
        if [ $(echo "$x > $L0" | bc -l ) -eq 1 ]; then  break;    # Exiting before writing
        else
            # Writing the detector
            mt=3; rho='1.1'
            writeSurface; writeCell; incTally; incrementNum

            # Adding the light guide / film spacing
            x=$(echo "scale=3; $x + $XMIN" | bc)
            if [ $(echo "$x > $L0" | bc -l ) -eq 1 ]; then  break; fi   # Exiting before writing
            # writing the light guide
            mt=10; rho='1.19'
            writeSurface; writeCell; incrementNum
        fi
        i=$(echo "$i+1" | bc)
    done
    # Write Tallies
    mt=3; writeTally

    # Spacing between detector layers
    x=$(echo "scale=3; $x + $XSPACE" | bc)
    if [ $(echo "$x > $L0" | bc -l ) -eq 1 ]; then  break; fi   # Exiting before writing
    # Writing more moderator
    mt=456; rho='0.93'
    writeSurface; writeCell; incrementNum
done
# Writing final cell
mt=456; rho='0.93'
sNum='501'; writeCell; incrementNum

# Updating the cell importances
cellCounter=$(echo "$cellCounter+4" | bc)
sed -i "s/NUMCELLS/$cellCounter/g" INP.mcnp

# Getting rid of '^M'
dos2unix INP.mcnp
