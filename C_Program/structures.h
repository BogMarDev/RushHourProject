#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef enum {
    true = 1,
    false = 0
}boolean;

typedef struct {
    char ligne;
    int colonne;
}coordonnee;

typedef struct {
    short taille;
    coordonnee debut;
    coordonnee fin;
    char symbole;
    char sens_vehicule;
}vehicule;

typedef struct{
    int id;
    int nb_lignes;
    int nb_colonnes;
    char **matrice;
    int nb_min_coups;
    coordonnee sortie;
    vehicule *liste_vehicule;
    int nb_total_vehicule;
}plateau;

typedef struct {
    vehicule * vehicule_a_deplacer;
    int ecart_horizontal;
    int ecart_vertical;
}deplacement;

typedef struct {
    int carte_id;
    int nb_coups_min;
    int nb_coups_joueur;
}score;

#endif
