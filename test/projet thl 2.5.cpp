#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_ETATS 100
#define MAX_SYMBOLES 100

typedef struct Transition {
    int symb;
    int dest;
    struct Transition *suiv;
} Transition;

typedef struct Etat {
    int nom;
    bool final;
    Transition *tliste;
} Etat;

typedef struct Automate {
    int nbSymboles;
    int alphabet[MAX_SYMBOLES];
    int nbEtat;
    int nbEtatsFinaux;
    Etat etats[MAX_ETATS];
} Automate;

int nombre_etats;
int nombre_symboles;
int nombre_etats_finaux;
int ensemble_E[MAX_ETATS];
int ensemble_sigma[MAX_SYMBOLES];
int etat_initial;
int ensemble_F[MAX_ETATS];
int transitions[MAX_ETATS][MAX_SYMBOLES];

void recupererAut(const char *nom_fichier) {
    int i;
    char ligne[100];
    FILE *fichier = fopen(nom_fichier, "r");
    fgets(ligne, sizeof(ligne), fichier);
    sscanf(ligne, "%d %d %d", &nombre_etats, &nombre_symboles, &nombre_etats_finaux);

    fgets(ligne, sizeof(ligne), fichier);
    char *token = strtok(ligne, "{,}");
    for (i = 0; i < nombre_etats; i++) {
        ensemble_E[i] = atoi(token);
        token = strtok(NULL, "{,}");
    }

    fgets(ligne, sizeof(ligne), fichier);
    token = strtok(ligne, "{,}");
    for (i = 0; i < nombre_symboles; i++) {
        ensemble_sigma[i] = atoi(token);
        token = strtok(NULL, "{,}");
    }

    fgets(ligne, sizeof(ligne), fichier);
    sscanf(ligne, "%d", &etat_initial);

    fgets(ligne, sizeof(ligne), fichier);
    token = strtok(ligne, "{,}");
    for (int i = 0; i < nombre_etats_finaux; i++) {
        ensemble_F[i] = atoi(token);
        token = strtok(NULL, "{,}");
    }

    for (i = 0; i < nombre_etats; i++) {
        for (int j = 0; j < nombre_symboles; j++) {
            fscanf(fichier, "delta(%d, %d) = %d\n", &transitions[i][j], &ensemble_sigma[j], &transitions[i][j]);
        }
    }

    fclose(fichier);
}

void construireAut(Automate *aut) {
    aut->nbSymboles = nombre_symboles;
    for (int i = 0; i < nombre_symboles; i++) {
        aut->alphabet[i] = ensemble_sigma[i];
    }

    aut->nbEtat = nombre_etats;
    aut->nbEtatsFinaux = nombre_etats_finaux;
    for (int i = 0; i < nombre_etats; i++) {
        aut->etats[i].nom = ensemble_E[i];
        aut->etats[i].final = false;
        for (int j = 0; j < nombre_etats_finaux; j++) {
            if (ensemble_E[i] == ensemble_F[j]) {
                aut->etats[i].final = true;
                break;
            }
        }
        aut->etats[i].tliste = NULL;
    }

    for (int i = 0; i < nombre_etats; i++) {
        for (int j = 0; j < nombre_symboles; j++) {
            int dest = transitions[i][j];
            Transition *new_transition = (Transition *)malloc(sizeof(Transition));
            new_transition->symb = ensemble_sigma[j];
            new_transition->dest = dest;
            new_transition->suiv = aut->etats[i].tliste;
            aut->etats[i].tliste = new_transition;
        }
    }
}

void afficherAut(Automate *aut) {
    printf("Nombre d'etats : %d\n", aut->nbEtat);
    printf("Nombre de symboles : %d\n", aut->nbSymboles);
    printf("Alphabet : {");
    for (int i = 0; i < aut->nbSymboles; i++) {
        printf("%d", aut->alphabet[i]);
        if (i < aut->nbSymboles - 1)
            printf(", ");
    }
    printf("}\n");

    printf("Etats : {");
    for (int i = 0; i < aut->nbEtat; i++) {
        printf("%d", aut->etats[i].nom);
        if (i < aut->nbEtat - 1)
            printf(", ");
    }
    printf("}\n");

    printf("Etat initial : %d\n", etat_initial);

    printf("Etats finaux : {");
    for (int i = 0; i < aut->nbEtatsFinaux; i++) {
        printf("%d", ensemble_F[i]);
        if (i < aut->nbEtatsFinaux - 1)
            printf(", ");
    }
    printf("}\n");

    printf("Transitions :\n");
    for (int i = 0; i < aut->nbEtat; i++) {
        Transition *transition = aut->etats[i].tliste;
        while (transition != NULL) {
            printf("delta(%d, %d) = %d\n", aut->etats[i].nom, transition->symb, transition->dest);
            transition = transition->suiv;
        }
    }
}

int main() {
    const char *nom_fichier = "automate.txt";
    recupererAut(nom_fichier);
    Automate aut;
    construireAut(&aut);
    afficherAut(&aut);

    return 0;
}

