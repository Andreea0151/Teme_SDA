#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Tema2.h"

// CITIRE

void alocare_Resursa (Resursa **inventarResurse, int *n)
{
    //alocare spatiu pentru vectorul de resurse
    *inventarResurse = malloc(*n * sizeof(Resursa));
}

void citire_Resursa (FILE *in, Resursa *inventarResurse, int *n)
{
    for (int i = 0; i < *n; i++)
    {
      //alocare spatiu pt sirul de caractere
      inventarResurse[i].nume = (char *)malloc(sizeof(char) * 20);

      //citire nume resura
      fscanf(in, "%s", inventarResurse[i].nume);

      //citire cantitate resursa
      fscanf (in, "%d", &inventarResurse[i].cantitate);
    }
}

void alocare_Island (FILE *in, Island **island, int *n)
{
    //alocare spatiu vector insule
    *island = (Island *)malloc(*n * sizeof(Island));
}

void citire_Island (FILE *in, Island *island, Resursa *inventarResurse, int *n)
{
      for (int i = 0; i < *n; i++)
      {
        //alocare spatiu sir de caractere
        island[i].nume = (char *)malloc(sizeof(char) * 10);
        //citire nume insula
        fscanf (in, "%s", island[i].nume);

        //citire nr de resurse al fiecarei insule
        fscanf (in, "%d", &(island[i].nrResurse));

        alocare_Resursa (&(island[i].inventarResurse), &island[i].nrResurse);
        citire_Resursa (in, island[i].inventarResurse, &(island[i].nrResurse));
      }
}

void afisare_vector_insule (Island *island, int n)
{
    printf ("Afisare vector insule\n");
    printf ("Nr insule: %d\n", n);
    for (int i = 0; i < n; i++)
    {
        printf ("%s\n", island[i].nume);
        for (int j = 0; j < island[i].nrResurse; j++)
            printf ("  %s %d\n",island[i].inventarResurse[j].nume, island[i].inventarResurse[j].cantitate);
    }
}

Graph_island *create (FILE *in, int n)
{
    //alocare memorie graf
    Graph_island *g = malloc(sizeof(Graph_island));
    if (g == NULL)
    {
      printf ("No memory for this graph.\n");
      return NULL;
    }

    //numarul de noduri este = cu numarul de insule
    g->N = n;

    //alocare memorie matrice de adiacenta
    g->matrice = (int **)malloc(g->N * sizeof(int*));
    for (int i = 0; i < g->N; i++)
        g->matrice[i] = (int *)calloc(g->N, sizeof(int));
    if (g->matrice == NULL)
    {
      printf ("No memory for this matrix.\n");
      return NULL;
    }
    g->M = 0;

    int nr;
    fscanf(in, "%d", &nr);
    printf ("%d\n",nr);

    char *sir1, *sir2, *sir3;
    int durata_zbor;
    for (int k = 0; k < nr; k++)
    {
//se citesc din fisier text insulele intre care exista legatura

        sir1 = (char *)malloc(sizeof(char) * 10);
        fscanf (in, "%s", sir1);
        printf ("%s", sir1);

        sir2 = (char *)malloc(sizeof(char) * 2);
        fscanf (in, "%s", sir2);
        printf (" %s ", sir2);

        sir3 = (char *)malloc(sizeof(char) * 10);
        fscanf (in, "%s", sir3);
        printf ("%s ", sir3);

        fscanf (in, "%d", &durata_zbor);
        printf ("%d\n", durata_zbor);

// se converteste prin functia "atoi" sirul de caractere care indica numarul insulei
// in cifra; deoarece matrice incepe de la linia, respectiv coloana 0
// nr insulei se va decrementa cu o unitate
        int indexInsula1 = atoi(sir1 + 6) - 1;
        int indexInsula2 = atoi(sir3 + 6) - 1;

        //graful este neorientat, ponderat
        //se creeaza matricea de adiacenta
        g->matrice[indexInsula1][indexInsula2] =
        g->matrice[indexInsula2][indexInsula1] = durata_zbor;

        g->M += 2;
    }
    return g;

    //eliberare memorie siruri
    free(sir1);
    free(sir2);
    free(sir3);
}

