#ifndef SYSTEMEDITEURNIVEAUCANDYCRUSH_H
#define SYSTEMEDITEURNIVEAUCANDYCRUSH_H

#include <QObject>
#include<QList>
#include "caseterrain.h"
#include <QFile>
#include <QTextStream>
#include <c++/cmath>
#include "tableaudesscores.h"
enum ACTION{BLOC,FROZ};
class systemediteurniveauCandyCrush : public QObject
{
    Q_OBJECT
public:
    explicit systemediteurniveauCandyCrush(QObject *parent = 0);
    void         creeGrilleRectangulaire(int largeur, int hauteur);//permet de cree unne grille carre
    void         chargerUneGrille(QString chemin);                  //charge une grille depuis un fichier (boule random)
    CaseTerrain* getCaseJeux(int x , int y);                        //permet d'obtenir les case de la grille y=0=> 5 case du tab car on traduit le decalage du au 5 ligne cacher
    int          getLargeurGrilleJeux()const;                       //donne la largeur de la grille
    int          getHauteurGrilleJeux(int colonne)const;            //donne la hauteur Visible
    void         sauvegarderUneGrille(QString chemin);
    bool         grilleExiste()const;
    void setNbCouleur(int nbCouleur);
    void setObjectif(int objectif , int couleur);
    void setLimiteCoup(int limite);
    int getNbCouleur()const;
    void setAction(ACTION act);

    signals:
        void OnATrouverUneErreur(QString);
        void passerUnMessage(QString texte,QString titre);



    public slots:
        void OnSignaleUneErreur(QString texte);                 //sert a m'engueler
        void bloquerLaCase(CaseTerrain* caseS);                     //sert a bloquer la case

    private:
        //fonction

        //variable
        QList<QList<CaseTerrain*> > grilleJeux;
        bool grilleVide;
        int nbCouleurDeBoule;
        TableaudesScores* score;
        ACTION action;
};

#endif // SYSTEMEDITEURNIVEAUCANDYCRUSH_H
