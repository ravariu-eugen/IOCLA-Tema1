#include "lista.h"
// operatii de baza pentru lista simplu inlantuita

TLista AlocCelula(void *e)          
{ 	// intoarce adresa celulei create sau NULL 
    TLista aux = (TLista)malloc(sizeof(TCelula)); 
	if(!aux)
		return NULL;
    aux->info = e;                   
    aux->urm = NULL;
 	return aux;            
}

int InsertL(TLista *L, void *e)
{	// insereaza o celula cu informatia e la inceputul listei L
	// intoarce 1 pt inserare reusita si 0 pt esec
	TLista aux = AlocCelula(e);
	if(!aux)
		return 0;
	aux->urm = *L;
	*L = aux;
	return 1;
}

int InsertEndL(TLista *L, void *e)
{	// insereaza o celula cu informatia e la sfarsitul listei L
	// intoarce 1 pt inserare reusita si 0 pt esec
	TLista aux = AlocCelula(e);
	aux->urm = NULL;
	if(!aux)
		return 0;
	if(*L == NULL){
		*L = aux;
		return 1;
	}
	TLista p;
	// gasim ultima celula din lista
	for(p = *L; p->urm!=NULL; p = p->urm);
	p->urm = aux;
	return 1;
}

int InsertOrdL(TLista *L, void *e, COMP comp)
{	// insereaza o celula cu informatia e in lista ordonata crescator L asa 
	// incat lista sa ramana ordonata crescator
	// ordinea listei este data de functia de comparare comp
	// intoarce pozitia la care ajunge in lista sau -1 daca esueaza inserarea
	int pozitie = 1;
	if(*L == NULL)
	{	// la o lista vida, se insereaza la inceput
		if(InsertL(L, e) == 0)
			return -1;
		return pozitie;
	}
	if(comp(e, (*L)->info) < 0)
	{
		// daca e mai mic decat primul, se insereaza la inceput
		if(InsertL(L, e) == 0)
			return -1;
		return pozitie;
	}
	TLista aux;
	for(aux = *L; aux->urm != NULL; aux = aux->urm)
	{
		pozitie++;
		// daca e mai mare decat un element, 
		// se insereaza la intre acela si urmatorul 
		if(comp(e, aux->info) >= 0 && comp(e, aux->urm->info) < 0)
		{
			if(InsertL(&(aux->urm), e) == 0)
				return -1;
			return pozitie;
		}
	}
	pozitie++;
	// daca e mai mare decat toate, se inereaza la sfarsit
	if(InsertL(&(aux->urm), e) == 0)
		return -1;
	return pozitie;
}

void *ExtrL(TLista *L)
{	// extrage o celula de la inceputul listei L si intoarce pointer la 
	// informatia extrasa, dezalocand celula
	// alternativ, daca primeste &(aux->urm) ca argument in loc de &L,
	// va extrage celula aux->urm si la conecta aux la aux->urm->urm
	// intoarce pointer la informatia extrasa sau NULL in caz de esec
	if(*L == NULL)
		return NULL;
	TLista aux = *L;
	*L = aux->urm;
	void *info = aux->info;
	free(aux);
	return info;
}

void *FindL(TLista L, void *e, COMP comp)
{	// cauta si intoarce un pointer la informatia din prima celula pentru care
	// functia comp da 0 pentru e si informatia din celula
	if(L == NULL)
		// lista vida
		return NULL;
	TLista aux;
	for(aux = L; aux; aux = aux->urm)
	{
		if(comp(e, aux->info) == 0)
			return aux->info;
	}
	return NULL;
}

void *Find_ExtrL(TLista *L, void *e, COMP comp)
{	// cauta si extrage celula care contine un pointer la informatia din prima
	// celula pentru care functia comp da 0 pentru e si informatia din celula
	if(*L == NULL)
		// lista vida
		return NULL;
	if(comp((*L)->info, e) == 0)
		return ExtrL(L);
	TLista aux;
	for(aux = *L; aux->urm; aux = aux->urm)
	{
		if(comp(e, aux->urm->info) == 0)
			return ExtrL(&(aux->urm));
	}
	return NULL;
}

void AfisareL(TLista L, AFIS afis, FILE *out)
{	// afiseaza valorile elementelor din lista folosind functia afis
                  
  	for (; L != NULL; L = L->urm)       
    {	// afiseaza fiecare element al listei
		afis(L->info, out);     
		fprintf(out, "\n");
	}                                        
}

void ResetL(TLista *L, DEL del)
{	// elimina toate elementele din lista folosind functia del
  	void *aux;
	if(L == NULL)
		return;
  	while(*L)
  	{
   		aux = ExtrL(L);
    	del(aux);
  	}
	*L = NULL;
}

int SizeL(TLista L)
{	// intoarce numarul de elemente din lista
	int result = 0;
	while(L)
	{
		result++;
		L = L->urm;
	}
	return result;
}


