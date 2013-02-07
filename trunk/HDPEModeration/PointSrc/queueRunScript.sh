#!/bin/bash

# running MCNP by submitting jobs to TORQUE/Maui
#PBS -q gen3
#PBS -V
#PBS -l nodes=2:ppn=8
cd $PBS_O_WORKDIR
mpirun mcnpx inp=INP_RADIUS_cm_ENERGY_MeV.mcnp name=PointSrcMod_RADIUS_cm_ENERGY_MeV.

