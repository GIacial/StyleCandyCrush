#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    interfaceJeu = NULL;
    interfaceSelectionNiveau=NULL;
    niveauAFaire=1;
    progressionPossible=false;
    cheminSave="save.sav";

    //debut des fonction d'initialisation
    chargement();
    creationInterfaceSelectionNiveau();
    this->setCentralWidget(interfaceSelectionNiveau);

    //les action


        charger= new QShortcut(QKeySequence(Qt::Key_L ,Qt::Key_M),this);
        QObject::connect(charger,SIGNAL(activated()),this,SLOT(chargeGrille()));
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

MainWindow::~MainWindow()
{
    if(interfaceJeu!=NULL){
        delete interfaceJeu;
    }

    if(interfaceSelectionNiveau!=NULL){
        delete interfaceSelectionNiveau;
    }

    delete charger;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void MainWindow::chargeGrille(){
    QString chemin =QFileDialog::getOpenFileName(this, "Ouvrir un fichier", QString(), "Files (*.map)");
    if(!chemin.isEmpty()){
        changementVersJeu();
        interfaceJeu->chargerMap(chemin);
    }

    progressionPossible=false;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainWindow::chargerNiveau(int numNiveau){
    QString chemin="MapCandyCrush/lv"+QString::number(numNiveau)+".map";
    progressionPossible=numNiveau==niveauAFaire;


    changementVersJeu();
    interfaceJeu->chargerMap(chemin);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainWindow::creationInterfaceSelectionNiveau(){
    if(interfaceSelectionNiveau==NULL){

        interfaceSelectionNiveau= new SelecteurNiveau(niveauAFaire,this);
        QObject::connect(interfaceSelectionNiveau,SIGNAL(chargerNiveau(int)),this,SLOT(chargerNiveau(int)));
    }
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainWindow::creationInterfaceJeu(){
    if(interfaceJeu==NULL){
        interfaceJeu= new InterfaceV2(this);
        QObject::connect(interfaceJeu,SIGNAL(onAFini(bool)),this,SLOT(finDePartie(bool)));
    }
}



//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainWindow::changementVersJeu(){
    if(interfaceJeu==NULL){
        creationInterfaceJeu();
        setCentralWidget(interfaceJeu);
        interfaceSelectionNiveau=NULL;
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainWindow::changementVersChoixNiveau(){
    if(interfaceSelectionNiveau==NULL){
        creationInterfaceSelectionNiveau();
        setCentralWidget(interfaceSelectionNiveau);
        interfaceJeu=NULL;
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainWindow::finDePartie(bool victoire){
    if(progressionPossible && victoire){
        niveauAFaire++;
        sauvegarde();
    }
    changementVersChoixNiveau();
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainWindow::sauvegarde() const{
    QFile file(cheminSave);//lien vers le fichier

        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){//vérifie si le fichier s'ouvre

          QMessageBox::information(this->centralWidget(),"Impossible de sauvegarder","erreur");//informe si ne s'ouvre pas

        }
        else{

        QTextStream save(&file);//lie un text au fichier
        save << niveauAFaire<< "\n";//ecrit dans le fichier

        file.close();//ferme le fichier
        }
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainWindow::chargement(){
    QFile file(cheminSave);//lien vers le fichier

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){//vérifie si le fichier s'ouvre

          niveauAFaire=1;   //debut du jeu

        }
        else{

        QTextStream save(&file);//lie un text au fichier
        niveauAFaire= save.readLine().toInt();

        file.close();//ferme le fichier
        }
}


