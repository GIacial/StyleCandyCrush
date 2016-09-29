#ifndef SYSTEMCANDYCRUSH_V2_H
#define SYSTEMCANDYCRUSH_V2_H

#include <QObject>
#include<QList>
#include <QTimer>
#include "caseterrain.h"
#include <QFile>
#include <QTextStream>
#include <c++/cmath>
#include "tableaudesscores.h"

class SystemCandyCrush_V2 : public QObject
{
    Q_OBJECT
public:
    //const
    static int const tempsAnimation_Classique=100;
    static int const tempsAnimation_BouleCombo=200;

    //
    explicit SystemCandyCrush_V2(QObject *parent = 0);
    ~SystemCandyCrush_V2();
    void         creeGrilleRectangulaire(int largeur, int hauteur);//permet de cree unne grille carre
    void         chargerUneGrille(QString chemin);                  //charge une grille depuis un fichier (boule random)
    CaseTerrain* getCaseJeux(int x , int y);                        //permet d'obtenir les case de la grille y=0=> 5 case du tab car on traduit le decalage du au 5 ligne cacher
    int          getLargeurGrilleJeux()const;                       //donne la largeur de la grille
    int          getHauteurGrilleJeux(int colonne=0)const;            //donne la hauteur Visible
    bool         existeGrille()const;
    int          getNbCouleur()const;
    void         demandeUpdate();                                   //envoie tout les demande d'update;
    void         changerCouleurBouleFrozen();                       //change les couleurs des boule frozen

signals:
    void OnATrouverUneErreur(QString);
    void passerUnMessage(QString texte,QString titre);
    void demandeUpdateBoule();
    void demandeUpdateCoup(int nbcoup ,int limite);
    void demandeUpdateScore(QList<int> detruit,QList<int> objectif);
    void onSignaleLaFinDePartie(bool);
    void demandeAnimationRecup(CaseTerrain*);
    void demandeAnimationEffectBouleCombo(CaseTerrain*);
    void demandeAffichageTextCombo(QString);

public slots:
    void OnSignaleUneErreur(QString texte);                 //sert a m'engueler
    void echangeBoule(CaseTerrain* CaseSelectionner);       //permet au joueur de changer 2 boule
    void deselectionAuto(CaseTerrain* CaseSelectionner);    //permet de deselectionner la boule prise
    void echangeBouleVerifCoupAutoriser();                  //verifie si le coup est autoriser apres le changement
    void remonterLesBoule();                                //sert a faire tomber les nouvelle boule
private:
    //fonction
    bool coupPossible(CaseTerrain* CaseSelectionner);                                   //regarde si il y a un coup possible avec cette case
    bool coupAutoriser(CaseTerrain *b1, CaseTerrain *b2);                               //regarde si le coup est autorise
    QList<CaseTerrain*> getCombo(CaseTerrain* CaseSelectionner);                        //renvoi toute les cases du combo
    void empecheLeJoueurDeJouer();                                                      //empeche les interaction du joueur avec les boule
    void autoriseLeJoueurAJouer();                                                      //autorise le joueur a toucher au boule
    void enleveComboDuTerrain(bool AvecTempsAnimation);                                 //enleve les combo sur le terrain
    void remonterLesBoule(bool AvecTempsAnimation);                                     //sert a faire tomber les nouvelle boule
    bool remonterBouleVersCaseSup(int x , int y, int xSup);                               //sert a remonter une boule sur la case sup
    bool ilResteDesCoup();                                                               //permet de savoir si il reste au moins un coup
    bool deplacementPosible(CaseTerrain* CaseAtester,int xCible ,int yCible);            //permet de savoir si la case a un deplacement qui cause un coup
    void melangeTerrain();                                                               //melange les boule jouable
    bool compteBouleTerrain();                                                           //indique si il y a au moins 3 boule de la meme couleurs
    bool pasDeCombosurTerrain();                                                         //permet de savoir si il y a deja une ligne de 3 ou plus sur le terrain
    void transformationBouleCombo(QList<CaseTerrain*>& combo);                            //transforme la boule initial en boule combo si c'est le cas
    void finDeTour();                                                                    //fait les verif de fin de tour
    bool effectBouleCombo(CaseTerrain* caseBouleCombo);                                  //active effect d'un boule combo renvoi t si effect combo activer
    void effectBouleCombo_Horizontal(CaseTerrain *caseBouleCombo);                       //detruit la ligne de la case selec
    void effectBouleCombo_Vertical(CaseTerrain *caseBouleCombo);                        // detruit la collonne de la case choisi
    void destructionCombo(QList<CaseTerrain*> &combo,bool avecTempsAnimation=true);                                   //permet d'activer la remonter d'un combo
    void afficherTextCombo();                                                           //permet d'afficher le text de combo


    //variable
    QList<QList<CaseTerrain*> > grilleJeux;                                                 //contient la grille
    bool grilleVide;                                                                        //si la grille est vide
    int nbCouleurDeBoule;
    CaseTerrain* caseSelec1;
    CaseTerrain* caseSelec2;
    QTimer* timer;
    TableaudesScores* score;
    int nbComboEnCour;

};

#endif // SYSTEMCANDYCRUSH_V2_H
