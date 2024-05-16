#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Précondition : /
 *  Postcondition : le tampon stdin est vide
 */
void vider_tampon_stdin(void){
    int c = -1;
    do{
        c = getchar();
    }while(c != '\n' && c != EOF);
}

/**
 * Précondition : /
 * Postcondition : c contient un caractère saisi par l'utilisateur
 * Résultat : retourne le caractère saisi par l'utilisateur
*/
char faire_choix(void){
    char c = -1;
    scanf("%c", &c);
    return c;
}

/**
 * Précondition : /
 * Postcondition : met le programme en pause pendant l'exécution si nécessaire
*/
void pause(void){
    printf("\n\x1b[31;47mAppuyez sur ENTER(↵) pour continuer\x1b[0m...");
    faire_choix();
}

/**
 * \fn is_between_limits
 * \brief verify if a number is between two numbers
 * \param number the number to check
 * \param min the minimum limit
 * \param max the maximum limit
 * @return true if the number is between min and max false if not
*/
boolean is_between_limits(int number, int min, int max){
    boolean is_ok = true;

    if(number < min || number > max){
        is_ok = false;
    }

    return is_ok;
}

void lire_entier_dans(FILE *file, int * number){
    if(fscanf(file,"%d", number) != 1){
        perror("Erreur lors de la lecture de l'entier");
        exit(EXIT_FAILURE);
    }
    char tampon = getc(file); //' ' || '\n'
}

long int find_position_with_string(FILE *flux, char * string_to_find, int limite){
    char buffer[255];
    int estTrouve = 0;
    for (int i = 0; i < limite && !estTrouve; i++) {
        char * line = fgets(buffer,255, flux);
        if(strcmp(string_to_find, line) == 0){
            estTrouve = 1;
        }
        free(line);
        line = NULL;
    }

    long int position_flux = ftell(flux);
    return position_flux;
}

int countLines(FILE *fichier, long debut, long fin) {
    int count = 0;
    char c = '0';

    fseek(fichier, debut, SEEK_SET);

    //Parcours du fichier caractère par caractère pour compter les sauts de ligne
    c = fgetc(fichier);
    while (ftell(fichier) < fin && c != EOF) {
        if (c == '\n') {
            count++;
        }
        c = fgetc(fichier);
    }

    c = fgetc(fichier);
    //Ajoute une ligne si le fichier ne se termine pas par un saut de ligne
    if (count > 0 && ftell(fichier) < fin && c != '\n') {
        count++;
    }

    return count;
}

void changer_position_pointeur_dans(FILE * file, int offset, int origin){
    fflush(file);
    fseek(file, offset, origin);
}

void determiner_taille(vehicule * v){
    if(v->sens_vehicule == 'H'){
        v->taille = (v->fin.colonne - v->debut.colonne) + 1;
    } else {
        v->taille = (v->fin.ligne - v->debut.ligne) + 1;
    }
}

void determiner_sens(vehicule * v){
    if(v->debut.ligne == v->fin.ligne){
        v->sens_vehicule = 'H';
    } else{
        v->sens_vehicule = 'V';
    }
}