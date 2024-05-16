#ifndef DISPLAY_H
#define DISPLAY_H

#include "structures.h"

extern void afficher_vide(void);
extern void afficher_ecran_lancement_partie(void);
extern void afficher_menu(void);
extern void afficher_regles(void);
extern void afficher_consignes(void);
extern void afficher_numero_colonne(int nb_colonnes);
extern void afficher_tableau(plateau * parking, char indice_ligne, int ligne);
extern void afficher_matrice(plateau * parking);

#endif
