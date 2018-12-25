#!/bin/bash  
echo "This is a shell script to test program"  
make
echo "Run program"  
./main $1 $2

echo "Done..."
make clean
echo "Deleted unsed file"
