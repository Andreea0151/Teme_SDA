#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Tema1.h"
int main (int argc, char const *argv[])
{
   //fisiere intrare
   FILE *in = fopen (argv[1],"r");
   FILE *f = fopen (argv[2],"r");
   //fisier iesire
   FILE *g = fopen (argv[3],"w");
   int *v;  //vectorul ce retine valorile din cerinte.in
   vector (&v,in);
   int n;
   Node *head = NULL; //declarare lista
   if (v[0])
   {
     head = creare(&n,f);   //creare lista
     if (!v[1]) afisare_tari (head,g);  //afiseaza lista tarilor direct din verificarea
                                        //pentru cerinta1, daca nu exista cerinta2
   }
   if (v[1])
   {
     float *med;
      scor_initial (&head,&med);
     loop_stergere (&n,&head,&med); //stergere tari din lista
     free(med); //vectorul de medii nu mai este necesar,
                //se poate elibera memoria ocupata de acesta
     afisare_tari (head,g); //afisarea tarilor ramase in lista
   }
   Arbore *root = NULL; //declarare arbore
   if (v[2])
   {
     Node *p;
     Stiva *stackTop = NULL;
     for(p = head; p != head->prev; p = p->next)
        push(&stackTop,p);  //adaugare tari in stiva
     Stiva *WINNER = NULL;  //declarare stiva castigatori
     int k = 0;
     Stiva *Aux = NULL; //declarare stiva auxiliara
     do
     {
       k++; //numarul etapei
       fprintf (g,"\n");
       fprintf (g,"====== ETAPA %d ======\n",k);
       etapa(&stackTop,&WINNER,g);
       fprintf (g,"\n=== WINNER ===\n");
       afisare_stiva (WINNER,g);
       if (v[3])
       {
         if (Stack_size(WINNER) == 4) //daca in stiva WINNER
                                      //se afla 4 tari
          {
                Stiva *s;
                for (s = WINNER; s != NULL; s = s->next)
                //tarile respective se adauga intr-o stiva auxiliara
                  push_WINNER (&Aux,s->tara);
          }
      }
       do
       {
         //se scoat țările din stiva WINNER și se adăuga pe rând în stiva inițială
         push_WINNER (&stackTop,pop(&WINNER));
       }
       while (!isEmpty_stack(WINNER)); //pana cand WINNER este goala
     }
     while (Stack_size(stackTop) != 1);
//se repeta procedeele pentru fiecare etapă a turneului până
//când în stiva WINNER va rămâne o singură țară câștigătoare
     if (v[3])
     {
       Stiva *s;
       //se adauga in arbore jucătorii ultimelor 4 țări rămase în turneu
       for (s = Aux; s != NULL; s = s->next)
       {
         for (int i = 0; i < Aux->tara->nr_players; i++)
         root = inserare(root,&(s->tara->players[i]));
       }
       fprintf (g,"\n====== CLASAMENT JUCATORI ======\n");
       afisare_arbore(root,g);
       while (isEmpty_stack(Aux))
       {
          free (Aux); //nu mai este nevoie de stiva auxiliara
          Aux = Aux->next;
       }
     }
   }
   if (v[4])
   {
     Player *p1,*p2;
     alocare_memorie (in,&p1);
     alocare_memorie (in,&p2);
     citire_cerinta5(in,p1,p2);
     if (cautare(root,p1) == 1 && cautare (root,p2) == 1)
     {
//se apeleaza functia numarare_jucatori
//dupa ce se verifica scorul mai mic, respectiv mai mare
       if (p1->score < p2->score )
       fprintf(g,"\n%d",numarare_jucatori(root,p1->score,p2->score));
       else fprintf(g,"\n%d",numarare_jucatori(root,p2->score,p1->score));
     }
//se specifica daca unul dintre jucători nu se afla în BST
     else if (cautare(root,p1)) fprintf (g,"\n%s %s nu poate fi identificat!",
                                        p2->last_name,p2->first_name);
    else fprintf (g,"\n%s %s nu poate fi identificat!",
                                       p1->last_name,p1->first_name);
   }
   //eliberare memorie
   free(v);
   Node *a;
   for(a = head; a != head->prev; a = a->next)
      free(a);
   fclose (in);
   fclose (f);
   fclose(g);
   return 0;
}
