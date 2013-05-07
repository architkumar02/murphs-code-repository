#!/bin/bash
function JobSetup()
{
  echo '#!/bin/bash' >> job
  echo "#PBS -q gen1" > job
  echo "#PBS -V" >> job
  echo "#PBS -l nodes=2:ppn=2" >> job
  echo 'cd $PBS_O_WORKDIR' >> job
}

function runMillerNeutron()
{
  JobSetup
  cp Miller_Config_GS20_SelfSheilding_Script.mcnp INP_$MT.mcnp
  sed -i "s/MTNUM/$MT/g" INP_$MT.mcnp
  echo "mpirun mcnpx inp=INP_$MT.mcnp name=out_$MT." >> job
  mv job nSub_$MT.qsub
  qsub nSub_$MT.qsub
}

MTNUM=(300 301 302 303 304)
for MT in "${MTNUM[@]}"
do
  echo "Material: $MT"    
  runMillerNeutron
done
