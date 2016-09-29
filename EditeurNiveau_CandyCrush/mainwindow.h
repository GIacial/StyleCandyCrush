#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"interfaceediteurniveaucandycrush.h"
#include <QMessageBox>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:

    void creeGrille();
    void chargeGrille();
    void sauvegardeGrille();
    void setObjectif();
    void setLimiteCoup();
    void setAction();
private:
    interfaceEditeurNiveauCandyCrush* interfaceJeu;
    QMenu* config;
    //fonction
};

#endif // MAINWINDOW_H
