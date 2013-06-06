#!/bin/bash
function JobSetup()
{
  echo "#PBS -q gen2" > job
  echo "#PBS -V" >> job
  echo "#PBS -l nodes=1:ppn=4" >> job
  echo "export PBS_O_WORKDIR=$G4WORKDIR" >> job
  echo 'cd $PBS_O_WORKDIR' >> job
}

function runGamma()
{
  JobSetup
  echo "exec $G4WORKDIR/bin/Linux-g++/polymerFilm $G4WORKDIR/macros/gRunIEEE.mac > GammaOutputIEEE.txt" >> job
  mv job gSub.qsub
  qsub gSub.qsub
  rm gSub.qsub
}
function runNeutron()
{
  JobSetup
  echo "exec $G4WORKDIR/bin/Linux-g++/polymerFilm $G4WORKDIR/macros/nRunIEEE.mac > NeutronOutputIEEE.txt" >> job
  mv job nSub.qsub
  qsub nSub.qsub
  rm nSub.qsub
}

# Running the jobs
if [[ $G4WORKDIR && ${G4WORKDIR-_} ]]
then
  runGamma
  #runNeutron
else
  echo "G4WORKDIR is not defined"
  exit
fi

