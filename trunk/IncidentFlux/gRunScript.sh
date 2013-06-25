#!/bin/bash

# running MCNP by submitting jobs to TORQUE/Maui
#PBS -q vgen
#PBS -V
#PBS -l nodes=1:ppn=3
cd $PBS_O_WORKDIR
mpirun mcnpx inp=Co60-INP_r-RDETECTOR_cm_t-TDETECTOR_cm.mcnp name=Co60Irridiator_r-RDETECTOR_cm_t-TDETECTOR_cm.

