#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include "carte.h"
#include "liste.h"
#include "file.h"
#include "pile.h"
#include "jeu.h"
static int get_any_key(void)
{
    int ch = _getch();
    while (_kbhit())
    {
        _getch();
    }
    return ch;
}

void help(void);
void menu(void);
void set_up(void);
void options(void);

int main()
{

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    char choice;
    system("cls");
    menu();
    while (1)
    {

        choice = (char)get_any_key();
        choice = (char)toupper(choice);
        switch (choice)
        {
        case 'S':
            system("cls");
            set_up();
            menu();
            break;
        case 'H':
            system("cls");
            help();
            printf("\nAppuyez sur une touche pour continuer au tour suivant...");
            get_any_key();
            system("cls");
            menu();
            break;
        case 'Q':
            system("cls");
            SetConsoleTextAttribute(hConsole, 4); // rouge
            puts(">>>quitter le jeu. Au revoir!");
            Sleep(vittesse);
            exit(0);
        case 'O':
            system("cls");
            options();
            system("cls");
            menu();
            break;
        default:
            break;
        }
    }
    return 0;
}

void menu(void)
{
    // changer la couleur du texte en vert
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 2); // vert
    system("cls");
    // Affichage du menu principal
    puts(">>>>> C Program jeu de bataille <<<<<<");
    puts("appuyer sur S pour démarrer le jeu");
    puts("appuyer sur H pour l'aide");
    puts("appuyer sur Q pour quitter");
    puts("appuyer sur O pour les options");
}

void help(void)
{
    // changer la couleur du texte en vert
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 2); // vert

    // Affichage des règles du jeu et des instructions
    puts("Bonjour et bienvenue dans la section d'aide du jeu Bataille!");
    puts("regles du jeu: ");
    puts("1. Chaque joueur reçoit un nombre égal de cartes, face cachée.");
    puts("2. À chaque tour, les deux joueurs retournent la carte du dessus de leur pile et la comparent.");
    puts("3. Le joueur avec la carte de valeur la plus élevée remporte les deux cartes et les place au bas de sa pile.");
    puts("4. En cas d'égalité (bataille), les joueurs placent une carte face cachée et une carte face visible. Le joueur avec la carte visible la plus élevée remporte toutes les cartes en jeu.");
    puts("\nVoici quelques instructions pour vous aider à naviguer dans le jeu:");
    puts("1. Démarrer la bataille: Appuyez sur 'S' dans le menu principal pour commencer la bataille.");
    puts("2. Réinitialiser la jeu: Appuyez sur 'R' pour réinitialiser le jeu apres au moins une partie (possible dans le premier mode seulement).");
    puts("3. Quitter le jeu: Appuyez sur 'Q' pour quitter le jeu vers le menu principal et 'Q' dans le menu principal pour quitter complètement.");
    puts("4. Répondre aux questions: Pendant la bataille, tapez 'o' pour continuer le jeu ou 'n' pour arrêter le jeu et déclarer l'autre joueur comme gagnant.");
    puts("Amusez-vous bien et bonne chance!");
}

void set_up()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    char choice;
    int mode = 0;
    // Choix du mode de jeu
    SetConsoleTextAttribute(hConsole, 6); // jaune
    printf("choix du mode de jeu:\n");
    SetConsoleTextAttribute(hConsole, 2); // vert
    printf("\n1. Mode nombre des tours déterminé (appuyer sur 1)\n\n2. Mode jusqu'à ce qu'un joueur perde (appuyer sur 2)\n");
    while (mode != 1 && mode != 2)
    {
        choice = (char)get_any_key();
        if (choice == '1')
            mode = 1;
        else if (choice == '2')
            mode = 2;
    }
    // debut du jeu
    restart_game(mode);
}

void options()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD new_vittesse = 1000;            // valeur par défaut
    SetConsoleTextAttribute(hConsole, 6); // jaune
    printf("Entrez la nouvelle vitesse d'affichage en millisecondes (actuelle: %lu ms): ", vittesse);
    while (getIntegerFromUser((int *)&new_vittesse, 0, 9999) != 0)
    {
        system("cls");
        SetConsoleTextAttribute(hConsole, 4); // rouge
        printf("Entrée invalide. Veuillez entrer un nombre entre 0 et 9999.\n");
        SetConsoleTextAttribute(hConsole, 2); // vert
        printf("Appuyez sur une touche pour continuer...");
        get_any_key();
        system("cls");
        SetConsoleTextAttribute(hConsole, 6); // jaune
        printf("Entrez la nouvelle vitesse d'affichage en millisecondes (actuelle: %lu ms): ", vittesse);
    }
    vittesse = new_vittesse;
    SetConsoleTextAttribute(hConsole, 2); // vert
    printf("Vitesse d'affichage mise à jour à %lu ms.\n", vittesse);
    printf("Appuyez sur une touche pour continuer...");
    get_any_key();
}
