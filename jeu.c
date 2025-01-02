#include "jeu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h> // Pour INT_MAX

int trouver_premier_joueur(Jeu *jeu) {
    int joueur_avec_double_max = -1;
    int valeur_max = -1;

    for (int i = 0; i < jeu->nombre_joueurs; i++) {
        Domino *courant = jeu->joueurs[i].pieces;
        while (courant) {
            if (courant->valeur1 == courant->valeur2 && courant->valeur1 > valeur_max) {
                valeur_max = courant->valeur1;
                joueur_avec_double_max = i;
            }
            courant = courant->suivant;
        }
    }

    if (joueur_avec_double_max == -1) {
        // Si aucun double trouvé on va choisir le domino le plus fort
        for (int i = 0; i < jeu->nombre_joueurs; i++) {
            Domino *courant = jeu->joueurs[i].pieces;
            while (courant) {
                int somme = courant->valeur1 + courant->valeur2;
                if (somme > valeur_max) {
                    valeur_max = somme;
                    joueur_avec_double_max = i;
                }
                courant = courant->suivant;
            }
        }
    }

    return joueur_avec_double_max;
}

Jeu* initialiser_jeu(int nombre_joueurs) {
    Jeu *jeu = (Jeu*)malloc(sizeof(Jeu));

    jeu->nombre_joueurs = nombre_joueurs;
    jeu->pioche = NULL;
    jeu->plateau = NULL;
    jeu->joueur_actuel = 0;

    // nous initialisons la pioche
    initialiser_pioche_dominos(&(jeu->pioche));

    // on mélanger la pioche
    melanger_pioche_dominos(&(jeu->pioche));

    // Initialisons des joueurs et on passe à la distribution des dominos
    for (int i = 0; i < nombre_joueurs; i++) {
        char pseudo[50];
        printf("Entrez le pseudo du joueur %d : ", i + 1);
        scanf("%s", pseudo);
        ajouter_joueur_domino(&(jeu->joueurs[i]), pseudo, 1);

        for (int j = 0; j < (nombre_joueurs == 2 ? 7 : 6); j++) {
            Domino *domino = piocher_domino(&(jeu->pioche));
            domino->suivant = jeu->joueurs[i].pieces;
            jeu->joueurs[i].pieces = domino;
        }
    }

    // pour trouver le premier joueur
    jeu->joueur_actuel = trouver_premier_joueur(jeu);
    printf("Le joueur %s commence avec le domino le plus fort.\n", jeu->joueurs[jeu->joueur_actuel].pseudo);

    return jeu;
}

// Vérifie si un joueur peut jouer un domino
int peut_jouer(Domino *main, int gauche, int droite) {
    Domino *courant = main;
    while (courant != NULL) {
        if (courant->valeur1 == gauche || courant->valeur2 == gauche ||
            courant->valeur1 == droite || courant->valeur2 == droite) {
            return 1;
        }
        courant = courant->suivant;
    }
    return 0;
}

// on tente à placer une pièce sur le plateau si elle est valide
int jouer_domino(Jeu *jeu, Domino *domino) {
    if (verifier_domino_valide(domino, jeu->plateau)) {
        domino->suivant = jeu->plateau;
        jeu->plateau = domino;
        return 1; // Coup valide
    }
    return 0; // Coup invalide
}

// Vérifie si une pièce peut être jouée selon les règles
int verifier_domino_valide(Domino *domino, Domino *plateau) {
    if (plateau == NULL) {
        return 1; 
    }
    int gauche = plateau->valeur1;
    int droite = plateau->valeur2;

    return (domino->valeur1 == gauche || domino->valeur2 == gauche ||
            domino->valeur1 == droite || domino->valeur2 == droite);
}

// on met à jour le score d'un joueur en fonction des pièces jouées
void calculer_score_domino(Joueur *joueur, Domino *domino) {
    joueur->score += domino->valeur1 + domino->valeur2;
}

void afficher_plateau_dominos(Domino *plateau) {
    if (plateau == NULL) {
        printf("Plateau : (vide)\n");
        return;
    }

    printf("Plateau : ");
    Domino *courant = plateau;
    while (courant) {
        printf("[%d|%d] ", courant->valeur1, courant->valeur2);
        if (courant->perpendiculaire) {
            printf("(Perp: [%d|%d]) ", courant->perpendiculaire->valeur1, courant->perpendiculaire->valeur2);
        }
        courant = courant->suivant;
    }
    printf("\n");
}


