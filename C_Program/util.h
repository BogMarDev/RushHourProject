#ifndef UTIL_H
#define UTIL_H

#include "structures.h"
#include <stdio.h>

extern void vider_tampon_stdin(void);
extern char faire_choix(void);
extern void pause(void);
extern boolean est_entre_limites(int nombre, int min, int max);
extern void lire_entier_dans(FILE *fichier, int * nombre);
extern long int trouver_position_avec_string(FILE *fichier, char * string_a_chercher, int limite);
extern int compteur_lignes(FILE *fichier, long debut, long fin);
extern void changer_position_pointeur_dans(FILE * fichier, int offset, int origin);
extern void determiner_taille(vehicule * v);
extern void determiner_sens(vehicule * v);
extern void conversion_coordonee(coordonnee coord, int * ligne, int * colonne);
extern coordonnee trouver_centre_camion(vehicule * camion);
extern int indice_voiture_recherche(plateau * parking, char symbole_a_rechercher);

#endif
