#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef enum {
    true = 1,
    false = 0
}boolean;

typedef struct {
    char ligne; //ex : A
    int colonne; //ex : 6
}coordonnee;

typedef struct {
    short taille; //2 ou 3
    coordonnee debut;
    coordonnee fin;
    char symbole;
    char sens_vehicule; //H ou V
}vehicule;

typedef struct{
    int id;
    int nb_lignes;
    int nb_colonnes;
    char **matrice; //grille de jeu avec les symbole de chaque vehicule
    int nb_min_coups;
    coordonnee sortie;
    vehicule *liste_vehicule; //liste contenant tous les vehicules
    int nb_total_vehicule;
}plateau;

//Pour visualiser le déplacement, imaginez un repère orthonormé sur lequel on se déplace de haut en bas ou de droite à gauche
typedef struct {
    vehicule * vehicule_a_deplacer;
    int ecart_horizontal;
    int ecart_vertical;
}deplacement;

typedef struct {
    int carte_id;
    int nb_coups_min;
    int nb_coups_joueur;
}score; //Censé etre utilisé pour l'historique

#endif
