#!/bin/bash

# running MCNP by submitting jobs to TORQUE/Maui
#PBS -q gen2
#PBS -V
#PBS -l nodes=1:ppn=4
cd $PBS_O_WORKDIR
mpirun mcnpx inp=INP_r-RDETECTOR_cm_t-TDETECTOR_cm.mcnp name=Miller-Config_r-RDETECTOR_cm_t-TDETECTOR_cm.

