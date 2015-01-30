#folder algoritmo1
falg1 = algoritmo1/
#folder zwik-paterson
fzp = zwickPaterson/
#folder algoritmo-selfloop
falgsl = algoritmo1-selfloop/

all: alg1 azp algsl

alg1:
	gcc $(falg1)/algoritmo.c -o alg-1

azp:
	gcc $(fzp)/zwik-paterson.c $(fzp)/helper/grafo.c -o alg-zp 

algsl:
	gcc $(falgsl)/algoritmo-new.c -o alg-sl
