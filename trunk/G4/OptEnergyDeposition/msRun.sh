#!/bin/bash
export NPS=10000
export MAT=MS1
function JobSetup()
{
  echo "#PBS -q gen1" > job
  echo "#PBS -V" >> job
  echo "#PBS -l nodes=1:ppn=2" >> job
  echo "export PBS_O_WORKDIR=$G4WORKDIR" >> job
  echo 'cd $PBS_O_WORKDIR' >> job
}

function runGamma()
{
  JobSetup
  cp MSSamplesMacros/gRun.mac gRun${NPS}_${MAT}.mac
  cat MSSamplesMacros/Thickness.mac >> gRun${NPS}_${MAT}.mac
  # Replacing the number of paritlces and detecotr materials
  sed -i "s/NUMPARTICLES/$NPS/g" gRun${NPS}_${MAT}.mac
  sed -i "s/DETECTORMAT/$MAT/g" gRun${NPS}_${MAT}.mac
  echo "exec $G4WORKDIR/bin/Linux-g++/polymerFilm $G4WORKDIR/gRun${NPS}_${MAT}.mac > MS_GammaOutput.txt" >> job
  mv job gSub.qsub
  qsub gSub.qsub
  #rm gRun${NPS}_${MAT}.mac
  rm gSub.qsub
}
function runNeutron()
{
  JobSetup
  cp MSSamplesMacros/nRun.mac nRun${NPS}_${MAT}.mac
  cat MSSamplesMacros/Thickness.mac >> nRun${NPS}_${MAT}.mac
  # Replacing the number of paritlces and detecotr materials
  sed -i "s/NUMPARTICLES/$NPS/g" nRun${NPS}_${MAT}.mac
  sed -i "s/DETECTORMAT/$MAT/g" nRun${NPS}_${MAT}.mac
  echo "exec $G4WORKDIR/bin/Linux-g++/polymerFilm $G4WORKDIR/nRun${NPS}_${MAT}.mac > MS_NeutronOutput.txt" >> job
  mv job nSub.qsub
  qsub nSub.qsub
  #rm nRun${NPS}_${MAT}.mac
  rm nSub.qsub
}

# Running the jobs
if [[ -n $G4WORKDIR ]]
then
  runGamma
  runNeutron
else
  echo "G4WORKDIR is not defined"
  exit
fi

