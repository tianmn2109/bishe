#!/bin/bash
echo "starting the process"

echo "Produce random data"
cd ./produceData
echo "Entry directory " $PWD
./produce.sh
echo "Leaving directory" $PWD
cd ..

echo "Runing the algorithm"
cd ./algorithm
echo "Entry directory " $PWD
./exec.sh
echo "Leaving directory" $PWD
cd ..

echo "Analise the result"
cd ./result
echo "Entry directory " $PWD
./analy.sh
echo "Leaving directory" $PWD
cd ..
echo "Total Finishing "
