#!/bin/bash

FILES=Output/OUT_*.o

# Column Headers
echo "Dist       F12 Dose              F1 In               F1 Out"
for f in $FILES
do
	# echo "Processing $f file"
	bf=$(basename $f .o)	
	# Parising file for detector thickness from title
	d=$(echo $bf | sed 's/[_]/ /g' |awk '{print $3}')

	# Parsing File For Dose Rate
	temp=$(cat $f | sed -n "/1tally  12/,/1analysis/p" | grep -A 1 '(500.2<600)')
	f12=$(echo $temp  | awk '{print $3, $4}')

	# Parsing Output for Number Crossing Detector	
	temp=$(cat $f | sed -n "/1tally   1/,/1analysis/p" | grep -A 5 'surface union total')
	f1Out=$(echo $temp | awk '{print $9,$10}')	
	f1In=$(echo $temp | awk '{print $19,$20}')	
	
	# Printing Output
	echo "$d |   $f12  |   $f1In  |    $f1Out"
done

