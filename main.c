#include "jeu.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    int nombre_joueurs;

    printf("Entrez le nombre de joueurs (2-4) : ");
    scanf("%d", &nombre_joueurs);

    if (nombre_joueurs < 2 || nombre_joueurs > 4) {
        printf("Le nombre de joueurs doit être entre 2 et 4.\n");
        return EXIT_FAILURE;
    }

    Jeu *jeu = initialiser_jeu(nombre_joueurs);
    printf("Le jeu est initialisé avec succès !\n");

    while (1) {
        afficher_plateau_dominos(jeu->plateau);

        jouer_tour(jeu);

        // Vérifier si la partie est bloquée
        verifier_partie_bloquee(jeu);

        for (int i = 0; i < jeu->nombre_joueurs; i++) {
            if (jeu->joueurs[i].pieces == NULL) {
                printf("\n%s a gagné la partie en posant tous ses dominos !\n", jeu->joueurs[i].pseudo);
                return 0;
            }
        }
    }

    return 0;
}

