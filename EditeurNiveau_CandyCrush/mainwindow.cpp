#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    interfaceJeu = new interfaceEditeurNiveauCandyCrush(this);
    this->setCentralWidget(interfaceJeu);

    //fabrication menu
        QMenu* menuOutils= new QMenu("Fichier");
        menuBar()->addMenu(menuOutils);

        config= new QMenu("Configuration");

        //les action
            QAction* nouveau= new QAction("Nouveau",this);
            menuOutils->addAction(nouveau);
            QObject::connect(nouveau,SIGNAL(triggered()),this,SLOT(creeGrille()));

            QAction* charger= new QAction("Charger",this);
            menuOutils->addAction(charger);
            QObject::connect(charger,SIGNAL(triggered()),this,SLOT(chargeGrille()));

            QAction* sauvegarder= new QAction("Sauvegarder",this);
            menuOutils->addAction(sauvegarder);
            QObject::connect(sauvegarder,SIGNAL(triggered()),this,SLOT(sauvegardeGrille()));

            QAction* objectif = new QAction("Objectif",this);
            config->addAction(objectif);
            QObject::connect(objectif,SIGNAL(triggered(bool)),this,SLOT(setObjectif()));

            QAction* limiteCoup = new QAction("Nb Coup",this);
            config->addAction(limiteCoup);
            QObject::connect(limiteCoup,SIGNAL(triggered(bool)),this,SLOT(setLimiteCoup()));

            QAction* action = new QAction("Changer action",this);
            config->addAction(action);
            QObject::connect(action,SIGNAL(triggered(bool)),this,SLOT(setAction()));

}

MainWindow::~MainWindow()
{

}

void MainWindow::creeGrille(){
    QDialog* fenetre = new QDialog(this);
    fenetre->setMinimumWidth(180);
    fenetre->setMaximumHeight(180);
    QVBoxLayout* layout = new QVBoxLayout(fenetre);
    layout->addWidget(new QLabel("Largeur",fenetre));
    QSpinBox* x = new QSpinBox(fenetre);
    layout->addWidget(x);
    x->setMinimum(3);
    x->setMaximum(12);
    layout->addWidget(new QLabel("Hauteur",fenetre));
    QSpinBox* y = new QSpinBox(fenetre);
    layout->addWidget(y);
    y->setMinimum(3);
    y->setMaximum(9);
    layout->addWidget(new QLabel("Nombre de couleur",fenetre));
    QSpinBox* nbCouleur = new QSpinBox(fenetre);
    layout->addWidget(nbCouleur);
    nbCouleur->setMinimum(3);
    nbCouleur->setMaximum(8);
    QPushButton* button = new QPushButton("Créer",fenetre);
    layout->addWidget(button);
    QObject::connect(button,SIGNAL(clicked(bool)),fenetre,SLOT(close()));

    if(fenetre->exec()){}

    interfaceJeu->creeUneGrille(x->value(),y->value(),nbCouleur->value());

    menuBar()->addMenu(config);
}

void MainWindow::chargeGrille(){
    QString chemin =QFileDialog::getOpenFileName(this, "Ouvrir un fichier", QString(), "Files (*.map)");
    if(!chemin.isEmpty()){
        interfaceJeu->chargerMap(chemin);

        menuBar()->addMenu(config);
    }
}

void MainWindow::sauvegardeGrille(){
    QString chemin =QFileDialog::getSaveFileName(this, "Enregistrer un fichier", QString(), "Files (*.map)");
    if(!chemin.isEmpty()){
        interfaceJeu->sauvegarderMap(chemin);
    }
}

void MainWindow::setLimiteCoup(){
    QDialog* fenetre = new QDialog(this);
    fenetre->setMinimumWidth(180);
    fenetre->setMaximumHeight(180);
    QVBoxLayout* layout = new QVBoxLayout(fenetre);
    layout->addWidget(new QLabel("Nb Coup",fenetre));
    QSpinBox* x = new QSpinBox(fenetre);
    layout->addWidget(x);
    x->setMinimum(1);
    x->setMaximum(500);
    QPushButton* button = new QPushButton("Appliquer",fenetre);
    layout->addWidget(button);
    QObject::connect(button,SIGNAL(clicked(bool)),fenetre,SLOT(close()));

    if(fenetre->exec()){}

    interfaceJeu->setLimiteCoup(x->value());
}

void MainWindow::setObjectif(){
    QDialog* fenetre = new QDialog(this);
    fenetre->setMinimumWidth(180);
    fenetre->setMaximumHeight(180);
    QVBoxLayout* layout = new QVBoxLayout(fenetre);
    QList<QSpinBox*> x ;
    QPalette palette;

    for(int i=0; i< interfaceJeu->getNbCouleur() ; i++){
        QLabel* text =new QLabel("Objectif",fenetre);
        palette.setColor(QPalette::WindowText,Boule::getColorPredefini(i));
        text->setPalette(palette); //on change la couleur ici
        layout->addWidget(text);
        x << new QSpinBox(fenetre);
        layout->addWidget(x[i]);
        x[i]->setMinimum(0);
        x[i]->setMaximum(100);
    }
    QPushButton* button = new QPushButton("Appliquer",fenetre);
    layout->addWidget(button);
    QObject::connect(button,SIGNAL(clicked(bool)),fenetre,SLOT(close()));

    if(fenetre->exec()){}

    for(int i=0; i< interfaceJeu->getNbCouleur() ; i++){
        interfaceJeu->setObjectif(x[i]->value(),i);
    }
}

void MainWindow::setAction(){
    QDialog* fenetre = new QDialog(this);
    fenetre->setMinimumWidth(180);
    fenetre->setMaximumHeight(180);
    QVBoxLayout* layout = new QVBoxLayout(fenetre);
    QComboBox* action = new QComboBox();
    action->addItem("BLOQUER");
    action->addItem("FROZEN");
    layout->addWidget(action);
    QPushButton* button = new QPushButton("Appliquer",fenetre);
    layout->addWidget(button);
    QObject::connect(button,SIGNAL(clicked(bool)),fenetre,SLOT(close()));

    if(fenetre->exec()){}
    ACTION a=BLOC;
    switch(action->currentIndex()){
    case 1:a=FROZ;
        break;
    default:a=BLOC;
        break;
    }
    interfaceJeu->setAction(a);
}
