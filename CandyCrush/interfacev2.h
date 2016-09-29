#ifndef INTERFACEV2_H
#define INTERFACEV2_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "systemcandycrush_v2.h"
#include <QMessageBox>
#include <QState>
#include <QStateMachine>
#include <QParallelAnimationGroup>
#include <QAbstractTransition>
#include <QPropertyAnimation>
#include <QtWidgets>                //long a la compil
#include "afficheurscore.h"
#include "effectexplosion.h"
#include "graphicstextchrono.h"

class InterfaceV2 : public QGraphicsView
{
    Q_OBJECT
public:
    InterfaceV2(QWidget* parent);
    ~InterfaceV2();
    void creeUneGrille(int largeur , int hauteur);
    void chargerMap(QString chemin);

signals:
    void passeEtatDeFin();
    void onAFini(bool);
public slots:
    void afficheErreur(QString texte);
    void afficheInformation(QString texte,QString titre);
    void updateAnimationDeplacement();
    void updateTextNbCoup(int nbCoup,int limite);
    void updateScore(QList<int> detruit , QList<int> limite);
    void onSignaleLaFin(bool victoire);
    void animationRecupBoule(CaseTerrain* caseS);
    void animationEffectBouleCombo(CaseTerrain* caseS);

protected:
    void resizeEvent(QResizeEvent *event);//def comment reagie l'object au changement de taille
    void creationConnectionSystemInterface();

private:
    //fonction
    void creeAffichageGrille();
    void creeAffichageScore();
    void creeAnimationDeplacementBoule();
    void recreerLeJeu();
    void creationInterfaceMin();
    //variable
    SystemCandyCrush_V2* systemJeu;
    QGraphicsScene* scene;

        //interface
    QGraphicsTextItem* textNbCoup;
    QList<AfficheurScore*> scoreBouleDetruit;
    GraphicsTextChrono* textCombo;


        //system animation
    QState* conteneurEtat;
    QState* etatSuivant;
    QStateMachine* machineEtat;
    QParallelAnimationGroup* groupAnimation;
    QAbstractTransition *trans;

    //const
    static int const tempsAnimationRecup=500;
};

#endif // INTERFACEV2_H
