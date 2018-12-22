#!/bin/bash  
echo "This is a shell script to count data"  
gcc -o count count.c 
echo "Run program"  
./count

echo "Done..."
rm -rf count
echo "Deleted unsed file"
