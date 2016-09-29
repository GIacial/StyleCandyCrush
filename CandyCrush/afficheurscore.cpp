#include "afficheurscore.h"

AfficheurScore::AfficheurScore(QColor couleur, QGraphicsItem *parent):QGraphicsWidget(parent)
{
    boule = new Boule(this);
    boule->setColor(couleur);
    text = new QGraphicsTextItem(this);
    text->setPlainText("");
    positionnerTexte();
    text->setZValue(2);

    QObject::connect(boule,SIGNAL(pressed()),boule,SLOT(bouleDeselectionner()));
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

AfficheurScore::~AfficheurScore(){
    delete boule;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void AfficheurScore::setText(QString newText){
    text->setPlainText(newText);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int AfficheurScore::getHauteur()const{
    return boundingRect().height();
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int AfficheurScore::getLargeur()const{
    return boundingRect().width();
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

QRectF AfficheurScore::boundingRect() const{
       return boule->boundingRect();
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AfficheurScore::positionnerTexte(){
    switch (Boule::getColorPredefini(boule->getColor())) {
   // case 0:  //rouge
   //         break;
   // case 1: //vert
   //     break;
    case 2: //bleu
        text->setPos(getLargeur()/4,getHauteur()/2);
        break;
    case 4: //jaune
        text->setPos(getLargeur()/4,getHauteur()/4);
        break;
   // case 5://rose coeur
   //     break;
    case 6://cyan
        //goutte
        text->setPos(getLargeur()/5,getHauteur()/3);
        break;
    case 7://orange
        //carotte
        text->setPos(getLargeur()/5,getHauteur()/4);
        break;
    default:
        text->setPos(getLargeur()/4,getHauteur()/3);
        break;
    }
}

