#include "interfacev2.h"

InterfaceV2::InterfaceV2(QWidget *parent) : QGraphicsView(parent)
{
    creationInterfaceMin();

    //le reste des variable
    conteneurEtat=NULL;
    etatSuivant=NULL;
    machineEtat=NULL;
    groupAnimation=NULL;
    trans=NULL;


}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

InterfaceV2::~InterfaceV2(){
    delete systemJeu;
    delete scene;

    //l'intefrcae est delete en meme temps que la scene

    //system animation
    delete conteneurEtat;
    delete machineEtat;
    delete groupAnimation;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InterfaceV2::creationConnectionSystemInterface(){

    QObject::connect(systemJeu,SIGNAL(OnATrouverUneErreur(QString)),this,SLOT(afficheErreur(QString)));
    QObject::connect(systemJeu,SIGNAL(passerUnMessage(QString,QString)),this,SLOT(afficheInformation(QString,QString)));
    QObject::connect(systemJeu,SIGNAL(demandeUpdateCoup(int,int)),this,SLOT(updateTextNbCoup(int,int)));
    QObject::connect(systemJeu,SIGNAL(onSignaleLaFinDePartie(bool)),this,SLOT(onSignaleLaFin(bool)));
    QObject::connect(systemJeu,SIGNAL(demandeAnimationRecup(CaseTerrain*)),this,SLOT(animationRecupBoule(CaseTerrain*)));
    QObject::connect(systemJeu,SIGNAL(demandeAnimationEffectBouleCombo(CaseTerrain*)),this,SLOT(animationEffectBouleCombo(CaseTerrain*)));
    QObject::connect(systemJeu,SIGNAL(demandeAffichageTextCombo(QString)),textCombo,SLOT(showChrono(QString)));
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void InterfaceV2::creationInterfaceMin(){
    systemJeu = new SystemCandyCrush_V2(this);
    scene = new QGraphicsScene(0,0,1000,600,this);
    scene->setBackgroundBrush(QBrush(QColor(50,50,50)));
    this->setScene(scene);

    //interface

        //text nb coup
        textNbCoup= new QGraphicsTextItem("Coup restant :");
        scene->addItem(textNbCoup);
        textNbCoup->setPos(10,10);
        textNbCoup->setDefaultTextColor(Qt::white);
        textNbCoup->setScale(2);

        //text de combo
        textCombo= new GraphicsTextChrono("Amazing !!",500);
        scene->addItem(textCombo);
        textCombo->setPos(150,50);
        textCombo->setDefaultTextColor(Qt::white);
        textCombo->setRotation(30);
        textCombo->hide();//on le cache
        textCombo->setZValue(4);


        //connection
        creationConnectionSystemInterface();
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void InterfaceV2::resizeEvent(QResizeEvent *event){
        QGraphicsView::resizeEvent(event);
        QGraphicsView::fitInView(QGraphicsView::sceneRect(), Qt::KeepAspectRatio);//permet de garder le ratio de taille a la remodif de la taille
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void InterfaceV2::creeUneGrille(int largeur, int hauteur){
    recreerLeJeu();
    systemJeu->creeGrilleRectangulaire(largeur,hauteur);
    creeAffichageGrille();
    creeAffichageScore();
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void InterfaceV2::chargerMap(QString chemin){
    recreerLeJeu();
    systemJeu->chargerUneGrille(chemin);
    creeAffichageGrille();
    creeAffichageScore();
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void InterfaceV2::creeAffichageGrille(){
    for(int x=0 ; x<systemJeu->getLargeurGrilleJeux() ;x++){
        for(int y=0; y< systemJeu->getHauteurGrilleJeux(x);y++){
           scene->addItem( systemJeu->getCaseJeux(x,y));
           systemJeu->getCaseJeux(x,y)->setZValue(0);
           systemJeu->getCaseJeux(x,y)->setPos(100+x*systemJeu->getCaseJeux(x,y)->boundingRect().width(),50+y*systemJeu->getCaseJeux(x,y)->boundingRect().height());
        }
    }
    //on redimensionne
    scene->setSceneRect(0,0,120+systemJeu->getLargeurGrilleJeux()*systemJeu->getCaseJeux(0,0)->boundingRect().width(),70+systemJeu->getHauteurGrilleJeux()*systemJeu->getCaseJeux(0,0)->boundingRect().height());
    textCombo->setScale(systemJeu->getLargeurGrilleJeux());

    //force l'update de la secene
    resize(this->width()-1,this->height()-1);
    resize(this->width()+1,this->height()+1);

    //on fait les animation
    creeAnimationDeplacementBoule();
   update();
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------


void InterfaceV2::afficheErreur(QString texte){
    afficheInformation(texte,"Erreur");
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void InterfaceV2::afficheInformation(QString texte, QString titre){
    QMessageBox::information(this,titre,texte);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void InterfaceV2::creeAnimationDeplacementBoule(){
    //def des etat
        conteneurEtat= new QState();
        etatSuivant= new QState(conteneurEtat);

        //def de la mach d'etat
        machineEtat= new QStateMachine(this);
        machineEtat->addState(conteneurEtat);
        machineEtat->setInitialState(conteneurEtat);
        conteneurEtat->setInitialState(etatSuivant);

        //def des animation
        groupAnimation= new QParallelAnimationGroup(this);

        //def des transition
        trans = conteneurEtat->addTransition(this, SIGNAL(passeEtatDeFin()), etatSuivant);//declancheur de l'etat
        trans->addAnimation(groupAnimation);//sert a def la transition

        //mise en place des boule
        for(int x=0;x<systemJeu->getLargeurGrilleJeux();x++){
            for(int y=CaseTerrain::numDernierLigne;y<systemJeu->getHauteurGrilleJeux(x);y++){

                CaseTerrain* caseItem=systemJeu->getCaseJeux(x,y);
                Boule* item=caseItem->getBouleContenu();

                    //ajout a la scene

                    scene->addItem(item);//ajoute item a la scene
                    item->setZValue(1);//permet de definir z pour qu'il soit au desus des item avec z inferieur
                    item->setPos(105+x*caseItem->boundingRect().width(),55+y*caseItem->boundingRect().height());

                    QPropertyAnimation *anim = new QPropertyAnimation(item, "pos");//sert a definir les priopriete d'animation d'un  item
                    anim->setDuration(SystemCandyCrush_V2::tempsAnimation_Classique);//temps de l'animation

                    anim->setEasingCurve(QEasingCurve::Linear);//style de l'animation
                    groupAnimation->addAnimation(anim);//ajout au group d'animation


            }
        }
        updateAnimationDeplacement();
        machineEtat->start();


        QObject::connect(systemJeu,SIGNAL(demandeUpdateBoule()),this,SLOT(updateAnimationDeplacement()));

}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void InterfaceV2::updateAnimationDeplacement(){
    for(int x=0;x<systemJeu->getLargeurGrilleJeux();x++){
        for(int y=CaseTerrain::numDernierLigne;y<systemJeu->getHauteurGrilleJeux(x);y++){

                CaseTerrain* caseItem=systemJeu->getCaseJeux(x,y);
                Boule* item=caseItem->getBouleContenu();
                etatSuivant->assignProperty(item,"pos",QPointF(105+x*caseItem->boundingRect().width(),55+y*caseItem->boundingRect().height()));

        }
    }
    emit passeEtatDeFin();
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InterfaceV2::animationRecupBoule(CaseTerrain* caseS){
    if(scoreBouleDetruit[Boule::getColorPredefini(caseS->getBouleContenu()->getColor())]->isVisible()){
        Boule* b= new Boule();
        b->setColor(caseS->getBouleContenu()->getColor());
        b->setPos(caseS->getBouleContenu()->pos());
        scene->addItem(b);
        b->setZValue(3);

        QPropertyAnimation *anim = new QPropertyAnimation(b, "pos",b);//sert a definir les priopriete d'animation d'un  item
        anim->setDuration(tempsAnimationRecup);//temps de l'animation
        anim->setEasingCurve(QEasingCurve::Linear);//style de l'animation
        anim->setEndValue(scoreBouleDetruit[Boule::getColorPredefini(b->getColor())]->pos());
        anim->start();

        QObject::connect(anim,SIGNAL(finished()),b,SLOT(deleteLater()));
    }

}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InterfaceV2::animationEffectBouleCombo(CaseTerrain *caseS){
    EffectExplosion* e=NULL;
    EffectExplosion* e2=NULL;

    QRectF depart =QRectF(caseS->pos(),QPointF(caseS->pos().x()+caseS->boundingRect().width(),caseS->pos().y()+caseS->boundingRect().height()));
    //cas horizontal
    QPointF finHoriBottomRigth=QPointF(systemJeu->getCaseJeux(systemJeu->getLargeurGrilleJeux()-1,caseS->getCooY())->pos().x()+caseS->boundingRect().width(),
                                       systemJeu->getCaseJeux(systemJeu->getLargeurGrilleJeux()-1,caseS->getCooY())->pos().y()+caseS->boundingRect().height());
    QRectF finHori= QRectF(systemJeu->getCaseJeux(0,caseS->getCooY())->pos(),finHoriBottomRigth);
    //cas vertical
    QPointF finVertiBottomRigth=QPointF(systemJeu->getCaseJeux(caseS->getCooX(),systemJeu->getHauteurGrilleJeux(caseS->getCooX())-1)->pos().x()+caseS->boundingRect().width(),
                                        systemJeu->getCaseJeux(caseS->getCooX(),systemJeu->getHauteurGrilleJeux(caseS->getCooX())-1)->pos().y()+caseS->boundingRect().height());
    QRectF finVerti=QRectF(systemJeu->getCaseJeux(caseS->getCooX(),0)->pos(),finVertiBottomRigth);


    switch (caseS->getBouleContenu()->getTypeCombo()) {
    case Boule::TypeCombo_Horizontal:
        e=new EffectExplosion(depart,finHori,SystemCandyCrush_V2::tempsAnimation_BouleCombo);
        e->setDirection(EffectExplosion::DIRECTION_HORIZONTAL);
        scene->addItem(e);
        e->setPos(caseS->pos());
        e->setZValue(2);

        e->lancerAnimation();
        break;
    case Boule::TypeCombo_Vertical:
        e=new EffectExplosion(depart,finVerti,SystemCandyCrush_V2::tempsAnimation_BouleCombo);
        e->setDirection(EffectExplosion::DIRECTION_VERTICAL);
        scene->addItem(e);
        e->setPos(caseS->pos());
        e->setZValue(2);

        e->lancerAnimation();
        break;
    case Boule::TypeCombo_X:
        e=new EffectExplosion(depart,finHori,SystemCandyCrush_V2::tempsAnimation_BouleCombo);
        e2=new EffectExplosion(depart,finVerti,SystemCandyCrush_V2::tempsAnimation_BouleCombo);
        scene->addItem(e);
        e->setDirection(EffectExplosion::DIRECTION_HORIZONTAL);
        e->setPos(caseS->pos());
        e->setZValue(2);
        e2->setDirection(EffectExplosion::DIRECTION_VERTICAL);
        scene->addItem(e2);
        e2->setPos(caseS->pos());
        e2->setZValue(2);

        e2->lancerAnimation();
        e->lancerAnimation();

        break;
    default:afficheErreur("Ce n'est pas une boule combo");
        break;
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void InterfaceV2::recreerLeJeu(){
    if(systemJeu->existeGrille()){
        delete systemJeu;
        delete scene;
        delete conteneurEtat;
        delete machineEtat;
        delete groupAnimation;

        while(scoreBouleDetruit.size()!=0){
            scoreBouleDetruit.pop_back();
        }


        creationInterfaceMin();

    }
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void InterfaceV2::creeAffichageScore(){

        for(int i=0 ; i<systemJeu->getNbCouleur() ; i++){           //on cree toute les boule possible
            scoreBouleDetruit << new AfficheurScore(Boule::getColorPredefini(i));
            scoreBouleDetruit[i]->setPos(10,80+i*(scoreBouleDetruit[i]->getHauteur()-CaseTerrain::numDernierLigne));
            scene->addItem(scoreBouleDetruit[i]);
        }
        QObject::connect(systemJeu,SIGNAL(demandeUpdateScore(QList<int>,QList<int>)),this,SLOT(updateScore(QList<int>,QList<int>)));

    systemJeu->demandeUpdate();
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void InterfaceV2::updateTextNbCoup(int nbCoup, int limite){
    textNbCoup->setPlainText("Coup Restant : "+QString::number(limite-nbCoup));
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void InterfaceV2::updateScore(QList<int> detruit, QList<int> limite){
    int y=0;
    for(int i=0; i<systemJeu->getNbCouleur(); i++){
            if(limite[i]!=0){
                scoreBouleDetruit[i]->show();
                scoreBouleDetruit[i]->setText(QString::number(detruit[i])+"/"+QString::number(limite[i]));
                scoreBouleDetruit[i]->setPos(10,80+y*(scoreBouleDetruit[i]->getHauteur()-CaseTerrain::numDernierLigne));
                y++;
            }
            else{
                scoreBouleDetruit[i]->hide();
            }

    }
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InterfaceV2::onSignaleLaFin(bool victoire){
    emit onAFini(victoire);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
