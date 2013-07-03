#!/bin/bash
function JobSetup()
{
  echo '#!/bin/bash' >> job
  echo "#PBS -q gen2" > job
  echo "#PBS -V" >> job
  echo "#PBS -l nodes=6:ppn=4" >> job
  echo 'cd $PBS_O_WORKDIR' >> job
}

function runGS20()
{
  JobSetup
  echo "mpirun mcnpx inp=GS20.mcnp name=NIrridiator_GS20." >> job
  mv job sub.qsub
  qsub sub.qsub
}

# Running the scripts
runGS20
