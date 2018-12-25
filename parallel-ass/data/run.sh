#!/bin/bash  
echo "This is a shell script to write data"  
rm -rf data.txt
echo "delete data sample"
g++ -std=c++11 writedata.cpp -o write
echo "Run program"  
./write

echo "Done..."
rm -rf write
echo "Deleted unsed file"
