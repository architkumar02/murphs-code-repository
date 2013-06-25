#!/bin/bash
function JobSetup()
{
  echo '#!/bin/bash' >> job
  echo "#PBS -q gen1" > job
  echo "#PBS -V" >> job
  echo "#PBS -l nodes=1:ppn=2" >> job
  echo 'cd $PBS_O_WORKDIR' >> job
}

function runPointSrc()
{
  JobSetup
  cp Script_PointSrc.mcnp Co60Pt-INP_r-${r}_t-${t}_d-${d}.mcnp
  tTrans=$(echo "scale=5; $t+0.3175+0.00001" | bc) # adding on 1/8 " (arcylic disc)
  tmax=$(echo "scale=5; $t+0.00001" | bc)
  tDist=$(echo "scale=4; $d+0.189" | bc)           # Adding on thickness of cap
  sed -i "s/RDETECTOR/$r/g" Co60Pt-INP_r-${r}_t-${t}_d-${d}.mcnp
  sed -i "s/TDETECTOR/$t/g" Co60Pt-INP_r-${r}_t-${t}_d-${d}.mcnp
  sed -i "s/TMAXDETECTOR/$tmax/g" Co60Pt-INP_r-${r}_t-${t}_d-${d}.mcnp
  sed -i "s/TTRANS/$tTrans/g" Co60Pt-INP_r-${r}_t-${t}_d-${d}.mcnp
  sed -i "s/DISTANCE/$tDist/g" Co60Pt-INP_r-${r}_t-${t}_d-${d}.mcnp
  echo "mpirun mcnpx inp=Co60Pt-INP_r-${r}_t-${t}_d-${d}.mcnp name=out_${r}_${t}_${d}." >> job
  mv job sub.qsub
  qsub sub.qsub
}
distance=(1 0.5 0.25 0.125 0.0626)
radius=(1 1.27 1.905 2 2.5 2.5399)
thickness=(0.0025 0.005 0.01 0.015 0.03 0.1 0.2 0.5 1 2 )
for r in "${radius[@]}"
do
  for t in "${thickness[@]}"
  do
    for d in "${distance[@]}"
    do
    runPointSrc
    done
  done
done
