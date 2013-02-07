#!/bin/bash

# Preparing
rm -rf nohup.out out* comou* runtp* mcta* INP.mcnp
export PBS_O_WORKDIR=$(pwd)

# Radius
radius=(0.5 1 2 4 8 16 32)
energy=(1E-8 1E-6 1E-3 1 10)
for r in "${radius[@]}"
do

    for e in "${energy[@]}"
    do
    # Setting the radius of the HDPE
    cat SCRIPT.mcnp > INP_${r}_cm_${e}_MeV.mcnp
    sed -i "s/RHDPE/$r/g" INP_${r}_cm_${e}_MeV.mcnp 
    sed -i "s/ENERGY/$e/g" INP_${r}_cm_${e}_MeV.mcnp 
    cat queueRunScript.sh > queueRun.sh
    sed -i "s/RADIUS/$r/g" queueRun.sh
    sed -i "s/ENERGY/$e/g" queueRun.sh
    # Output
    echo "Starting radius $r with energy $e"
    echo ""
    
    # running MCNP by submitting jobs to TORQUE/Maui
    qsub queueRun.sh 
    done

done

