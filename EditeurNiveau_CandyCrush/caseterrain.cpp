#include "caseterrain.h"

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CaseTerrain::CaseTerrain(int x, int y, QGraphicsItem *parent):QGraphicsWidget(parent)
{
    etatDeLaCase=LIBRE;
    etatDeVisibiliter=VISIBLE;
    ContenuBoule=NULL;
    CooX=x;
    CooY=y;
    creeBouleDansCase();
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CaseTerrain::~CaseTerrain(){
    if(!estLibre()){
        delete ContenuBoule;    //on supprime la boule
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
EtatCase CaseTerrain::getEtatCase()const{
    return etatDeLaCase;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CaseTerrain::setCaseBloquer(){
    if(!estLibre()){
        //ContenuBoule->hide();
        if(estBloquer()){
            etatDeLaCase=OCCUPER;
        }
        else{
            etatDeLaCase=BLOQUER;
        }
        update();
    }

}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Boule* CaseTerrain::getBouleContenu(){
    return ContenuBoule;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CaseTerrain::setBouleDansCase(Boule* bouleAMettre){
    if(this->estLibre()){
        ContenuBoule=bouleAMettre;
        QObject::connect(ContenuBoule,SIGNAL(pressed()),this,SLOT(OnMaCliquer()));
        etatDeLaCase=OCCUPER;

        if(estCacher()){
            ContenuBoule->hide();
        }
        else{
            ContenuBoule->show();
        }

    }
    else{
            emit OnATrouverUnProbleme("La case avait déjà une boule");

    }
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Boule* CaseTerrain::enleverBouleDansCase(){
    Boule* retour = ContenuBoule;
    if(!estLibre()){
        QObject::disconnect(ContenuBoule,SIGNAL(pressed()),this,SLOT(OnMaCliquer()));
        ContenuBoule=NULL;
        etatDeLaCase=LIBRE;
    }
    else{
        emit OnATrouverUnProbleme("Tu enleve une boule inexistante");
    }
    return retour;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CaseTerrain::echangeBoule(CaseTerrain *autreCase){
    if(this->estEchangeable() && autreCase->estEchangeable()){
        //on echange
        Boule* temp = this->enleverBouleDansCase();
        this->setBouleDansCase(autreCase->enleverBouleDansCase());
        autreCase->setBouleDansCase(temp);
        temp=NULL;
    }
    else{
        emit OnATrouverUnProbleme("Au moins une des cases n'a pas de boule");
    }
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CaseTerrain::creeBouleDansCase(){
    if(this->estLibre()){
        ContenuBoule= new Boule();
        etatDeLaCase=OCCUPER;
        QObject::connect(ContenuBoule,SIGNAL(pressed()),this,SLOT(OnMaCliquer()));
    }
    else{
        emit OnATrouverUnProbleme("Impossible d'y mettre une boule");
    }
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

QRectF CaseTerrain::boundingRect()const{
    QRectF r = ContenuBoule->boundingRect();
    r.setWidth(r.width()+10);
    r.setHeight(r.height()+10);
    return r;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CaseTerrain::randomBouleColor(int nbCouleur){
    if(!estLibre()){
        ContenuBoule->changeColor(nbCouleur);
    }
    else{
        emit OnATrouverUnProbleme("La case n'a pas de boule");
    }
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int CaseTerrain::getCooX()const{
    return CooX;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int CaseTerrain::getCooY()const{
    return CooY;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CaseTerrain::OnMaCliquer(){
    emit cliked(this);
    ContenuBoule->bouleDeselectionner();
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CaseTerrain::deselection(){
    if(!estLibre()){
        ContenuBoule->bouleDeselectionner();
    }
    else{
        emit OnATrouverUnProbleme("la case n'est pas occupée");
    }
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void CaseTerrain::setCacher(){
    if(!estLibre()){
        ContenuBoule->hide();
        etatDeVisibiliter=CACHER;
    }
    else{
        emit OnATrouverUnProbleme("La case n'est pas en occupé donc impossible de la cacher");
    }
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CaseTerrain::setARemonter(){
    if(estOccuper()){
        etatDeLaCase=AREMONTER;
        ContenuBoule->hide();
        update();
    }
    else{
        emit OnATrouverUnProbleme("Aucune boule a remonter");
    }
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CaseTerrain::recupBouleCaseAuDessus(CaseTerrain *autreCase){
    if(estARemonter()){
       if(autreCase->estEchangeable()){
          echangeBoule(autreCase);

          if(autreCase->getCooY()!=-5){
            autreCase->setARemonter();      //si on est pas a la fin on continue a remonter
          }
       }
       else{
           emit OnATrouverUnProbleme("L'autre case n'a pas de boule deplasable");
       }

    }
    else{
        emit OnATrouverUnProbleme("La case ne veut pas etre remonter");
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CaseTerrain::setFrozen(){
    if(estOccuper()){
        etatDeLaCase=FROZEN;
        ContenuBoule->setFrozen(true);
    }
}

void CaseTerrain::unfrozen(){
    if(estFrozen()){
        etatDeLaCase=OCCUPER;
        ContenuBoule->setFrozen(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------Design----------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void CaseTerrain::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *){
    QRectF r = this->boundingRect();

    if(estUneCaseDuJeu()){
        painter->setPen(Qt::white);
            if(estFrozen()){

                painter->setBrush(QBrush(QColor(150,150,250,100)));
            }
            else{

                painter->setBrush(QBrush(QColor(150,150,150,100)));
            }

        painter->drawRect(r);
    }


}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------VERIFICATION-------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

bool CaseTerrain::estBloquer()const{
    return etatDeLaCase==BLOQUER;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CaseTerrain::estLibre()const{
    return etatDeLaCase==LIBRE;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CaseTerrain::estOccuper()const{
    return etatDeLaCase==OCCUPER;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

bool CaseTerrain::estCacher()const{
    return etatDeVisibiliter==CACHER;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CaseTerrain::contientMemeCouleurBoule(CaseTerrain *autreCase)const{
    return ContenuBoule->estMemeCouleur(autreCase->getBouleContenu());
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CaseTerrain::estJouable()const{
    return etatDeLaCase==OCCUPER && etatDeVisibiliter==VISIBLE;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CaseTerrain::estARemonter()const{
    return etatDeLaCase==AREMONTER;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CaseTerrain::estEchangeable()const{
    return etatDeLaCase==AREMONTER || etatDeLaCase==OCCUPER;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CaseTerrain::estUneCaseDuJeu()const{
    return (etatDeLaCase==AREMONTER || etatDeLaCase==OCCUPER || etatDeLaCase==FROZEN) && etatDeVisibiliter==VISIBLE;
}

bool CaseTerrain::estFrozen()const{
    return etatDeLaCase==FROZEN;
}
