#!/bin/bash

FILES=Outputs/*GammaFlux_SrcPosX_*.o
echo "Pos F212 (Det. Encasing)  F212 (First Film)"
for f in $FILES
do
    name=${f%\.*}
	xpos=$(echo $name | sed 's/[_]/ /g' | awk '{print $3}')
	
    # Parsing File For Dose Rate
	temp=$(cat $f | sed -n "/1tally 212/,/total/p" | grep total )
	f212Film=$(echo $temp  | awk '{print $2, $3}')
	
	# Parsing File For Dose Rate
	temp=$(cat $f | sed -n "/1tally 212/,/1analysis/p" | grep total | tail -1)
	f212Det=$(echo $temp  | awk '{print $2, $3}')
	
    echo "$xpos $f212Det   $f212Film"
done
