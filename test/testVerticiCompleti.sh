#!/bin/bash
echo "start testVerticiCompleti"

echo "" > outZ_VerticiCompleti
echo "" > out1_VerticiCompleti
echo "" > outsl_VerticiCompleti
for ((i=5; i<=200; i=i+5 )) ; 
do
	echo "input N=$i"
	for ((j=1; j<=10; j++));
	do
		r=$RANDOM 
		echo "rand=$r"
		echo -e "iterazione $j" >> outZ_VerticiCompleti
		echo -e "iterazione $j" >> out1_VerticiCompleti
		echo -e "iterazione $j" >> outsl_VerticiCompleti
		../alg-zp -r $i $r $i >> outZ_VerticiCompleti
		../alg-1 -r $i $r $i >> out1_VerticiCompleti
		../alg-sl -r $i $r $i >> outsl_VerticiCompleti
	done
	echo -e "\n\n" >> outZ_VerticiCompleti
	echo -e "\n\n" >> out1_VerticiCompleti
	echo -e "\n\n" >> outsl_VerticiCompleti  
done