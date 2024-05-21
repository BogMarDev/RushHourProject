#include "move.h"
#include <stdlib.h>
#include <stdio.h>
#include "util.h"

/**
 * Précondition : parking est initialisé
 * Postcondition : parking est inchangé
 * Résultat : retourne un deplacement saisi par l'utilisateur
*/
deplacement * creer_deplacement(plateau * plateau1) {
    deplacement *dep = malloc(sizeof(deplacement));
    if(dep == NULL){
        printf("Erreur : la mémoire n'as pas pu etre alloué\n");
        exit(EXIT_FAILURE);
    }

    int index = 0;
    do {
        vider_tampon_stdin();
        char symbole = 0;
        printf("Entrez le symbole du vehicule que vous voulez deplacer : ");
        scanf("%c", &symbole);
        index = indice_voiture_recherche(plateau1, symbole);
    } while (index == -1);

    dep->vehicule_a_deplacer = &plateau1->liste_vehicule[index];

    dep->ecart_horizontal = 0;
    dep->ecart_vertical = 0;

    if (dep->vehicule_a_deplacer->sens_vehicule == 'H') {
        printf("Entrez le nombre de cases à déplacer horizontalement (positif pour droite, négatif pour gauche) : ");
        scanf("%d", &dep->ecart_horizontal);
    } else {
        printf("Entrez le nombre de cases à déplacer verticalement (positif pour haut, négatif pour bas) : ");
        scanf("%d", &dep->ecart_vertical);
        dep->ecart_vertical = -(dep->ecart_vertical);
    }

    return dep;
}

/**
 * Vérifie si les nouvelles coordonnées du véhicule sont dans les limites de la grille.
 *
 * @param plat Pointeur vers la structure plateau contenant les informations sur la grille et les véhicules.
 * @param vehicule_a_deplacer Structure vehicule représentant le véhicule à déplacer.
 * @param nouvelle_colonne Nouvelle coordonnée colonne après déplacement.
 * @param nouvelle_ligne Nouvelle coordonnée ligne après déplacement.
 * @return true si les coordonnées sont dans les limites de la grille, false sinon.
 */
 //a tester
boolean dans_limites_grille(plateau *plat, vehicule vehicule_a_deplacer, int nouvelle_colonne, int nouvelle_ligne) {
    boolean est_dans_limites = true;

    if (vehicule_a_deplacer.sens_vehicule == 'H') {
        if (nouvelle_colonne < 0 || (nouvelle_colonne + vehicule_a_deplacer.taille) > plat->nb_colonnes || nouvelle_ligne < 0 || nouvelle_ligne >= plat->nb_lignes) {
            est_dans_limites = false;
        }
    } else {
        if (nouvelle_ligne < 0 || (nouvelle_ligne + vehicule_a_deplacer.taille) > plat->nb_lignes || nouvelle_colonne < 0 || nouvelle_colonne >= plat->nb_colonnes) {
            est_dans_limites = false;
        }
    }

    return est_dans_limites;
}

/**
 * Vérifie s'il y a une collision avec d'autres véhicules aux nouvelles coordonnées.
 *
 * @param plat Pointeur vers la structure plateau contenant les informations sur la grille et les véhicules.
 * @param vehicule_a_deplacer Structure vehicule représentant le véhicule à déplacer.
 * @param nouvelle_colonne Nouvelle coordonnée colonne après déplacement.
 * @param nouvelle_ligne Nouvelle coordonnée ligne après déplacement.
 * @return true s'il y a une collision, false sinon.
 */
 //a tester
boolean collision_detectee(plateau *plat, vehicule vehicule_a_deplacer, int nouvelle_colonne, int nouvelle_ligne) {
    boolean collision = false;

    if (vehicule_a_deplacer.sens_vehicule == 'H') {
        for (int j = 0; j < vehicule_a_deplacer.taille; ++j) {
            int cible_colonne = nouvelle_colonne + j;
            int cible_ligne = nouvelle_ligne;
            if (cible_colonne >= 0 && cible_colonne < plat->nb_colonnes) {
                if (plat->matrice[cible_ligne][cible_colonne] != ' ' &&
                    (cible_colonne < vehicule_a_deplacer.debut.colonne - 1 || cible_colonne > vehicule_a_deplacer.fin.colonne - 1)) {
                    collision = true;
                }
            }
        }
    } else {
        for (int j = 0; j < vehicule_a_deplacer.taille; ++j) {
            int cible_colonne = nouvelle_colonne;
            int cible_ligne = nouvelle_ligne + j;
            if (cible_ligne >= 0 && cible_ligne < plat->nb_lignes) {
                if (plat->matrice[cible_ligne][cible_colonne] != ' ' &&
                    (cible_ligne < vehicule_a_deplacer.debut.ligne - 65 || cible_ligne > vehicule_a_deplacer.fin.ligne - 65)) {
                    collision = true;
                }
            }
        }
    }

    return collision;
}