void printGraph (Graph_island *g)
{
    //afisare matrice de adiacenta
    printf ("\nMatrice cost:\n");
    for (int i = 0; i < g->N; i++)
    {
      for (int j = 0; j < g->N; j++)
        printf( " %d ",g->matrice[i][j]);
      printf("\n");
    }
    printf ("\nNr muchii: %d\n\n", g->M);
}

// 1 ----------PROIECTARE----------

void conexiune (Graph_island *g, int x, int y, FILE *out)
{
//se verifica​ daca exista conexiune directa intre doua insule
    if (g->matrice[x][y] != 0)  fprintf (out, "OK\n");
    else fprintf (out, "NO\n");
}

void legatura (Graph_island *g, int x, FILE *out)
{
//se verifica daca exista​ legatura directa​ intre
//Insula X cu celelate insule din arhipelag
    for (int i = 0; i < g->N; i++)
    {
      if (g->matrice[x][i] != 0)
      fprintf (out, "Island%d ", i + 1);
    }
    fprintf (out, "\n");
}

void adauga_zbor (Graph_island *g, int x, int y, int z)
{
//se adauga un ​drum direct de durata ​cost(z)​ intre Insula X si Insula Y
    g->matrice[x][y] = g->matrice[y][x] = z;
}

void anulare_zbor (Graph_island *g, int x, int y)
{
//se sterge drumul direct dintre Insula X si Insula Y
    g->matrice[x][y] = g->matrice[y][x] = 0;
}

void max_resurse (Graph_island *g, Island *island, FILE *out)
{
//se afiseaza numarul de resurse distincte de pe tot arhipelagul, urmat
//de numele resurselor separate printr-un spatiu in ordine lexicografica

    int nr = 0; //numar elemente vector de siruri de caractere
    char **vector = NULL; //vector de siruri de caractere
                          //in care se adauga numele de resurse
    for (int i = 0; i < g->N; i++) //parcurgere insule
    for (int j = 0; j < island[i].nrResurse; j++) //parcurgere a resurselor fiecarei insule
    {
        if (nr == 0)  //daca  nu exista resursa in noul vector (**vector)
        {
            vector = malloc(sizeof(char*)); //alocare memorie vector
            vector[nr] = malloc(sizeof(char) * 10); //alocare memorie numele resursei (sir de caractere din vector)
            strcpy(vector[nr], island[i].inventarResurse[j].nume);  //se adauga prima resursa in vectorul creat
            nr++; //incrementare nr elemente din "**vector"
        }
        else  //daca exista elemente/e in vector
        {
            int gasit = 0;  //variabila care verifica daca resursa actuala este deja in vector
            for (int k = 0; k < nr; k++)  //parcurgere vector
            {
              //se compara lexicografic resursa actuala cu elementul de pe pozitia "k" din vector
              if (strcmp(island[i].inventarResurse[j].nume, vector[k]) == 0)
              {
                gasit  = 1; //daca resursa acutuala este deja in vector, gasit devine 1
              }
            }
            if (gasit == 0) //daca resursa actuala nu se gaseste in vector, se adauga
            {
              vector = realloc(vector, (nr + 1) * sizeof(char*));     //vectorul este realocat
              vector[nr] = malloc(sizeof(char) * 10);                 //alocare memorie nume resura
              strcpy(vector[nr], island[i].inventarResurse[j].nume);  //resursa este adaugata in vector
              nr++;                                                   //incrementare nr elemente
            }
        }
    }
    fprintf (out, "%d ", nr); //nr final de resurse din vector
    char *aux = (char *)malloc (sizeof(char) * 20); //variabila auxiliara pentru sortarea numelor
    for (int i = 0; i < nr - 1; i++)
    {
      for (int j = i + 1; j < nr; j++)
        if (strcmp (vector[i], vector[j]) > 0)  //daca primul nume este mai mare dpdv lexicografic
        {
          //=> interschimbare
          strcpy(aux, vector[i]);
          strcpy(vector[i], vector[j]);
          strcpy(vector[j], aux);
       }
    }
    //afisarea resurselor din vector dupa sortarea lexicografica
    for (int i = 0; i  < nr; i++)
        fprintf (out, "%s ", vector[i]);
      fprintf(out, "\n");
    for (int i = 0; i < nr; i++)
        free(vector[i]);  //eliberare memorie a fiecrui sir de caractere (nume resursa)
    free(vector); //eliberare vector de sir de caractere
}

