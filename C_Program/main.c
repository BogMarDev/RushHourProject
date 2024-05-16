/**
 * \authors Bogatu Marco, Lois Rosman
 * \copyright HERS RUSH_HOUR PROJECT
 * \date 29/04/2024
 * \version Proof of Concept
 */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "display.h"
#include "util.h"

#define TAILLE 20

/**
 * Précondition : coord initialisé. INDICE_PREMIERE_LIGNE <= coord.ligne <= INDICE_DERNIERE_LIGNE &&
 *                INDICE_PREMIERE_COLONNE <= coord.colonne <= INDICE_DERNIERE_COLONNE
 * Postcondition : coord inchangé. Convertit ligne en un entier. ligne = coord.ligne - 65 et colonne = coord.colonne - 1
 */
//Variable globale utilise seulement a chaque appel de la fonction conversion_coordonnee
int ligne = 0;
int colonne = 0;
void conversion_coordonee(coordonnee coord, int * line, int * column){
    *line = coord.ligne - 65;
    *column = coord.colonne - 1;
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

//GAMEPLAY
/**
 * Précondition : symboles est un tableau de caractère initialisé et nb_symboles_restants est initialisé
 * Postcondition : nb_symboles_restants est inchangé, symboles est modifié. Le dernier symbole de tableau
 *                 passe a l'emplacement du symbole tiré et le symbole tiré passe au dernier emplacement du tableau
 * Résultat : retourne le symbole contenu à l'indice de nb_aleatoire dans le tableau symboles
*/
char tire_symbole_aleatoire(char symboles[TAILLE], int * nb_symboles_restants){
    int nb_aleatoire = rand() % (*nb_symboles_restants);
    char symbole = symboles[nb_aleatoire];
    //modifie le tableau
    symboles[nb_aleatoire] = symboles[*nb_symboles_restants];
    symboles[*nb_symboles_restants] = symbole;

    return symbole;
}

/**
 * Précondition : parking est un plateau initialisé
 * Postcondition : la matrice de parking est initialisée. Chaque case vide contient le caractère espace,
 *                 la case de la sortie contient 'S', les cases contenant le véhicule rouge contiennent 'R' et
 *                 les autres cases contenant un véhicule contiennent des symboles aléatoires (1 par véhicule)
 */
void init_matrice(plateau * parking){
    parking->matrice = malloc(parking->nb_lignes * sizeof (char *));
    if(parking->matrice == NULL){
        printf("Erreur dans l'allocation dynamique !");
        exit(EXIT_FAILURE);
    }
    for (int lines = 0; lines < parking->nb_lignes; ++lines) {
        parking->matrice[lines] = malloc(parking->nb_colonnes);
        if(parking->matrice[lines] == NULL){
            printf("Erreur dans l'allocation dynamique !");
            exit(EXIT_FAILURE);
        }
        for(int columns = 0; columns < parking->nb_colonnes; columns++){
            parking->matrice[lines][columns] = ' ';
        }
    }

    for(int indice = 0; indice < parking->nb_total_vehicule; indice++){
        conversion_coordonee(parking->liste_vehicule[indice].debut, &ligne, &colonne);
        parking->matrice[ligne][colonne] = parking->liste_vehicule[indice].symbole;

        conversion_coordonee(parking->liste_vehicule[indice].fin, &ligne, &colonne);
        parking->matrice[ligne][colonne] = parking->liste_vehicule[indice].symbole;

        if(parking->liste_vehicule[indice].taille == 3){
            coordonnee milieu = trouver_centre_camion(&parking->liste_vehicule[indice]);
            conversion_coordonee(milieu, &ligne, &colonne);
            parking->matrice[ligne][colonne] = parking->liste_vehicule[indice].symbole;
        }
    }
}

/**
 * Précondition : /
 * Postcondition : /
 * Résultat : retourne le parking déclaré et initialisé
 */
plateau * creer_parking_defaut(char filename[]){
    FILE *f = fopen(filename, "r");
    if(f == NULL){
        printf("Le fichier %s n'as pas pu etre ouvert\n", filename);
        exit(EXIT_FAILURE);
    }

    int nombre_total_cartes = 0;
    lire_entier_dans(f, &nombre_total_cartes);

    printf("Choissisez parmi %d cartes\n",nombre_total_cartes);
    for (int i = 1; i <= nombre_total_cartes; ++i) {
        printf("%d - CARTE %d\n", i, i);
    }

    int choix_carte = 0;
    do{
        scanf("%d", &choix_carte);
    } while (!is_between_limits(choix_carte, 1, nombre_total_cartes));

    //rechercher dans le fichier la ligne avec CARTE choix_carte
    //ftell a doc
    //long int debut_fichier = ftell(f);
    changer_position_pointeur_dans(f, 0, SEEK_END);
    long int fin_fichier = ftell(f);
    //int nombre_total_lignes = countLines(f, debut_fichier, fin_fichier + 9);
    //printf("Le nombre total de lignes est de %d\n", nombre_total_lignes);

    char res[50] = "CARTE ";
    char nombre_str[] = "25";
    sprintf(nombre_str, "%d", choix_carte); //conversion en chaine de caractere
    strcat(res, nombre_str);
    strcat(res, "\n");

    changer_position_pointeur_dans(f, 1, SEEK_SET);

    plateau * parking = malloc(sizeof(plateau));
    parking->id = choix_carte;
    lire_entier_dans(f, &parking->nb_lignes);
    lire_entier_dans(f, &parking->nb_colonnes);

    char ligne_sortie = getc(f);
    int colonne_sortie = 0;
    lire_entier_dans(f, &colonne_sortie);
    parking->sortie = (coordonnee) {ligne_sortie, colonne_sortie};

    lire_entier_dans(f, &parking->nb_min_coups);

    char ligne_debut = getc(f);
    int colonne_debut = 0;
    lire_entier_dans(f, &colonne_debut);
    char ligne_fin = getc(f);
    int colonne_fin = 0;
    lire_entier_dans(f, &colonne_fin);
    vehicule voiture_rouge = {2, {ligne_debut, colonne_debut}, {ligne_fin, colonne_debut}, 'R', 'H'};

    long int debut_flux = ftell(f);
    //debut du comptage
    //FAIRE TRES ATTENTION SI DES ESPACES TRAINE SINON CA CASSE TOUT
    //Si choix_carte = 3 càd derniere carte alors ne pas faire tout ca et aller direct a la fin du fichier
    long int fin_flux = 0;
    if(choix_carte < nombre_total_cartes){
        char res1[50] = "CARTE ";
        int nouveau_nombre = choix_carte + 1;
        sprintf(nombre_str, "%d", nouveau_nombre);
        strcat(res1, nombre_str);
        strcat(res1, "\n");
        fin_flux = find_position_with_string(f, res1, 20); //remplace nombre max de vehicule qu'il pourrait y avoir + 9
        changer_position_pointeur_dans(f, fin_flux-9, SEEK_SET);
        fin_flux = ftell(f);
    } else{
        fin_flux = fin_fichier;
    }
    int nombre_vehicule_total = countLines(f, debut_flux, fin_flux);

    changer_position_pointeur_dans(f, debut_flux, SEEK_SET);

    if(choix_carte == nombre_total_cartes){
        nombre_vehicule_total--;
    }

    parking->liste_vehicule = malloc(sizeof(vehicule) * nombre_vehicule_total + 1);
    parking->liste_vehicule[0] = voiture_rouge;

    char symboles[TAILLE] = {'+', '-', '=', '?', ';', '$', '*', '#', '%', '{',
                             '&', '@', '^', '>', '<', '/', '(', ')', '!', '|'};
    int nb_symboles_restants = TAILLE - 1;
    for (int i = 1; i < nombre_vehicule_total && ftell(f) < fin_flux; i++) { //Si ca fait une erreur c'est peut-etre ici que je dois mettre un <= au lieu de <
        ligne_debut = getc(f);
        lire_entier_dans(f, &colonne_debut);
        ligne_fin = getc(f);
        lire_entier_dans(f, &colonne_fin);
        vehicule vehic = {0, {ligne_debut, colonne_debut}, {ligne_fin, colonne_fin}, '0', 'H'};
        determiner_sens(&vehic);
        determiner_taille(&vehic);
        vehic.symbole = tire_symbole_aleatoire(symboles, &nb_symboles_restants);
        nb_symboles_restants--;
        parking->liste_vehicule[i] = vehic;
    }
    /*for(int indice = 1; indice < nombre_vehicule_total; indice++) {
        parking->liste_vehicule[indice].symbole = tire_symbole_aleatoire(symboles, &nb_symboles_restants);
        nb_symboles_restants--;
    }*/
    init_matrice(parking);
    return parking;
}

/**
 * Précondition : parking initialisé
 * Postcondition : parking inchangé
 * Résultat : retourne 1 si la sortie est atteinte. Sinon, retourne 0
*/
boolean verifier_victoire(plateau * parking){
    int ligne_voiture_rouge = 0;
    int colonne_voiture_rouge = 0;
    conversion_coordonee(parking->liste_vehicule[0].debut, &ligne_voiture_rouge, &colonne_voiture_rouge);

    int ligne_sortie = 0;
    int colonne_sortie = 0;
    conversion_coordonee(parking->sortie, &ligne_sortie, &colonne_sortie);

    boolean est_gagne = false;
    if(colonne_voiture_rouge == colonne_sortie && ligne_voiture_rouge == ligne_sortie){
        est_gagne = true;
    }
    return est_gagne;
}

/**
 * Précondition : parking est initialisé
 * Postcondition : parking est inchangé
 * Résultat : retourne un deplacement saisi par l'utilisateur
*/
deplacement * creer_deplacement(plateau * plateau1) {
    deplacement *dep = malloc(sizeof(deplacement));

    int index = 0;
    do {
        char symbole = 0;
        printf("Entrez le symbole du vehicule que vous voulez deplacer : ");
        scanf("%c", &symbole);
        vider_tampon_stdin();
        index = indice_voiture_recherche(plateau1, symbole);
    } while (index == -1);

    dep->vehicule_a_deplacer = &plateau1->liste_vehicule[index];

    dep->delta_x = 0;
    dep->delta_y = 0;

    if (dep->vehicule_a_deplacer->sens_vehicule == 'H') {
        printf("Entrez le nombre de cases à déplacer horizontalement (positif pour droite, négatif pour gauche) : ");
        scanf("%d", &dep->delta_x);
    } else {
        printf("Entrez le nombre de cases à déplacer verticalement (positif pour haut, négatif pour bas) : ");
        scanf("%d", &dep->delta_y);
    }

    return dep;
}

/**
 * Précondition : dep et parking sont initialisés
 * Postcondition : parking change en fonction de dep, dep est le deplacement qui est realise
 */
void effectuer_deplacement(deplacement * dep, plateau * parking){
    //regarde dans le projet untilted
}

char * choisir_difficulte(){
    printf("Choissisez une difficulte\n");
    printf("1. Facile\n");
    printf("2. Medium\n");
    printf("3. Difficile\n");

    int choix_difficulte = 0;
    do {
        scanf("%d", &choix_difficulte);
        vider_tampon_stdin();
    } while (!is_between_limits(choix_difficulte, 1, 3));

    char * filename = "..\\Files\\rushHourDIFFICULTE.txt";
    if(choix_difficulte == 1){
        strcpy(filename, "..\\Files\\rushHourEasy.txt");
    } else if(choix_difficulte == 2){
        strcpy(filename, "..\\Files\\rushHourMedium.txt");
    } else if(choix_difficulte == 3){
        strcpy(filename, "..\\Files\\rushHourHard.txt");
    }

    return filename;
}

/**
 * Précondition : /
 * Postcondition : deroulement d'une partie de jeu
 */
void deroulement_partie(void) {
    vider_tampon_stdin();
    char * filename = choisir_difficulte();
    plateau * parking = creer_parking_defaut(filename);
    score s1 = {parking->id, parking->nb_min_coups, 0};
    afficher_consignes();
    pause();

    int nbCoupJ = 0;
    deplacement * dep = creer_deplacement(parking);
    while (!verifier_victoire(parking)){
        afficher_vide();
        afficher_matrice(parking);

        printf("Faites votre choix(d/D = deplacer un vehicule, r/R = reinitialiser le plateau) : ");
        char choix = faire_choix();

        switch (choix) {
            case 'd': case 'D': effectuer_deplacement(dep, parking); nbCoupJ++; break;
            case 'r': case 'R':
            for (int i = 0; i < parking->nb_lignes; ++i) {
                free(parking->matrice[i]);
                parking->matrice[i] = NULL;
            }
            free(parking->matrice);
            parking->matrice = NULL;
            init_matrice(parking);
            nbCoupJ = 0;
            break;
            default: printf("Vous avez appuye sur une mauvaise touche\n"); pause(); vider_tampon_stdin(); break;
        }
    }

    s1.nb_coups_joueur = nbCoupJ;
    afficher_vide();
    afficher_matrice(parking);
    printf("Bravo, vous avez gagné avec %d coups. Le nombre de coups minimum pour ce plateau etait de %d", s1.nb_coups_joueur, s1.nb_coups_min);
    pause();



    free(parking->liste_vehicule);
    parking->liste_vehicule = NULL;

    free(parking);
    parking = NULL;

    free(dep->vehicule_a_deplacer);
    dep->vehicule_a_deplacer = NULL;

    free(dep);
    dep = NULL;

    free(filename);
    filename = NULL;
}

int main(void){
    srand(time(NULL));

    afficher_vide();
    printf("Bienvenue dans le jeu du rush hour\n\n");

    int is_finish = 1;
    while (is_finish){
        afficher_ecran_lancement_partie();
        afficher_menu();
        printf("Faites votre choix : ");
        char choix_utilisateur = faire_choix();

        afficher_vide();
        switch(choix_utilisateur){
            case 'r': case 'R': afficher_regles(); pause(); break;
            case 'l': case 'L': deroulement_partie(); break;
            case 'q': case 'Q': is_finish--; break;
            default: printf("Vous avez appuye sur une mauvaise touche\n\n"); pause(); break;
        }
        vider_tampon_stdin();
        afficher_vide();
    }
    printf("\nMerci d'avoir joue au Rush Hour ! \n");
    return EXIT_SUCCESS;
}
