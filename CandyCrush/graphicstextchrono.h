#ifndef GRAPHICSTEXTCHRONO_H
#define GRAPHICSTEXTCHRONO_H

#include <QObject>
#include <QGraphicsTextItem>
#include <QTimer>

class GraphicsTextChrono : public QGraphicsTextItem
{
    Q_OBJECT
public:
    explicit GraphicsTextChrono(QString texte,int temps,QGraphicsItem *parent = 0);

signals:

public slots:
    void cacher();  //cahce le texte
    void showChrono(QString texte);                                                      //affiche le texte et le cahce apres un certain temps

private:
    QTimer* timer;
    int tempsAffichage;
};

#endif // GRAPHICSTEXTCHRONO_H
