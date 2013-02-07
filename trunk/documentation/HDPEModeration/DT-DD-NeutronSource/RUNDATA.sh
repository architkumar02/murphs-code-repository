#!/bin/bash

# Preparing
rm -rf nohup.out out* comou* runtp* mcta* INP.mcnp

# Radius
#radius=(20 30 40 50 60)
#radius=(70 80 90 100)
radius=(100 110 120 130)
energy=(14.1)
RDETECTOR=300
for r in "${radius[@]}"
do
    for e in ${energy[@]}
    do

    # Setting the energy
    cat SCRIPT.mcnp | sed "s/ENERGY/$e/g" > INP.mcnp

    # Setting the radius of the HDPE and detecotr
     sed -i "s/RHDPE/$r/g" INP.mcnp
     sed -i "s/RDETECTOR/$RDETECTOR/g"  INP.mcnp


    # Output
    echo "Starting radius $r"
    echo "     The energy is $e"
    echo ""
    
    # running MCNP
    nohup mcnp5.mpi TASKS 8 inp=INP.mcnp name=E_${e}_MeV_${r}_cm.
# Cleanup
   # rm INP.mcnp

   done
done

