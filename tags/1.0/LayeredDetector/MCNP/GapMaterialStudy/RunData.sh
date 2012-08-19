#!/bin/bash

# ScriptFile
FILE='SCRIPT_GapMaterialStudyLayer_PS.mcnp'

# Radius
xposlog=(-2 -4 -8 -16 -32 -64)
for x in ${xposlog[@]}
do

# Procesing file
    echo "Processing x position: $x" 
    cat $FILE | sed "s/GAMMAXPOS/$x/g"  > INP.mcnp

    nohup /opt/MCNPX-2.60/bin/mcnpx ixr inp=INP.mcnp name=GammaFlux_SrcPos_${x}.

done
