#ifndef CASETERRAIN_H
#define CASETERRAIN_H

#include <QObject>
#include <QGraphicsWidget>
#include "boule.h"
#include <QPainter>



class CaseTerrain : public QGraphicsWidget
{
    Q_OBJECT
private://type
    enum EtatCase {BLOQUER,LIBRE,OCCUPER,AREMONTER,FROZEN};  //etat de la case
    enum Visibiliter{CACHER,VISIBLE};

public:
    static int const numDernierLigne=-2;
    //constructeur
    CaseTerrain( int x,int y,QGraphicsItem *parent = 0);
    ~CaseTerrain();

    //mutation
    void     setCaseBloquer();                          //permet de bloquer la case
    Boule*    getBouleContenu();                        //renvoi la boule
    void     setBouleDansCase(Boule *bouleAMettre);     //permet de mettre une boule dans la case
    Boule*   enleverBouleDansCase();                    //enleve la boule de la case
    void     echangeBoule(CaseTerrain* autreCase);      //echange les boule de 2 case
    QRectF boundingRect()const;                         //renvoi le rectangle qui def la case
    void   randomBouleColor(int nbCouleur);             //randomise la couleur de la boule
    int getCooX()const;
    int getCooY()const;
    void deselection();                                 //permet de deselectionner la boule
    void setCacher();                                   //rend la case invisible et injouable mais les boule glisse
    void setARemonter();                                //permet de dire que la boule doit remonter
    void recupBouleCaseAuDessus(CaseTerrain* autreCase);//permet de prendre la boule de la case au desus quand on est en etat AREMONTER
    void transformerBouleEnBouleCombo(Boule::TypeCombo type);  //permet de transformer la boule en boule combo et bloquer la remonter de la case une fois
    void enleverResistanceRemonter();                   //enleve la resistance a la remonter
    void setFrozen();                                   //gele la case
    void unfrozen();                                    //degele la case


    //design
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *);//style graphics de item

//verification
    bool  estBloquer()const;
    bool  estLibre()const;
    bool  estOccuper()const;
    bool  estCacher()const;
    bool  contientMemeCouleurBoule(CaseTerrain* autreCase)const;
    bool  estJouable()const;                                        //permet de savoir si la case peux faire des combo
    bool  estARemonter()const;
    bool  estEchangeable()const;
    bool  estUneCaseDuJeu()const;
    bool  estResistantRemonter()const;
    bool  estExplosif()const;
    bool  estFrozen()const;
    bool  estCapableDetreBougerParJoueur()const;                    //permet de savoir si le joueur peut bouger cette boule

public slots:
    void OnMaCliquer();
signals:
   void OnATrouverUnProbleme (QString);
   void cliked(CaseTerrain*);
private:
    //fonction  
   void     creeBouleDansCase();                       //cree une boule dans la case
    //variable
    EtatCase etatDeLaCase;
    Visibiliter etatDeVisibiliter;
    Boule*   ContenuBoule;
    int CooX;
    int CooY;
};

#endif // CASETERRAIN_H
