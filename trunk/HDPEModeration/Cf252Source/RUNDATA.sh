#!/bin/bash

# Preparing
rm -rf nohup.out out* comou* runtp* mcta* INP.mcnp
export PBS_O_WORKDIR=$(pwd)
# Radius
radius=(0.0001 2.5 5 7.5 10 12.5 15 17.5 20)
for r in "${radius[@]}"
do

    # Setting the radius of the HDPE
    cat SCRIPT.mcnp | sed "s/RHDPE/$r/g" > INP_${r}.mcnp
    cat queueRunScript.sh | sed "s/RADIUS/$r/g" > queueRun.sh
    # Output
    echo "Starting radius $r"
    echo ""
    
    # running MCNP by submitting jobs to TORQUE/Maui
    qsub queueRun.sh 

# Cleanup
   # rm INP.mcnp

done

