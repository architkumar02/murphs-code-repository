#!/usr/bin/bash

echo "Merging Histograms with HADD"
echo "Comand Line argument is: $1"

if [ $1 = "cs137" ]
then
    hadd cs137_25um.root output/run_0_node*
    hadd cs137_50um.root output/run_1_node*
    hadd cs137_100um.root output/run_2_node*
    hadd cs137_150um.root output/run_3_node*
    hadd cs137_300um.root output/run_4_node*
    hadd cs137_600um.root output/run_5_node*
    hadd cs137_1mm.root output/run_6_node*
    hadd cs137_5mm.root output/run_7_node*
    hadd cs137_1cm.root output/run_8_node*
fi

if [ $1 = "run1" ]
then
   
    echo "Merging data from run1.mac"

    #time mpiexec -np 80 -machinefile NodeList bin/Linux-g++/mpiPolymerFilm run1.mac > Results_Run1.txt
    hadd 15um_PSDetector_Co60.root output/run_0_node* output/run_1_node*
    hadd 15um_PSDetector_neutron.root output/run_2_node*
    
    hadd 25um_PSDetector_Co60.root output/run_3_node* output/run_4_node*
    hadd 25um_PSDetector_neutron.root output/run_5_node*
    
    hadd 50um_PSDetector_Co60.root output/run_6_node* output/run_7_node*
    hadd 50um_PSDetector_neutron.root output/run_8_node*

    hadd 150um_PSDetector_Co60.root output/run_9_node* output/run_10_node*
    hadd 150um_PSDetector_neutron.root output/run_11_node*
    
    hadd 300um_PSDetector_Co60.root output/run_12_node* output/run_13_node*
    hadd 300um_PSDetector_neutron.root output/run_14_node*
    
    hadd 600um_PSDetector_Co60.root output/run_15_node* output/run_16_node*
    hadd 600um_PSDetector_neutron.root output/run_17_node*
    
    hadd 1mm_PSDetector_Co60.root output/run_18_node* output/run_19_node*
    hadd 1mm_PSDetector_neutron.root output/run_20_node*
    
    hadd 1cm_PSDetector_Co60.root output/run_21_node* output/run_22_node*
    hadd 1cm_PSDetector_neutron.root output/run_23_node*
    
    hadd 2cm_PSDetector_Co60.root output/run_24_node* output/run_25_node*
    hadd 2cm_PSDetector_neutron.root output/run_26_node*
    
    hadd 5cm_PSDetector_Co60.root output/run_27_node* output/run_28_node*
    hadd 5cm_PSDetector_neutron.root output/run_29_node*
   
elif [ $1 = "run1_neutron" ] 
then
    rm *_PSDetector_neutron.root
    hadd 15um_PSDetector_neutron.root output/run_0_node*
    hadd 25um_PSDetector_neutron.root output/run_1_node*
    hadd 50um_PSDetector_neutron.root output/run_2_node*
    hadd 150um_PSDetector_neutron.root output/run_3_node*
    hadd 300um_PSDetector_neutron.root output/run_4_node*
    hadd 600um_PSDetector_neutron.root output/run_5_node*
    hadd 1mm_PSDetector_neutron.root output/run_6_node*
    hadd 1cm_PSDetector_neutron.root output/run_7_node*
    hadd 2cm_PSDetector_neutron.root output/run_8_node*
    hadd 5cm_PSDetector_neutron.root output/run_9_node*
else
    echo "$1 is not an option.  Options are run1 and run1_neutron."
fi
