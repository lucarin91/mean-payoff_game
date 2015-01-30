#!/bin/bash
echo "start testVertici"

echo "" > outZ_Vertici
echo "" > out1_Vertici
for ((i=5; i<=200; i=i+5 )) ; 
do
	echo "input N=$i"
	for ((j=1; j<=3; j++));
	do
		r=$RANDOM 
		echo "rand=$r"
		echo -e "iterazione $j" >> outZ_Vertici
		echo -e "iterazione $j" >> out1_Vertici
		../alg-zp -r $i $r >> outZ_Vertici
		../alg-1 -r $i $r >> out1_Vertici
	done
	echo -e "\n\n" >> outZ_Vertici
	echo -e "\n\n" >> out1_Vertici  
done