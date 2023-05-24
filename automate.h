#ifndef __AUTOMATE_H__
#define __AUTOMATE_H__

#define MAXIMUM_ETATS 256
#define MAXIMUM_SYMBOLES 256
#define MAXIMUM_NOMFICHIER 256

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "list.h"

typedef struct Automate Automate;
typedef struct Etat Etat;
typedef struct Transition Transition;

struct Automate
{
    int nbSymboles;
    int nbEtats;
    int nbEtatsFinaux;
    char *alphabet;
    // TODO: change to string ?
    int etatInitial;
    Etat **etats;
};

struct Etat
{
    // TODO: change to string
    int numero;
    bool estFinal;
    List *transitions;
};

struct Transition
{
    char symbole;
    // TODO: change to string
    int source;
    // TODO: change to string
    int destination;
};

char *expression(Automate *automate);

Automate *construireAutomateAPartirDExpression(const char *expression);
Automate *construireAutomateAPartirDeFichier(const char *fichier);

bool estDeterministe(Automate *automate);
Etat *parcours(Automate *automate, Etat *etatAcutel, char symbole);
bool test(Automate *automate, const char *mot);
void affichageAutomate(Automate *automate);
void detruireAutomate(Automate *automate);
Etat *getEtat(Automate *automate, int numeroEtat);
Etat *getInitialState(Automate *automate);

Etat *construireEtat(int numero, bool isFinal);
bool estInitial(Automate *automate, int numeroEtat);
bool estFinal(Automate *automate, int numeroEtat);
void affichageEtat(const Etat *etat);
void detruireEtat(Etat *etat);

Transition *construireTransition(int source, char symbole, int destination);
void ajouterTransition(Etat *etat, Transition *transition);
void affichageTransition(const Transition *transition);
void affichageTransitions(const Etat *etat, const char *prefix);
void detruireTransition(Transition *transition);

#endif