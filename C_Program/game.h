#ifndef GAME_H
#define GAME_H

#define TAILLE 20
#include "structures.h"

extern char tire_symbole_aleatoire(char symboles[TAILLE], int * nb_symboles_restants);
extern void init_matrice(plateau * parking);
extern plateau * creer_parking_defaut(char filename[]);
extern boolean verifier_victoire(plateau * parking, deplacement * dep);
extern char * choisir_difficulte(void);
extern void deroulement_partie(void);

#endif
