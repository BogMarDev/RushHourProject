#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>

#define TAILLE 20
#define NB_TOTAL_VEHICULE 4
#define INDICE_PREMIERE_LIGNE 'A'
#define INDICE_DERNIERE_LIGNE 'E'
#define INDICE_PREMIERE_COLONNE 1
#define INDICE_DERNIERE_COLONNE 7

typedef struct {
    unsigned char ligne;
    unsigned int colonne;
}coordonnee;

typedef struct {
    short taille;
    coordonnee debut;
    coordonnee fin;
    char symbole;
}vehicule;

typedef struct{
    unsigned int nbLignes;
    unsigned int nbColonnes;
    unsigned int nbMinCoups;
    coordonnee sortie;
    char matrice[5][7];
    vehicule ** listeVehicule;
}plateau;

typedef struct {
    coordonnee caseArrivee;
    char symbole;
}deplacement;

//utile
/** \fn flush_stdin_buffer
 *  \brief : flush stdin
 */
void flush_stdin_buffer(void){
    int c = -1;
    do{
        c = getchar();
    }while(c != '\n' && c != EOF);
}

//Afficher
void display_clear(void){
    system("@cls|clear");
}

void display_splash_screen(void){
    printf("|R|===|R|  |U|     |U|   |S|S|S|  |H|    |H|    |H|    |H|   |O||O|    |U|     |U|  |R|===|R|\n");
    printf("|R|    |R| |U|     |U| |S|        |H|    |H|    |H|    |H| |O|    |O|  |U|     |U|  |R|    |R|\n");
    printf("|R|===|R|  |U|     |U|   |S||S|   |H|====|H|    |H|====|H| |O|    |O|  |U|     |U|  |R|===|R|\n");
    printf("|R|  |R|   |U|     |U|        |S| |H|====|H|    |H|====|H| |O|    |O|  |U|     |U|  |R|  |R|\n");
    printf("|R|   |R|   |U|   |U|        |S|  |H|    |H|    |H|    |H| |O|    |O|   |U|   |U|   |R|   |R|\n");
    printf("|R|    |R|    | U |    |S|S|S|    |H|    |H|    |H|    |H|   |O||O|       | U |     |R|    |R|\n\n");
}

void afficher_menu(void){
    printf("MENU\n");
    printf("l/L = lancer une partie\n");
    printf("r/R = lire les régles\n");
    printf("q/Q = quitter\n");
}

void afficher_regles(){
    printf("Les règles du jeu sont simples.\n");
    printf("Une voiture rouge est présente sur le plateau\n");
    printf("Le but du jeu est de déplacer le véhicule rouge pour qu'elle atteigne la sortie\n");
    printf("Pour atteindre cet objectif, vous devrez déplacer les autres véhicules\n\n");
    printf("Voici comment se déroule un tour de jeu :\n");
    printf("Tout d'abord, vous devez choisir un véhicule à déplacer\n");
    printf("Ensuite, vous devez déplacer votre véhicule verticalement ou horizontalement du nombre de case que vous souhaiter\n");
    printf("Enfin, vous constatez les conséquence de votre déplacement et adaptez vos futures actions\n\n");
}

void init_parking(plateau parking){
    for(int indiceLigne = 0; indiceLigne < parking.nbLignes; indiceLigne++){
        for(int indiceColonne = 0; indiceColonne < parking.nbColonnes; indiceColonne++){
            parking.matrice[indiceLigne][indiceColonne] = (char) {0};
        }
    }
}

/** \fn modify_symbols
 *  \brief shift all boxes containing symbols by one of the left
 *  \param symbols : array with all symbols of the game
 *  \param index : index of the box containing the selected symbol
*/
void modifier_symboles(char symboles[TAILLE], int indice){
    while(indice < TAILLE && symboles[indice]){
        if(indice < TAILLE - 1){
            symboles[indice] = symboles[indice + 1];
        }
        else{
            symboles[indice] = 0;
        }
        indice++;
    }
}

/** \fn random_symbol
 *  \brief select a random symbol
 *  \param symbols : array with all symbols of the game
 *  \param nb_symbols_remaining : number of symbols that can still be taken randomly
*/
char tire_symbole_aleatoire(char symboles[TAILLE], int nb_symboles_restants){
    int nb_aleatoire = rand() % (nb_symboles_restants + 1);
    char symbole = symboles[nb_aleatoire];
    modifier_symboles(symboles, nb_aleatoire);

    return symbole;
}

char ligne_utilisateur(){
    char deplacement_utilisateur = 0;

    do {
        printf("Veuillez entrer la lettre de la ligne d'une des cases comprenant le véhicule que vous souhaitez déplacer : ");
        scanf("%c", &deplacement_utilisateur);
        flush_stdin_buffer();
    }while(deplacement_utilisateur >= INDICE_PREMIERE_LIGNE && deplacement_utilisateur <= INDICE_DERNIERE_LIGNE);

    return deplacement_utilisateur;
}

int colonne_utilisateur(){
    unsigned int deplacement_utilisateur = 0;

    do{
        printf("Veuillez entrer le chiffre de la colonne d'une des cases comprenant le véhicule que vous souhaitez déplacer : ");
        scanf("%d", &deplacement_utilisateur);

        if(deplacement_utilisateur >= INDICE_PREMIERE_COLONNE && deplacement_utilisateur <= INDICE_DERNIERE_COLONNE){
            printf("Vous avez entré un numéro de colonne invalide. Vous devez entrer un numéro entre %d et %d\n",
                   INDICE_PREMIERE_COLONNE, INDICE_DERNIERE_COLONNE);
        }
    }while(deplacement_utilisateur >= INDICE_PREMIERE_COLONNE && deplacement_utilisateur <= INDICE_DERNIERE_COLONNE);

    return deplacement_utilisateur;
}

