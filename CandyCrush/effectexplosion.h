#ifndef EFFECTEXPLOSION_H
#define EFFECTEXPLOSION_H

#include <QObject>
#include <QGraphicsWidget>
#include <QPropertyAnimation>
#include <QPainter>

class EffectExplosion : public QGraphicsWidget
{
    Q_OBJECT


public:
    enum Direction{DIRECTION_HORIZONTAL,DIRECTION_VERTICAL};
    explicit EffectExplosion(QRectF depart, QRectF fin,int tempsAnimation, QGraphicsWidget *parent = 0);

     void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *);//style graphics de item
     void setDirection(Direction d);
     void lancerAnimation();

     bool estHorizontal()const;
     bool estVertical()const;

signals:

public slots:

private:
     QPropertyAnimation* animation;
     Direction directionEffect;

};

#endif // EFFECTEXPLOSION_H
