#!/bin/bash

# Checking input arguments
if (( $# != 2 ))
then
  echo "Usage: ./runSim.sh macroName.mac outputFile"
  exit 1
fi
if [[ -z $G4WORKDIR ]]
then
  echo "G4WORKDIR is not defined"
  exit
fi

# Now Running as a Torque/MAUI job
echo "Submitting the job to Torque / Maui"
echo "#PBS -q gen2" > job
echo "#PBS -V" >> job
echo "#PBS -l nodes=1:ppn=4" >> job
echo "#PBS -N job_$1" >> job
echo "export PBS_O_WORKDIR=$G4WORKDIR" >> job
echo 'cd $PBS_O_WORKDIR' >> job
echo "exec $G4WORKDIR/bin/Linux-g++/rangeSim $1 > $2" >> job
qsub job
rm job
