#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#include <string.h>

#include "helper/grafo.h"

int FLAG_DEBUG = 0;
int FLAG_OUTPUT = 0;

void algoritmoZP(nodo *nodi[], int N, int k, int val[]){
	int i;
	int j;
	int max;
	int min;
	for (i=1;i<=k;i++){
		if (FLAG_DEBUG) printf("k=%d\n",i);
		for (j=0;j<N;j++){
			//se il nodo è di tipo 0
			if (nodi[j]->tipo == 0){
				nodoSp *tmp = nodi[j]->succ;
				max = INT_MIN;
				//per ogni successore di j
				while (tmp != NULL){
					int calVal = val[tmp->n] + tmp->pesoArc;
					if (max < calVal)
						max=calVal;
					tmp = tmp->next;
				}
				nodi[j]->val = max;
			//se il nodo è di tipo 1
			}else{	
				nodoSp *tmp = nodi[j]->succ;
				min = INT_MAX;
				//per ogni successore di j
				while (tmp != NULL){
					int calVal = val[tmp->n] + tmp->pesoArc;
					if (min > calVal)
						min=calVal;
					tmp = tmp->next;
				}
				nodi[j]->val = min;
			}
			if(FLAG_DEBUG) printf("v = %d valk = %d\n", j, nodi[j]->val);	
		}

		//DATI FINALI
		for(j=0;j<N;j++){
			val[j] = nodi[j]->val;
		}
	}
}

int rand_range(int min_n, int max_n)
{
    return rand() % (max_n - min_n + 1) + min_n;
}

void shuffle(int *array, size_t n)
{
    if (n > 1) 
    {
        size_t i;
        for (i = 0; i < n - 1; i++) 
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          int t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}

int main(int argc, char *argv[]){
	int T = 0;
	int N;
	nodo **nodi = NULL;
	if(argc==2 && strcmp(argv[1],"-h")==0){
		printf("usage:\n - no parameter for default test graph\n -r [num_nodi] [seed_random] [numMax_archi] for create random graph\n -d for debug mode\n -i for print results\n");
		return 0;
	}
	if (argc>=2 && strstr(argv[1], "-")!=NULL && strstr(argv[1], "d")!=NULL){
		printf("DEBUG-mode.\n");
		FLAG_DEBUG = 1;
	}
	if (argc>=2 && strstr(argv[1], "-")!=NULL && strstr(argv[1], "i")!=NULL){
		printf("INFO-mode.\n");
		FLAG_OUTPUT = 1;
	}
	if (argc>=4 && strcmp(argv[1],"-r")==0){
		N = strtol(argv[2], NULL, 0);
		long seedRand = strtol(argv[3], NULL, 0);
		printf("paramentri N=%d Rand=%ld\n", N, seedRand);
		srand ( seedRand );
		
		nodi = malloc(sizeof(nodo*)*N);

		int j;
		for (j=0;j<N;j++){
			nodi[j]=newNodo(rand_range(0,1));
		}

		int maxNArchi = (argc==5) ? strtol(argv[4], NULL, 0) : 4;
		printf("maxArchi=%d\n", maxNArchi);
		int vArray[N];
		int r=0;
		for (j=0;j<N;j++){
			vArray[j]=j;
		}

		int t;
		int nArchi;
		for (j=0;j<N;j++){
			nArchi = rand_range(1,maxNArchi);
			shuffle(vArray,N);
			for (t=0;t<nArchi;t++)
				addSucc(j,vArray[t],rand_range(-10,10),nodi);
		}

		for (j=0;j<N;j++){
			printf("v%d tipo=%d ", j, nodi[j]->tipo);
			nodoSp *p = nodi[j]->succ;
			while (p!=NULL){
				printf(" -%d-> %d  ", p->pesoArc, p->n);
				p=p->next;
			}
			printf("\n");
		}
	}else{
		printf("DEFAULT-mode! -h for help\n");
		N=5;	
		nodi = malloc(sizeof(nodo*)*N);
		//costruzione Grafo
		//GRAFO1 
		nodi[0]=newNodo(1);
		nodi[1]=newNodo(0);
		nodi[2]=newNodo(1);
		nodi[3]=newNodo(0);
		nodi[4]=newNodo(0);
		addSucc(0,1,3,nodi);
		addSucc(3,0,2,nodi);
		addSucc(3,1,1,nodi);
		addSucc(1,3,-3,nodi);
		addSucc(1,4,1,nodi);
		addSucc(2,1,1,nodi);
		addSucc(2,4,0,nodi);
		addSucc(4,2,-4,nodi);
		/*
		//GRAFO2
		nodi[0]=newNodo(0);
		nodi[1]=newNodo(0);
		nodi[2]=newNodo(1);
		nodi[3]=newNodo(1);
		nodi[4]=newNodo(1);
		nodi[5]=newNodo(0);
		addSucc(0,1,0,nodi,T);
		addSucc(0,3,1,nodi,T);
		addSucc(1,1,-2,nodi,T);
		addSucc(1,5,-1,nodi,T);
		addSucc(5,2,3,nodi,T);
		addSucc(2,5,0,nodi,T);
		addSucc(3,4,-1,nodi,T);
		addSucc(4,3,-1,nodi,T);
		addSucc(4,5,2,nodi,T);
		*/
	}
	int k = 4*N*N*(10);
	
	int i;
	int val[N];
	for(i=0;i<N;i++){
		val[i] = 0;
	}
	printf("start!\n");
	
	clock_t begin = clock();
	algoritmoZP(nodi, N, k, val);
	clock_t end = clock();

	if(FLAG_OUTPUT){
		printf("\nDATI FINALI\n");
		double r = (double)1/N;
		printf("r = %lf - k = %d\n", r, k);
		for(i=0;i<N;i++){
			printf("%d (%lf, %lf) \n", i, (double)val[i]/k -r, (double)val[i]/k +r);
		}
	}
	
	printf("time %f second\n", (double)(end - begin) / CLOCKS_PER_SEC);

}