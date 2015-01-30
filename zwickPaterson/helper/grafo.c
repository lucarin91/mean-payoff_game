#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#include "grafo.h"

nodo *newNodo(int tipo){
	nodo *n = malloc(sizeof(nodo));
	n->tipo=tipo;
	n->val=0;
	n->succ=NULL;
	n->prec=NULL;
	return n;
}

nodoSp *newNodoSp(int n, int pesoArc, nodoSp *next){
	nodoSp *new = malloc(sizeof(nodoSp));
	new->next=next;
	new->pesoArc=pesoArc;
	new->n=n;
	return new;
}

void addSucc (int n1, int n2, int pesoArc, nodo *nodi[]){	
	nodoSp *newSucc = newNodoSp(n2,pesoArc,nodi[n1]->succ);
	nodi[n1]->succ=newSucc;
	
	nodoSp *newPrec = newNodoSp(n1,pesoArc,nodi[n2]->prec);
	nodi[n2]->prec=newPrec;
}