#!/bin/bash

# ScriptFile
#FILE='ScriptLEUCritSize.mcnp'
#FILE='ScriptHEUCritSize.mcnp'
FILE='ScriptNatUCritSize.mcnp'

# Radius
radius=(0.5 1 5 10 15 20 25 50 75 100 125)
for r in ${radius[@]}
do

# Procesing file
    echo "Processing Source Radius: $r" 
# Parsing the File for Radius
    cat $FILE | sed "s/RADIUS/$r/g"  > INP.mcnp

    nohup mpirun -np 14 -machinefile NodeList2.txt /opt/MCNPX-2.60/bin/mcnpx ixr inp=INP.mcnp name=Nat_R_${r}.

done
