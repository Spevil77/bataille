#ifndef CARTE_H
#define CARTE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Carte
{
    int valeur;            // 2 a 14 (As = 14)
    char couleur[10];      // Coeur, Carreau, Trefle, Pique
    struct Carte *suivant; // Pointeur vers la carte suivante
} Carte;
extern Carte *tete;
extern Carte *queue;

#endif