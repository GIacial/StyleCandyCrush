#include "selecteurniveau.h"

SelecteurNiveau::SelecteurNiveau(int niveauAFaire, QWidget *parent) : QGraphicsView(parent)
{  levelMax=10;                  //limite de niveau

    scene = new QGraphicsScene(0,0,1000,600,this);
    scene->setBackgroundBrush(QBrush(QColor(50,50,50)));
    this->setScene(scene);

    int ligne=-1;
    int posX=0;

    for(int i=0; i<dernierNiveau(niveauAFaire) ; i++){
        listeNiveaux << new GraphicsBouton();
        scene->addItem(listeNiveaux[i]);
        listeNiveaux[i]->setText(QString::number(i+1));

  //calcul des coo du bouton
        if(ligne!=ligneNiveau(i)){
            posX=0;
            ligne=ligneNiveau(i);
        }
        else{
            posX+= (listeNiveaux[i]->boundingRect().width()+5);
        }
        listeNiveaux[i]->setPos(posX,ligne*(listeNiveaux[i]->boundingRect().height()+5));

//coloration du bouton
        if(i!= niveauAFaire-1){
            listeNiveaux[i]->setColor(QColor(0,155,0));
        }
        else{
            listeNiveaux[i]->setColor(QColor(255,120,0));
        }


        QObject::connect(listeNiveaux[i],SIGNAL(clicked(QGraphicsWidget*)),this,SLOT(selectionNiveau(QGraphicsWidget*)));
    }
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SelecteurNiveau::~SelecteurNiveau(){
    delete scene;

}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int SelecteurNiveau::dernierNiveau(int niveauAFaire){
    if(niveauAFaire>levelMax){
        niveauAFaire=levelMax;
    }
    return niveauAFaire;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SelecteurNiveau::resizeEvent(QResizeEvent *event){
        QGraphicsView::resizeEvent(event);
        QGraphicsView::fitInView(QGraphicsView::sceneRect(), Qt::KeepAspectRatio);//permet de garder le ratio de taille a la remodif de la taille
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SelecteurNiveau::selectionNiveau(QGraphicsWidget *bouton){
    int i=0;

    while(listeNiveaux[i]!=bouton){
        i++;
    }

    emit chargerNiveau(i+1);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int SelecteurNiveau::ligneNiveau(int i){
    i++;    //on verifie si le bouton ne va pas deborder
    i*=(listeNiveaux[0]->boundingRect().width()+5);
    i/=scene->width();
    return i;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