void max_cantitate_resursa (Graph_island *g, Island *island, char *resursa, FILE *out)
{
//se afiseaza insula (insulele) cu cantitatea
//cea mai mare de resursa disponibila

     int mx = 0;  //maximul este initializat cu o valoare minima
     for (int i = 0; i < g->N; i++)                  //parcurgere insule
     for (int j = 0; j < island[i].nrResurse; j++)  //parcurgere a resurselor fiecarei insule
       {
         //daca resursa transmisa ca parametru coincide cu una din cele parcurse
         if (strcmp(resursa, island[i].inventarResurse[j].nume) == 0)
            //se calculeaza maximul
             if (mx < island[i].inventarResurse[j].cantitate) //daca este gasita o valoare mai mare decat cea actuala a lui mx
               mx  = island[i].inventarResurse[j].cantitate;  //atunci lui mx i se atribuie valoarea respectiva
       }
       for (int i = 0; i < g->N; i++)                 //parcurgere insule
       for (int j = 0; j < island[i].nrResurse; j++)  //parcurgere a resurselor fiecarei insule
          if (strcmp(resursa, island[i].inventarResurse[j].nume) == 0)  //daca in alta insula se afla aceeasi resursa
          if (mx == island[i].inventarResurse[j].cantitate) //se verifica daca se afla in aceeasi cantitate maxima
              fprintf (out, "%s ", island[i].nume); //in caz afirmativ, se afiseaza numele insulolor cu
                                                    //cantitatea cea mai mare de resursa disponibila
      //altfel se afiseaza insula cu cea mai mare cantitate de resura ceruta
      fprintf (out, "\n");
}

// 2 ----------ANALIZA----------

//algoritmul lui Dijkstra, astfel incat sa tina evidenta predecesorilor pentru
//a putea recupera calea de cost minim de la varful sursa la fiecare alt varf
void Dijkstra (Graph_island *g, int nod, int *T, int *D, int *viz)
{
    int mn, i, j, k;
//se stabileste nodul de plecare
//initial, nodul de plecare este singurul vizitat
    viz[nod] = 1;
    for (i = 0; i < g->N; i++)
    for (j = 0; j < g->N; j++)
        if (g->matrice[i][j] == 0)  g->matrice[i][j] = 3200;
    for (i = 0; i < g->N; i++)
    {
      //se construieste vectorul D cu valori din matricea
      //costurilor de pe linia nodului de plecare
        D[i] = g->matrice[nod][i];
        if (i != nod)
      //si vectorul T daca exista muchie intre nodul de plecare si celelalte noduri
          if (D[i] != 3200) T[i] = nod;
    }
//de N - 1 ori se selecteaza valoarea minima dintre cele nevizitate
//din vect D si se considera acel nod ca viitor nod de plecare
    int gasit = 0;
    for (int i = 0; i < g->N; i++)
        if (D[i] != 3200) gasit = 1;
    if (gasit == 1)
    {
      for (i = 0; i < g->N - 1; i++)
      {
        mn = 3200;
        for (j = 0; j < g->N; j++)
        if (!(viz[j]))
        if (D[j] < mn)
        {
          mn = D[j];
          k = j;
        }
        viz[k] = 1;
        for (j = 0; j < g->N; j++)
          if (viz[j] == 0)
            if (D[j] > D[k] + g->matrice[k][j])
            {
                  D[j] = D[k] + g->matrice[k][j];
                  T[j] = k;
            }
        }
    }
}

