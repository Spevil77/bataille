#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <errno.h>
#include <limits.h>
#include "carte.h"
#include "file.h"
#include "pile.h"
#include "liste.h"
static int get_any_key(void)
{
    int ch = _getch();
    while (_kbhit())
    {
        _getch();
    }
    return ch;
}

DWORD vittesse = 1000;

int comparer_cartes(Carte *, Carte *);
void battaille(int);
void restart_game(int);
int getIntegerFromUser(int *, int, int);
void afficher_tour(int);
char confirmer_quitter(char, HANDLE, int);
void file_vide_pendant_bataille(HANDLE);
void afficher_valeur_carte(Carte *);

void jeu(int mode)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    system("cls");
    char choice;
    if (mode == 1)
    {
        int tours = 0;
        // validation de l'entrée de l'utilisateur pour le nombre de tours
        // changer la couleur du texte
        system("cls");
        // validation de l'entrée de l'utilisateur pour le nombre de tours
        SetConsoleTextAttribute(hConsole, 4); // rouge
        printf("Mode de jeu: nombre des tours determiné\n");
        SetConsoleTextAttribute(hConsole, 2); // vert
        printf("Entrez le nombre de tours que vous souhaitez jouer: ");
        while (getIntegerFromUser(&tours, 1, 999) != 0)
        {
            system("cls");
            SetConsoleTextAttribute(hConsole, 4); // rouge
            printf("Entrée invalide. Veuillez entrer un nombre entier entre 1 et 999.\n");
            SetConsoleTextAttribute(hConsole, 2); // vert
            printf("Entrez le nombre de tours que vous souhaitez jouer: ");
        }
        // boucle de jeu pour le mode nombre de tours déterminé
        for (int i = 0; i < tours; i++)
        {
            afficher_tour(i + 1);
            if (file_vide(tete_joueur1) || file_vide(tete_joueur2))
                break;
            SetConsoleTextAttribute(hConsole, 2); // vert
            printf("Appuyez sur une touche pour continuer au tour suivant(R pour recommencer, Q pour quitter)...");
            choice = (char)get_any_key();
            choice = (char)toupper(choice);
            if (choice == 'R' || choice == 'Q')
            {
                system("cls");
                break; // sortir de la boucle pour recommencer ou quitter
            }
        }
        if (choice == 'R')
            restart_game(mode);
        else if (choice == 'Q')
        {
            // faire le nettoyage avant de quitter
            liberer_deck(&tete_joueur1);
            liberer_deck(&tete_joueur2);
        }
        else
        {
            // Affichage du gagnant ou égalité
            SetConsoleTextAttribute(hConsole, 4); // rouge
            system("cls");
            if (taille_file(tete_joueur1) > taille_file(tete_joueur2))
                printf(">>> Joueur 1 gagne la partie avec %d cartes restantes !\n", taille_file(tete_joueur1));
            else if (taille_file(tete_joueur1) < taille_file(tete_joueur2))
                printf(">>> Joueur 2 gagne la partie avec %d cartes restantes !\n", taille_file(tete_joueur2));
            else
                printf(">>> La partie se termine en égalité avec %d cartes chacun !\n", taille_file(tete_joueur1));
            // retour au menu ou recommencer le jeu
            SetConsoleTextAttribute(hConsole, 2); // vert
            printf("Appuyez sur une touche pour retourner au menu principal ou R pour recommencer...");
            choice = (char)get_any_key();
            choice = (char)toupper(choice);
            if (choice == 'R')
                restart_game(mode);
        }
    }
    // boucle de jeu pour le mode jusqu'à ce qu'un joueur perde
    else if (mode == 2)
    {
        int i = 1;
        SetConsoleTextAttribute(hConsole, 4); // rouge
        printf("Mode de jeu: jusqu'à ce qu'un joueur perde\n");
        while (!file_vide(tete_joueur1) && !file_vide(tete_joueur2))
        {
            afficher_tour(i);
            i++;
            // reponse de l'utilisateur pour continuer ou arreter le jeu
            choice = confirmer_quitter(choice, hConsole, 1);
            if (choice == 'N')
                break;
            // reponse de jouer 2
            choice = confirmer_quitter(choice, hConsole, 2);
            if (choice == 'N')
                break;
        }
        // Si le jeu s'est terminé naturellement (un joueur n'a plus de cartes), on affiche le gagnant
        if (choice != 'N')
        {
            SetConsoleTextAttribute(hConsole, 4); // rouge
            system("cls");
            if (file_vide(tete_joueur1))
                printf(">>> Joueur 2 gagne la partie avec %d cartes restantes !\n", taille_file(tete_joueur2));
            else
                printf(">>> Joueur 1 gagne la partie avec %d cartes restantes !\n", taille_file(tete_joueur1));
        }
        // retour au menu ou recommencer le jeu
        SetConsoleTextAttribute(hConsole, 2); // vert
        printf("Appuyez sur une touche pour retourner au menu principal ou R pour recommencer...");
        choice = (char)get_any_key();
        choice = (char)toupper(choice);
        if (choice == 'R')
            restart_game(mode);
    }
    else
    {
        SetConsoleTextAttribute(hConsole, 4); // rouge
        system("cls");
        printf("Mode de jeu invalide. Retour au menu principal.\n");
        SetConsoleTextAttribute(hConsole, 2); // vert
        printf("Appuyez sur une touche pour retourner au menu principal...");
        get_any_key();
    }
}

