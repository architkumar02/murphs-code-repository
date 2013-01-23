#!/bin/bash

FILES=*.o
echo "r energy tally total fraction"
for f in $FILES
do
    bf=$(basename $f .o)	

    # Parsing file for HDPE Radius
    r=$(echo $bf | sed 's/[-_L]/ /g' |awk '{print $2}')
    

    # Getting the fraction of thermal neutrons 
    F1=$(cat $f | sed -n "/1tally  21/,/1.0000E-07/p" | tail -1 | awk '{print $2 " " $3}' ) 
    F1total=$(cat $f | sed -n "/1tally  21/,/total/p" | tail -1 | awk '{print $2 " " $3}')
    
    echo "$r $F1 $F1total $F2 $F2total"

# Cleanup
done

