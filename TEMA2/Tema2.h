#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//Structura pentru resurse:
typedef struct Resursa
{
    char *nume;     //nume resursa
    int cantitate;  //cantitate resursa
} Resursa;  //vector resursa

//Structura pentru insula:
typedef struct Island
{
    char *nume;               //nume insula
    int nrResurse;            //nr de resurse al insulei
    Resursa *inventarResurse; //fiecare insulela, avand resursele proprii
    int nrAvioane;            //nr de avioane al fiecarei insule
    int *avioane;             //vectorul de avioane
    int tolAvioane;           //toleranta
} Island; //vector insule

//Structura pentru graf
typedef struct Graph
{
    int N, M;       //N = nr noduri; M = nr muchii
    int **matrice;  //matrice de adiacenta
} Graph_island;

//declarare functii
void alocare_Resursa (Resursa **inventarResurse, int *n);
void citire_Resursa (FILE *in, Resursa *inventarResurse, int *n);
void alocare_Island (FILE *in, Island **island, int *n);
void citire_Island (FILE *in, Island *island, Resursa *inventarResurse, int *n);
void afisare_vector_insule (Island *island, int n);
Graph_island *create (FILE *in, int n);
void printGraph (Graph_island *g);
void conexiune (Graph_island *g, int x, int y, FILE *out);
void legatura (Graph_island *g, int x, FILE *out);
void adauga_zbor (Graph_island *g, int x, int y, int z);
void anulare_zbor (Graph_island *g, int x, int y);
void max_resurse (Graph_island *g, Island *island, FILE *out);
void max_cantitate_resursa (Graph_island *g, Island *island, char *resursa, FILE *out);
void Dijkstra (Graph_island *g, int nod, int *T, int *D, int *viz);
void drum_zbor (int x, int *T, FILE *out);
void timp_zbor (int *D, int x, FILE *out);
int min_zbor (int x, int *T, int *D);
void after_Dijkstra (Graph_island *g);
void alocare_matrice (int ***m, int *n);
void citire_matrice (int **m, int n, FILE *in);
void citire_bkt (Island *island, FILE *in, int x, int ***m);
int nr_caz1 (Island *island, int n);
void afisare_avioane (Island *island, int n, FILE *out);
int nr_caz2 (Island *island, int n);
void BKT (int i, int k, Island *island, int n, int **m, int insula_actuala, FILE *out);
void cerinte(Graph_island *g, Island *island, FILE *in, FILE *out, int *x);
