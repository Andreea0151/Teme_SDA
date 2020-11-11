#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//Definire structuri de date
//Structura pentru jucator:
typedef struct Player
{
   char *last_name;   //nume
   char *first_name;  //prenume
   int score;         //scor
} Player;
//Structura pentru tara:
typedef struct Country
{
   char *name;        //echipa
   int nr_players;    //nr de jucatori
   int global_score;  //scorul global
   Player *players;   //jucatorii
} Country;
//Structura pentru lista
typedef struct Lista
{
  Country *c;       //variabila de tip Country 
  int pozitie;      //variabila retine pozitia pe care
                    //se afla tara in vectorul de medii
  struct Lista *next, *prev;  //leg catre nodul urmator
                              //leg catre nodul anterior
} Node;
//Structura pentru Stiva
typedef struct Stiva
{
    Country *tara;      //variabila de tip Country
    struct Stiva *next; //leg catre nodul urmator
}Stiva;
//Structura pentru cei 2 jucatori
//care se afla in meci
typedef struct Meci
{
    Player *player1;
    Player *player2;
    struct Meci *next;
}Meci;
//Structura ce contine primul, respectiv ultimul element
typedef struct Coada
{
    Meci *front,*rear;
}Queue;
//Structura pentru arbore
typedef struct Arbore
{
  Player *jucatori;           //variabila de tip Player
  struct Arbore *left,*right; //legatura catre subarborii din stanga si dreapta
}Arbore;
//declarare functii
void alocare_Player (int n, Player **players);
void citire_Player (int n, Player *players, FILE *f);
void citire_Country (Country *c,Player *players, FILE *f);
Node *creare(int *n, FILE *f);
void afisare_tari (Node *head, FILE *g);
void vector (int **v,FILE *in);
void scor_initial(Node **head, float **med);
int scor_minim (int nr,float *med);
void stergere_el_vector (float **v,int *n,int pozitie);
void stergere_tara (Node **head, int n,float *med);
void loop_stergere (int *n,Node **head,float **med);
void push (Stiva **top, Node *head);
int isEmpty_stack(Stiva *top);
Country *pop(Stiva **top);
Queue* createQueue();
void enQueue(Queue*q, Meci *joc);
int isEmpty_queue(Queue*q);
Meci *deQueue(Queue *q);
void etapa(Stiva **top,Stiva **WINNER,FILE *g);
void meci(Country *c1,Country *c2,Stiva **WINNER,FILE *g);
void punctaj (Meci *m,int *local_score1,int *local_score2);
int maxim (Country *c);
Country *verificare(Country *c1,Country *c2,int local_score1,int local_score2);
void push_WINNER(Stiva **top, Country *winner);
void afisare_stiva (Stiva *top,FILE *g);
int Stack_size (Stiva *WINNER);
Arbore *radacina (Player *player);
Arbore *inserare (Arbore *Node,Player *player);
void afisare_arbore(Arbore *root,FILE *g);
void alocare_memorie(FILE *in, Player **p);
void citire_cerinta5(FILE *in,Player *p1, Player *p2);
int cautare (Arbore *root,Player *p);
int numarare_jucatori (Arbore *node, int x,int y);
