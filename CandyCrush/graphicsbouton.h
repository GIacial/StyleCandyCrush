#ifndef GRAPHICSBOUTON_H
#define GRAPHICSBOUTON_H

#include <QGraphicsWidget>
#include <QObject>
#include <QColor>
#include <QPixmap>
#include <QStyleOptionGraphicsItem>
#include <QPainterPath>
#include <QPainter>

class GraphicsBouton : public QGraphicsWidget
{
    Q_OBJECT
public:
    GraphicsBouton(QGraphicsItem *parent = 0);
    QRectF boundingRect() const;//sert a determiner la taille de l'object
    QPainterPath shape() const;//determine la zone cliquable
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *);//style graphics de item
    QColor getColor()const;//renvoi la couleur de la boule
    void setColor(QColor couleur);//permet de mettre une couleur a la boule
    void setText(QString texte);
    void setTaille(QRectF nouvelleTaille);



signals:
    void clicked();
    void clicked(QGraphicsWidget*);
public slots:


protected:
    QColor color;//couleurs de la boule
    QRectF taille;
    QString texteBouton;
    void mousePressEvent(QGraphicsSceneMouseEvent* event);

private:

};

#endif // GRAPHICSBOUTON_H
