/**
 * \authors Bogatu Marco, Lois Rosman
 * \copyright HERS RUSH_HOUR PROJECT
 * \date 29/04/2024
 * \version Proof of Concept
 */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define TAILLE 20
#define NB_TOTAL_VEHICULE 4
#define INDICE_PREMIERE_LIGNE 'A'
#define INDICE_DERNIERE_LIGNE 'E'
#define INDICE_PREMIERE_COLONNE 1
#define INDICE_DERNIERE_COLONNE 7

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
    int nb_lignes;
    int nb_colonnes;
    int nb_min_coups;
    coordonnee sortie;
    char matrice[5][7];
    vehicule liste_vehicule[NB_TOTAL_VEHICULE];
}plateau;

typedef struct {
    coordonnee case_debut;
    coordonnee case_arrivee;
    int nb_cases_besoin;
    char sens_dep;
    char symbole;
}deplacement;

//Affichage
/**
 * Précondition : /
 * Postcondition : efface le contenu du terminal
 */
void afficher_vide(void){
    system("@cls||clear");
}

/**
 * Précondition : /
 * Postcondition : affiche l'écran de lancement d'une partie
 */
void afficher_ecran_lancement_partie(void){
    printf("|R|===|R|  |U|     |U|   |S|S|S|  |H|    |H|    |H|    |H|   |O||O|    |U|     |U|  |R|===|R|\n");
    printf("|R|    |R| |U|     |U| |S|        |H|    |H|    |H|    |H| |O|    |O|  |U|     |U|  |R|    |R|\n");
    printf("|R|===|R|  |U|     |U|   |S||S|   |H|====|H|    |H|====|H| |O|    |O|  |U|     |U|  |R|===|R|\n");
    printf("|R|  |R|   |U|     |U|        |S| |H|====|H|    |H|====|H| |O|    |O|  |U|     |U|  |R|  |R|\n");
    printf("|R|   |R|   |U|   |U|        |S|  |H|    |H|    |H|    |H| |O|    |O|   |U|   |U|   |R|   |R|\n");
    printf("|R|    |R|    | U |    |S|S|S|    |H|    |H|    |H|    |H|   |O||O|       | U |     |R|    |R|\n\n");
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
    printf("Pour atteindre cet objectif, vous devrez deplacer les autres vehicules\n\n");
    printf("Voici comment se deroule un tour de jeu :\n");
    printf("Tout d'abord, vous devez choisir un véhicule à deplacer\n");
    printf("Ensuite, vous devez deplacer votre vehicule verticalement ou horizontalement du nombre de case que vous souhaitez\n");
    printf("Enfin, vous constatez les consequences de votre deplacement et adaptez vos futures actions\n\n");
}

/**
 * Précondition : /
 * Postcondition : affiche des consignes de jeu
 */
