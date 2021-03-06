#include "tableaudesscores.h"

TableaudesScores::TableaudesScores(int nbCouleurBoule, QObject *parent) : QObject(parent){
    for(int i=0 ; i<nbCouleurBoule ; i++){
        nbBouledetruit  << 0;
        nbBouleObjectif << 0;
    }
    nbCoup=0;
    nbCoupLimite=0;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
TableaudesScores::~TableaudesScores(){

}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TableaudesScores::utiliserCoup(){
    nbCoup++;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TableaudesScores::compterBouleDetruit(QColor couleur){
    compterBouleDetruit(Boule::getColorPredefini(couleur));
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TableaudesScores::compterBouleDetruit(int numCouleur){
    if(numCouleur>=0 && numCouleur<nbBouledetruit.size()){
        nbBouledetruit[numCouleur]++;
    }
    else{
        emit onAUnProbleme("La couleur ne correspond a aucune boule");
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------SET-----------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TableaudesScores::setLimiteCoup(int limite){
    nbCoupLimite=limite;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TableaudesScores::setObjectifBoule(int objectif, QColor couleur){
    setObjectifBoule(objectif,Boule::getColorPredefini(couleur));
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TableaudesScores::setObjectifBoule(int objectif, int numCouleur){
    if(numCouleur>=0 && numCouleur<nbBouleObjectif.size()){
        nbBouleObjectif[numCouleur]=objectif;
    }
    else{
        emit onAUnProbleme("L'objectif ne correspond a aucune boule");
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------GET-----------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int TableaudesScores::getLimiteCoup()const{
    return nbCoupLimite;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int TableaudesScores::getNbCoup()const{
    return nbCoup;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int TableaudesScores::getObjectifBoule(QColor couleur) {
    return getObjectifBoule(Boule::getColorPredefini(couleur));
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int TableaudesScores::getObjectifBoule(int numCouleur) {
    int retour=0;

    if(numCouleur>=0 && numCouleur<nbBouleObjectif.size()){
        retour=nbBouleObjectif[numCouleur];
    }
    else{
        emit onAUnProbleme("La couleur ne correspond a aucune boule");
    }

    return retour;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
QList<int> TableaudesScores::getObjectifBoule() const{
    return nbBouleObjectif;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

QList<int> TableaudesScores::getNbBouleDetruit() const{
    return nbBouledetruit;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int TableaudesScores::getNbBouleDetruit(QColor couleur){
    return getNbBouleDetruit(Boule::getColorPredefini(couleur));
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int TableaudesScores::getNbBouleDetruit(int numCouleur) {
    int retour=0;

    if(numCouleur>=0 && numCouleur<nbBouledetruit.size()){
        retour=nbBouledetruit[numCouleur];
    }
    else{
        emit onAUnProbleme("La couleur ne correspond a aucune boule");
    }

    return retour;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
