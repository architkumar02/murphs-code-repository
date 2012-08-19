#!/bin/bash

#FILES=Outputs/HEU_*.o
#FILES=Outputs/LEU_*.o
FILES=Outputs/Nat_*.o
echo "Radius k   kErr"
for f in $FILES
do
	name=${f%\.*}
	radius=$(echo $name | sed 's/[-_]/ /g' | awk '{print $3}')
	line=$(cat $f | grep 'final result')
	k=$(echo $line | awk '{print $3}')
	kErr=$(echo $line | awk '{print $4}')
	
	echo "$radius $k $kErr"
done
