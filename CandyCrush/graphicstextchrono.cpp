#include "graphicstextchrono.h"

GraphicsTextChrono::GraphicsTextChrono(QString texte, int temps, QGraphicsItem *parent) : QGraphicsTextItem(parent){
    timer = new QTimer(this);
    timer->setSingleShot(true);
    tempsAffichage=temps;
    this->setPlainText(texte);
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(cacher()));
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void GraphicsTextChrono::showChrono(QString texte){
    this->setPlainText(texte);
    this->show();
    timer->start(tempsAffichage);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void GraphicsTextChrono::cacher(){
    this->hide();
}
