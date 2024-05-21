#ifndef MOVE_H
#define MOVE_H

#include "structures.h"

extern deplacement * creer_deplacement(plateau * plateau1);
extern boolean dans_limites_grille(plateau *plat, vehicule vehicule_a_deplacer, int nouvelle_colonne, int nouvelle_ligne);
extern boolean collision_detectee(plateau *plat, vehicule vehicule_a_deplacer, int nouvelle_colonne, int nouvelle_ligne);
extern boolean deplacement_est_valide(plateau *plat, deplacement *deplace);
extern void deplacer_vehicule(plateau *plat, deplacement * deplace);

#endif
