#ifndef AFFICHEURSCORE_H
#define AFFICHEURSCORE_H

#include <QObject>
#include <QGraphicsWidget>
#include "boule.h"

class AfficheurScore : public QGraphicsWidget
{
    Q_OBJECT
public:
    AfficheurScore(QColor couleur, QGraphicsItem *parent=0);
    ~AfficheurScore();
    void setText(QString newText);
    int getHauteur()const;
    int getLargeur()const;
    QRectF boundingRect() const;

signals:
public slots:

protected:
    void positionnerTexte();
private:
    QGraphicsTextItem* text;
    Boule* boule;
};

#endif // AFFICHEURSCORE_H
