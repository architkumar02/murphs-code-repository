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
