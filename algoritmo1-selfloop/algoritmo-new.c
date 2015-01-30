#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#include <string.h>

typedef struct nodoSp{
	struct nodoSp *next;
	int n;
	int pesoArc;
}nodoSp;

typedef struct lista{
	struct nodoL *first;
}lista;

typedef struct nodoL{
	struct nodoL *next;
	int n;
}nodoL;

typedef struct nodo{
	int tipo;
	nodoSp *succ;
	nodoSp *prec;
	int count;
	int f;
	int inLista;
}nodo;


void insLista(lista *l, int n, nodo *nodi[]){
	if (!nodi[n]->inLista){
		nodoL *new = malloc(sizeof(struct nodoL));
		new->n=n;
		new->next=l->first;
		l->first=new;
		nodi[n]->inLista=1;
	}
}

void removeFirstLista(lista *l, nodo *nodi[]){
	nodoL *tmp=l->first;
	if (tmp!=NULL){
		nodi[tmp->n]->inLista=0;
		l->first=tmp->next;
		free(tmp); //non sono sicuro se in questo modo vado a eliminare il nodoL oppure solo il puntatore
		//grandi lacune nei puntatori di c
	}
}

nodo *newNodo(int tipo){
	nodo *n = malloc(sizeof(nodo));
	n->tipo=tipo;
	n->count=0;
	n->f=0;
	n->succ=NULL;
	n->prec=NULL;
	n->inLista=0;
	return n;
}

nodoSp *newNodoSp(int n, int pesoArc, nodoSp *next){
	nodoSp *new = malloc(sizeof(nodoSp));
	new->next=next;
	new->pesoArc=pesoArc;
	new->n=n;
	return new;
}

void addSucc (int n1, int n2, int pesoArc, nodo *nodi[], int T){	
	nodoSp *newSucc = newNodoSp(n2,pesoArc-T,nodi[n1]->succ);
	nodi[n1]->succ=newSucc;
	
	nodoSp *newPrec = newNodoSp(n1,pesoArc-T,nodi[n2]->prec);
	nodi[n2]->prec=newPrec;
}

void inizializzazioneLista(lista *l, nodo *nodi[], int N){
	int i;
	nodoSp *tmp;
	int condizione;
	for (i=0;i<N;i++){
		tmp = nodi[i]->succ;
		//nodo di tipo 0
		if (nodi[i]->tipo==0){
			condizione=0;
			//tutti i nodi
			while (tmp!=NULL && !condizione){
				if (tmp->pesoArc>=0)
					condizione=1;
				tmp=tmp->next;
			}
		//nodo di tipo 1
		}else{
			condizione=1;
			//almeno un nodo
			while (tmp!=NULL && condizione){
				if (tmp->pesoArc<0)
					condizione=0;
				tmp=tmp->next;
			}
		}
		if (!condizione)
			insLista(l,i, nodi);
	}
	
}

void aggCount(int n, nodo *nodi[]){
	nodoSp *succ=nodi[n]->succ;
	int conto=0;
	//conto quanti successori verificano la proprietà f>=f-w
	while (succ!=NULL){
		if ((nodi[n]->f) >= (nodi[succ->n]->f) - (succ->pesoArc))
			conto++;
		succ=succ->next;
	}
	//aggiorno count con il numero di successori che verificano la proprietà
	nodi[n]->count=conto;
}

void inizializzazioneCount(nodo *nodi[], int N){
	int i;
	for (i=0;i<N;i++){
		//nodi di tipo 0
		if (!nodi[i]->tipo){
			//se sono presenti in L
			if (nodi[i]->inLista)
				nodi[i]->count=0;
			//se non sono present in L
			else
				aggCount(i, nodi);
		}
	}
}


int calcoloMg (nodo *nodi[], int N){
	int i;
	int somma=0;
	nodoSp *succ;
	//per ogni arco
	for (i=0;i<N;i++){
		succ=nodi[i]->succ;
		while (succ!=NULL){
			if (succ->pesoArc<0)
				somma=somma+succ->pesoArc;
			succ=succ->next;
		}	
	}
	return abs(somma);
}

int FLAG_DEBUG = 0;
int FLAG_OUTPUT = 0;

