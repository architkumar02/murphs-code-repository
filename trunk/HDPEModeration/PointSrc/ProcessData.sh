#!/bin/bash

FILES=*.o
echo "r e tally total"
for f in $FILES
do
    bf=$(basename $f .o)	

    # Parsing file for HDPE Radius
    r=$(echo $bf | sed 's/[_]/ /g' |awk '{print $2}')
    e=$(echo $bf | sed 's/[_]/ /g' |awk '{print $4}')
    

    # Getting the fraction of thermal neutrons 
    F1=$(cat $f | sed -n "/1tally  21/,/5.0000E-07/p" | tail -1 | awk '{print $2 " " $3}' ) 
    F1total=$(cat $f | sed -n "/1tally  21/,/total/p" | tail -1 | awk '{print $2 " " $3}')
    
    echo "$r $e $F1 $F1total"

# Cleanup
done

