#!/bin/bash  
echo "This is a shell script to write data"  
rm -rf data.txt
echo "delete data sample"
g++ -o file writedata.cpp 
echo "Run program"  
./file

echo "Done..."
rm -rf file
echo "Deleted unsed file"
