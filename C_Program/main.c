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
    int nbLignes;
    int nbColonnes;
    int nbMinCoups;
    coordonnee sortie;
    char matrice[5][7];
    vehicule ** listeVehicule;
}plateau;

typedef struct {
    coordonnee caseDebut;
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
    printf("\n\x1b[31;47mAppuyez sur ENTER(↵) pour continuer\x1b[0m...");
    get_choice();
}

//Afficher
void display_clear(void){
    system("cls|clear");
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

void afficher_regles(void){
    printf("Les règles du jeu sont simples.\n");
    printf("Une voiture rouge est présente sur le plateau\n");
    printf("Le but du jeu est de déplacer le véhicule rouge pour qu'elle atteigne la sortie\n");
    printf("Pour atteindre cet objectif, vous devrez déplacer les autres véhicules\n\n");
    printf("Voici comment se déroule un tour de jeu :\n");
    printf("Tout d'abord, vous devez choisir un véhicule à déplacer\n");
    printf("Ensuite, vous devez déplacer votre véhicule verticalement ou horizontalement du nombre de case que vous souhaiter\n");
    printf("Enfin, vous constatez les conséquence de votre déplacement et adaptez vos futures actions\n\n");
}

void afficher_consignes(void){
    printf("Pour faire deplacer un vehicule vous devez d'abord entrez la lettre d.\n");
    printf("Pour recommencer avec le meme plateau de zero vous devez entrez la lettre r.\n");
    printf("Attention lorsque vous etes EN MODE DEPLACEMENT, vous ne pouvez pas remettre le plateau comme il était au début.\n");
}

void afficher_index_colonne(int columns_max){
    for (int i = 0; i < columns_max; i++) {
        if(i == 0){
            printf("   %d", i+1);
        } else {
            printf("  %d",i+1);
        }
    }
    printf("\n");
}

void afficher_parking(plateau parking){
    afficher_index_colonne(parking.nbColonnes);

    for(int line = 0; line < parking.nbLignes; line++){
        char indice_col = 'A';
        for (int column = 0; column < parking.nbColonnes; column++) {
            indice_col += line;
            if(column == 0){
                printf("%c |%c|",indice_col, parking.matrice[line][column]);
            } else{
                printf("|%c|", parking.matrice[line][column]);
            }
        }
        printf("\n");
    }
}

//GAMEPLAY
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

void conversion_coordonee(coordonnee c, int * line, int * column){
    *line = c.ligne - 65;
    *column = c.colonne - 1;
}

coordonnee trouver_centre_camion(vehicule camion) {
    coordonnee milieu = {0, 0};
    if (camion.debut.ligne == camion.fin.ligne) {
        milieu.ligne = camion.debut.ligne;
        milieu.colonne = camion.debut.colonne + 1;
    } else {
        milieu.colonne = camion.debut.colonne;
        milieu.ligne = camion.debut.ligne + 1;
    }

    return milieu;
}

void init_matrice(plateau * p){
    for(int indiceLigne = 0; indiceLigne < p->nbLignes; indiceLigne++){
        for(int indiceColonne = 0; indiceColonne < p->nbColonnes; indiceColonne++){
            p->matrice[indiceLigne][indiceColonne] = ' ';
        }
    }

    for(int index = 0; index < NB_TOTAL_VEHICULE; index++){
        int line = 0;
        int col = 0;
        conversion_coordonee(p->listeVehicule[index]->debut, &line, &col);
        p->matrice[line][col] = p->listeVehicule[index]->symbole;
        line = 0;
        col = 0;
        conversion_coordonee(p->listeVehicule[index]->fin, &line, &col);
        p->matrice[line][col] = p->listeVehicule[index]->symbole;
        if(p->listeVehicule[index]->taille == 3){
            line = 0;
            col = 0;
            coordonnee milieu = trouver_centre_camion(p->listeVehicule[index][0]);
            conversion_coordonee(milieu, &line, &col);
            p->matrice[line][col] = p->listeVehicule[index]->symbole;
        }
    }

    int line_sortie = 0;
    int col_sortie = 0;
    conversion_coordonee(p->sortie, &line_sortie, &col_sortie);
    p->matrice[line_sortie][col_sortie] = 'S';
}

plateau init_parking_defaut(){
    plateau parking = {};
    parking.listeVehicule = malloc(NB_TOTAL_VEHICULE * sizeof(vehicule *));
    assert(parking.listeVehicule != NULL);
    if(parking.listeVehicule == NULL){
        printf("Memoire non-alloué\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < NB_TOTAL_VEHICULE; ++i) {
        parking.listeVehicule[i] = malloc(sizeof(vehicule));
        assert(parking.listeVehicule[i] != NULL);
        if(parking.listeVehicule[i] == NULL){
            printf("Memoire non-alloué\n");
            exit(EXIT_FAILURE);
        }
    }

    parking.nbLignes = 5;
    parking.nbColonnes = 7;
    parking.nbMinCoups = 2;
    parking.sortie = (coordonnee) {'E', 7};

    //sens du vehic facile a definir car si c'est la meme lettre on est H = Horizontal sinon on est V = Vertical
    coordonnee debut = {'E', 2};
    coordonnee fin = {'E', 3};
    vehicule voitureRouge = {2, debut, fin, 'R', 'H'};
    parking.listeVehicule[0] = &voitureRouge;

    //3. camion n°1
    debut.ligne = 'B';
    debut.colonne = 1;
    //case au milieu B2
    fin.ligne = 'B';
    fin.colonne = 3;
    vehicule camion1 = {3, debut, fin, '*', 'H'};
    parking.listeVehicule[1] = &camion1;

    //4. camion n°2
    debut.ligne = 'C';
    debut.colonne = 5;
    //case au milieu D5
    fin.ligne = 'E';
    fin.colonne = 5;
    vehicule camion2 = {3, debut, fin, '+', 'V'};
    parking.listeVehicule[2] = &camion2;

    //5. voiture n°1
    debut.ligne = 'A';
    debut.colonne = 6;
    fin.ligne = 'A';
    fin.colonne = 7;
    vehicule voiture = {2, debut, fin, '?', 'H'};
    parking.listeVehicule[3] = &voiture;


    for(int indice = 1, nb_symboles_restants = TAILLE; indice < NB_TOTAL_VEHICULE; indice++, nb_symboles_restants--) {
        char symboles[TAILLE] = {'+', '-', '=', '?', '~', '$', '*', '#', '%', '{',
                                 '&', '@', '^', '>', '<', '/', '(', ')', '[', ']'};
        parking.listeVehicule[indice]->symbole = tire_symbole_aleatoire(symboles, nb_symboles_restants);
    }

    init_matrice(&parking);

    return parking;
}

int a_atteint_sortie(plateau parking){
    int line_voiture_rouge = 0;
    int col_voiture_rouge = 0;
    //J'ai mis la fin ca je ne sais si c'est le debut ou la fin qu'on a mis a droite
    conversion_coordonee(parking.listeVehicule[0]->fin, &line_voiture_rouge, &col_voiture_rouge);
    int line_sortie = 0;
    int col_sortie = 0;
    conversion_coordonee(parking.sortie, &line_sortie, &col_sortie);

    int a_atteint_sortie = 1;
    if(line_voiture_rouge == line_sortie && col_voiture_rouge == col_sortie){
        a_atteint_sortie--;
    }

    return a_atteint_sortie;
}

void free_matrice(plateau parking){
    for (int i = 0; i < NB_TOTAL_VEHICULE; ++i) {
        free(parking.listeVehicule[i]);
        parking.listeVehicule[i] = NULL;
    }
    free(parking.listeVehicule);
    parking.listeVehicule = NULL;
}

//Deplacement

//LIGNE ET COLONNE USER ONT LE MEME CODE PEUT-ETRE QU'ON PEUT EN FAIRE QU'UNE SEULE
char ligne_utilisateur(){
    char deplacement_utilisateur = 0;
    int is_good = 1;
    while(is_good){
        flush_stdin_buffer();
        printf("Veuillez entrer la lettre de la ligne de la case : ");
        deplacement_utilisateur = get_choice();

        if(deplacement_utilisateur < INDICE_PREMIERE_LIGNE || deplacement_utilisateur > INDICE_DERNIERE_LIGNE){
            printf("Vous avez entré une lettre de ligne invalide. Vous devez entrer une ligne entre %c et %c\n",INDICE_PREMIERE_LIGNE, INDICE_DERNIERE_LIGNE);
        } else{
            is_good--;
        }

    }


    return deplacement_utilisateur;
}

int colonne_utilisateur(){
    int deplacement_utilisateur = 0;
    int is_good = 1;
    while(is_good){
        printf("Veuillez entrer le chiffre de la colonne de la case : ");
        scanf("%d", &deplacement_utilisateur);

        if(deplacement_utilisateur < INDICE_PREMIERE_COLONNE || deplacement_utilisateur > INDICE_DERNIERE_COLONNE){
            printf("Vous avez entré un numéro de colonne invalide. Vous devez entrer un numéro entre %d et %d\n",INDICE_PREMIERE_COLONNE, INDICE_DERNIERE_COLONNE);
        } else{
            is_good--;
        }
    }

    return deplacement_utilisateur;
}

coordonnee saisie_coordonnee(){
    coordonnee emplacement = {0, 0};
    emplacement.ligne = ligne_utilisateur();
    emplacement.colonne = colonne_utilisateur();
    return emplacement;
}

int case_contient_vehicule(plateau * parking, coordonnee emplacement){
    int contient_vehicule = 0;

    int line = 0;
    int col = 0;
    conversion_coordonee(emplacement, &line, &col);

    if(parking->matrice[line][col] != ' '){
        contient_vehicule = 1;
    }

    return contient_vehicule;
}

deplacement init_deplacement(plateau * parking){
    deplacement d = {};

    int is_good = 1;
    coordonnee vehicule_a_deplacer = {0,0};
    while (is_good){
        printf("Vous devez saisir une case contenant un vehicule, càd une case avec un symbole.\n");
        vehicule_a_deplacer = saisie_coordonnee();
        if(!(case_contient_vehicule(parking, vehicule_a_deplacer))){
            printf("\nAttention: vous avez entre une case qui est vide\n");
            printf("Pour déplacer un véhicule sur une case, il faut choisir une case avec un vehicule\n\n");
        } else{
            is_good--;
        }
    }

    is_good = 1;
    coordonnee nouvel_emplacement = {0,0};
    while (is_good){
        printf("Vous devez saisir une case vide ou vous souhaitez deplacer le vehicule choisi.\n");
        nouvel_emplacement = saisie_coordonnee();
        if(case_contient_vehicule(parking, nouvel_emplacement)){
            printf("\nAttention: vous avez entre une case qui contient un véhicule\n");
            printf("Pour déplacer un véhicule sur une case, il faut que cette case soit libre\n\n");
        } else{
            is_good--;
        }
    }

    d.caseDebut = vehicule_a_deplacer;
    d.caseArrivee = nouvel_emplacement;

    int line = 0;
    int col = 0;
    conversion_coordonee(vehicule_a_deplacer, &line, &col);
    d.symbole = parking->matrice[line][col];

    return d;
}

int est_valide(deplacement depl, plateau * parking){
    int est_valide = 1;
    //sens de déplacement doit etre le meme que le vehicule en question
    //Premiere verif
    char sens_depl = 0;
    if(depl.caseDebut.ligne == depl.caseArrivee.ligne){
        sens_depl = 'H';
    } else if(depl.caseDebut.colonne == depl.caseArrivee.colonne){
        sens_depl = 'V';
    } else{
        est_valide = -1;
    }

    //deuxieme verif
    int est_trouve = 0;
    int i = 0;
    while (i < NB_TOTAL_VEHICULE && !est_trouve) {
        if(depl.symbole == parking->listeVehicule[i]->symbole){
            if(sens_depl != parking->listeVehicule[i]->sens_vehicule){
                est_valide = -2;
            }
            est_trouve = 1;
        } else{
            i++;
        }
    }

    int nb_cases_besoin = 0;
    if(parking->listeVehicule[i]->taille == 2){
        nb_cases_besoin = 1;
    } else{
        nb_cases_besoin = 2;
    }
    //troisieme verif
    //voir si les autres cases qui sont derriere celle d'arrive sont vides ou contiene le meme symbole
    if(sens_depl == 'H'){
        int line = 0;
        int col = 0;
        conversion_coordonee(depl.caseArrivee, &line, &col);
        if(parking->matrice[line][col-1] != ' ' && parking->matrice[line][col-1] != depl.symbole){
            est_valide = -3;
        }

        if(nb_cases_besoin == 2) {
            if(parking->matrice[line][col-2] != ' ' && parking->matrice[line][col-2] != depl.symbole){
                est_valide = -3;
            }
        }
    } else{
        int line = 0;
        int col = 0;
        conversion_coordonee(depl.caseArrivee, &line, &col);
        if(parking->matrice[line-1][col] != ' ' && parking->matrice[line-1][col] != depl.symbole){
            est_valide = -3;
        }

        if(nb_cases_besoin == 2) {
            if(parking->matrice[line-2][col] != ' ' && parking->matrice[line-2][col] != depl.symbole){
                est_valide = -3;
            }
        }
    }

    return est_valide;
}

void deplacement_vehicule(deplacement d, plateau * parking){
    //A finir
}

void effectuer_deplacement(plateau * parking){
    deplacement d1 = init_deplacement(parking);

    while (est_valide(d1, parking) < 0){
        printf("Le deplacement ne peut s'effectuer car vous essayez de deplacer la voiture ");
        //trouver la raison et faire un message d'erreur en fonction DONC plusieurs code erreurs
        if(est_valide(d1, parking) == -1){
            printf("de maniere diagonale.\n");
        } else if(est_valide(d1, parking) == -2){
            printf("dans le sens opposé du vehicule.\n");
        } else {
            printf("sur une case deja occupé par un vehicule.\n");
        }
        d1 = init_deplacement(parking);
    }

    deplacement_vehicule(d1, parking);
    printf("Deplacement_reussi\n");
    pause();
}

void deroulement_partie(void){
    flush_stdin_buffer();
    plateau parking = init_parking_defaut();
    afficher_consignes();
    pause();

    int nbCoupJ = 0;
    while (a_atteint_sortie(parking)){
        display_clear();
        afficher_parking(parking);

        printf("Faites votre choix(d/D = deplacer un vehicule, r/R = reinitialiser le plateau) : ");
        char choix = get_choice();

        switch (choix) {
            case 'd': case 'D': effectuer_deplacement(&parking); nbCoupJ++; break;
            case 'r': case 'R': init_matrice(&parking); nbCoupJ = 0; break;
            default: printf("Vous avez appuyé sur une mauvaise touche\n"); break;
        }
        flush_stdin_buffer();
    }

    free_matrice(parking);
}

int main(void){
    srand(time(NULL));

    display_clear();
    printf("Bienvenue dans le jeu du rush hour\n\n");

    int is_finish = 1;
    while (is_finish){
        display_splash_screen();
        afficher_menu();
        printf("Faites votre choix : ");
        char choix_utilisateur = get_choice();

        display_clear();
        switch(choix_utilisateur){
            case 'r': case 'R': afficher_regles(); pause(); break;
            case 'l': case 'L': deroulement_partie(); break;
            case 'q': case 'Q': is_finish--; break;
            default: printf("Vous avez appuye sur une mauvaise touche\n"); break;
        }

        flush_stdin_buffer();
        display_clear();
    }

    return EXIT_SUCCESS;
}