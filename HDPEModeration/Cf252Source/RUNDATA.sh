#!/bin/bash

# Preparing
rm -rf nohup.out out* comou* runtp* mcta* INP.mcnp
export PBS_O_WORKDIR=$(pwd)


# Radius
rPb=0.5
radius=(0.5 1 2.5 5 7.5 10 12.5 15 17.5 20 32 64)
for r in "${radius[@]}"
do

    # Setting the radius of the HDPE
    rNew=$(echo "scale=2; $rPb+$r;" | bc)
    cat SCRIPT.mcnp | sed "s/RHDPE/$rNew/g" > INP_${r}.mcnp
    cat queueRunScript.sh | sed "s/RADIUS/$r/g" > queueRun.sh
    # Output
    echo "Starting radius $r"
    echo ""
    
    # running MCNP by submitting jobs to TORQUE/Maui
    qsub queueRun.sh 

# Cleanup
   # rm INP.mcnp

done

