#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "carte.h"
#include "file.h"

Carte *tete = NULL;
Carte *queue = NULL;

void shuffle();
void afficher_deck(Carte *);
void afficher_carte(Carte *);
void liberer_deck(Carte **);

void creer_deck()
{
    int i, j;
    char *couleurs[] = {"Coeur", "Carreau", "Trefle", "Pique"};
    for (j = 0; j < 4; j++)
    {
        for (i = 2; i <= 14; i++)
        {
            Carte *nouveau = malloc(sizeof(Carte));
            nouveau->valeur = i;
            strcpy(nouveau->couleur, couleurs[j]);
            nouveau->suivant = NULL;
            if (tete == NULL)
            {
                tete = nouveau;
                queue = nouveau;
            }
            else
            {
                queue->suivant = nouveau;
                queue = nouveau;
            }
        }
    }
}

void afficher_carte(Carte *carte)
{
    if (carte->valeur <= 10)
    {
        printf("%d de %s", carte->valeur, carte->couleur);
    }
    switch (carte->valeur)
    {
    case 11:
        printf("Valet de %s", carte->couleur);
        break;
    case 12:
        printf("Dame de %s", carte->couleur);
        break;
    case 13:
        printf("Roi de %s", carte->couleur);
        break;
    case 14:
        printf("As de %s", carte->couleur);
        break;
    default:
        break;
    }
}

void afficher_deck(Carte *current_tete)
{
    Carte *parcour = current_tete;
    printf("\n");
    while (parcour != NULL)
    {
        afficher_carte(parcour);
        printf("\n");
        parcour = parcour->suivant;
    }
}

void shuffle()
{
    srand((unsigned int)time(NULL));
    Carte *current = NULL;
    Carte *swapped = NULL;
    int temp_valeur;
    char temp_couleur[10];
    for (int i = 51; i > 1; i--)
    {
        int j = rand() % (i + 1);
        // Trouver les cartes à échanger
        current = tete;
        swapped = tete;
        for (int k = 0; k < i; k++)
        {
            if (k < j)
            {
                swapped = swapped->suivant;
            }
            current = current->suivant;
        }
        // Echanger les cartes
        temp_valeur = swapped->valeur;
        strcpy(temp_couleur, swapped->couleur);
        swapped->valeur = current->valeur;
        strcpy(swapped->couleur, current->couleur);
        current->valeur = temp_valeur;
        strcpy(current->couleur, temp_couleur);
    }
}

void liberer_deck(Carte **tete_liste)
{
    Carte *temp;
    while (*tete_liste != NULL)
    {
        temp = *tete_liste;
        *tete_liste = (*tete_liste)->suivant;
        free(temp);
    }
}