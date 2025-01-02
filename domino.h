
#ifndef DOMINO_H
#define DOMINO_H

typedef struct Domino {
    int valeur1;
    int valeur2;
    struct Domino *suivant;
    struct Domino *perpendiculaire; // Nouvel élément pour déplacer les doubles d'une manière perpendiculaire au contraire d'un domino normal qui se déplace horizonatalement
} Domino;

Domino* creer_domino(int valeur1, int valeur2);
void initialiser_pioche_dominos(Domino **pioche);
void melanger_pioche_dominos(Domino **pioche);

#endif 
