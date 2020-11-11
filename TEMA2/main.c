#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Tema2.h"
int main(int argc, char const *argv[])
{
    //fisier intrare
    FILE *in = fopen (argv[1], "r");
    if (in == NULL)
    {
        printf("Fisierul  nu poate fi deschis!\n");
        exit(1);
    }

    //fisier iesire
    FILE *out = fopen (argv[2], "w");
    if (out == NULL)
    {
        printf("Nu se poate deschide fisierul!\n");
        exit(1);
    }

    int n;
    fscanf (in, "%d", &n);

    Island *island;
    alocare_Island (in, &island, &n);
    citire_Island (in, island, island->inventarResurse, &n);
    afisare_vector_insule (island, n);

    Graph_island *g = create(in, n);
    printGraph (g);

    // x = nr de insule pt etapa de Gestiune
    int x;
    cerinte (g, island, in, out, &x);

    //eliberare memorie
    for (int i = 0; i < g->N; i++)
          free (g->matrice[i]);
    free (g->matrice);
    for (int i = 0; i < g->N; i++)
        free(island[i].nume);

    return 0;
}