void drum_zbor (int x, int *T, FILE *out)
{
    //printf ("%d %d", T[x], x);
    if (T[x] != -1)                        //daca exista predecesori
    {
      if (T[T[x]] != -1)                   //condie de opriere pe penultimul nod
      drum_zbor (T[x], T, out);            //recursivitate
      fprintf (out, "Island%d ", x + 1);   //se afiseaza insulele care det drumul minim
    }
    else fprintf (out, "NO");              //in caz contrar, nu exista drum intre 2 insule

}

void timp_zbor (int *D, int x, FILE *out)
{
    if  (D[x] == 3200) fprintf (out, "INF\n");
    else fprintf (out, "%d\n", D[x]);      //se afiseaza timpul minim
}

int min_zbor (int x, int *T, int *D)
{
    if (T[x] != -1)             //daca exista predecesori
    {
        D[x] += 15;             //se adauga timpul de stationare
        min_zbor (T[x],T, D);   //pentru fiecare insula prin care se trece
    }
    return D[x] + 15;           //se adauga timpul de stationare ultimei insule
}

void after_Dijkstra (Graph_island *g)
{
//functia revine la matricea initiala
    for (int i = 0; i < g->N; i++)
    {
        for (int j = 0; j < g->N; j++)
          if (g->matrice[i][j] == 3200) g->matrice[i][j] = 0;
    }
}

// 3 ----------GESTIUNE----------

void alocare_matrice (int ***m, int *n)
{
    *m = (int** )malloc(*n * sizeof(int*));
    for (int i = 0; i < *n; i++)
        (*m)[i] = (int* )malloc(*n * sizeof(int));
}

void citire_matrice (int **m, int n, FILE *in)
{
    for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
        fscanf(in, "%d", &m[i][j]);
}

void afisare_matrice (int **m, int n)
{
    for (int i = 0; i < n; i++)
    {
      for (int j = 0; j < n; j++)
          printf ("%d ", m[i][j]);
        printf ("\n");
    }
}

void citire_bkt (Island *island, FILE *in, int n, int ***m)
{
    printf ("\nNr insule: %d", n);
    int toleranta;
    fscanf (in, "%d", &toleranta);
    printf ("\nToleranta: %d\n", toleranta);
    for (int i = 0; i < n; i++)
    {
        island[i].tolAvioane = toleranta;
        fscanf(in, "%d", &island[i].nrAvioane);
        printf ("%d ", island[i].nrAvioane);
    }
    printf ("\n");
    for (int i = 0; i < n; i++)
        island[i].avioane = (int *)malloc(island[i].nrAvioane * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < island[i].nrAvioane; j++)
        {
          fscanf(in, "%d", &island[i].avioane[j]);
          printf ("%d ", island[i].avioane[j]);
        }
        printf ("\n");
    }
    alocare_matrice(m, &n);
    citire_matrice (*m, n, in);
    afisare_matrice (*m, n);
}

void afisare_avioane (Island *island, int n, FILE *out)
{
    for (int i = 0; i < n; i++)
    {
        fprintf (out, "Island%d\n", i + 1);
        for (int j = 0; j < island[i].nrAvioane; j++)
            fprintf (out, "%d ", island[i].avioane[j]);
        fprintf (out, "\n");
    }
}

int nr_caz1 (Island *island, int n)
{
    for (int i = 0; i < n; i++)
        if (island[i].nrAvioane > island[i].tolAvioane) return 0;
    return 1;
}

int nr_caz2 (Island *island, int n)
{
    int nr_tot = 0;
    for (int i = 0; i < n; i++)
      nr_tot += island[i].nrAvioane;
        if (n * island->tolAvioane < nr_tot) return 1;
    return 0;
}

