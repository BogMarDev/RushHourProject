#ifndef UTIL_H
#define UTIL_H

#include "structures.h"

extern void vider_tampon_stdin(void);
extern char faire_choix(void);
extern void pause(void);
extern boolean is_between_limits(int number, int min, int max);
extern void lire_entier_dans(FILE *file, int * number);
extern long int find_position_with_string(FILE *flux, char * string_to_find, int limite);
extern int countLines(FILE *fichier, long debut, long fin);
extern void changer_position_pointeur_dans(FILE * file, int offset, int origin);
extern void determiner_taille(vehicule * v);
extern void determiner_sens(vehicule * v);

#endif
