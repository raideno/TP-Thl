#include "automate.h"

#include "utils.h"
#include "list.h"

#include <string.h>

Automate *construireAutomate(const char *fichier)
{
    int *Etats, *EtatsFinaux;

    FILE *file = NULL;

    file = fopen(fichier, "r");

    if (file == NULL)
        return NULL;

    Automate *automate = (Automate *)malloc(sizeof(automate));

    if (automate == NULL)
        return NULL;

    fscanf(file, "%d, %d, %d\n", &automate->nbEtats, &automate->nbSymboles, &automate->nbEtatsFinaux);

    automate->alphabet = NULL;
    automate->etats = (Etat **)malloc(sizeof(Etat *) * automate->nbEtats);

    Etats = readIntListFromArray(file, automate->nbEtats);

    automate->alphabet = readCharListFromArray(file, automate->nbSymboles);

    fscanf(file, "%d\n", &automate->etatInitial);

    EtatsFinaux = readIntListFromArray(file, automate->nbEtatsFinaux);

    for (int i = 0; i < automate->nbEtats; i++)
    {
        bool isFinal = intArrayIncludes(EtatsFinaux, Etats[i], automate->nbEtatsFinaux) != -1;
        Etat *etat = construireEtat(Etats[i], isFinal);

        automate->etats[i] = etat;
    }

    int Source, Destination;
    char Symbole;
    while (EOF != fscanf(file, "delta(%d, %c) = %d\n", &Source, &Symbole, &Destination))
    {
        Etat *etat = automate->etats[Source];

        Transition *transition = construireTransition(Source, Symbole, Destination);

        // List *node = createNode((void *)transition, NULL);
        // etat->transitions = pushNode(etat->transitions, node);

        ajouterTransition(etat, transition);
    }

    free(Etats);

    free(EtatsFinaux);

    // fclose(file);

    return automate;
}

bool estDeterministe(Automate *automate)
{
    bool result = true;

    for (int i = 0; i < automate->nbEtats; i++)
    {
        Etat *etat = automate->etats[i];

        int j = 0;
        char array[10];

        List *transitionList = etat->transitions;

        while (transitionList != NULL)
        {
            // array[j] = transitionList
            Transition *transition = (Transition *)transitionList->value;

            if (charArrayIncludes(array, transition->symbole, j) != -1)
            {
                printf("FOUND A NON-DETERMINISTE TRANSITION\n");
                printf("ETAT (%d) (%d, %c, %d)\n", etat->numero, transition->source, transition->symbole, transition->destination);
                result = false;
            }

            array[j] = transition->symbole;
            j++;

            transitionList = transitionList->next;
        }
    }

    return result;
}

Etat *parcours(Automate *automate, Etat *etatActuel, char symbole)
{
    Transition *transition = NULL;
    Etat *etat = NULL;
    List *transitionsList = etatActuel->transitions;

    while (transitionsList != NULL)
    {
        transition = (Transition *)transitionsList->value;

        if (transition->symbole == symbole)
            break;

        transitionsList = transitionsList->next;
    }

    etat = getEtat(automate, transition->destination);

    return etat;
}

bool test(Automate *automate, const char *mot)
{
    // what if there is no route ?

    int length = strlen(mot);

    Etat *etat = getInitialState(automate);

    for (int i = 0; i < length; i++)
    {
        char symbole = mot[i];

        printf("(%d, %c, %s) %s", etat->numero, symbole, etat->estFinal ? "final" : "no-final", i == length - 1 ? "\n" : "=>");

        etat = parcours(automate, etat, symbole);
    }

    return etat->estFinal;
}

void affichageAutomate(Automate *automate)
{
    printf("- Nombre D'Etats: %d\n", automate->nbEtats);
    printf("- Nombre De Symboles: %d\n", automate->nbSymboles);
    printf("- Nombre D'Etats Finaux: %d\n", automate->nbEtatsFinaux);

    printf("- Symboles: ");
    printCharArray(automate->alphabet, automate->nbSymboles);

    for (int i = 0; i < automate->nbEtats; i++)
    {
        Etat *etat = automate->etats[i];

        printf("\n\n");
        affichageEtat(etat);
        affichageTransitions(etat, "\t");
    }
}

void detruireAutomate(Automate *automate)
{
    free(automate->alphabet);

    for (int i = 0; i < automate->nbEtats; i++)
        detruireEtat(automate->etats[i]);

    free(automate->etats);

    // free(automate);
}

Etat *getInitialState(Automate *automate)
{
    return getEtat(automate, automate->etatInitial);
}

Etat *getEtat(Automate *automate, int numeroEtat)
{
    for (int i = 0; i < automate->nbEtats; i++)
        if (automate->etats[i]->numero == numeroEtat)
            return automate->etats[i];

    return NULL;
}

bool estInitial(Automate *automate, int numeroEtat)
{
    return automate->etatInitial == numeroEtat;
}
bool estFinal(Automate *automate, int numeroEtat)
{
    Etat *etat = NULL;

    for (int i = 0; i < automate->nbEtats; i++)
    {
        if (automate->etats[i]->numero == numeroEtat)
        {
            etat = automate->etats[i];
            break;
        }
    }

    if (etat == NULL)
        return false;

    return etat->estFinal;
}

Etat *construireEtat(int numero, bool isFinal)
{
    Etat *etat = (Etat *)malloc(sizeof(Etat));

    etat->numero = numero;
    etat->estFinal = isFinal;
    etat->transitions = NULL;

    return etat;
}

Transition *construireTransition(int source, char symbole, int destination)
{
    Transition *transition = (Transition *)malloc(sizeof(Transition));

    transition->source = source;
    transition->symbole = symbole;
    transition->destination = destination;

    return transition;
}

void affichageEtat(const Etat *etat)
{
    printf("Etat(%d): (%d transitions) %s\n", etat->numero, listLength(etat->transitions), etat->estFinal ? "final" : "non-final");
}

void affichageTransition(const Transition *transition)
{
    printf("delta(%d, %c) = %d\n", transition->source, transition->symbole, transition->destination);
}

void affichageTransitions(const Etat *etat, const char *prefix)
{
    List *tnrasitionsList = etat->transitions;
    while (tnrasitionsList != NULL)
    {
        Transition *transition = (Transition *)tnrasitionsList->value;
        printf("%s", prefix == NULL ? "\0" : prefix);
        affichageTransition(transition);
        tnrasitionsList = tnrasitionsList->next;
    }
}

void detruireEtat(Etat *etat)
{
    while (etat->transitions != NULL)
    {
        List *next = NULL;

        Transition *transition = (Transition *)etat->transitions->value;

        detruireTransition(transition);

        etat->transitions->value = NULL;

        next = etat->transitions->next;

        detruireNode(etat->transitions);

        etat->transitions = next;
    }

    free(etat);
}

void detruireTransition(Transition *transition)
{
    free(transition);
}

void ajouterTransition(Etat *etat, Transition *transition)
{
    List *node = createNode((void *)transition, NULL);

    etat->transitions = pushNode(etat->transitions, node);
}