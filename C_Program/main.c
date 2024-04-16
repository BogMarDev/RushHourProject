#include <stdlib.h>
#include <stdio.h>

typedef struct {
    unsigned char ligne;
    unsigned int colonne;
}coordonee;

typedef struct{
    unsigned int nbLignes;
    unsigned int nbColonnes;
    unsigned int nbMinCoups;
    coordonee sortie;
}plateau;

typedef struct {
    short taille;
    coordonee debut;
    coordonee fin;
    char symbole;
}vehicule;

typedef struct {
    coordonee caseArrivee;
    char symbole;
}deplacement;


//savoir faire avec des fichiers binaires pour l'exam de C
int main(void){
    char symbols[] = {'-', '=', '+', '~', '$', '_', '*', '#', '%', '&', '@', '^', '>', '<', '/', '(', ')', '[', ']'};
    //Trouver avec le fichier
    int LINES = 6;
    int COLUMNS = 7;
    int nbMin = 8;
    //trouver avec le fichier
    coordonee sortie = {'C', 7};
    plateau parking = {LINES,COLUMNS,nbMin, sortie};
    //trouver avec le fichier
    coordonee d1 = {'C', 2};
    coordonee f1 = {'C', 3};
    //compter le nombre de case entre le debut et la fin pour avoir la taille
    vehicule voitureRouge = {2, d1, f1, 'R'};
    //symbole trouver grace au systeme de lois
    //pour les voitures on fait une fonction qui les initialise grace aux fichiers
    return EXIT_SUCCESS;
}
