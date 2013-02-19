#!/bin/bash

# running MCNP by submitting jobs to TORQUE/Maui
#PBS -q gen3
#PBS -V
#PBS -l nodes=3:ppn=8
cd $PBS_O_WORKDIR
mpirun mcnpx inp=INP_HDPE_cm_NUMASSEM_asm_SPACE_cm.mcnp name=DHSDNDO_PEN_HDPE_cm_NUMASSEM_asm_SPACE_cm.

