#!/bin/bash  
echo "This is a shell script to write data"  
gcc -o file code.c && ./file
echo "Run program"  


echo "Done..."
rm -rf file
echo "Deleted unsed file"
