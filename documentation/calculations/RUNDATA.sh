#!/bin/bash

# Preparing
rm -rf nohup.out out* comou* runtp* mcta* INP.mcnp
export PBS_O_WORKDIR=$(pwd)

# Radius and Thickness (in cm)
radius=(1 1.27 1.905 2 2.5 2.5399)
thickness=(0.0025 0.005 0.01 0.015 0.03 0.1 0.2 0.5 1 2 )
for r in "${radius[@]}"
do

    for t in "${thickness[@]}"
    do
    # Computing TMAX
    tmax=$(echo "scale=5; $t+0.00001" | bc)
    
    # Setting the radius of the Detector
    cat Miller-Config_Script.mcnp > INP_r-${r}_cm_t-${t}_cm.mcnp
    sed -i "s/RDETECTOR/$r/g" INP_r-${r}_cm_t-${t}_cm.mcnp
    sed -i "s/TDETECTOR/$t/g" INP_r-${r}_cm_t-${t}_cm.mcnp
    sed -i "s/TMAXDETECTOR/$tmax/g" INP_r-${r}_cm_t-${t}_cm.mcnp
    cat nRunScript.sh > nQueueRun.sh
    sed -i "s/RDETECTOR/$r/g" nQueueRun.sh
    sed -i "s/TDETECTOR/$t/g" nQueueRun.sh
    
    cat Script_Irridiator.mcnp > Co60-INP_r-${r}_cm_t-${t}_cm.mcnp
    tTrans=$(echo "scale=5; $t+0.3175+0.00001" | bc)
    tmax=$(echo "scale=5; $t+0.00001" | bc)
    sed -i "s/RDETECTOR/$r/g" Co60-INP_r-${r}_cm_t-${t}_cm.mcnp
    sed -i "s/TDETECTOR/$t/g" Co60-INP_r-${r}_cm_t-${t}_cm.mcnp
    sed -i "s/TMAXDETECTOR/$tmax/g" Co60-INP_r-${r}_cm_t-${t}_cm.mcnp
    sed -i "s/TTRANS/$tTrans/g" Co60-INP_r-${r}_cm_t-${t}_cm.mcnp
   
    cat gRunScript.sh > gQueueRun.sh
    sed -i "s/RDETECTOR/$r/g" gQueueRun.sh
    sed -i "s/TDETECTOR/$t/g" gQueueRun.sh
   
    # Output
    echo "Starting radius $r with thickness $t"
    
    # running MCNP by submitting jobs to TORQUE/Maui
    qsub gQueueRun.sh
    qsub nQueueRun.sh
    done

done

