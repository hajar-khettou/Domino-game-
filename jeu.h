#ifndef JEU_H
#define JEU_H

#include "domino.h"
#include "joueur.h"

typedef struct Jeu {
    int nombre_joueurs;
    Joueur joueurs[4];
    Domino *pioche;  
    Domino *plateau;           
    int joueur_actuel;// présente l'index du joueur actuel
} Jeu;

Jeu* initialiser_jeu(int nombre_joueurs);
int peut_jouer(Domino *main, int gauche, int droite);
int jouer_domino(Jeu *jeu, Domino *domino);
int verifier_domino_valide(Domino *domino, Domino *plateau);
void calculer_score_domino(Joueur *joueur, Domino *domino);
void afficher_plateau_dominos(Domino *plateau);
void jouer_tour(Jeu *jeu);
void verifier_partie_bloquee(Jeu *jeu);


#endif
