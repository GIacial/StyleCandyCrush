#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"interfacev2.h"
#include "selecteurniveau.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void chargeGrille();                    //permet e charger un niveau au choix irect sur le disque
    void chargerNiveau(int numNiveau);      //permet de charger le niveau du numNiveau
    void finDePartie(bool victoire);

protected:
    void creationInterfaceSelectionNiveau();
    void creationInterfaceJeu();
    void sauvegarde()const;
    void chargement();

    //changement de fenetre

    void changementVersChoixNiveau();       //permet de retourner au choix de niveau
    void changementVersJeu();               //permet de retourner au jeu


private:
    InterfaceV2* interfaceJeu;
    SelecteurNiveau* interfaceSelectionNiveau;
    int niveauAFaire;
    bool progressionPossible;
    QString cheminSave;

    //les Actions
    QShortcut* charger;


};

#endif // MAINWINDOW_H
