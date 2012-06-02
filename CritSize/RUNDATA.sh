#!/bin/bash

# ScriptFile
FILE='ScriptLEUCritSizeReflector.mcnp'
#FILE='ScriptNatUCritSizeReflector.mcnp'
#FILE='ScriptLEUCritSize.mcnp'
#FILE='ScriptHEUCritSize.mcnp'
#FILE='ScriptNatUCritSize.mcnp'

# Radius
radius=(5 10 15 20 25 50 75 100 125)
thickness=(5 10 15 20)
for r in ${radius[@]}
do
	
	for t in ${thickness[@]}
	do
		# Procesing file
		echo "Processing Source Radius: $r Reflector Thickness $t"
		totalRadius=$(echo "scale=2; $r + $t" | bc)
		# Parsing the File for Radius
		cat $FILE | sed "s/SRCRADIUS/$r/g" | sed "s/RADIUS/$totalRadius/g"  > INP.mcnp
		#sed "s/RADIUS/$r/g" < $FILE > "INP_NatU_Radius_${r}.mcnp"

		nohup mpirun -np 14 -machinefile NodeList2.txt /opt/MCNPX-2.60/bin/mcnpx ixr inp=INP.mcnp name=LEU_R_${r}_Ref_${t}.
		#nohup mcnp5 ixr inp=INP_NatU_Radius_${r}.mcnp name=NatU_Radius_${r}.
	done
done
