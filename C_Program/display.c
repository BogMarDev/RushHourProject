#include "display.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Précondition : /
 * Postcondition : efface le contenu du terminal
 */
void afficher_vide(void){
    system("@cls||clear");
}

/**
 * Précondition : /
 * Postcondition : affiche l'écran de lancement d'une partie grace à un fichier
 */
void afficher_ecran_lancement_partie(void){
    char filename[] = "../Files/splash_screen.txt";
    FILE *flux = fopen(filename, "r");
    if(flux == NULL){
        printf("Impossible d'ouvrir/créer le fichier %s", filename);
        exit(EXIT_FAILURE);
    }

    //Lecture caractere par caracter dans le fichier
    char c = getc(flux);
    while (c != EOF){
        printf("%c", c);
        c = getc(flux);
    }

    if(fclose(flux) == EOF){
        perror("Erreur lors de la fermeture du flux !");
        exit(EXIT_FAILURE);
    }
}

/**
 * Précondition : /
 * Postcondition : affiche le menu du jeu
 */
void afficher_menu(void){
    printf("MENU\n");
    printf("l/L = lancer une partie\n");
    printf("r/R = lire les regles\n");
    printf("q/Q = quitter\n");
}

/**
 * Précondition : /
 * Postcondition : affiche les règles du jeu
 */
void afficher_regles(void){
    printf("Les regles du jeu sont simples.\n");
    printf("Une voiture rouge est presente sur le plateau. Elle correspond au symbole 'R'.\n");
    printf("Le but du jeu est de deplacer la voiture rouge pour qu'elle atteigne la sortie\n");
    printf("Atteindre la sortie signifie que le 'R' doit etre collé au dessin --> S présent sur le plateau\n");
    printf("Pour atteindre cet objectif, vous devrez deplacer les autres vehicules\n\n");
    printf("Voici comment se deroule un tour de jeu :\n");
    printf("Tout d'abord, vous devez choisir le symbole d'un véhicule à deplacer.\n");
    printf("Ensuite, vous pouvez deplacer votre vehicule du nombre de case que vous souhaitez.\n");
    printf("Enfin, vous constatez les consequences de votre deplacement et adaptez vos futures actions.\n\n");
}

/**
 * Précondition : /
 * Postcondition : affiche des consignes de jeu
 */
void afficher_consignes(void){
    printf("Pour deplacer un vehicule vous devez d'abord entrer EN MODE DEPLACEMENT en appuyant sur la lettre d/D.\n");
    printf("Pour recommencer avec le meme plateau de zero vous devez entrez la lettre r/R.\n");
    printf("Pour gagner il faut que la voiture rouge se trouve sur la case juste a coté du dessin --> S qui symbolise la sortie\n");
    printf("Attention lorsque vous etes EN MODE DEPLACEMENT, vous ne pouvez pas reinitialiser le plateau.\n");
    printf("Vous devez terminer votre tour avant de pouvoir recommencer!\n");
}

/**
 * Précondition : nb_colonnes initialisé
 * Postcondition : affiche les numeros des colonnes du plateau
 */
void afficher_numero_colonne(int nb_colonnes){
    for (int indice = 0; indice < nb_colonnes; indice++) {
        if(indice == 0){
            printf("   %d", indice+1);
        } else {
            printf("  %d",indice+1);
        }
    }
    printf("\n");
}

/**
 * Précondition : parking, indice_colonne et ligne initialisés. indice_colonne = 'A' et 0 <= ligne < parking->nb_lignes
 *  Postcondition : affiche une ligne du plateau de jeu
 */
void afficher_tableau(plateau * parking, char indice_ligne, int ligne){
    int colonne = 0;
    while (colonne < parking->nb_colonnes) {
        if(colonne == 0){
            printf("%c ",indice_ligne);
        }
        printf("[%c]", parking->matrice[ligne][colonne]);
        colonne++;
    }
}

/**
 * Précondition : parking est un plateau initialisé
 * Postcondition : affiche le plateau de jeu
 */
void afficher_matrice(plateau * parking){
    afficher_numero_colonne(parking->nb_colonnes);
    for(int ligne = 0; ligne < parking->nb_lignes; ligne++){
        char indice_colonne = 'A';
        indice_colonne = indice_colonne + ligne;
        afficher_tableau(parking, indice_colonne, ligne);
        if(indice_colonne == parking->sortie.ligne){
            printf(" --> S");
        }
        printf("\n");
    }
}