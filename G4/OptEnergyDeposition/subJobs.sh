#!/bin/bash
# Submits the neturon and gamma runs seperately
# This is so we don't wait for one to finish before the other
export G4WORKDIR=/home/murffer/G4/OptEnergyDeposition
export PBS_O_WORKDIR=$G4WORKDIR
echo "Submitting Gamma Run"
qsub gSub.qsub
echo "Submitting Neturon Run"
qsub nSub.qsub


