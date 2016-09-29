#ifndef SELECTEURNIVEAU_H
#define SELECTEURNIVEAU_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "graphicsbouton.h"

class SelecteurNiveau : public QGraphicsView
{
    Q_OBJECT
public:
    explicit SelecteurNiveau(int niveauAFaire,QWidget *parent = 0);
    ~SelecteurNiveau();

signals:
    void chargerNiveau(int i);

public slots:
    void selectionNiveau(QGraphicsWidget* bouton);

protected:
    void resizeEvent(QResizeEvent *event);
    int  dernierNiveau(int niveauAFaire);
    int  ligneNiveau(int i);

private:
    QGraphicsScene* scene;
    QList<GraphicsBouton*> listeNiveaux;
    int levelMax;
};

#endif // SELECTEURNIVEAU_H
