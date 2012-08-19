#!/bin/bash

#FILES=Outputs/NatU_R_*.o
FILES=Outputs/HEU_R_*.o
echo "Radius Ref k   kErr"
for f in $FILES
do
	name=${f%\.*}
	radius=$(echo $name | sed 's/[-_]/ /g' | awk '{print $3}')
	ref=$(echo $name | sed 's/[-_]/ /g' | awk '{print $5}')
	line=$(cat $f | grep 'final result')
	k=$(echo $line | awk '{print $3}')
	kErr=$(echo $line | awk '{print $4}')
	
	echo "$radius $ref $k $kErr"
done
