#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "display.h"
#include "util.h"
#include "move.h"

/**
 * Précondition : symboles est un tableau de caractère initialisé et nb_symboles_restants est initialisé
 * Postcondition : nb_symboles_restants est inchangé, symboles est modifié. Le dernier symbole de tableau
 *                 passe a l'emplacement du symbole tiré et le symbole tiré passe au dernier emplacement du tableau
 * Résultat : retourne le symbole contenu à l'indice de nb_aleatoire dans le tableau symboles
*/
char tire_symbole_aleatoire(char symboles[TAILLE], int * nb_symboles_restants){
    //Recupere un symbole aleatoire dans le tableau
    int nb_aleatoire = rand() % (*nb_symboles_restants);
    char symbole = symboles[nb_aleatoire];
    //Modifie le tableau qui contient tous les symboles
    symboles[nb_aleatoire] = symboles[*nb_symboles_restants];
    symboles[*nb_symboles_restants] = symbole;

    return symbole;
}

/**
 * Précondition : parking est un plateau initialisé
 * Postcondition : la matrice de parking est initialisée. Chaque case vide contient le caractère espace,
 *                 les cases contenant le véhicule rouge contiennent 'R' et les autres cases contenant
 *                 un véhicule contiennent des symboles aléatoires (1 par véhicule)
 */
void init_matrice(plateau * parking){
    //Allocation dynamique de la matrice de caractere
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
    }

    //Remplissage vide de toute la matrice
    for (int lines = 0; lines < parking->nb_lignes; ++lines) {
        for(int columns = 0; columns < parking->nb_colonnes; columns++){
            parking->matrice[lines][columns] = ' ';
        }
    }

    //Remplissage de la matrice avec les symboles de chaque voiture
    for(int indice = 0; indice < parking->nb_total_vehicule; indice++){
        int ligne_debut, colonne_debut = 0;
        conversion_coordonee(parking->liste_vehicule[indice].debut, &ligne_debut, &colonne_debut);
        parking->matrice[ligne_debut][colonne_debut] = parking->liste_vehicule[indice].symbole;

        int ligne_fin, colonne_fin = 0;
        conversion_coordonee(parking->liste_vehicule[indice].fin, &ligne_fin, &colonne_fin);
        parking->matrice[ligne_fin][colonne_fin] = parking->liste_vehicule[indice].symbole;

        if(parking->liste_vehicule[indice].taille == 3){
            int ligne_milieu, colonne_milieu = 0;
            coordonnee milieu = trouver_centre_camion(&parking->liste_vehicule[indice]);
            conversion_coordonee(milieu, &ligne_milieu, &colonne_milieu);
            parking->matrice[ligne_milieu][colonne_milieu] = parking->liste_vehicule[indice].symbole;
        }
    }
}

