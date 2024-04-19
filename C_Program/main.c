#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>

#define TAILLE 20
#define NB_TOTAL_VEHICULE 4

typedef struct {
    unsigned char ligne;
    unsigned int colonne;
}coordonee;

typedef struct {
    short taille;
    coordonee debut;
    coordonee fin;
    char symbole;
}vehicule;

typedef struct{
    unsigned int nbLignes;
    unsigned int nbColonnes;
    unsigned int nbMinCoups;
    coordonee sortie;
    char matrice[5][7];
    vehicule ** listeVehicule;
}plateau;

typedef struct {
    coordonee caseArrivee;
    char symbole;
}deplacement;

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

void init_matrice(plateau parking){
    for(int indiceLigne = 0; indiceLigne < parking.nbLignes; indiceLigne++){
        for(int indiceColonne = 0; indiceColonne < parking.nbColonnes; indiceColonne++){
            parking.matrice[indiceLigne][indiceColonne] = (char) {0};
        }
    }
}

void fixer_matrice(plateau parking){
    //1. on initialise le plateau
    parking.nbLignes = 5;
    parking.nbColonnes = 7;
    parking.nbMinCoups = 2;
    parking.sortie = (coordonee) {'E', 7};
    //2. on initalise la matrice qui contient les voitures donc les voitures avec
    //2.1 la voiture rouge est initilise
    coordonee debut = {'E', 2};
    coordonee fin = {'E', 3};
    //compter le nombre de case entre le debut et la fin pour avoir la taille
    vehicule voitureRouge = {2, debut, fin, 'R'};
    //symbole trouver grace au systeme de lois
    init_matrice(parking);
    //2.1.1 ajout de la voiture rouge dans la matrice
    parking.listeVehicule[0] = &voitureRouge;
    //2.2 boucle qui initilise les autres voitures puis les ajoute dans la matrice
    debut.ligne = 'B';
    debut.colonne = 1;
    fin.ligne = 'B';
    fin.colonne = 3;
    vehicule camion1 = {3, debut, fin, ' '};
    parking.listeVehicule[1] = &camion1;

    debut.ligne = 'C';
    debut.colonne = 5;
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

    for(int indice = 1, nb_symboles_restants = TAILLE; indice < NB_TOTAL_VEHICULE; indice++, nb_symboles_restants--){
        char symboles[TAILLE] = {'+', '-', '=', '?', '~', '$', '*', '#', '%','{',
                              '&', '@', '^', '>', '<', '/', '(', ')', '[', ']'};
        parking.listeVehicule[indice]->symbole = tire_symbole_aleatoire(symboles, nb_symboles_restants);
    }
}

//savoir faire avec des fichiers binaires pour l'exam de C
int main(void){
    srand(time(NULL));
    plateau parking;
    fixer_matrice(parking);
    return EXIT_SUCCESS;
}
