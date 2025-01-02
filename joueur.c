#include "joueur.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// pour ajouter un joueur
void ajouter_joueur_domino(Joueur *joueur, const char *pseudo, int est_humain) {
    strcpy(joueur->pseudo, pseudo);
    joueur->score = 0;
    joueur->pieces = NULL;
    joueur->est_humain = est_humain;
}

// Permet à un joueur de piocher une pièce
Domino* piocher_domino(Domino **pioche) {
    if (*pioche == NULL) {
        return NULL; // la pioche est vide
    }
    Domino *domino = *pioche;
    *pioche = (*pioche)->suivant;
    return domino;
}

// Affiche les dominos d'un joueur
void afficher_dominos_joueur(Domino *pieces) {
    printf("Main : ");
    Domino *courant = pieces;
    while (courant) {
        printf("[%d|%d] ", courant->valeur1, courant->valeur2);
        courant = courant->suivant;
    }
    printf("\n");
}
