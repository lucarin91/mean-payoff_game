//#include "structGrafo.h"

typedef struct nodoSp{
	struct nodoSp *next;
	int n;
	int pesoArc;
}nodoSp;

typedef struct nodo{
	int tipo;
	nodoSp *succ;
	nodoSp *prec;
	int val;
}nodo;

nodo *newNodo(int tipo);

nodoSp *newNodoSp(int n, int pesoArc, nodoSp *next);

void addSucc (int n1, int n2, int pesoArc, nodo *nodi[]);