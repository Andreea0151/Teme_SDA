#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "Tema1.h"
void vector (int **v,FILE *in)
{
    //functie creare vector
    int x;
    //alocare spatiu
    (*v) = (int*)malloc(0);
    for (int i = 0; i < 5; i++)
    {
      fscanf(in,"%d",&x); //citire elemente din fisierul cerinte.in
      (*v) = (int*)realloc(*v,sizeof(int)); //vector este realocat
      (*v)[i] = x;  //se introduc valorile in vector
    }
}
void alocare_Player (int n, Player **players)
{
    //alocare spatiu pentru vectorul de jucatori
    *players = (Player*)malloc(n * sizeof(Player));
}
void citire_Player (int n, Player *players, FILE *f)
{
    //buffer folosit pentru citirea cuvintelor
    char sir1[50],sir2[50],sir3[50];
    for (int i = 0; i < n; i++)
    {
      fscanf (f,"%s",sir1); //citire sir1 - numele jucatorului
      players[i].last_name = malloc(sizeof(char) * (strlen(sir1) + 1)); //alocare spatiu pt sirul de caractere
      strcpy (players[i].last_name,sir1); //se copiaza numele in campul alocat din structura
      fscanf (f,"%s",sir2); //citire sir2 - prenumele jucatorului
      fscanf (f,"%s",sir3); //sir auxiliar pentru situatia in care
                            //jucatorul are 2 prenume
      if (!isdigit(sir3[0]))  //daca nu e caracter cifra
      {
          strcat (sir2,"-");  //concatenare "-" la primul prenume
          strcat(sir2,sir3);  //concatenarea celui de-al doilea prenume la primul
                              // => un singur sir de caractere
          //alocare memorie
          players[i].first_name = (char*)malloc((strlen(sir2) + 1) * sizeof(char));
          strcpy (players[i].first_name,sir2);  //sirul2 este copiat in structra
          fscanf (f,"%d",&players[i].score);    //in urma verificarii
                                                //se citeste scorul jucatorului
      }
      else  //daca urmatorul caracter este cifra
      {
        //se aloca spatiu pentru un singur prenume
        players[i].first_name = (char*)malloc((strlen(sir2) + 1) * sizeof(char));
        strcpy (players[i].first_name,sir2);  //acesta este copiat in structura
        players[i].score = atoi (sir3);     //transforma sirul in numar
      }
    }
}
void citire_Country (Country *c,Player *players, FILE *f)
{
    //buffer folosit pentru citirea tarii
    char buffer[50];
    fscanf (f,"%s",buffer); //citire denumire
    c->name = (char*)malloc((strlen(buffer)+1)*sizeof(char)); //alocare memorie
    strcpy (c->name,buffer);  //numele tarii este copiat in structura
    citire_Player(c->nr_players,c->players,f);  //apelare citire jucatori
}
Node* creare(int *n, FILE *f)
{
//functie creare lista dublu inlantuita
    Country *c;
    Node *head, *final, *new;
    //alocare spatiu pentru santinela
    Node *s = (Node*)malloc(sizeof(Node));
    //nodul dummy "pointeaza" catre sine
    s -> next = s;
    s -> prev = s;
    //alocare spatiu pentru primul si ultimul nod
    head = (Node*)malloc(sizeof(Node));
    final = (Node*)malloc(sizeof(Node));
    final = head = NULL;
    fscanf (f,"%d",n);  //citire n = numarul de echipe din fisier
    for (int i = 0; i < *n; i++)
    {
        //aloca spatiu pentru un nou nod
        new = (Node*)malloc(sizeof(Node));
        if (new == NULL)  //verificare alocare
        {
          printf ("Alocare esuata");
          exit (1);
        }
        //alocare memorie pentru variabila ce reprezinta tara
        c = (Country*)malloc(sizeof(Country));
        fscanf (f,"%d",&(c->nr_players)); //citire numar de jucatori
        //apelare functie pentru alocarea de memorie a vectorului de jucatori
        alocare_Player (c->nr_players,&(c->players));
        //apelare functie citire tari
        citire_Country(c,c->players,f);
        c -> global_score = 0;  //fiecare țară va începe turneul cu scorul global 0
        //tara este stocata in nod
        new -> c = c;

        if(head == NULL)  //daca nu exista noduri
        {
            //nodul initial este legat de santinela
            new -> next = s;
            new -> prev = s;
            s -> next = new;
            s -> prev = new;
            final = head = new;
        }
        else //altfel se adauga noduri la final, circular
             //  s- > head -> new
             //  |              |
             //final  <- ... <- new
        {
            new -> next = s;
            new -> prev = final;
            final -> next = new;
            s -> prev = new;
            final = new;
        }
    }
    return head;
}
void afisare_tari (Node *head, FILE *g)
{
    //functie care afiseaza tarile din lista
    Node *p;
    //parcurgere de la primul nod la santinela
    for(p = head; p != head->prev; p = p->next)
          fprintf(g,"%s\n",p->c->name);
}
void scor_initial(Node **head, float **med)
{
    //functie care creeaza un vector de medii
    //pentru scorurile jucatorilor din fiecare tara
    int nr = 0;
    //alocare spatiu pentru vectorul de medii
    (*med) = (float*)malloc(0);
    Node *p;
    for (p = *head; p != (*head)->prev; p = p->next)
    {
      float s = 0.00;
      float m = 0.00;
      for (int i = 0; i < p->c-> nr_players; i++)
      //calculare scor initial
      //media scorurilor initiale ale jucatorilor dintr-o tara
          s += p->c->players[i].score;
          m = s / p ->c ->nr_players;
      //campul "pozitie" retine pozitia pe care se afla scorul initial
        p -> pozitie = nr;
        (nr)++;
        (*med) = (float*)realloc(*med,sizeof(float) * (nr));  //realocarea memoriei
        (*med)[nr-1] = m; //se introduce media in vector
    }
}
int scor_minim (int nr,float *med)
{
    //functie care calculeaza scorul minim din vectorul med
    int i,j;
    //se initializeaza o variabila cu prima valoare din vector
    float mn = med[0];
    for (i = 0; i < nr; i++)
        if (mn > med[i]) //daca este gasita o valoare mai mica decat cea initiala
        {
            mn = med[i];  //minimul ia valoarea respectiva
            j = i;        //este retinuta pozitia minimului
        }
    return j; //se returneaza pozitia scorului minim din vector
}
void stergere_el_vector (float **v,int *n,int pozitie)
{
    //functia sterge un element dintr-un vector
      for (int i = pozitie; i < *n; i++)  //incepand cu pozitia
                                          //de unde se doreste eliminarea
      {
          (*v)[i] = (*v)[i+1];  //elementele sunt mutate un spatiu la stanga
      }
      (*n)--; //decrementare
      *v = (float*)realloc(*v,(*n)*sizeof(float));  //realocare
}
void stergere_tara (Node **head, int n,float *med)
{
    Node *p = *head;
    if (*head == (*head)->prev) return;
    Node *headcopy = *head;
    //stergere cap lista
  //daca pozitia nodul este aceeasi cu pozitia
  //pe care se afla tara cu scor initial minim
    if (headcopy->pozitie == scor_minim(n,med))
    {
        *head = (*head)->next;
        if ((*head) != (*head)->prev) (*head)->prev = (*head)->prev->prev;
        free(headcopy);
        for (p = *head; p != (*head)->prev; p = p->next)
              (p->pozitie)--; //decrementare nr pozitie
        return;
    }
  //daca pozitia nodului curent nu coincide cu pozitia
  //pe care se afla tara cu scor minim
    while (headcopy->pozitie != scor_minim(n,med))
      {
        headcopy = headcopy->next;  //se continua parcurgerea
      }
    Node *copy = headcopy->next;  //dupa iesirea din ciclul repetitiv, se retine
                                  //adresa pe care se afla scorul minim
    if(headcopy->next != (*head)->prev) (headcopy->next)->prev = headcopy->prev;
    if (headcopy->prev != (*head)->prev) (headcopy->prev)->next = headcopy->next;
    free(headcopy);
    for (p = copy; p != (*head)->prev; p = p->next)
            (p->pozitie)--; //decrementare pozitie
    return;
}
void loop_stergere (int *n,Node **head,float **med)
{
//se vor elimina țări, până când numărul lor
//devine cea mai mare putere a lui 2 posibilă
    int x = 0;
    do
    {
      x++;  //se mareste valoarea lui x
    }
    while(*n > pow(2,x)); //pana cand numarul tarinlor devine > 2^x
    int new_nr = pow(2,(x-1));
    do  //stergere
    {
    //apelare functie pentru stergerea tarii
        stergere_tara (&(*head), *n, *med);
    //odata stearsa tara cu scor minim, este eliminat si scorul minim din vect med
        stergere_el_vector(&(*med), &(*n), scor_minim(*n,*med));
    } while(*n != new_nr);
}
void push(Stiva **top, Node *head)
{
  //functie care creeaza un nou nod pe stiva
  //si pune tara din lista in acest nod

  //alocare spatiu pentru un nod
    Stiva *newNode = (Stiva*)malloc(sizeof(Stiva));
  //se transefa tara din nodul din lista(head) in nodul din stiva(newNode)
    newNode->tara = head->c;
  //se seteaza adresa urmatorului nod ca fiind nivelul anterior
    newNode->next = *top;
  //se schimba adresa varfului
    *top = newNode;
}
int isEmpty_stack(Stiva *top)
{
  //functie care verifica daca stiva e goala
    return top == NULL;
}
Country *pop(Stiva **top)
{
  // functie care returneaza informatia stocata in varf
  // si se stege nodul

  //daca stiva e goala se returneze NULL
    if (isEmpty_stack(*top)) return NULL;
  //se stocheaza adresa varfului in temp
    Stiva *temp = (*top);
  //se stocheaza valoarea din varf in aux
    Country *aux = temp->tara;
  //se sterge elementul din varf
    *top = (*top)->next;
  //eliberarea spatiului ocupat de temp
    free(temp);
    return aux;
}
Queue *createQueue()  //functie pentru crearea cozii
{
    Queue *q;
    q = (Queue *)malloc(sizeof(Queue));
    if (q == NULL) return NULL;
    q->front = q->rear = NULL;
    return q;
}
void enQueue(Queue*q, Meci *joc)
{
  //functie pentru adaugarea unui meci in coada
    Meci* newNode;
    newNode = joc;  //newNode ia adresa jocului
  //seteaza pointerul spre urmatorul element ca fiind gol
    newNode->next = NULL;
  //daca nu exista niciun nod in coada
  //nodurile noi se adauga la finalul cozii
    if (q->rear == NULL) q->rear = newNode;
    else
    {
        (q->rear)->next = newNode;
        (q->rear) = newNode;
    }
    // daca e singurul element din coada
    if (q->front == NULL) q->front = q->rear;
}
int isEmpty_queue(Queue*q)
{
  //functie verifica daca este goala coada
    return (q->front == NULL);
}
Meci *deQueue(Queue *q)
{
  //functie pentru scoaterea unui element din coada
    Meci* aux;
  //daca coada este goala returneaza NULL
    if (isEmpty_queue(q)) return NULL;
    aux = q->front;
    q->front = (q->front)->next;
    return aux;
}
void punctaj (Meci *m,int *local_score1,int *local_score2)
{
  //functia calculeaza punctajul obtinut
  //de jucatori pe parcursul unui meci

  //jucătorului cu scorul personal mai mare
    if (m->player1->score > m->player2->score)
    {
      //i se adaugă 5 puncte la scorul personal
      m->player1->score += 5;
      //țara pentru care joacă primește 3 puncte la scorul local
      *local_score1 += 3;
    }
    else if (m->player2->score > m->player1->score)
    {
      m->player2->score += 5;
      *local_score2 +=3;
    }
    //daca cei doi jucători au scoruri personale egale
    else
    {
    //fiecărui jucător i se adaugă câte 2 puncte la scorul personal
      m->player1->score += 2;
      m->player2->score += 2;
    //fiecare țară primește câte 1 punct la scorul local
      *local_score1 += 1;
      *local_score2 += 1;
    }
}
int maxim (Country *c)
{
//functie care returneaza jucătorul cu
//cel mai mare scor personal dintr-o tara
    int mx = 0;
    for (int i = 0; i <c->nr_players; i++)
    if (mx < c->players[i].score)
        mx = c->players[i].score;
    return mx;
}
Country *verificare(Country *c1,Country *c2,int local_score1,int local_score2)
{
//celor doua țări li se adaugă scorul local, acumulat pe
//parcursul meciurilor dintre jucători, la scorul global
    c1->global_score += local_score1;
    c2->global_score += local_score2;
//câștigătoare este țara cu scorul local mai mare
    if (local_score1 > local_score2) return c1;
    else if (local_score2 > local_score1) return c2;
//daca scorurile locale ale celor două țări sunt egale, câștigătoare este
//țara în lotul căreia se află jucătorul cu cel mai mare scor personal
    else if (maxim(c1) > maxim(c2)) return c1;
    else if (maxim(c1) < maxim(c2)) return c2;
//altfel, câștigătoare este prima țară scoasă din stivă.
    else return c1;

}
void meci(Country *c1,Country *c2, Stiva **WINNER,FILE *g)
{
    Queue *q;
    q = createQueue (); //creare coada
    //jucatorii primei tari
    for (int i = 0; i < c1->nr_players; i++)
    {
        //si jucatorii celei de-a 2a tari
        for (int j = 0; j < c2->nr_players; j++)
        {
          Meci *m;
          m = malloc (sizeof(Meci));  //alocare memorie
          m->player1 = &c1->players[i]; //jucatorul din meci ia adresa
          m->player2 = &c2->players[j]; //jucatorului din tara care se va confrunta
          enQueue(q,m); //sunt adaugate meciurile dintre jucatori
        }
    }
    fprintf (g,"\n");
    fprintf (g,"%s %d ",c1->name,c1->global_score);
    fprintf (g,"-----");
    fprintf (g," %s %d\n",c2->name,c2->global_score);
    int local_score1 = 0;
    int local_score2 = 0;
    while (!isEmpty_queue(q)) //cat timp exista meciuri in coada
    {
        Meci *actual = deQueue(q);  //meciul jucat este eliminat
        fprintf (g,"%s %s %d ",actual->player1->last_name,actual->player1->first_name,actual->player1->score);
        fprintf (g,"vs");
        fprintf (g," %s %s %d",actual->player2->last_name,actual->player2->first_name,actual->player2->score);
        fprintf (g,"\n");
        punctaj(actual,&local_score1,&local_score2);  //se calculeaza punctajul jucatorilor si al tarinlor obtinute
                                                      //in urma meciului, apeland functia "punctaj"
        free(actual);  //se elibereaza memoria
    }
    //tara castigatoare este adaugata in stiva WINNER
    push_WINNER(WINNER,verificare(c1,c2,local_score1,local_score2));
}
void etapa(Stiva **top,Stiva **WINNER,FILE *g)
{
    Country *tara1, *tara2;
//se scoat din stivă succesiv câte 2 țări
    do
    {
      tara1 = pop(top);
      tara2 = pop(top);
      //între care urmează să fie jucate meciurile
      meci(tara1,tara2,WINNER,g);
    }
    while (!isEmpty_stack(*top));   //până când stiva este goală
}
void push_WINNER(Stiva **top, Country *winner)
{
//functia creeza o stivă WINNER, în care sunt adăugate
//țările câștigătoare în urma meciurilor
    Stiva* newNode = (Stiva*)malloc(sizeof(Stiva));
    newNode->tara = winner;
    newNode->next = *top;
    *top = newNode;
}
void afisare_stiva (Stiva *top,FILE *g)
{
    Stiva *s;
    for (s = top; s != NULL; s = s->next)
    fprintf (g,"%s --- %d\n",s->tara->name, s->tara->global_score);
}
int Stack_size (Stiva *WINNER)
{
//functia returneaza numarul de elemente al stivei
    int nr = 0;
    Stiva *p;
    for (p = WINNER; p != NULL; p = p->next)
          nr++;
    return nr;
}
Arbore *radacina (Player *player)
{
   //functie creare radacina
    //alocare spatiu nod
    Arbore *root = (Arbore*)malloc (sizeof(Arbore));
    root -> left = root -> right = NULL;
    //se adauga jucator
    root -> jucatori = player;
    return root;
}
Arbore *inserare (Arbore *Node,Player *player)
{
  //functie creare BST
    if (Node == NULL) return radacina(player);
    //se insereaza jucatori in arbore în funcție de scorul personal
    //obținut odată cu terminarea ultimei etape a turneului
    if (player->score < Node->jucatori->score) Node->left = inserare(Node->left,player);
    else if (player->score > Node->jucatori->score) Node->right = inserare(Node->right,player);
//daca unul dintre jucători are același scor personal cu unul deja existent în arbore
    else if (player->score == Node->jucatori->score)
    {
      //este comparat intai numele de familie
      if (strcmp(player->last_name,Node->jucatori->last_name) < 0) Node->jucatori = player;
      //in cazul egalitatii se compara prenumele jucatorilor
      else if (strcmp(player->last_name,Node->jucatori->last_name) == 0 &&
              strcmp (player->first_name,Node->jucatori->first_name) < 0) Node->jucatori = player;
    }
    return Node;
}
void afisare_arbore(Arbore *root,FILE *g)
{
//functie care afiseaza jucatorii aflati in arborele binar de cautare
//în ordine descrescătoare după scorul personal al acestora (DRS)
    if (root != NULL)
    {
      afisare_arbore(root -> right,g);
      fprintf(g,"%s %s %d\n",root->jucatori->last_name,root->jucatori->first_name,root->jucatori->score);
      afisare_arbore(root -> left,g);
    }
}
void alocare_memorie(FILE *in, Player **p)
{
  //functie care aloca memorie unui jucator
  //(unei variabile de tip Player)
    *p = (Player*)malloc(sizeof(Player));
}
void citire_cerinta5(FILE *in, Player *p1, Player *p2)
{
// citirea celor 2 jucatori din fisierul cerinte.in
    //buffer folosit pentru citire
    char buffer[50];
    fscanf (in,"%s",buffer);
    p1->last_name = (char*)malloc((strlen(buffer) + 1) * sizeof(char)); //alocare memorie
    strcpy(p1->last_name,buffer); //copiaza numele in structura
    fscanf (in,"%s",buffer);
    p1->first_name = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
    strcpy(p1->first_name,buffer);
    fscanf (in,"%d",&p1->score);  //citire scor jucator

    fscanf (in,"%s",buffer);
    p2->last_name = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
    strcpy(p2->last_name,buffer);
    fscanf (in,"%s",buffer);
    p2->first_name = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
    strcpy(p2->first_name,buffer);
    fscanf (in,"%d",&p2->score);
}
int cautare (Arbore *root,Player *p)
{
//functia verifica daca un jucător din cei doi primiți ca input nu se găsește în BST
    if (root == NULL) return 0;
    else if (strcmp(root->jucatori->last_name,p->last_name) == 0
              && strcmp (root->jucatori->first_name,p->first_name) == 0)
              return 1;
        else return cautare(root->left,p) || cautare (root->right,p);
}
int numarare_jucatori (Arbore *node,int x,int y)
{
  //functia număra jucătorii care au scorul personal inclus între scorurile
  //personale ale celor 2 jucatori cititi din fisierul cerinte.in
    if (node != NULL)
    {
      if (node->jucatori->score > x && node->jucatori->score < y)
              return 1 + numarare_jucatori (node->left,x,y) +
                         numarare_jucatori (node->right,x,y);
      else return numarare_jucatori(node->left,x,y) + numarare_jucatori (node->right,x,y);
    }
}