/**
 * Précondition : filename est un nom de fichier initialisé d'une certaine difficulté
 * Postcondition : filename reste inchangé
 * Résultat : crée un parking initalisé grace à la carte choisi dans le fichier
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
        printf("Faites votre choix : ");
        scanf("%d", &choix_carte);
    } while (!est_entre_limites(choix_carte, 1, nombre_total_cartes));

    //Creation de la chaine de caractere à rechercher dans le fichier
    char recherche[20] = "CARTE ";
    char nombre_str[] = "25";
    sprintf(nombre_str, "%d", choix_carte); //conversion du nombre en chaine de caractere
    strcat(recherche, nombre_str);
    strcat(recherche, "\r\n");

    //Deplacement du pointeur a la fin du fichier pour prendre la position en mémoire
    changer_position_pointeur_dans(f, 0, SEEK_END);
    long int fin_fichier = ftell(f);
    //Deplacement du pointeur au debut du fichier pour prendre la position en mémoire
    changer_position_pointeur_dans(f, 0, SEEK_SET);
    long int debut_fichier = ftell(f);

    int nb_lines = compteur_lignes(f, debut_fichier, fin_fichier);

    //Rechercher dans tout le fichier la ligne avec CARTE + choix_carte + \r\n
    long int position = trouver_position_avec_string(f, recherche, nb_lines);
    changer_position_pointeur_dans(f, position, SEEK_SET);

    //Initalisation du plateau de jeu
    plateau * parking = malloc(sizeof(plateau));
    parking->id = choix_carte;
    lire_entier_dans(f, &parking->nb_lignes);
    lire_entier_dans(f, &parking->nb_colonnes);

    //Initalisation de la sortie
    char ligne_sortie = getc(f);
    int colonne_sortie = 0;
    lire_entier_dans(f, &colonne_sortie);
    parking->sortie = (coordonnee) {ligne_sortie, colonne_sortie};

    lire_entier_dans(f, &parking->nb_min_coups);

    //Initalisation de la voiture rouge
    char ligne_debut = getc(f);
    int colonne_debut = 0;
    lire_entier_dans(f, &colonne_debut);
    char ligne_fin = getc(f);
    int colonne_fin = 0;
    lire_entier_dans(f, &colonne_fin);
    vehicule voiture_rouge = {2, {ligne_debut, colonne_debut}, {ligne_fin, colonne_fin}, 'R', 'H'};
    determiner_sens(&voiture_rouge);

    //Sauvegarde de la postition du pointeur dans le fichier juste apres la lecture des coordonnées de la voiture rouge
    //pour compter le nombre de voitur
    long int debut_flux = ftell(f);

    //Si choix_carte = derniere carte alors on va aller directement à la fin du fichier
    long int fin_flux = 0;
    if(choix_carte < nombre_total_cartes){
        //Creation d'une nouvelle chaine de caractere à rechercher
        char tmp[20] = "CARTE ";
        int nouveau_nombre = choix_carte + 1;
        sprintf(nombre_str, "%d", nouveau_nombre);
        strcat(tmp, nombre_str);
        strcat(tmp, "\r\n");
        //Recherche de la prochaine CARTE pour ensuite pouvoir compter le nombre total de vehicule
        fin_flux = trouver_position_avec_string(f, tmp, TAILLE); //20 est utiliser comme marge pour compter le nombre de ligne mais ca s'arrete avant
        changer_position_pointeur_dans(f, fin_flux-8, SEEK_SET);
        fin_flux = ftell(f);
    } else{
        fin_flux = fin_fichier + 1;
    }

    int nombre_vehicule_total = compteur_lignes(f, debut_flux, fin_flux);

    parking->nb_total_vehicule = nombre_vehicule_total;

    //Initialisation de tous les vehicules
    parking->liste_vehicule = malloc(sizeof(vehicule) * nombre_vehicule_total + 1);
    parking->liste_vehicule[0] = voiture_rouge; //deja initalisé auparavant

    char symboles[TAILLE] = {'+', '-', '=', '?', ';', '$', '*', '#', '%', '{',
                             '&', '@', '}', '>', '<', '/', '(', ')', '!', '|'};
    int nb_symboles_restants = TAILLE - 1;
    for (int i = 1; i < nombre_vehicule_total && ftell(f) < fin_flux; i++) {
        //Initalisation de chaque vehicule
        ligne_debut = getc(f);
        lire_entier_dans(f, &colonne_debut);
        ligne_fin = getc(f);
        lire_entier_dans(f, &colonne_fin);
        vehicule vehic = {0, {ligne_debut, colonne_debut}, {ligne_fin, colonne_fin}, '0', 'H'};
        determiner_sens(&vehic);
        determiner_taille(&vehic);
        vehic.symbole = tire_symbole_aleatoire(symboles, &nb_symboles_restants);
        parking->liste_vehicule[i] = vehic;
        nb_symboles_restants--;
    }

    init_matrice(parking);

    if(fclose(f) == EOF){
        perror("Erreur lors de la fermeture du flux !");
        exit(EXIT_FAILURE);
    }

    return parking;
}

/**
 * Précondition : parking et dep sont initialisé
 * Postcondition : parking et dep reste inchangé
 * Résultat : retourne true si la sortie est atteinte, false sinon
 * A TESTER
*/
boolean verifier_victoire(plateau * parking, deplacement * dep){
    int ligne_fin_voiture_rouge, colonne_fin_voiture_rouge = 0;
    conversion_coordonee(dep->vehicule_a_deplacer->fin, &ligne_fin_voiture_rouge, &colonne_fin_voiture_rouge);

    int ligne_sortie, colonne_sortie = 0;
    conversion_coordonee(parking->sortie, &ligne_sortie, &colonne_sortie);

    boolean est_gagne = false;
    if(dep->vehicule_a_deplacer->symbole == 'R' && colonne_fin_voiture_rouge == colonne_sortie && ligne_fin_voiture_rouge == ligne_sortie){
        est_gagne = true;
    }
    return est_gagne;
}

