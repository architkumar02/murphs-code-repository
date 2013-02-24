#!/bin/bash

# running MCNP by submitting jobs to TORQUE/Maui
#PBS -q gen3
#PBS -V
#PBS -l nodes=4:ppn=4
cd $PBS_O_WORKDIR
RUNCOMMAND