void algoritmo1(nodo *nodi[], int N, int f[]){
	//DEBUG			
	if(FLAG_DEBUG) printf("Inizializzanzione\n");
	//DEBUG
	
	int Mg=calcoloMg(nodi, N);

	//DEBUG
	printf("Mg=%d\n", Mg);
	//DEBUG
	
	//Inizializzazione della lista
	lista *l = malloc(sizeof(lista));
	inizializzazioneLista(l, nodi, N);
	
	//DEBUG
	int nNodi=0;
	if(FLAG_DEBUG){
		nodoL *p1=l->first;
		printf("nodi nella lista: ");
		while (p1!=NULL){
			printf("%d ",p1->n);
			nNodi++;
			p1=p1->next;
		}
		printf("[num=%d]\n",nNodi);
	}
	//DEBUG
	
	//inizializzazione count
	inizializzazioneCount(nodi,N);

	//DEBUG	
	if(FLAG_DEBUG){
		int i;		
		for (i=0;i<N;i++)
			printf("%d count=%d\n",i,nodi[i]->count);
		printf("\n");
	}
	//DEBUG
	
	//DEBUG			
	if(FLAG_DEBUG) printf("Inizio dell'algoritmo\n");
	//DEBUG
	
	int fOld;
	int v;
	int tmpI;
	int tmpI_v=-1;
	int valore;
	nodoSp *succ;
	nodoSp *prec;
	nodoL *p=l->first;
	//DEBUG
	int numIterazioni=0;
	//DEBUG
	while (p!=NULL){
		v = p->n;
		fOld = nodi[v]->f;
		removeFirstLista(l, nodi);
		
		//applico la funzione &(f,v)
		succ=nodi[v]->succ;
		//se il nodo è di tipo 0
		if (!nodi[v]->tipo){
			tmpI=INT_MAX;
			//cerco il sucessore con la f minore
			while (succ!=NULL){
				valore=(nodi[succ->n]->f)-(succ->pesoArc);
				if (valore<tmpI)
					tmpI=valore;
				succ=succ->next;
				}
		//se il nodo è di tipo 1
		}else{
			tmpI=0;
			//cerco il successore con la f maggiore
			while (succ!=NULL){
				valore=(nodi[succ->n]->f)-(succ->pesoArc);
				if(valore>tmpI){
					tmpI=valore;
					tmpI_v = succ->n;
				}
				succ=succ->next;
			}
		}
		
		//verifico se la f calcolata è maggiore di Mg
		if (tmpI_v==v)
			nodi[v]->f = INT_MAX;
		else
			nodi[v]->f = (tmpI>Mg) ? INT_MAX : tmpI;
		
		//DEBUG			
		if(FLAG_DEBUG) printf("nodo %d f=%d ",v,nodi[v]->f);
		//DEBUG

		//se il nodo è di tipo 0 vado ad aggiornare il count
		if (!nodi[v]->tipo)
			aggCount(v, nodi);
		
		//DEBUG	
		if(FLAG_DEBUG) printf("count=%d\n",nodi[v]->count);
		//DEBUG
				
		//controllo se i nodi predecessori del nodo v devono esere aggiunti alla lista
		prec=nodi[v]->prec;
		int fPrec;
		int nPrec;
		while (prec!=NULL){
			nPrec = prec->n;
			fPrec = nodi[nPrec]->f;
			if ((fPrec) < (nodi[v]->f)-(prec->pesoArc)){
				//se il predecessore è i tipo 0
				if (!nodi[nPrec]->tipo){
					if (fPrec >= fOld -(prec->pesoArc))
						nodi[nPrec]->count = (nodi[nPrec]->count)-1;
					if (nodi[nPrec]->count<=0)
						insLista(l,nPrec,nodi);
				//se il predecessore è di tipo 1
				}else
					insLista(l,nPrec,nodi);
			}
			prec=prec->next;
		}
		
		//DEBUG
		if(FLAG_DEBUG) {
			nodoL *p1=l->first;
			nNodi=0;
			printf("elementi nella lista: ");
			while (p1!=NULL){
				printf("%d ",p1->n);
				nNodi++;
				p1=p1->next;
			}
			printf("[num=%d]\n\n",nNodi);
		}
		//DEBUG
			
		p=l->first;
		numIterazioni++;
	}
	int i;
	for (i=0;i<N;i++){
		//if(FLAG_DEBUG) printf("nodo %d f=%d \n",i,nodi[i]->f);
		f[i] = nodi[i]->f;
	}

	if(FLAG_DEBUG) printf("numIterazioni=%d\n", numIterazioni);
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
	if (argc>=2 && strstr(argv[1], "-")!=NULL && strstr(argv[1], "d")!=NULL){
		printf("DEBUG-mode.\n");
		FLAG_DEBUG = 1;
	}
	if (argc>=2 && strstr(argv[1], "-")!=NULL && strstr(argv[1], "i")!=NULL){
		printf("INFO-mode.\n");
		FLAG_OUTPUT = 1;
	}
	if (argc>=4 && strstr(argv[1], "-")!=NULL && strstr(argv[1],"r")!=NULL){
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
				addSucc(j,vArray[t],rand_range(-10,10),nodi,T);
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
		printf("default!\n");
		N=5;	
		nodi = malloc(sizeof(nodo*)*N);
		//costruzione Grafo
		//GRAFO1 
		nodi[0]=newNodo(1);
		nodi[1]=newNodo(0);
		nodi[2]=newNodo(1);
		nodi[3]=newNodo(0);
		nodi[4]=newNodo(0);
		addSucc(0,1,3,nodi,T);
		addSucc(3,0,2,nodi,T);
		addSucc(3,1,1,nodi,T);
		addSucc(1,3,-3,nodi,T);
		addSucc(1,4,1,nodi,T);
		addSucc(2,1,1,nodi,T);
		addSucc(2,4,0,nodi,T);
		addSucc(4,2,-4,nodi,T);
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
	
	int f[N];

	clock_t begin = clock();
	algoritmo1(nodi, N, f);
	clock_t end = clock();

	if (FLAG_OUTPUT){
		printf("\nDATI FINALI\n");
		int numT = 0;
		int i;
		for (i=0;i<N;i++){
			if(f[i] == INT_MAX) numT++;
			printf("nodo %d f=%d \n", i, f[i]);
		}
		printf("numT=%d\n", numT);
	}

	printf("time %f second\n", (double)(end - begin) / CLOCKS_PER_SEC);
}
