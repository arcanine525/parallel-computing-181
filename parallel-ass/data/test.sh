#!/bin/bash  
echo "This is a shell script to test data"  
# rm -rf data.txt
# echo "delete data sample"
echo "Compiling...."
g++ -std=c++11 apriori.cpp -o main
# g++ -o file writedata.cpp 
echo "Run program"  
./main data.txt 0.03 0.5 a

echo "Done..."
rm -rf main
echo "Deleted unsed file"