void battaille(int cacher)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    // Vérification si un joueur n'a plus de cartes
    if (file_vide(tete_joueur1) || file_vide(tete_joueur2))
    {
        file_vide_pendant_bataille(hConsole);
        return;
    }

    // Chaque joueur joue une carte
    Carte *carte1 = defiler(&tete_joueur1, &queue_joueur1);
    Carte *carte2 = defiler(&tete_joueur2, &queue_joueur2);

    empiler(carte1, &tete_pile1, &queue_pile1);
    empiler(carte2, &tete_pile2, &queue_pile2);

    // Affichage des cartes jouées si ce n'est pas une bataille cachée
    if (cacher == 0)
    {
        // nombre des cartes dans les piles
        int nb_cartes;
        nb_cartes = taille_file(tete_pile1) + taille_file(tete_pile2);
        if (nb_cartes > 2)
        {
            SetConsoleTextAttribute(hConsole, 4); // rouge
            printf("\n >>> BATAILLE ! Cartes cachées posees...\n");
        }
        Sleep(vittesse);
        SetConsoleTextAttribute(hConsole, 2); // vert

        // afichage du carte de joueur 1 printf("\nJoueur 1 joue: ");
        printf("Joueur 1 joue: ");
        afficher_valeur_carte(carte1);
        // aafichage du carte de joueur 2
        printf("Joueur 2 joue: ");
        afficher_valeur_carte(carte2);

        // Comparaison des cartes
        int resultat = comparer_cartes(carte1, carte2);
        if (resultat > 0)
        {
            SetConsoleTextAttribute(hConsole, 6); // jaune
            vider_dans_file(&tete_pile1, &queue_pile1, &tete_joueur1, &queue_joueur1);
            vider_dans_file(&tete_pile2, &queue_pile2, &tete_joueur1, &queue_joueur1);
            printf("\n>>> joueur 1 remporte la bataille ! (+%d cartes)\n", nb_cartes);
        }
        else if (resultat < 0)
        {
            SetConsoleTextAttribute(hConsole, 6); // jaune
            vider_dans_file(&tete_pile1, &queue_pile1, &tete_joueur2, &queue_joueur2);
            vider_dans_file(&tete_pile2, &queue_pile2, &tete_joueur2, &queue_joueur2);
            printf("\n>>> joueur 2 remporte la bataille ! (+%d cartes)\n", nb_cartes);
        }
        else
        {
            Sleep(vittesse);
            cacher = 1;
            battaille(cacher);
        }
    }

    // Si c'est une bataille cachée, on continue à jouer
    else
    {
        cacher = 0;
        battaille(cacher);
    }
    SetConsoleTextAttribute(hConsole, 2); // vert
}

