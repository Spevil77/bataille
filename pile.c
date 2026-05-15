#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "carte.h"
#include "file.h"

Carte *tete_pile1 = NULL;
Carte *queue_pile1 = NULL;
Carte *tete_pile2 = NULL;
Carte *queue_pile2 = NULL;

void empiler(Carte *, Carte **, Carte **);
Carte *depiler(Carte **, Carte **);
int pile_vide(Carte *);
void vider_dans_file(Carte **, Carte **, Carte **, Carte **);

void empiler(Carte *card, Carte **tete_pile, Carte **queue_pile)
{
    card->suivant = NULL;
    if (*tete_pile == NULL)
    {
        *tete_pile = card;
        *queue_pile = card;
    }
    else
    {
        card->suivant = *tete_pile;
        *tete_pile = card;
    }
}

Carte *depiler(Carte **tete_pile, Carte **queue_pile)
{
    if (*tete_pile == NULL)
        return NULL;
    Carte *temp = *tete_pile;
    *tete_pile = (*tete_pile)->suivant;
    if (*tete_pile == NULL)
        *queue_pile = NULL;
    return temp;
}

int pile_vide(Carte *tete_pile)
{
    return tete_pile == NULL;
}

void vider_dans_file(Carte **tete_pile, Carte **queue_pile, Carte **tete_joueur, Carte **queue_joueur)
{
    while (!pile_vide(*tete_pile))
    {
        Carte *card = depiler(tete_pile, queue_pile);
        enfiler(card, tete_joueur, queue_joueur);
    }
}