#include "domino.h"
#include <stdlib.h>
#include <stdio.h>

// on utilise les liste pour créer le domino
Domino* creer_domino(int valeur1, int valeur2) {
    Domino *nouveau = (Domino*)malloc(sizeof(Domino));
    nouveau->valeur1 = valeur1;
    nouveau->valeur2 = valeur2;
    nouveau->suivant = NULL;
    return nouveau;
}

// on génère toutes les pièces possibles pour la pioche
void initialiser_pioche_dominos(Domino **pioche) {
    for (int i = 0; i <= 6; i++) {
        for (int j = i; j <= 6; j++) {
            Domino *nouveau = creer_domino(i, j);
            nouveau->suivant = *pioche;
            *pioche = nouveau;
        }
    }
}

// après on mélange aléatoirement la pioche
void melanger_pioche_dominos(Domino **pioche) {
    Domino *liste[28];
    int compteur = 0;

    Domino *courant = *pioche;
    while (courant) {
        liste[compteur++] = courant;
        courant = courant->suivant;
    }

    for (int i = 0; i < compteur; i++) {
        int j = rand() % compteur;
        Domino *temp = liste[i];
        liste[i] = liste[j];
        liste[j] = temp;
    }

    *pioche = liste[0];
    courant = *pioche;
    for (int i = 1; i < compteur; i++) {
        courant->suivant = liste[i];
        courant = courant->suivant;
    }
    courant->suivant = NULL;
}
