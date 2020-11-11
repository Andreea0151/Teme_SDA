//Se da un graf orientat G=(V,E).
//Sa se determine componentele sale tare conexe - numarul lor, iar pentru fiecare sa se enumere varfurile.
#include "stdio.h"
#include "stdlib.h"
//Componenta tare conexa: intre oricare 2 noduri exista un drum care le leaga

typedef struct
{
    int V;    //nr. varfuri
    int E;    //nr. muchii
    int ** a; //matricea de adiacenta
}Graph;
Graph *create (FILE *in)  //graf orientat
{
    //alocare memorie graf
    Graph *g = (Graph *) malloc (sizeof(Graph));
    if (g == NULL)
    {
      printf ("No memory for this graph.\n");
      return NULL;
    }

    fscanf (in, "%d", &(g->V));

    //alocare memorie matrice
    g->a = (int **) malloc(g->V * sizeof(int*));
    for (int i = 0; i < g->V; i++)
        g->a[i] = (int *) malloc(g->V * sizeof(int));
    if (g->a == NULL)
    {
      printf ("No memory for this matrix.\n");
      return NULL;
    }
    g->E = 0;

    //citire matrice de adiacenta
    for (int i = 0; i < g->V; i++)
    for (int j = 0; j < g->V; j++)
    {
        fscanf (in, "%d", &g->a[i][j]);
        if(g->a[i][j] != 0) g->E ++;
    }

    return g;
}

void printGraph(Graph *g)
{
    //afisare matrice de adiacenta
    for (int i = 0; i < g->V; i++)
    {
      for (int j = 0; j < g->V; j++)
        printf( " %d ",g->a[i][j]);
      printf("\n");
    }
}

void dfsuc(Graph *g, int nod, int *suc, int nrc)
{
    suc[nod] = nrc; //varful este vizitat
    //printf ("%d ", nod);
    for (int i = 0; i < g->V; i++) //pt varful respectiv
    //se verifica daca exista arc de la acesta la alt varf neprocesat i
        if(g->a[nod][i] == 1 && suc[i] == 0)
    dfsuc(g, i, suc, nrc);  //apoi, urmatorul varf gasit este procesat
}

void dfpred(Graph *g, int nod, int *pred, int nrc)
{
    pred[nod] = nrc;  //varful este vizitat
    //printf ("%d ", nod);
    for (int i = 0; i < g->V; i++)  //pt varful respectiv
    //se verifica daca exista arc de la varfurile neprocesate i catre acesta
      if(g->a[i][nod] == 1 && pred[i] == 0)
    dfpred(g, i, pred, nrc);  //apoi, urmatorul varf gasit este procesat
}

int main()
{
    FILE *in = fopen("Fisier.in", "r");
    Graph *g = create(in);
    printf ("Matrice:\n");
    printGraph (g);

    //se creeaza vectorii succesor si predecesor care
    //tin evidenta varfurilor vizitate
    int *pred = malloc(g->V * sizeof(int));
    int *suc = malloc(g->V * sizeof(int));

    int nrc = 0;  //nrc: nr componente tare conexe
    for (int i = 0; i < g->V; i++)
      if(suc[i] == 0) //daca exista noduri nevizitate
      {
        nrc++;  //=> exista o componenta tare conexa
        //se apeleaza pana cand nu mai sunt varfuri nevizitate
        dfsuc (g, i, suc, nrc);
        dfpred (g, i, pred, nrc);
        for (int j = 0; j < g->V; j++)
        //reinitializarea verifica daca varfurile ar putea face parte din alta comp tare conexa
          if (suc[j] != pred[j]) suc[j] = pred[j] = 0;
      }

    printf ("\nNr componente tare conexe: %d\n", nrc);
    printf ("\n");

    //afisare componente conexe
    for (int i = 1; i <= nrc; i++)
    {
        printf ("Componenta %d: ", i);
        for (int j = 0; j < g->V; j++)
        //daca valoarea vectorului succesor coincide cu numarul componentei tare conexe
            if (suc[j] == i) printf ("%d ", j); //atunci nodul respectiv apartine comp tare conexe i
        printf ("\n");
    }

    return 0;
}