int case_contient_vehicule(plateau parking, coordonnee vehicule_a_deplacer){
    int contient_vehicule = 0;
    if(parking.matrice[vehicule_a_deplacer.ligne][vehicule_a_deplacer.colonne] != ' '){
        contient_vehicule = 1;
    }
    else{
        printf("Vous avez entré les coordonnées d'une case qui ne contient pas de véhicule\n");
        printf("Veuillez entrer les coordonnées d'une case contenant le véhicule que vous souhaitez déplacer\n");
    }

    return contient_vehicule;
}

coordonnee saisie_coordonnee(plateau parking){
    coordonnee emplacement = {0, 0};
    do {
        emplacement.ligne = ligne_utilisateur();
        emplacement.colonne = colonne_utilisateur();
    }while(case_contient_vehicule(parking, emplacement) );

    return emplacement;
}

void display_index_column(int columns_max){
    for (int i = 0; i < columns_max; i++) {
        printf("%d", i+1); //à adapter comment sera le plateau
    }
    printf("\n");
}

void afficher_parking(plateau parking){
    display_index_column(parking.nbColonnes);
    init_parking(parking);
    //TRUC AVEC LA MATRICE
}

void effectuer_deplacement(plateau parking){
    coordonnee vehicule_a_deplacer = saisie_coordonnee(parking);
    coordonnee nouvel_emplacement = saisie_coordonnee(parking);
}

void deroulement_partie(){
    plateau parking;
    //1. on initialise le plateau
    parking.nbLignes = 5;
    parking.nbColonnes = 7;
    parking.nbMinCoups = 2;
    parking.sortie = (coordonnee) {'E', 7};
    //2. on initalise la matrice qui contient les voitures donc les voitures avec
    //2.1 la voiture rouge est initilise
    coordonnee debut = {'E', 2};
    coordonnee fin = {'E', 3};
    //compter le nombre de case entre le debut et la fin pour avoir la taille
    vehicule voitureRouge = {2, debut, fin, 'R'};
    //symbole trouver grace au systeme de lois
    init_parking(parking);
    //2.1.1 ajout de la voiture rouge dans la matrice
    parking.listeVehicule[0] = &voitureRouge;
    //2.2 boucle qui initilise les autres voitures puis les ajoute dans la matrice
    debut.ligne = 'B';
    debut.colonne = 1;
    //case au milieu B2
    fin.ligne = 'B';
    fin.colonne = 3;
    vehicule camion1 = {3, debut, fin, ' '};
    parking.listeVehicule[1] = &camion1;

    debut.ligne = 'C';
    debut.colonne = 5;
    //case au milieu D5
    fin.ligne = 'E';
    fin.colonne = 5;
    vehicule camion2 = {3, debut, fin, ' '};
    parking.listeVehicule[2] = &camion2;

    debut.ligne = 'A';
    debut.colonne = 6;
    fin.ligne = 'A';
    fin.colonne = 7;
    vehicule voiture = {2, debut, fin, ' '};
    parking.listeVehicule[3] = &camion2;

    for(int indice = 1, nb_symboles_restants = TAILLE; indice < NB_TOTAL_VEHICULE; indice++, nb_symboles_restants--) {
        char symboles[TAILLE] = {'+', '-', '=', '?', '~', '$', '*', '#', '%', '{',
                                 '&', '@', '^', '>', '<', '/', '(', ')', '[', ']'};
        parking.listeVehicule[indice]->symbole = tire_symbole_aleatoire(symboles, nb_symboles_restants);
    }

    effectuer_deplacement(parking);
}

/** \fn choose_action
 *  \brief determine the action to made based on the value input by the user
 *  \param command : value input by the user
*/
void choisir_action(char choix){
    switch(choix){
        case 'r':
        case 'R':
            afficher_regles();
            break;
        case 'l':
        case 'L':
            deroulement_partie();
            break;
        default:
            printf("Vous avez appuye sur une mauvaise touche\n\n");
            break;
    }
}

//savoir faire avec des fichiers binaires pour l'exam de C
int main(void){
    srand(time(NULL));
    display_clear();
    display_splash_screen();
    printf("Bienvenue dans le jeu du rush hour\n");

    char choix_utilisateur = 0;
    do {
        afficher_menu();
        printf("Faites votre choix : ");
        scanf("%c", &choix_utilisateur);

        if (choix_utilisateur != 'q' && choix_utilisateur != 'Q') {
            choisir_action(choix_utilisateur);
        }
    }while(choix_utilisateur != 'q' && choix_utilisateur != 'Q');

    printf("Merci d'avoir joué, à bientôt\n");

    return EXIT_SUCCESS;
}









coordonnee trouver_centre_camion(vehicule camion){
    coordonnee milieu = {0, 0};
    if(camion.debut.ligne == camion.fin.ligne){
        milieu.ligne = camion.debut.ligne;
        milieu.colonne = camion.debut.colonne + 1;
    }
    else{
        milieu.colonne = camion.debut.colonne;
        milieu.ligne = camion.debut.ligne + 1;
    }

    return milieu;
}

/**
 * \fn get_choice
 * \brief get a character choose by the user
*/
char get_choice(void){
    char c = -1;
    scanf("%c", &c);
    return c;
}

/**
 * \fn pause
 * \brief pauses the program during execution when necessary
*/
void pause(void){
    printf("\n\x1b[31;47mEntrez n'importe quelle caractere pour revenir au menu\x1b[0m...");
    get_choice();
}


