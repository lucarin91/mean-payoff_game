#!/bin/bash
echo "start testArchi"

echo "" > outZ_Archi
echo "" > out1_Archi
for ((i=1; i<=50; i++ )) ; 
do
	echo "input N=$i"
	for ((j=1; j<=3; j++));
	do
		r=$RANDOM 
		echo "rand=$r"
		echo -e "iterazione $j" >> outZ_Archi
		echo -e "iterazione $j" >> out1_Archi
		../alg-zp -r 50 $r $i >> outZ_Archi
		../alg-1 -r 50 $r $i >> out1_Archi
	done
	echo -e "\n\n" >> outZ_Archi
	echo -e "\n\n" >> out1_Archi  
done