// INCERCARE BKT
int conditie (Island *island, int **m, int pozitie, int k)
{
    if (m[pozitie][k] != 1) return 0;
    if (island[pozitie].nrAvioane >= island[pozitie].tolAvioane) return 0;
    return 1;
}

int solutie (Island *island, int pozitie)
{
    if (island[pozitie].nrAvioane == island[pozitie].tolAvioane)
        return 1;
    else return 0;
}

void BKT (int i, int k, Island *island, int n, int **m, int insula_actuala, FILE *out)
{
    //conditie de oprire at cand se ajunge pe ultima insula
    if (i == n - 1) return;
    else
    {
      if (conditie(island, m, i, insula_actuala))
      {
        //pleaca avioane de la insula actuala la insula i
        for (int j = island[insula_actuala].nrAvioane - 1; j > island[insula_actuala].tolAvioane; j--)
        {
          island[i].avioane = realloc (island[i].avioane, (island[i].nrAvioane + 1) * sizeof(int));
          island[i].nrAvioane++;
          //se pune pe pozitia corecta(ordonat crescator) in insula i
          int l = island[i].nrAvioane - 2;
          while (island[i].avioane[l - 1] > island[i].avioane[l] && l >= 1)
          {
            l--;  //se cauta pozitia corecta
          }
          //se face loc pt noul avion
          for (int p = island[i].nrAvioane - 1; p >= l; p--)
          island[i].avioane[p] = island[i].avioane[p - 1];
          //se aduce avionul pe insula i
          island[i].avioane[l] = island[insula_actuala].avioane[j];
          island[insula_actuala].nrAvioane--;
        }
        if (solutie (island, insula_actuala)) afisare_avioane (island, n, out);
        else BKT (i + 1, k + 1, island, n, m, insula_actuala, out);
      }
    }
}

// Cerintele fiecarei etape sunt apelate in functia "cerinte"

