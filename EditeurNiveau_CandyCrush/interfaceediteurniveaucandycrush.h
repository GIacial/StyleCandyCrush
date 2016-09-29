#ifndef INTERFACEEDITEURNIVEAUCANDYCRUSH_H
#define INTERFACEEDITEURNIVEAUCANDYCRUSH_H

#include <QObject>
#include <QGraphicsView>
#include <QtWidgets>
#include "systemediteurniveaucandycrush.h"

class interfaceEditeurNiveauCandyCrush : public QGraphicsView
{
    Q_OBJECT
public:
    interfaceEditeurNiveauCandyCrush(QWidget* parent=0);
    void creeUneGrille(int largeur, int hauteur, int nbCouleur=4);
    void chargerMap(QString chemin);
    void sauvegarderMap(QString chemin);
    void setLimiteCoup(int limite);
    void setObjectif(int objectif,int couleur);
    int getNbCouleur()const;
    void setAction(ACTION a);


signals:
public slots:
    void afficheErreur(QString texte);
    void afficheInformation(QString texte,QString titre);

protected:
    void resizeEvent(QResizeEvent *event);//def comment reagie l'object au changement de taille

private:
    //fonction
    void creeAffichageGrille();
    systemediteurniveauCandyCrush* systemJeu;
    //variable
    QGraphicsScene* scene;
};

#endif // INTERFACEEDITEURNIVEAUCANDYCRUSH_H
