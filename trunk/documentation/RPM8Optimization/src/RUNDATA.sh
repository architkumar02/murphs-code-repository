#!/bin/bash

# Preparing
rm -rf nohup.out out* comou* runtp* mcta* INP.mcnp
export PBS_O_WORKDIR=$(pwd)

# Radius
HDPE='2.5'
numLayers=(1 2 3 4 5 6)
assemblySpace=(1 2 3 4)
for n in "${numLayers[@]}"
do

    for s in "${assemblySpace[@]}"
    do
    # Output
    echo "Starting $n layers spaced by $s cm with front moderator $HDPE"
    echo ""
    # Setting up the input deck
    python createSurfaceCells.py --modSpace $HDPE --filmLayers $n --assemblySpace $s
    python mergeInputDecks.py
    mv INP.mcnp INP_${HDPE}_cm_${n}_asm_${s}_cm.mcnp
    cat queueRunScript.sh > queueRun.sh
    sed -i "s/HDPE/$HDPE/g" queueRun.sh
    sed -i "s/NUMASSEM/$n/g" queueRun.sh
    sed -i "s/SPACE/$s/g" queueRun.sh

    # Removing files
    rm cells.txt surfaces.txt tallies.txt 
    # Error checking input deck
    #mcnpx i inp=INP_${HDPE}_cm_${NUMASSEM}_asm_${SPACE}_cm.mcnp | grep "error"
    # running MCNP by submitting jobs to TORQUE/Maui
    qsub queueRun.sh 
    done

done

