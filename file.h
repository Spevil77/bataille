#include "carte.h"

extern Carte *tete_joueur1;
extern Carte *queue_joueur1;
extern Carte *tete_joueur2;
extern Carte *queue_joueur2;

void enfiler(Carte *, Carte **, Carte **);
Carte *defiler(Carte **, Carte **);
int taille_file(Carte *);
int file_vide(Carte *);
void distribution(Carte **, Carte **, Carte **, Carte **, Carte **);