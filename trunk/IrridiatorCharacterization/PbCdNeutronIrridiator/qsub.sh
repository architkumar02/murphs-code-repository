#!/bin/bash

# running MCNP by submitting jobs to TORQUE/Maui
#PBS -q gen1
#PBS -V
#PBS -l nodes=2:ppn=2
cd $PBS_O_WORKDIR
mpirun mcnpx inp=Miller_Config_GS20_PbCdGamma.mcnp name=MillerConfig_GS20_PbCdGamma.

