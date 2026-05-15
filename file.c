#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "carte.h"
#include "file.h"

Carte *tete_joueur1 = NULL;
Carte *queue_joueur1 = NULL;
Carte *tete_joueur2 = NULL;
Carte *queue_joueur2 = NULL;

void enfiler(Carte *, Carte **, Carte **);
Carte *defiler(Carte **, Carte **);
int taille_file(Carte *);
int file_vide(Carte *);

void distribution(Carte **deck_tete, Carte **tete_player1, Carte **queue_player1, Carte **tete_player2, Carte **queue_player2)
{
    int i = 0;
    Carte *parcour = *deck_tete;
    while (parcour != NULL && i < 52)
    {
        Carte *card = parcour;
        parcour = parcour->suivant;
        if (i % 2 == 0)
        {
            enfiler(card, tete_player1, queue_player1);
        }
        else
        {
            enfiler(card, tete_player2, queue_player2);
        }
        i++;
    }
    *deck_tete = parcour;
}

void enfiler(Carte *card, Carte **tete_joueur, Carte **queue_joueur)
{
    card->suivant = NULL;
    if (*tete_joueur == NULL)
    {
        *tete_joueur = card;
        *queue_joueur = card;
    }
    else
    {
        (*queue_joueur)->suivant = card;
        *queue_joueur = card;
    }
}

Carte *defiler(Carte **tete_joueur, Carte **queue_joueur)
{
    if (*tete_joueur == NULL)
        return NULL;
    Carte *temp = *tete_joueur;
    *tete_joueur = (*tete_joueur)->suivant;
    if (*tete_joueur == NULL)
        *queue_joueur = NULL;
    return temp;
}

int taille_file(Carte *tete_joueur)
{
    int count = 0;
    Carte *parcour = tete_joueur;
    while (parcour != NULL)
    {
        count++;
        parcour = parcour->suivant;
    }
    return count;
}

int file_vide(Carte *tete_joueur)
{
    return tete_joueur == NULL;
}