void cerinte (Graph_island *g, Island *island, FILE *in, FILE *out, int *x)
{
    char *cerinte, *sir1, *sir2;
    cerinte = (char *)malloc(sizeof(char) * 10);
    sir1 = (char *)malloc(sizeof(char) * 10);
    sir2 = (char *)malloc(sizeof(char) * 10);
    int *D, *T, *viz;

    do
    {
      //citirea sirului de caractere cerinta, urmat de verificarea
      //lexicografica determina apelarea functiilor implementate
      fscanf (in, "%s", cerinte);
      if (feof(in)) return;

      //verificare caracter cifra pt testele 9-14
      if (isdigit(cerinte[0]))
      {
        //acesta este transformat intr-un int si transmis ca parametru
        //a.i. valoarea respectiva sa poata fi recuperata valoarea in main
        *x = atoi(cerinte);
        break;
      }
      printf ("%s ", cerinte);

      if (strcmp ("conexiune", cerinte) == 0)
      {
        fscanf (in, "%s", sir1);
        printf ("%s ", sir1);
        fscanf (in, "%s", sir2);
        printf ("%s\n", sir2);

        int indexInsula1 = atoi(sir1 + 6) - 1;
        int indexInsula2 = atoi(sir2 + 6) - 1;

        conexiune (g, indexInsula1, indexInsula2, out);
      }

      if (strcmp ("legatura", cerinte) == 0)
      {
          fscanf (in, "%s", sir1);
          printf ("%s\n", sir1);

          int indexInsula1 = atoi(sir1 + 6) - 1;

          legatura (g, indexInsula1, out);
      }

      if (strcmp("adauga_zbor", cerinte) == 0)
      {
          fscanf (in, "%s", sir1);
          printf ("%s ", sir1);
          fscanf (in, "%s", sir2);
          printf ("%s", sir2);
          int durata;
          fscanf(in, "%d", &durata);
          printf (" %d\n", durata);

          int indexInsula1 = atoi(sir1 + 6) - 1;
          int indexInsula2 = atoi(sir2 + 6) - 1;

          adauga_zbor (g, indexInsula1, indexInsula2, durata);
      }

      if (strcmp("anulare_zbor", cerinte) == 0)
      {
          fscanf (in, "%s", sir1);
          printf ("%s ", sir1);
          fscanf (in, "%s", sir2);
          printf ("%s\n", sir2);

          int indexInsula1 = atoi(sir1 + 6) - 1;
          int indexInsula2 = atoi(sir2 + 6) - 1;

          anulare_zbor (g, indexInsula1, indexInsula2);
      }

      if (strcmp("max_resurse", cerinte) == 0)
          max_resurse (g, island, out);

      if (strcmp("max_cantitate", cerinte) == 0)
      {
          fscanf (in, "%s", sir1);
          printf ("%s\n", sir1);
          max_cantitate_resursa (g, island, sir1, out);
      }

      //vector predecesori
      T = (int *)malloc(g->N * sizeof(int));
      for (int i = 0; i < g->N; i++)
          T[i] = -1;
      //retinte pt fiecare nod val costului/timpul
      D = (int *)malloc(g->N * sizeof(int));
      //vector cu val 1 si 0 daca nodul a fost sau nu vizitat
      viz = (int *)calloc(g->N, sizeof(int));

      if (strcmp("drum_zbor", cerinte) == 0)
      {
          fscanf (in, "%s", sir1);
          printf ("%s ", sir1);
          fscanf (in, "%s", sir2);
          printf ("%s\n", sir2);

          int indexInsula1 = atoi(sir1 + 6) - 1;
          int indexInsula2 = atoi(sir2 + 6) - 1;

          Dijkstra (g, indexInsula1, T, D, viz);
          drum_zbor (indexInsula2, T, out);
          after_Dijkstra (g);
          fprintf (out,"\n");
      }

      if (strcmp("timp_zbor", cerinte) == 0)
      {
          fscanf (in, "%s", sir1);
          printf ("%s ", sir1);
          fscanf (in, "%s", sir2);
          printf ("%s\n", sir2);

          int indexInsula1 = atoi(sir1 + 6) - 1;
          int indexInsula2 = atoi(sir2 + 6) - 1;

          Dijkstra (g, indexInsula1, T, D, viz);
          timp_zbor (D, indexInsula2, out);
          after_Dijkstra (g);
      }

      if (strcmp("min_zbor", cerinte) == 0)
      {
        fscanf (in, "%s", sir1);
        printf ("%s ", sir1);
        fscanf (in, "%s", sir2);
        printf ("%s\n", sir2);

        int indexInsula1 = atoi(sir1 + 6) - 1;
        int indexInsula2 = atoi(sir2 + 6) - 1;

        Dijkstra (g, indexInsula1, T, D, viz);

        int zbor = min_zbor(indexInsula2, T, D);
        //daca drumul minim la care se adauga timpul de stationare este mai
        //mare decat drumul direct dintre cele doua insule
        if (g->matrice[indexInsula1][indexInsula2] < zbor)
        //atunci se va afisa drumul direct (mai mic) dintre acestea
            fprintf (out, "%d\n", g->matrice[indexInsula1][indexInsula2]);
        else fprintf (out, "%d\n", zbor);
          after_Dijkstra (g);
      }

    *x = 0;
    } while (!feof(in));

    //eliberare memorie
    free (D);
    free (T);
    free (viz);

    if (*x)  //conditie care verifica daca x s-a schimbat in while
     {
        int **m;
        island = realloc (island, *x);
        citire_bkt (island, in, *x, &m);
        if (nr_caz1 (island, *x) == 1) afisare_avioane (island, *x, out);
        else if (nr_caz2 (island, *x) == 1) fprintf (out, "Stack overflow!");
        else for (int i = 0; i < *x; i++)
        {
          if (island[i].nrAvioane > island[i].tolAvioane)
          BKT (0, 0, island, *x, m, i, out);
        }
        for (int i = 0; i < *x; i++)
            free (m[i]);
        free (m);
      }
}
