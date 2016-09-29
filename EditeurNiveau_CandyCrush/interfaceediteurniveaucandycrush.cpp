#include "interfaceediteurniveaucandycrush.h"

interfaceEditeurNiveauCandyCrush::interfaceEditeurNiveauCandyCrush(QWidget *parent) : QGraphicsView(parent)
{
    scene = new QGraphicsScene(0,0,1000,600,this);
    scene->setBackgroundBrush(QBrush(QColor(50,50,50)));
    this->setScene(scene);

    systemJeu = new systemediteurniveauCandyCrush(this);
    QObject::connect(systemJeu,SIGNAL(OnATrouverUneErreur(QString)),this,SLOT(afficheErreur(QString)));
    QObject::connect(systemJeu,SIGNAL(passerUnMessage(QString,QString)),this,SLOT(afficheInformation(QString,QString)));
}

void interfaceEditeurNiveauCandyCrush::resizeEvent(QResizeEvent *event){
        QGraphicsView::resizeEvent(event);
        QGraphicsView::fitInView(QGraphicsView::sceneRect(), Qt::KeepAspectRatio);//permet de garder le ratio de taille a la remodif de la taille
}

void interfaceEditeurNiveauCandyCrush::creeUneGrille(int largeur, int hauteur, int nbCouleur){
    if(systemJeu->grilleExiste()){
        delete systemJeu;
        delete scene;

        scene = new QGraphicsScene(0,0,1000,600,this);
        scene->setBackgroundBrush(QBrush(QColor(50,50,50)));
        this->setScene(scene);

        systemJeu = new systemediteurniveauCandyCrush(this);
        QObject::connect(systemJeu,SIGNAL(OnATrouverUneErreur(QString)),this,SLOT(afficheErreur(QString)));
        QObject::connect(systemJeu,SIGNAL(passerUnMessage(QString,QString)),this,SLOT(afficheInformation(QString,QString)));
    }
        systemJeu->setNbCouleur(nbCouleur);
        systemJeu->creeGrilleRectangulaire(largeur,hauteur);
        creeAffichageGrille();

}

void interfaceEditeurNiveauCandyCrush::chargerMap(QString chemin){
    if(systemJeu->grilleExiste()){
        delete systemJeu;
        delete scene;

        scene = new QGraphicsScene(0,0,1000,600,this);
        scene->setBackgroundBrush(QBrush(QColor(50,50,50)));
        this->setScene(scene);

        systemJeu = new systemediteurniveauCandyCrush(this);
        QObject::connect(systemJeu,SIGNAL(OnATrouverUneErreur(QString)),this,SLOT(afficheErreur(QString)));
        QObject::connect(systemJeu,SIGNAL(passerUnMessage(QString,QString)),this,SLOT(afficheInformation(QString,QString)));
    }
    systemJeu->chargerUneGrille(chemin);
    creeAffichageGrille();
}

void interfaceEditeurNiveauCandyCrush::sauvegarderMap(QString chemin){
    systemJeu->sauvegarderUneGrille(chemin);
}

void interfaceEditeurNiveauCandyCrush::creeAffichageGrille(){
    for(int x=0 ; x<systemJeu->getLargeurGrilleJeux() ;x++){
        for(int y=0; y< systemJeu->getHauteurGrilleJeux(x);y++){
           scene->addItem( systemJeu->getCaseJeux(x,y));
           scene->addItem(systemJeu->getCaseJeux(x,y)->getBouleContenu());
           systemJeu->getCaseJeux(x,y)->setPos(100+x*systemJeu->getCaseJeux(x,y)->boundingRect().width(),50+y*systemJeu->getCaseJeux(x,y)->boundingRect().height());

           systemJeu->getCaseJeux(x,y)->getBouleContenu()->setPos(105+x*systemJeu->getCaseJeux(x,y)->boundingRect().width(),55+y*systemJeu->getCaseJeux(x,y)->boundingRect().height());
        }
    }
   update();
}


void interfaceEditeurNiveauCandyCrush::afficheErreur(QString texte){
    afficheInformation(texte,"Erreur");
}

void interfaceEditeurNiveauCandyCrush::afficheInformation(QString texte, QString titre){
    QMessageBox::information(this,titre,texte);
}

void interfaceEditeurNiveauCandyCrush::setLimiteCoup(int limite){
    systemJeu->setLimiteCoup(limite);
}

void interfaceEditeurNiveauCandyCrush::setObjectif(int objectif, int couleur){
    systemJeu->setObjectif(objectif,couleur);
}

int interfaceEditeurNiveauCandyCrush::getNbCouleur()const{
    return systemJeu->getNbCouleur();
}

void interfaceEditeurNiveauCandyCrush::setAction(ACTION a){
    systemJeu->setAction(a);
}
