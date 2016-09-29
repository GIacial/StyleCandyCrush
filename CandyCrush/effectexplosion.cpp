#include "effectexplosion.h"

EffectExplosion::EffectExplosion(QRectF depart, QRectF fin, int tempsAnimation, QGraphicsWidget *parent) : QGraphicsWidget(parent){
    animation= new QPropertyAnimation(this,"geometry",this);
    animation->setDuration(tempsAnimation);
    animation->setStartValue(depart);
    animation->setEndValue(fin);
    QObject::connect(animation,SIGNAL(finished()),this,SLOT(deleteLater()));
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EffectExplosion::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *){
    QRectF r=boundingRect();
    QLinearGradient grad;
    if(estHorizontal()){
        grad= QLinearGradient(r.topLeft(),r.bottomLeft());
    }
    else{
        grad= QLinearGradient(r.topLeft(),r.topRight());
    }
    for(double i=0.0; i<=0.9 ; i+=0.2){
        grad.setColorAt(i,QColor(255,255,255,255));
        grad.setColorAt(i+0.1,QColor(255,255,255,0));
    }

    grad.setColorAt(1,QColor(255,255,255,255));
    painter->setBrush(grad);
    painter->setPen(Qt::NoPen);

    painter->drawRect(r);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool EffectExplosion::estHorizontal()const{
    return directionEffect==DIRECTION_HORIZONTAL;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool EffectExplosion::estVertical()const{
   return directionEffect==DIRECTION_VERTICAL;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EffectExplosion::lancerAnimation(){
    animation->start();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EffectExplosion::setDirection(Direction d){
    directionEffect=d;
}
