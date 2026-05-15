#include "carte.h"
#include <windows.h>

extern DWORD vittesse;

int comparer_cartes(Carte *, Carte *);
void battaille(int);
void restart_game(int);
int getIntegerFromUser(int *, int, int);
void afficher_tour(int);
char confirmer_quitter(char, HANDLE, int);
void file_vide_pendant_bataille(HANDLE);
void afficher_valeur_carte(Carte *);
void jeu(int);