/**
 * Vérifie si un déplacement de véhicule est possible sur la grille.
 *
 * @param plat Pointeur vers la structure plateau contenant les informations sur la grille et les véhicules.
 * @param deplace Structure deplacement représentant le déplacement à effectuer.
 * @return true si le déplacement est possible, false sinon.
 */
boolean deplacement_est_valide(plateau *plat, deplacement *deplace) {
    boolean est_valide = true;

    // Vérification de la validité de la direction de déplacement
    if ((deplace->vehicule_a_deplacer->sens_vehicule == 'H' && deplace->ecart_vertical != 0) ||
        (deplace->vehicule_a_deplacer->sens_vehicule == 'V' && deplace->ecart_horizontal != 0)) {
        printf("Erreur : Déplacement impossible dans cette direction.\n");
        est_valide = false;
    }

    // Calcul des nouvelles coordonnées après le déplacement
    int ligne, colonne = 0;
    conversion_coordonee(deplace->vehicule_a_deplacer->debut, &ligne, &colonne);
    int nouvelle_colonne = colonne + deplace->ecart_horizontal;
    int nouvelle_ligne = ligne + deplace->ecart_vertical;

    // Vérification des limites de la grille
    if (est_valide && !dans_limites_grille(plat, *deplace->vehicule_a_deplacer, nouvelle_colonne, nouvelle_ligne)) {
        printf("Erreur : Déplacement en dehors des limites de la grille.\n");
        est_valide = false;
    }

    // Vérification des collisions avec d'autres véhicules
    if (est_valide && collision_detectee(plat, *deplace->vehicule_a_deplacer, nouvelle_colonne, nouvelle_ligne)) {
        printf("Erreur : La position cible est occupée.\n");
        est_valide = false;
    }

    return est_valide;
}

/**
 * Déplace un véhicule sur la grille.
 *
 * @param plat Pointeur vers la structure plateau contenant les informations sur la grille et les véhicules.
 * @param deplace Structure deplacement représentant le déplacement à effectuer.
 */
void deplacer_vehicule(plateau *plat, deplacement * deplace) {
    while (!deplacement_est_valide(plat, deplace)) {
        printf("Déplacement impossible.\n");
        printf("Veuillez refaire le déplacement.\n");
        deplace = creer_deplacement(plat);
    }
    vehicule * vehicule_a_deplacer = deplace->vehicule_a_deplacer;

    int ligne, colonne = 0;
    conversion_coordonee(vehicule_a_deplacer->debut, &ligne, &colonne);

    // Effacement de l'ancienne position
    if (vehicule_a_deplacer->sens_vehicule == 'H') {
        for (int i = 0; i < vehicule_a_deplacer->taille; i++) {
            plat->matrice[ligne][colonne + i] = ' ';
        }
    } else {
        for (int i = 0; i < vehicule_a_deplacer->taille; i++) {
            plat->matrice[ligne + i][colonne] = ' ';
        }
    }

    // Mise à jour des coordonnées du véhicule
    colonne += deplace->ecart_horizontal;
    ligne += deplace->ecart_vertical;
    vehicule_a_deplacer->debut.ligne = ligne + 65;
    vehicule_a_deplacer->debut.colonne = colonne + 1;
    if (vehicule_a_deplacer->sens_vehicule == 'H') {

        vehicule_a_deplacer->fin.ligne = vehicule_a_deplacer->debut.ligne;
        vehicule_a_deplacer->fin.colonne = vehicule_a_deplacer->debut.colonne + vehicule_a_deplacer->taille - 1;
    } else {
        vehicule_a_deplacer->fin.ligne = vehicule_a_deplacer->debut.ligne + vehicule_a_deplacer->taille - 1;
        vehicule_a_deplacer->fin.colonne = vehicule_a_deplacer->debut.colonne;
    }

    // Mise à jour de la nouvelle position
    if (vehicule_a_deplacer->sens_vehicule == 'H') {
        for (int i = 0; i < vehicule_a_deplacer->taille; i++) {
            plat->matrice[ligne][colonne + i] = vehicule_a_deplacer->symbole;
        }
    } else {
        for (int i = 0; i < vehicule_a_deplacer->taille; i++) {
            plat->matrice[ligne + i][colonne] = vehicule_a_deplacer->symbole;
        }
    }

    vehicule_a_deplacer = NULL;
}

