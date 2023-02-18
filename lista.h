#include <stdlib.h>
#include <stdio.h>

#ifndef _FUNCTII_
#define _FUNCTII_
  typedef void(*DEL)(void *);
  typedef int(*COMP)(const void *, const void *);
  //comp(a,b) intoarce valori:
  // <0, daca a este inainte de b
  // >0, daca a este dupa b
  // =0, daca a si b sunt echivalente
  typedef void(*AFIS)(void *, FILE *);
#endif

#ifndef _LISTA_SIMPLU_INLANTUITA_
#define _LISTA_SIMPLU_INLANTUITA_

typedef struct celula
{ 
    void *info;
    // pointer la informatia 
    struct celula * urm;
}   TCelula, *TLista; 
#define VidaL(L) ((L) == NULL)

TLista AlocCelula(void *e);         
// aloca si intoarce pointer la o celula
int InsertL(TLista *L, void *e);
// insereaza o celula la inceputul unei liste
int InsertEndL(TLista *L, void *e);
// insereaza o celula la sfarsitul unei liste
int InsertOrdL(TLista *L, void *e, COMP comp);
// insereaza o celula intr-o lista ordonata astfel incat 
// sa se mentina ordinea data de functia comp
void *ExtrL(TLista *L);
// extrage o celula de la inceputul unei liste si intoarce informatia continuta
void *FindL(TLista L, void *e, COMP comp);
// gaseste celula cu informatia egala cu e (adica comp(e,info) == 0)
void *Find_ExtrL(TLista *L, void *e, COMP comp);
// gaseste si extrage celula cu informatia egala cu e (adica comp(e,info) == 0)
void AfisareL(TLista L, AFIS afis, FILE *out);
// afiseaza continutul listei
void ResetL(TLista *L, DEL del);
// sterge continutul listei
int SizeL(TLista L);
// intoarce numarul de elemente din lista

#endif
