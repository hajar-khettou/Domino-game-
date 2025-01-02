#ifndef JOUEUR_H
#define JOUEUR_H

#include "domino.h"

typedef struct Joueur {
    char pseudo[50];
    int score;
    Domino *pieces;
    int est_humain;
} Joueur;


void ajouter_joueur_domino(Joueur *joueurs, const char *pseudo, int est_humain);
Domino* piocher_domino(Domino **pioche);
void afficher_dominos_joueur(Domino *pieces);

#endif 