int comparer_cartes(Carte *carte1, Carte *carte2)
{
    if (carte1->valeur > carte2->valeur)
        return 1;
    else if (carte1->valeur < carte2->valeur)
        return -1;
    else
        return 0;
}

void restart_game(int mode)
{
    system("cls");
    liberer_deck(&tete_joueur1);
    liberer_deck(&tete_joueur2);
    // initialisation du deck
    creer_deck();
    shuffle();
    // distribution des cartes aux joueurs
    distribution(&tete, &tete_joueur1, &queue_joueur1, &tete_joueur2, &queue_joueur2);
    // boucle de jeu
    jeu(mode);
}

int getIntegerFromUser(int *number, int min, int max)
{
    char buffer[100];
    char *endptr;
    long value;

    if (fgets(buffer, sizeof(buffer), stdin) == NULL)
    {
        return 1; // Erreur de lecture
    }

    // Supprimer le saut de ligne
    buffer[strcspn(buffer, "\n")] = '\0';

    errno = 0; // Réinitialiser errno avant l'appel à strtol
    value = strtol(buffer, &endptr, 10);

    // Vérifier les erreurs de conversion
    if (errno != 0 || endptr == buffer || *endptr != '\0' || value < min || value > max)
    {
        return 1; // Entrée invalide
    }

    *number = (int)value;
    return 0; // Succès
}

void afficher_tour(int tour)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    system("cls");
    SetConsoleTextAttribute(hConsole, 6); // jaune
    printf("--- Tour %d ---\n", tour);
    battaille(0);
    SetConsoleTextAttribute(hConsole, 6);                                                                      // jaune
    printf("Scores -> Joueur 1: %d  || Joueur 2: %d\n", taille_file(tete_joueur1), taille_file(tete_joueur2)); // score des joueurs
}

char confirmer_quitter(char choice, HANDLE hConsole, int joueur)
{
    SetConsoleTextAttribute(hConsole, 2); // vert
    printf("Voulez-vous continuer le jeu joueur %d? (O/N): ", joueur);
    choice = (char)get_any_key();
    choice = (char)toupper(choice);
    if (choice == 'N')
    {
        system("cls");
        SetConsoleTextAttribute(hConsole, 4); // rouge
        printf(">>> Joueur %d a décidé d'arrêter le jeu. Joueur %d gagne la partie avec %d cartes restantes !\n", joueur, (joueur == 1) ? 2 : 1, taille_file((joueur == 1) ? tete_joueur2 : tete_joueur1));
        return 'N';
    }
    printf("O\n");
    return 'O';
}

void file_vide_pendant_bataille(HANDLE hConsole)
{
    int joueur = file_vide(tete_joueur1) ? 1 : 2;
    SetConsoleTextAttribute(hConsole, 4); // rouge
    // vider les piles dans le main joueur qui a encore des cartes
    if (joueur == 1)
    {
        vider_dans_file(&tete_pile1, &queue_pile1, &tete_joueur2, &queue_joueur2);
        vider_dans_file(&tete_pile2, &queue_pile2, &tete_joueur2, &queue_joueur2);
    }
    else
    {
        vider_dans_file(&tete_pile1, &queue_pile1, &tete_joueur1, &queue_joueur1);
        vider_dans_file(&tete_pile2, &queue_pile2, &tete_joueur1, &queue_joueur1);
    }
    printf("\n>>> Joueur %d n'a plus de cartes, Joueur %d gagne la bataille !\n", joueur, (joueur == 1) ? 2 : 1);
}

void afficher_valeur_carte(Carte *carte)
{
    afficher_carte(carte);
    if (carte->valeur > 10)
        printf("  Valeur [%d] \n", carte->valeur);
    else
        printf("\n");
    Sleep(vittesse);
}
