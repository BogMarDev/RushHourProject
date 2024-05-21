/**
 * \authors Bogatu Marco, Lois Rosman
 * \copyright HERS RUSH_HOUR PROJECT
 * \date 22/05/2024
 * \version final
 */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "display.h"
#include "util.h"
#include "game.h"

int main(void){
    srand(time(NULL));

    afficher_vide();
    printf("Bienvenue dans le jeu du rush hour\n\n");

    boolean is_finish = true;
    while (is_finish){
        afficher_ecran_lancement_partie();
        afficher_menu();

        printf("Faites votre choix : ");
        char choix_utilisateur = faire_choix();

        afficher_vide();
        switch(choix_utilisateur){
            case 'r': case 'R': afficher_regles(); pause(); break;
            case 'l': case 'L': deroulement_partie(); break;
            case 'q': case 'Q': is_finish = false; break;
            default: printf("Vous avez appuye sur une mauvaise touche\n\n"); pause(); break;
        }
        vider_tampon_stdin();
        afficher_vide();
    }
    printf("Merci d'avoir joue au Rush Hour ! \n");
    return EXIT_SUCCESS;
}