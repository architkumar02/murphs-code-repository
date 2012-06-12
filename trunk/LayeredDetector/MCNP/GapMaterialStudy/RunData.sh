#!/bin/bash

# ScriptFile
FILE='SCRIPT_GapMaterialStudyLayer_PS.mcnp'

# Radius
xposlog=(-1 -2 -4 -8 -16 -32 -64 -128)
for x in ${xposlog[@]}
do

# Procesing file
    echo "Processing x position: $x" 
    cat $FILE | sed "s/GAMMAXPOS/$x/g"  > INP.mcnp

    nohup mpirun -np 32 -machinefile NodeList /opt/MCNPX-2.60/bin/mcnpx ixr inp=INP.mcnp name=GammaFlux_SrcPosX_${x}.

done
