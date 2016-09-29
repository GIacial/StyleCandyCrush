#ifndef TABLEAUDESSCORES_H
#define TABLEAUDESSCORES_H

#include <QObject>
#include <QList>
#include "boule.h"

class TableaudesScores : public QObject
{
    Q_OBJECT
public:
    explicit TableaudesScores(int nbCouleurBoule, QObject *parent = 0);
    ~TableaudesScores();
    void utiliserCoup();
    void compterBouleDetruit(QColor couleur);
    void compterBouleDetruit(int numCouleur);

    //les set
    void setLimiteCoup(int limite);
    void setObjectifBoule(int objectif , QColor couleur);
    void setObjectifBoule(int objectif , int  numCouleur);

    //les get
    int getLimiteCoup()const;
    int getNbCoup()const;
    int getObjectifBoule( QColor couleur);
    int getObjectifBoule(int  numCouleur);
    QList<int> getObjectifBoule()const;
    QList<int> getNbBouleDetruit()const;
    int getNbBouleDetruit( QColor couleur);
    int getNbBouleDetruit(int  numCouleur);

    //comparaison
    bool estVictoire()const;
    bool estDefaite()const;

signals:
    void onAUnProbleme(QString);
public slots:

private:
    int nbCoup;
    int nbCoupLimite;
    QList<int> nbBouledetruit;
    QList<int> nbBouleObjectif;

};

#endif // TABLEAUDESSCORES_H
