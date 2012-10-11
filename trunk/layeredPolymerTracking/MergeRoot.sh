#!/usr/bin/bash

echo "Merging Histograms with HADD"
echo "  Ensure the fileName is correct"

  
hadd cs137_25um.root output/run_0_node*
hadd cs137_50um.root output/run_1_node*
hadd cs137_100um.root output/run_2_node*
hadd cs137_150um.root output/run_3_node*
hadd cs137_300um.root output/run_4_node*
hadd cs137_600um.root output/run_5_node*
hadd cs137_1mm.root output/run_6_node*
hadd cs137_5mm.root output/run_7_node*
hadd cs137_1cm.root output/run_8_node*


#time mpiexec -np 80 -machinefile NodeList bin/Linux-g++/mpiPolymerFilm run1.mac > Results_Run1.txt
hadd 25um_PSDetector_Co60.root output/run_0_node* output/run_1_node*
hadd 50um_PSDetector_Co60.root output/run_3_node* output/run_4_node*
hadd 100um_PSDetector_Co60.root output/run_6_node* output/run_7_node*
hadd 1mm_PSDetector_Co60.root output/run_9_node* output/run_10_node*
hadd 1cm_PSDetector_Co60.root output/run_12_node* output/run_13_node*
hadd 25um_PSDetector_neutron.root output/run_2_node*
hadd 50um_PSDetector_neutron.root output/run_5_node*
hadd 100um_PSDetector_neutron.root output/run_8_node*
hadd 1mm_PSDetector_neutron.root output/run_11_node*
hadd 1cm_PSDetector_neutron.root output/run_14_node*
