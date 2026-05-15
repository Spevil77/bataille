#include "carte.h"

extern Carte *tete;

void creer_deck();
void afficher_carte(Carte *);
void afficher_deck(Carte *);
void shuffle();
void liberer_deck(Carte **); 