/**
 * Précondition : /
 * Postcondition : /
 * Retourne : une chaine de caractere qui contient le chemin d'acces vers un fichier d'une certaine difficulté
 */
char * choisir_difficulte(void){
    printf("Choissisez une difficulte\n");
    printf("1. Debutant\n");
    printf("2. Intermediaire\n");
    printf("3. Avance\n");
    printf("4. Expert\n");

    int choix_difficulte = 0;
    do {
        printf("Faites votre choix : ");
        scanf("%d", &choix_difficulte);
        vider_tampon_stdin();
    } while (!est_entre_limites(choix_difficulte, 1, 4));

    char * filename = malloc(36); //36 car taille du plus grand nom

    switch (choix_difficulte) {
        case 1: strcpy(filename, "../Files/rushHourDebutant.txt"); break;
        case 2: strcpy(filename, "../Files/rushHourIntermediaire.txt"); break;
        case 3: strcpy(filename, "../Files/rushHourAvance.txt"); break;
        case 4: strcpy(filename, "../Files/rushHourExpert.txt"); break;
        default: break;
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
    afficher_vide();
    plateau * parking = creer_parking_defaut(filename);
    //réalisation d'une copie de la liste de vehicule car on besoin des emplament de départ pour reinitialiser le plateau de jeu
    vehicule copie_liste_vehicule[parking->nb_total_vehicule];
    for (int i = 0; i < parking->nb_total_vehicule; i++) {
        copie_liste_vehicule[i] = parking->liste_vehicule[i];
    }

    score s1 = {parking->id, parking->nb_min_coups, 0};
    afficher_vide();
    afficher_consignes();
    vider_tampon_stdin();
    pause();

    int nbCoupJ = 0;
    deplacement * dep = NULL;
    do{
        afficher_vide();
        afficher_matrice(parking);

        printf("Faites votre choix(d/D = deplacer un vehicule, r/R = reinitialiser le plateau) : ");
        char choix = faire_choix();

        switch (choix) {
            case 'd': case 'D': dep = creer_deplacement(parking); deplacer_vehicule(parking, dep);
                nbCoupJ++; break;
            case 'r': case 'R':
                //Liberation de la matrice de caractere
                for (int i = 0; i < parking->nb_lignes; ++i) {
                    free(parking->matrice[i]);
                    parking->matrice[i] = NULL;
                }
                free(parking->matrice);
                parking->matrice = NULL;

                //on remet les emplacement de depart de chaque voitures
                for (int i = 0; i < parking->nb_total_vehicule; i++) {
                    parking->liste_vehicule[i] = copie_liste_vehicule[i];
                }
                init_matrice(parking);
                nbCoupJ = 0; break;
            default:
                printf("Vous avez appuye sur une mauvaise touche\n");
                vider_tampon_stdin(); pause(); break;
        }
        vider_tampon_stdin();
    }while (!verifier_victoire(parking, dep));

    s1.nb_coups_joueur = nbCoupJ;
    afficher_vide();
    afficher_matrice(parking);
    printf("Bravo, vous avez gagné avec %d coups. Le nombre de coups minimum pour ce plateau etait de %d", s1.nb_coups_joueur, s1.nb_coups_min);
    pause();

    //Liberation des allocations réaliser au cours de la partie
    dep->vehicule_a_deplacer = NULL;
    free(dep);
    dep = NULL;

    for (int i = 0; i < parking->nb_lignes; ++i) {
        free(parking->matrice[i]);
        parking->matrice[i] = NULL;
    }
    free(parking->matrice);
    parking->matrice = NULL;
    free(parking->liste_vehicule);
    parking->liste_vehicule = NULL;
    free(parking);
    parking = NULL;

    free(filename);
    filename = NULL;
}
