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
 * Précondition : number, min, max sont initalisé
 * Postcondition : number, min, max reste inchangé
 * Resultat : true si le nombre est compris entre le min et le max false sinon
 * A TESTER
*/
boolean est_entre_limites(int number, int min, int max){
    boolean is_ok = true;

    if(number < min || number > max){
        is_ok = false;
    }

    return is_ok;
}

/**
 * Précondition : file et nombre sont initalisé
 * Postcondition : file reste inchangé mais nombre est initialisé avec le nombre récuperé dans le fichier
 */
void lire_entier_dans(FILE *file, int *nombre) {
    // Tente de lire un entier depuis le fichier
    if (fscanf(file, "%d", nombre) != 1) {
        perror("Erreur lors de la lecture de l'entier");
        exit(EXIT_FAILURE);
    }

    // Lecture des deux prochains caractères (y compris les espaces possibles)
    int premier_char = getc(file);
    if (premier_char == 13) {
        int second_char = getc(file); //utile pour récuper le deuxieme caractere de fin de ligne
        if(second_char == 10){} //Ce if ne sert a rien c'est simplement pour ne pas avoir une erreur lors de la compilation
    }
}

/**
 * Précondition : flux, string_to_find et limite sont initialisé
 * Postcondition : flux, string_to_find et limite reste inchangé
 * Resultat : recherche dans un fichier une chaine de caratere et retourne la position du pointeur a l'emplacement trouvé
 */
long int trouver_position_avec_string(FILE *flux, char * string_to_find, int limite){
    char buffer[255];
    int estTrouve = 0;
    char * line = NULL;
    for (int i = 0; i < limite && !estTrouve; i++) {
        line = fgets(buffer,255, flux);
        if(strcmp(string_to_find, line) == 0){
            estTrouve = 1;
        }
        line = NULL;
    }
    long int position_flux = ftell(flux);

    return position_flux;
}

/**
 * Précondition : file, offset et origin sont initalisé
 * Postcondition : change la position du pointeur dans un fichier
 */
void changer_position_pointeur_dans(FILE * file, int offset, int origin){
    fflush(file);
    fseek(file, offset, origin);
}

/**
 * Précondition : fichier, debut et fin sont initialisé
 * Postcondition : fichier, debut et fin reste inchangé
 * Resultat: le nombre de lignes entre le debut et la fin passé en paramètre
 */
int compteur_lignes(FILE *fichier, long debut, long fin) {
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

    //Replace le pointeur dans le fichier ou il etait avant le comptage
    changer_position_pointeur_dans(fichier, debut, SEEK_SET);

    return count;
}

//A tester
/**
 * Précondition : v est initialisé
 * Postcondition : la taille du vehicule est calculé (taille possible 2 ou 3)
 */
void determiner_taille(vehicule * v){
    if(v->sens_vehicule == 'H'){
        v->taille = (v->fin.colonne - v->debut.colonne) + 1;
    } else {
        v->taille = (v->fin.ligne - v->debut.ligne) + 1;
    }
}

// A TESTER
/**
 * Précondition : v est initialisé
 * Postcondition : le sens du vehicule est choisi (sens possible 'H' pour horizontal ou 'V' pour vertical)
 */
void determiner_sens(vehicule * v){
    if(v->debut.ligne == v->fin.ligne){
        v->sens_vehicule = 'H';
    } else{
        v->sens_vehicule = 'V';
    }
}

/**
 * Précondition : coord initialisé.
 * Postcondition : coord inchangé. Convertit ligne en un entier. ligne = coord.ligne - 65 et colonne = coord.colonne - 1
 */
void conversion_coordonee(coordonnee coord, int * ligne, int * colonne){
    *ligne = coord.ligne - 65;
    *colonne = coord.colonne - 1;
}

/**
 * Précondition : camion est un véhicule initialisé
 * Postcondition : camion inchangé
 * Résultat : retourne une coordonnée contenant la lettre de la ligne et le numero de la colonne
 *            de la case du milieu d'un camion
 */
coordonnee trouver_centre_camion(vehicule * camion) {
    coordonnee milieu = {0, 0};
    if (camion->debut.ligne == camion->fin.ligne) {
        milieu.ligne = camion->debut.ligne;
        milieu.colonne = camion->debut.colonne + 1;
    } else {
        milieu.ligne = camion->debut.ligne + 1;
        milieu.colonne = camion->debut.colonne;
    }
    return milieu;
}

// A TESTER
/**
 * Précondition : parking et symbole_a_rechercher sont initialisé
 * Postcondition : parking et symbole_a_rechercher reste inchangé
 * Resultat : l'indice du symbole qu'on recherche, si ce symbole n'existe pas dans la liste retourne -1
 */
int indice_voiture_recherche(plateau * parking, char symbole_a_rechercher){
    boolean est_trouve = false;
    int indice = 0;
    while (indice < parking->nb_total_vehicule && !est_trouve) {
        if(symbole_a_rechercher == parking->liste_vehicule[indice].symbole) {
            est_trouve = true;
        }
        else{
            indice++;
        }
    }

    if(!est_trouve){
        indice = -1;
    }

    return indice;
}