#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include "automate.h"

int main()
{
    char mot[64];
    char nomfichier[MAXIMUM_NOMFICHIER];

    printf("[automate]: nom du fichier: ");
    scanf("%s", nomfichier);

    Automate *automate = construireAutomate(nomfichier);

    if (automate == NULL)
    {
        printf("[system]: erreur de la creation de l'automate\n");
        return 1;
    }

    printf("\n");
    affichageAutomate(automate);

    bool testDeterministe = estDeterministe(automate);

    printf("\n");

    if (testDeterministe)
        printf("[automate]: deterministe\n");

    printf("[automate]: donnez un mot: ");
    scanf("%s", mot);

    bool testMot = test(automate, mot);
    if (testMot)
        printf("[automate]: \"%s\" appartient au langage\n", mot);
    else
        printf("[automate]: \"%s\" n'appartient pas au langage\n", mot);

    detruireAutomate(automate);

    return 0;
}