#include "carte.h"

extern Carte *tete_pile1;
extern Carte *queue_pile1;
extern Carte *tete_pile2;
extern Carte *queue_pile2;

void empiler(Carte *, Carte **, Carte **);
Carte *depiler(Carte **, Carte **);
int pile_vide(Carte *);
void vider_dans_file(Carte **, Carte **, Carte **, Carte **);
 