void jouer_tour(Jeu *jeu) {

    Joueur *joueur = &(jeu->joueurs[jeu->joueur_actuel]);
    printf("\nC'est au tour de %s de jouer.\n", joueur->pseudo);

    // pour vérifier s'il s'agit du premier tour (plateau vide)
    if (jeu->plateau == NULL) {
        Domino *domino = joueur->pieces;
        joueur->pieces = joueur->pieces->suivant; // Retirer le domino de la main
        domino->suivant = jeu->plateau;
        jeu->plateau = domino;

        printf("%s a joué [%d|%d] pour commencer le jeu.\n", joueur->pseudo, domino->valeur1, domino->valeur2);
        jeu->joueur_actuel = (jeu->joueur_actuel + 1) % jeu->nombre_joueurs;
        return;
    }

    Domino *prev = NULL;
    Domino *courant = joueur->pieces;

    // Tenter de jouer un domino depuis la main
    while (courant != NULL) {
        if (courant->valeur1 == jeu->plateau->valeur1 || courant->valeur2 == jeu->plateau->valeur1 ||
            courant->valeur1 == jeu->plateau->valeur2 || courant->valeur2 == jeu->plateau->valeur2) {

            // Inverser si nécessaire pour placement à gauche
            if (courant->valeur1 == jeu->plateau->valeur1 || courant->valeur2 == jeu->plateau->valeur1) {
                if (courant->valeur2 == jeu->plateau->valeur1) {
                    int temp = courant->valeur1;
                    courant->valeur1 = courant->valeur2;
                    courant->valeur2 = temp;
                }

                if (prev == NULL) {
                    joueur->pieces = courant->suivant;
                } else {
                    prev->suivant = courant->suivant;
                }

                courant->suivant = jeu->plateau;
                jeu->plateau = courant;

                printf("%s a joué [%d|%d] à gauche.\n", joueur->pseudo, courant->valeur1, courant->valeur2);
                jeu->joueur_actuel = (jeu->joueur_actuel + 1) % jeu->nombre_joueurs;
                return;
            }

            // Inverser si nécessaire pour placement à droite
            if (courant->valeur1 == jeu->plateau->valeur2 || courant->valeur2 == jeu->plateau->valeur2) {
                if (courant->valeur1 == jeu->plateau->valeur2) {
                    int temp = courant->valeur1;
                    courant->valeur1 = courant->valeur2;
                    courant->valeur2 = temp;
                }

                Domino *temp = jeu->plateau;
                while (temp->suivant != NULL) {
                    temp = temp->suivant;
                }
                temp->suivant = courant;

                if (prev == NULL) {
                    joueur->pieces = courant->suivant;
                } else {
                    prev->suivant = courant->suivant;
                }

                courant->suivant = NULL;

                printf("%s a joué [%d|%d] à droite.\n", joueur->pseudo, courant->valeur1, courant->valeur2);
                jeu->joueur_actuel = (jeu->joueur_actuel + 1) % jeu->nombre_joueurs;
                return;
            }
        }

        prev = courant;
        courant = courant->suivant;
    }

    // si aucun domino jouable il faut piocher
    printf("%s ne peut pas jouer et doit piocher.\n", joueur->pseudo);

    while (1) {
        Domino *pioche = piocher_domino(&(jeu->pioche));
        if (pioche == NULL) {
            printf("La pioche est vide. %s passe son tour.\n", joueur->pseudo);
            break;
        }

        pioche->suivant = joueur->pieces;
        joueur->pieces = pioche;

        if (pioche->valeur1 == jeu->plateau->valeur1 || pioche->valeur2 == jeu->plateau->valeur1 ||
            pioche->valeur1 == jeu->plateau->valeur2 || pioche->valeur2 == jeu->plateau->valeur2) {
            printf("%s a pioché [%d|%d] et peut jouer immédiatement.\n", joueur->pseudo, pioche->valeur1, pioche->valeur2);
            jouer_tour(jeu);
            return;
        }
    }

    jeu->joueur_actuel = (jeu->joueur_actuel + 1) % jeu->nombre_joueurs;
}




void verifier_partie_bloquee(Jeu *jeu) {

    // Vérifier si tous les joueurs ne peuvent pas jouer
    int tous_bloques = 1;
    for (int i = 0; i < jeu->nombre_joueurs; i++) {
        if (peut_jouer(jeu->joueurs[i].pieces, jeu->plateau->valeur1, jeu->plateau->valeur2)) {
            tous_bloques = 0;
            break;
        }
    }

    if (tous_bloques && jeu->pioche == NULL) {
        printf("\nLa partie est bloquée. Calcul du gagnant...\n");

        // on essaye de trouver le joueur avec le moins de dominos restants
        int index_gagnant = -1;
        int min_pieces = INT_MAX; // Nombre minimum de dominos restants
        int min_score = INT_MAX; // Score minimum pour départager en cas d'égalité

        for (int i = 0; i < jeu->nombre_joueurs; i++) {
            int pieces_restantes = 0;
            int somme_valeurs = 0;
            Domino *courant = jeu->joueurs[i].pieces;

            // Compter les dominos et calculer leur valeur totale
            while (courant) {
                pieces_restantes++;
                somme_valeurs += courant->valeur1 + courant->valeur2;
                courant = courant->suivant;
            }

            // Comparer pour trouver le gagnant
            if (pieces_restantes < min_pieces || 
                (pieces_restantes == min_pieces && somme_valeurs < min_score)) {
                min_pieces = pieces_restantes;
                min_score = somme_valeurs;
                index_gagnant = i;
            }
        }

        // Afficher le gagnant
        if (index_gagnant != -1) {
            printf("Le gagnant est %s avec %d dominos restants et un score de %d.\n",
                   jeu->joueurs[index_gagnant].pseudo, min_pieces, min_score);
        } else {
            printf("Erreur : Aucun gagnant trouvé.\n");
        }

        exit(0); // Fin de la partie
    }
}
