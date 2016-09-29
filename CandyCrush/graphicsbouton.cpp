#include "graphicsbouton.h"

GraphicsBouton::GraphicsBouton(QGraphicsItem* parent):QGraphicsWidget(parent)
{
    setAcceptHoverEvents(true);//permet d'activer la detection du passage de la souris sur item
    setCacheMode(DeviceCoordinateCache);//def la resolution de l'item
    taille=QRectF(0,0,50,50);
    color=QColor(255,0,0);
}


QRectF GraphicsBouton::boundingRect() const{
       return taille;
}

QPainterPath GraphicsBouton::shape() const{
        QPainterPath path ;
        path.addEllipse(boundingRect());//defini la zone cliquable
        return path;
}

void GraphicsBouton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *){
    bool down = option->state & QStyle::State_Sunken;
            QRectF r = boundingRect();
            QLinearGradient grad(r.topLeft(), r.bottomRight());
            grad.setColorAt(down ? 0 : 1, color);
            grad.setColorAt(down ? 1 : 0, option->state & QStyle::State_MouseOver ? Qt::white : color.lighter());
            painter->setPen(color.darker());
            painter->setBrush(grad);
            painter->drawRoundedRect(r,10,10);

            //texte
            painter->setPen(Qt::black);
            painter->drawText(r,texteBouton,QTextOption(Qt::AlignCenter));

}


QColor GraphicsBouton::getColor()const{
    return color;
}

void GraphicsBouton::setColor(QColor couleur){
    color=couleur;
    update();
}

void GraphicsBouton::mousePressEvent(QGraphicsSceneMouseEvent *event){
    emit clicked();
    emit clicked(this);
    QGraphicsWidget::mousePressEvent(event);
}

void GraphicsBouton::setText(QString texte){
    texteBouton=texte;
    update();
}

void GraphicsBouton::setTaille(QRectF nouvelleTaille){
    taille=nouvelleTaille;
    update();
}
