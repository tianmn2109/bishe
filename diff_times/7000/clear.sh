#!/bin/bash
echo "Removing files in examples"
cd ./examples
	rm -rf *
cd ..

echo "Removing algorithm results"
cd ./algorithm

for dir in `ls -F | grep "/$"`
do
	rm -rf $PWD/$dir/result
done

cd ..

echo "Removing previous result data"
cd ./result

for dir in 'result_analysis'
do
	rm -rf $PWD/$dir
done
	
cd ..
