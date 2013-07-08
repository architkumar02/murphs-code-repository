 #!/bin/bash
 
echo "Creating the directory"
cp -r ~/G4/LXe GS20Calibration
 
echo "Removing the LXe tags"
cd GS20Calibration/
for file in src/*; do mv "${file}" "${file/LXe/}"; done
for file in include/*; do mv "${file}" "${file/LXe/}"; done

echo "Removing the CERN Dislaimer"
ls include/*.hh | xargs sed -i 's/LXe//g'
ls include/*.hh | xargs sed -i '1,25d'
ls src/*.cc | xargs sed -i 's/LXe//g'
ls src/*.cc | xargs sed -i '1,25d'
ls *.cc | xargs sed -i 's/LXe//g'
#ls *.cc | xargs sed -i '1,/#include/d'
ls *.cc | xargs sed -i '1,25d'

echo "Setting up the MakeFile"
mv LXe.cc GS20LightCalibration.cc
sed -i 's/LXe/GS20LightCalibration/g' GNUmakefile
