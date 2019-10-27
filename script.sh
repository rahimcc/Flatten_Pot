#!/bin/bash
#gcc shifting.c 
#a=($(ls  AzCroppedPot*.bmp))
#for ((i=0; i<${#a[@]}-1; i+=1)); do
#  ./a.out ${a[i]}  ${a[i+1]}
#done

gcc resize.c
a=($(ls  AzCroppedPot*.bmp))
b=($(ls  Pot*.txt))
for ((i=0; i<${#a[@]}; i+=1)); do
 ./a.out ${a[i]}  ${b[i]}
done

gcc extract.c 

for j in resized*.bmp; do 
./a.out $j 480
done

gcc stitch.c
./a.out resizedAzCroppedPot*_S480.bmp 
