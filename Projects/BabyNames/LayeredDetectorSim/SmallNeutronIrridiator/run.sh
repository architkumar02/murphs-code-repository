#!/bin/bash
function JobSetup()
{
  echo '#!/bin/bash' >> job
  echo "#PBS -q gen1" > job
  echo "#PBS -V" >> job
  echo "#PBS -l nodes=3:ppn=2" >> job
  echo 'cd $PBS_O_WORKDIR' >> job
}

function runMCNPEJ426()
{
  JobSetup
  echo "mpirun mcnpx inp=NeutronIrridiator_EJ426.mcnp name=NIrridiator_EJ426." >> job
  mv job sub.qsub
  qsub sub.qsub
}

function runMCNPPS()
{
  JobSetup
  echo "mpirun mcnpx inp=NeutronIrridiator_PS.mcnp name=NIrridiator_PS." >> job
  mv job sub.qsub
  qsub sub.qsub
}
# Running the scripts
runMCNPEJ426