void afficher_consignes(void){
    printf("Pour deplacer un vehicule vous devez d'abord entrer EN MODE DEPLACEMENT en appuyant sur la lettre d/D.\n");
    printf("Pour recommencer avec le meme plateau de zero vous devez entrez la lettre r/R.\n");
    printf("Pour gagner il faut que la voiture rouge se trouve sur la derniere case juste avant la sortie sinon la partie ne se fini pas.\n");
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
//Utilitaire
/**
 * Précondition : /
 *  Postcondition : le tampon stdin est vide
 */
void vider_tampon_stdin(void){
    int c;
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

int case_contient(plateau * parking, int line, int column, char symbole){
    int contient_vehicule = 0;

    if(parking->matrice[line][column] == symbole){
        contient_vehicule = 1;
    }

    return contient_vehicule;
}

int indice_voiture_recherche(plateau * parking, char symbole_a_rechercher){
    int est_trouve = 0;
    int indice = 0;
    while (indice < NB_TOTAL_VEHICULE && !est_trouve) {
        if(symbole_a_rechercher == parking->liste_vehicule[indice].symbole) {
            est_trouve = 1;
        }
        else{
            indice++;
        }
    }

    return indice;
}

//GAMEPLAY
/**
 * Précondition : symboles est un tableau de caractère initialisé et indice est initialisé.
 *                indice se voit affecter l'indice de la case où se situe le symbole qui a été tiré au sort
 * Postcondition : Un décalage a eu lieu dans le tableau symboles.
 *                 Le contenu de la case d'indice indice a été décalé à la dernière case contenant autre chose que null
 *                 indice est incrémenté tant qu'il faut encore effectuer le décalage
*/
void modifier_symboles(char symboles[TAILLE], int indice_symbole_tire, int * nb_symboles_restants){
    int indice = TAILLE - 1;
    while(symboles[indice] == *nb_symboles_restants - 1){
        indice--;
    }
    char temp = symboles[indice];
    symboles[indice] = symboles[indice_symbole_tire];
    symboles[indice_symbole_tire] = temp;
}

/**
 * Précondition : symboles est un tableau de caractère initialisé et nb_symboles_restants est initialisé
 * Postcondition : nb_symboles_restants est inchangé, symboles est modifié. Un décalage vers la droite est effectué
 *                 pour que les symboles restant soient toujours dans la partie de gauche de symboles
 * Résultat : retourne le symbole contenu à l'indice de nb_aleatoire dans le tableau symboles
*/
char tire_symbole_aleatoire(char symboles[TAILLE], int * nb_symboles_restants){
    int nb_aleatoire = rand() % (*nb_symboles_restants + 1);
    char symbole = symboles[nb_aleatoire];
    nb_symboles_restants--;
    modifier_symboles(symboles, nb_aleatoire, nb_symboles_restants);
    return symbole;
}

/**
 * Précondition : parking est un plateau initialisé
 * Postcondition : la matrice de parking est initialisée. Chaque case vide contient le caractère espace,
 *                 la case de la sortie contient 'S', les cases contenant le véhicule rouge contiennent 'R' et
 *                 les autres cases contenant un véhicule contiennent des symboles aléatoires (1 par véhicule)
 */
void init_matrice(plateau * parking){
    for(int indice_ligne = 0; indice_ligne < parking->nb_lignes; indice_ligne++){
        for(int indice_colonne = 0; indice_colonne < parking->nb_colonnes; indice_colonne++){
            parking->matrice[indice_ligne][indice_colonne] = ' ';
        }
    }

    for(int indice = 0; indice < NB_TOTAL_VEHICULE; indice++){
        conversion_coordonee(parking->liste_vehicule[indice].debut, &ligne, &colonne);
        parking->matrice[ligne][colonne] = parking->liste_vehicule[indice].symbole;

        conversion_coordonee(parking->liste_vehicule[indice].fin, &ligne, &colonne);
        parking->matrice[ligne][colonne] = parking->liste_vehicule[indice].symbole;
        if(parking->liste_vehicule[indice].taille == 3){
            coordonnee milieu = trouver_centre_camion(parking->liste_vehicule[indice]);
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
plateau init_parking_defaut(){
    plateau parking = {};

    parking.nb_lignes = 5;
    parking.nb_colonnes = 7;
    parking.nb_min_coups = 2;
    parking.sortie = (coordonnee) {'E', 7};

    //sens du vehic facile a definir car si c'est la meme lettre on est H = Horizontal sinon on est V = Vertical
    coordonnee debut = {'E', 2};
    coordonnee fin = {'E', 3};
    vehicule voiture_rouge = {2, debut, fin, 'R', 'H'};
    parking.liste_vehicule[0] = voiture_rouge;

    //3. camion n°1
    debut.ligne = 'B';
    debut.colonne = 1;
    //case au milieu B2
    fin.ligne = 'B';
    fin.colonne = 3;
    vehicule camion1 = {3, debut, fin, '*', 'H'};
    parking.liste_vehicule[1] = camion1;

    //4. camion n°2
    debut.ligne = 'C';
    debut.colonne = 5;
    //case au milieu D5
    fin.ligne = 'E';
    fin.colonne = 5;
    vehicule camion2 = {3, debut, fin, '+', 'V'};
    parking.liste_vehicule[2] = camion2;

    //5. voiture n°1
    debut.ligne = 'A';
    debut.colonne = 6;
    fin.ligne = 'A';
    fin.colonne = 7;
    vehicule voiture = {2, debut, fin, '?', 'H'};
    parking.liste_vehicule[3] = voiture;

    for(int indice = 1, nb_symboles_restants = TAILLE; indice < NB_TOTAL_VEHICULE; indice++, nb_symboles_restants--) {
        char symboles[TAILLE] = {'+', '-', '=', '?', '~', '$', '*', '#', '%', '{',
                                 '&', '@', '^', '>', '<', '/', '(', ')', '!', '|'};
        parking.liste_vehicule[indice].symbole = tire_symbole_aleatoire(symboles, &nb_symboles_restants);
    }
    init_matrice(&parking);

    return parking;
}

/**
 * Précondition : parking initialisé
 * Postcondition : parking inchangé
 * Résultat : retourne 1 si la sortie est atteinte. Sinon, retourne 0
 */
int a_atteint_sortie(coordonnee v_rouge, coordonnee sortie){
    int ligne_voiture_rouge = 0;
    int colonne_voiture_rouge = 0;
    conversion_coordonee(v_rouge, &ligne_voiture_rouge, &colonne_voiture_rouge);

    int ligne_sortie = 0;
    int colonne_sortie = 0;
    conversion_coordonee(sortie, &ligne_sortie, &colonne_sortie);

    int sortie_est_atteinte = 0;
    if(colonne_voiture_rouge == colonne_sortie && ligne_voiture_rouge == ligne_sortie){
        sortie_est_atteinte = 1;
    }
    return sortie_est_atteinte;
}

//Deplacement

/**
 * Précondition : /
 * Postcondition : deplacement_utilisateur contient la lettre correspondant à la ligne de la case saisie par l'utilisateur
 * Résultat : retourne la lettre saisie par l'utilisateur
 */
char ligne_utilisateur(){
    char deplacement_utilisateur_ligne;

    int is_good = 1;
    while(is_good){
        vider_tampon_stdin();
        printf("Veuillez entrer la lettre de la ligne de la case: ");
        deplacement_utilisateur_ligne = faire_choix();

        if(deplacement_utilisateur_ligne < INDICE_PREMIERE_LIGNE || deplacement_utilisateur_ligne > INDICE_DERNIERE_LIGNE){
            printf("Vous avez entré une lettre de ligne invalide. Vous devez entrer une ligne entre %c et %c\n",
                   INDICE_PREMIERE_LIGNE, INDICE_DERNIERE_LIGNE);
        } else{
            is_good--;
        }
    }

    return deplacement_utilisateur_ligne;
}

/**
 * Précondition : /
 * Postcondition : deplacement_utilisateur contient le chiffre correspondant à la colonne de la case saisie par l'utilisateur
 * Résultat : retourne le chiffre saisi par l'utilisateur
*/
int colonne_utilisateur(){
    int deplacement_utilisateur_colonne = 0;
    int is_good = 1;
    while(is_good){
        printf("Veuillez entrer le chiffre de la colonne de la case : ");
        scanf("%d", &deplacement_utilisateur_colonne);
        if(deplacement_utilisateur_colonne < INDICE_PREMIERE_COLONNE || deplacement_utilisateur_colonne > INDICE_DERNIERE_COLONNE){
            printf("Vous avez entré un numéro de colonne invalide. Vous devez entrer un numéro entre %d et %d\n",
                   INDICE_PREMIERE_COLONNE, INDICE_DERNIERE_COLONNE);
        }
        else{
            is_good--;
        }
    }

    return deplacement_utilisateur_colonne;
}

/**
 * to do
 * @return
 */
coordonnee saisie_utilisateur(){
    coordonnee cor = {0,0};
    cor.ligne = ligne_utilisateur();
    vider_tampon_stdin();
    cor.colonne = colonne_utilisateur();
    return cor;
}

/**
 * to do
 * @param parking
 * @return
 */
deplacement init_deplacement(plateau * parking){
    deplacement dep;

    int est_valide = 1;
    coordonnee vehicule_a_deplacer;
    while (est_valide){
        printf("Vous devez saisir une case contenant un vehicule, c'est à dire une case avec un symbole.\n");
        vehicule_a_deplacer = saisie_utilisateur();

        conversion_coordonee(vehicule_a_deplacer, &ligne, &colonne);

        if(case_contient(parking, ligne, colonne, ' ')){
            printf("\nAttention: vous avez entre une case qui est vide\n");
            printf("Pour déplacer un véhicule sur une case, il faut choisir un vehicule\n\n");
        } else{
            est_valide--;
        }
    }
    dep.case_debut = vehicule_a_deplacer;

    conversion_coordonee(dep.case_debut, &ligne, &colonne);
    dep.symbole = parking->matrice[ligne][colonne];

    est_valide = 1;
    coordonnee nouvel_emplacement;
    while (est_valide){
        printf("Vous devez saisir une case vide ou vous souhaitez deplacer le vehicule choisi.\n");
        nouvel_emplacement = saisie_utilisateur();

        conversion_coordonee(nouvel_emplacement, &ligne, &colonne);

        if(case_contient(parking, ligne, colonne, dep.symbole) && case_contient(parking, ligne, colonne, ' ')){
            printf("\nAttention: vous avez entre une case qui contient un vehicule different de celui que vous souhaitez deplace\n");
            printf("Pour déplacer un véhicule sur une case, il faut que cette case soit libre ou \nqu'elle contiennent le symbole du véhicule que vous souhaitez déplacer\n\n");
        } else{
            est_valide--;
        }
    }
    dep.case_arrivee = nouvel_emplacement;

    char sens_deplacement;
    if(dep.case_debut.ligne == dep.case_arrivee.ligne){
        sens_deplacement = 'H';
    } else if(dep.case_debut.colonne == dep.case_arrivee.colonne){
        sens_deplacement = 'V';
    } else{
        sens_deplacement = 'D';
    }
    dep.sens_dep = sens_deplacement;

    int indice = indice_voiture_recherche(parking, dep.symbole);
    if(parking->liste_vehicule[indice].taille == 2){
        dep.nb_cases_besoin = 1;
    } else{
        dep.nb_cases_besoin = 2;
    }
    return dep;
}

/**
 * to do
 * @param dep
 * @param parking
 * @return
 */
int deplacement_est_valide(deplacement * dep, plateau * parking){
    int est_valide = 1;
    //Premiere verif
    //sens de déplacement doit être le meme que le véhicule en question
    int indice = indice_voiture_recherche(parking, dep->symbole);
    if(dep->sens_dep != parking->liste_vehicule[indice].sens_vehicule){
        est_valide = -1;
    }
    /*deuxieme verif
    //voir si les autres cases qui sont derriere celle d'arrive sont vides ou contiene le meme symbole pour ne pas se chevaucher d'autre vehicule
    //Fonction non-integré => explication dans le rapport
    */
    return est_valide;
}

/**
 * to do
 * @param d
 * @param parking
 * @param symbole
 */
void remplir(deplacement * d, plateau * parking, char symbole){
    if(symbole == ' '){
        conversion_coordonee(d->case_debut, &ligne, &colonne);
    } else{
        conversion_coordonee(d->case_arrivee, &ligne, &colonne);
    }
    parking->matrice[ligne][colonne] = symbole;

    int i = indice_voiture_recherche(parking, d->symbole);

    if(d->sens_dep == 'H'){
        parking->matrice[ligne][colonne-1] = symbole;

        if(parking->liste_vehicule[i].taille == 3){
            parking->matrice[ligne][colonne-2] = symbole;
        }
    } else{
        parking->matrice[ligne+1][colonne] = symbole;

        if(parking->liste_vehicule[i].taille == 3){
            parking->matrice[ligne+2][colonne] = symbole;
        }
    }

}

/**
 * to do
 * @param dep
 */
void converions_depl(deplacement * dep){
    int ecart;
    if(dep->sens_dep == 'H'){
        ecart = dep->case_arrivee.colonne - dep->case_debut.colonne;
        dep->case_debut = dep->case_arrivee;
        int nouvelle_colonne = dep->case_debut.colonne + ecart;
        coordonnee tmp = {dep->case_debut.ligne, nouvelle_colonne};
        dep->case_arrivee = tmp;
    } else{
        ecart = dep->case_arrivee.ligne - dep->case_debut.ligne;
        dep->case_debut = dep->case_arrivee;
        int nouvelle_ligne = dep->case_debut.ligne + ecart;
        coordonnee tmp =  {nouvelle_ligne, dep->case_debut.colonne};
        dep->case_arrivee = tmp;
    }
}

/**
 * to do
 * @param dep
 * @param parking
 */
void effectuer_deplacement(deplacement * dep, plateau * parking){
    *dep = init_deplacement(parking);
    //printf("Deplacement au départ \n Case de début: %c %d\n Case d'arrive: %c %d \n", dep.case_debut.ligne, dep.case_debut.colonne, dep.case_arrivee.ligne, dep.case_arrivee.colonne);
    //conversion si besoin
    if(dep->sens_dep == 'H' && dep->case_debut.colonne == INDICE_PREMIERE_COLONNE){
        converions_depl(dep);
    } else if(dep->sens_dep == 'H' && dep->case_debut.colonne == INDICE_DERNIERE_COLONNE) {
        converions_depl(dep);
    } else if(dep->sens_dep == 'V' && dep->case_debut.ligne == INDICE_PREMIERE_LIGNE){
        converions_depl(dep);
    } else if(dep->sens_dep == 'V' && dep->case_debut.ligne == INDICE_DERNIERE_LIGNE) {
        converions_depl(dep);
    }
    //printf("Deplacement a la conversion \n Case de début: %c %d\n Case d'arrive: %c %d \n", dep.case_debut.ligne, dep.case_debut.colonne, dep.case_arrivee.ligne, dep.case_arrivee.colonne);
    //verification
    int tmp = deplacement_est_valide(dep, parking);
    while (tmp < 0) {
        vider_tampon_stdin();
        printf("Le deplacement ne peut s'effectuer car vous essayez de deplacer la voiture ");
        //trouver la raison et faire un message d'erreur en fonction DONC plusieurs code erreurs
        if (tmp == -1) {
            printf("dans le sens opposé du vehicule.\n");
        }
        pause();
        afficher_vide();
        afficher_matrice(parking);

        printf("Veuillez recommencez\n\n");
        *dep = init_deplacement(parking);

        if(dep->sens_dep == 'H' && dep->case_debut.colonne == INDICE_PREMIERE_COLONNE){
            converions_depl(dep);
        } else if(dep->sens_dep == 'H' && dep->case_debut.colonne == INDICE_DERNIERE_COLONNE) {
            converions_depl(dep);
        } else if(dep->sens_dep == 'V' && dep->case_debut.ligne == INDICE_PREMIERE_LIGNE){
            converions_depl(dep);
        } else if(dep->sens_dep == 'V' && dep->case_debut.ligne == INDICE_DERNIERE_LIGNE) {
            converions_depl(dep);
        }
        tmp = deplacement_est_valide(dep, parking);
    }
    //realisation du deplacement
    remplir(dep, parking, ' ');
    remplir(dep, parking, dep->symbole);

    printf("Deplacement reussi\n");
    pause();
}

/**
 * to do
 */
void deroulement_partie(void) {
    vider_tampon_stdin();
    plateau parking = init_parking_defaut();
    afficher_consignes();
    pause();

    int nbCoupJ = 0;
    deplacement dep;
    while (!a_atteint_sortie(dep.case_arrivee, parking.sortie)){
        afficher_vide();
        afficher_matrice(&parking);

        printf("Faites votre choix(d/D = deplacer un vehicule, r/R = reinitialiser le plateau) : ");
        char choix = faire_choix();

        switch (choix) {
            case 'd': case 'D': effectuer_deplacement(&dep, &parking); nbCoupJ++; break;
            case 'r': case 'R': init_matrice(&parking); nbCoupJ = 0; break;
            default: printf("Vous avez appuye sur une mauvaise touche\n"); pause(); break;
        }

        vider_tampon_stdin();
    }
    afficher_vide();
    afficher_matrice(&parking);
    printf("Bravo vous avez gagné avec %d coups. Le nombre coups minimum pour ce plateau est de %d", nbCoupJ, parking.nb_min_coups);
    pause();
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
    printf("\nMerci d'avoir joué au Rush Hour ! \n");
    return EXIT_SUCCESS;
}