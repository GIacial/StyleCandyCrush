#include "systemcandycrush_v2.h"

SystemCandyCrush_V2::SystemCandyCrush_V2( QObject *parent) : QObject(parent)
{
    grilleVide = true;
    nbCouleurDeBoule=4;
    nbComboEnCour=0;
    caseSelec1=NULL;
    caseSelec2=NULL;
    score=NULL;
    timer= new QTimer(this);
    timer->setSingleShot(true);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SystemCandyCrush_V2::~SystemCandyCrush_V2(){
    if(!grilleVide){    //si la grille existe on la detruit
        for(int x=grilleJeux.size()-1 ; x>=0 ;x--){
            for(int y=grilleJeux[x].size()-1 ; y>=0 ; y--){

                CaseTerrain* stock = grilleJeux[x][y];
                grilleJeux[x].pop_back();
                delete stock;
                stock =NULL;
            }
        }
    }
    delete timer;
    delete score;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void SystemCandyCrush_V2::creeGrilleRectangulaire(int largeur, int hauteur){
  if(grilleVide){
      //on cree le score
      score = new TableaudesScores(nbCouleurDeBoule,this);
      QObject::connect(score,SIGNAL(onAUnProbleme(QString)),this,SLOT(OnSignaleUneErreur(QString)));

      //puis la grille
    for(int x=0 ; x<largeur ; x++){
         QList<CaseTerrain*> temp;
        for(int y =0 ; y<hauteur-CaseTerrain::numDernierLigne ; y++){//numDernierLigne est negatif donc + de ligne
            temp << new CaseTerrain(x,y+CaseTerrain::numDernierLigne);
            QObject::connect(temp[y],SIGNAL(OnATrouverUnProbleme(QString)),this,SLOT(OnSignaleUneErreur(QString)));
            QObject::connect(temp[y],SIGNAL(cliked(CaseTerrain*)),this,SLOT(echangeBoule(CaseTerrain*)));
            temp[y]->randomBouleColor(nbCouleurDeBoule);
            if(y<-CaseTerrain::numDernierLigne){
                temp[y]->setCacher();
            }
        }
        grilleJeux << temp;
    }
    grilleVide=false;
    while((!ilResteDesCoup()) || (!pasDeCombosurTerrain())){    //si pas de coup ou des combo sur le terrain
        melangeTerrain();
        changerCouleurBouleFrozen();
    }

  }
  else{
      OnSignaleUneErreur("Une grille existe déjà");
  }
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SystemCandyCrush_V2::chargerUneGrille(QString chemin){
    QFile map (chemin);

            if(!map.open(QIODevice::ReadOnly | QIODevice::Text)){
              emit OnATrouverUneErreur("Impossible de charger la map");
            }
            else{
                QTextStream lecture(&map);
                lecture.seek(0);
                if(lecture.readLine().compare("v0.00\n")){

                    nbCouleurDeBoule=lecture.readLine().toInt();                                    //on regarde le nb de couleur
                    int x= lecture.readLine().toInt();
                    int y=lecture.readLine().toInt();
                    creeGrilleRectangulaire(x,y); //on cree la grille
                    for(int x=0; x<getLargeurGrilleJeux();x++){
                        for(int y=0; y<getHauteurGrilleJeux(x);y++){
                            if(lecture.readLine().toInt()==0){    //si bloquer
                                getCaseJeux(x,y)->setCaseBloquer();
                            }
                            if(lecture.readLine().toInt()==0){
                                getCaseJeux(x,y)->setFrozen();
                            }
                        }
                    }

                    //on cree les score
                    for(int i=0; i<nbCouleurDeBoule ; i++){
                        score->setObjectifBoule(lecture.readLine().toInt(),i);
                    }

                    //on donne un nb de coup
                    score->setLimiteCoup(lecture.readLine().toInt());
                }
                else{
                    emit OnATrouverUneErreur("Ancienne Map :(");
                }

                map.close();
            }
            while((!ilResteDesCoup()) || (!pasDeCombosurTerrain())){    //si pas de coup ou des combo sur le terrain
                melangeTerrain();
            }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

CaseTerrain* SystemCandyCrush_V2::getCaseJeux(int x, int y){
    CaseTerrain* retour = NULL;
    y=y-CaseTerrain::numDernierLigne;//decalage des  cacher
  if(!grilleVide){
    if(x <grilleJeux.size() && x>=0){
        if(y <grilleJeux[x].size() && y>=0){
            retour= grilleJeux[x][y];
        }
        else{
            OnSignaleUneErreur("Y n'est pas correct :"+QString::number(y));
        }
    }
    else{
        OnSignaleUneErreur("X n'est pas correct :"+QString::number(x));
    }
  }
  else{
      OnSignaleUneErreur("Pas de grille");
     }
    return retour;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void SystemCandyCrush_V2::OnSignaleUneErreur(QString texte){
    emit OnATrouverUneErreur(texte);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int SystemCandyCrush_V2::getHauteurGrilleJeux( int colonne) const{
    int retour =-1;
    if(colonne<grilleJeux.size() && colonne>=0 ){
        retour=grilleJeux[colonne].size()+CaseTerrain::numDernierLigne;
    }
    return retour;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int SystemCandyCrush_V2::getLargeurGrilleJeux()const{
    return grilleJeux.size();
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void SystemCandyCrush_V2::echangeBoule(CaseTerrain *CaseSelectionner){
    if(caseSelec1==NULL){
        caseSelec1=CaseSelectionner;
    }
    else{
        if(caseSelec1==CaseSelectionner){
            caseSelec1->deselection();
            caseSelec1=NULL;
        }
        else{
            caseSelec2=CaseSelectionner;
            caseSelec1->deselection();
            caseSelec2->deselection();
            caseSelec1->echangeBoule(caseSelec2);

            emit demandeUpdateBoule();
            empecheLeJoueurDeJouer();
            QObject::connect(timer,SIGNAL(timeout()),this,SLOT(echangeBouleVerifCoupAutoriser()));
            timer->start(tempsAnimation_Classique);

        }
    }
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

bool SystemCandyCrush_V2::coupPossible(CaseTerrain* CaseSelectionner){
    int nbIdentique=1;

    //verif en horizontal
    for(int i=CaseSelectionner->getCooX()+1;i<getLargeurGrilleJeux();i++){//vers la droite

        if(CaseSelectionner->contientMemeCouleurBoule(getCaseJeux(i,CaseSelectionner->getCooY())) && getCaseJeux(i,CaseSelectionner->getCooY())->estJouable()){
            nbIdentique++;//meme couleur +1
        }
        else{
            i=getLargeurGrilleJeux();//pas besoin d'aller plus loin on s'arrete
        }
    }

    for(int i=CaseSelectionner->getCooX()-1;i>=0;i--){//vers la gauche

        if(CaseSelectionner->contientMemeCouleurBoule(getCaseJeux(i,CaseSelectionner->getCooY()))  && getCaseJeux(i,CaseSelectionner->getCooY())->estJouable()){
            nbIdentique++;//meme couleur +1
        }
        else{
            i=-1;//pas besoin d'aller plus loin on s'arrete
        }
    }
    //verification coup horizon
    if(nbIdentique<3){//si pas possible en horizon alors on regarde en vertical
        nbIdentique=1;//retour a 1 boule de la couleur


    //vertical

        for(int i=CaseSelectionner->getCooY()+1 ; i<getHauteurGrilleJeux(CaseSelectionner->getCooX()) ; i++){//vers le bas
            if(CaseSelectionner->contientMemeCouleurBoule(getCaseJeux(CaseSelectionner->getCooX(),i))  && getCaseJeux(CaseSelectionner->getCooX(),i)->estJouable()){
                nbIdentique++;//meme couleur +1
            }
            else{
                i=getHauteurGrilleJeux(CaseSelectionner->getCooX());//pas besoin d'aller plus loin on s'arrete
            }
        }

        //vers le haut
        for(int i=CaseSelectionner->getCooY()-1 ; i>=0 ; i--){//vers le bas
            if(CaseSelectionner->contientMemeCouleurBoule(getCaseJeux(CaseSelectionner->getCooX(),i)) && getCaseJeux(CaseSelectionner->getCooX(),i)->estJouable()){
                nbIdentique++;//meme couleur +1
            }
            else{
                i=-1;//pas besoin d'aller plus loin on s'arrete
            }
        }
    }

     return nbIdentique>=3;//coup possible si 3 boule de meme couleure
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SystemCandyCrush_V2::echangeBouleVerifCoupAutoriser(){
    QObject::disconnect(timer,SIGNAL(timeout()),this,SLOT(echangeBouleVerifCoupAutoriser()));

    if(coupAutoriser(caseSelec1,caseSelec2)){//verif si a nouvelle endroit il y a un coup possible
        nbComboEnCour=0;                //on reset les combo

        score->utiliserCoup();  //on utilise un coup
        emit demandeUpdateCoup(score->getNbCoup(),score->getLimiteCoup());

        QList<CaseTerrain*> temp1=getCombo(caseSelec1);
        QList<CaseTerrain*> temp2=getCombo(caseSelec2);

        //verification des combo
        transformationBouleCombo(temp1);
        transformationBouleCombo(temp2);

        QList<CaseTerrain*> temp =temp1+temp2;

        destructionCombo(temp); //on detruit le combo



    }
    else{
       caseSelec1->echangeBoule(caseSelec2);
       emit demandeUpdateBoule();//on demande l'animation du changement
       autoriseLeJoueurAJouer();
    }
    caseSelec1=NULL;
    caseSelec2=NULL;

}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SystemCandyCrush_V2::coupAutoriser(CaseTerrain *b1, CaseTerrain *b2){
    bool aCote = fabs( (b1->getCooX() - b2->getCooX()) + (b1->getCooY()-b2->getCooY()))==1;//verif si les 2 boule sont a cote
    //si mov en hori alors y1=y2 et |x1-x2|=1
    //si mov en verti alors x1=x2 et |y1-y2|=1
    bool triplet=coupPossible(b1) || coupPossible(b2);//verif si triplet pour l'un ou l'autre si il echange leur place
    return aCote && triplet;
    //return true;//ceci est fait juste pour les test de debug pour cree des situation
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

QList<CaseTerrain*> SystemCandyCrush_V2::getCombo(CaseTerrain* CaseSelectionner){
    QList<CaseTerrain*> retour;
    QList<CaseTerrain*> Hori;
    QList<CaseTerrain*> Vert;

    int nbIdentique=1;

    //verif en horizontal
    for(int i=CaseSelectionner->getCooX()+1;i<getLargeurGrilleJeux();i++){//vers la droite

        if(CaseSelectionner->contientMemeCouleurBoule(getCaseJeux(i,CaseSelectionner->getCooY())) && getCaseJeux(i,CaseSelectionner->getCooY())->estJouable()){
            nbIdentique++;//meme couleur +1
            Hori << getCaseJeux(i,CaseSelectionner->getCooY());
        }
        else{
            i=getLargeurGrilleJeux();//pas besoin d'aller plus loin on s'arrete
        }
    }

    for(int i=CaseSelectionner->getCooX()-1;i>=0;i--){//vers la gauche

        if(CaseSelectionner->contientMemeCouleurBoule(getCaseJeux(i,CaseSelectionner->getCooY()))  && getCaseJeux(i,CaseSelectionner->getCooY())->estJouable()){
            nbIdentique++;//meme couleur +1
            Hori << getCaseJeux(i,CaseSelectionner->getCooY());
        }
        else{
            i=-1;//pas besoin d'aller plus loin on s'arrete
        }
    }
    //verification coup horizon
    if(nbIdentique>=3){//si pas possible en horizon alors on regarde en vertical
        for(int i=0;i<Hori.size();i++){
            retour << Hori[i];
         }
    }
    nbIdentique=1;

    //vertical

        for(int i=CaseSelectionner->getCooY()+1 ; i<getHauteurGrilleJeux(CaseSelectionner->getCooX()) ; i++){//vers le bas
            if(CaseSelectionner->contientMemeCouleurBoule(getCaseJeux(CaseSelectionner->getCooX(),i))  && getCaseJeux(CaseSelectionner->getCooX(),i)->estJouable()){
                nbIdentique++;//meme couleur +1
                Vert << getCaseJeux(CaseSelectionner->getCooX(),i);
            }
            else{
                i=getHauteurGrilleJeux(CaseSelectionner->getCooX());//pas besoin d'aller plus loin on s'arrete
            }
        }

        //vers le haut
        for(int i=CaseSelectionner->getCooY()-1 ; i>=0 ; i--){//vers le bas
            if(CaseSelectionner->contientMemeCouleurBoule(getCaseJeux(CaseSelectionner->getCooX(),i)) && getCaseJeux(CaseSelectionner->getCooX(),i)->estJouable()){
                nbIdentique++;//meme couleur +1
                Vert << getCaseJeux(CaseSelectionner->getCooX(),i);
            }
            else{
                i=-1;//pas besoin d'aller plus loin on s'arrete
            }
        }

        if(nbIdentique>=3){//si pas possible en horizon alors on regarde en vertical
            for(int i=0;i<Vert.size();i++){
                retour << Vert[i];
             }
        }

        if(retour.size()!=0){
            retour << CaseSelectionner;
        }

     return retour;
  }
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void SystemCandyCrush_V2::remonterLesBoule(bool AvecTempsAnimation){
    bool onarienfait = true;
    bool onARienFaitDeLaFonction=true;
    QObject::disconnect(timer,SIGNAL(timeout()),this,SLOT(remonterLesBoule()));

    int x=0 ;
        for(int y=CaseTerrain::numDernierLigne+1 ; y <getHauteurGrilleJeux(x) ; y++){
            do{

            if(getCaseJeux(x,y)->estARemonter()){//on remonte la case vers
                 if(onarienfait){//haut
                  onarienfait=remonterBouleVersCaseSup(x,y,x);
                 }
                 if(onarienfait){//gauche
                  onarienfait=remonterBouleVersCaseSup(x,y,x-1);
                 }
                 if(onarienfait){//droite
                  onarienfait=remonterBouleVersCaseSup(x,y,x+1);
                 }


            }

            onARienFaitDeLaFonction= onARienFaitDeLaFonction && onarienfait;
            onarienfait = true;

            x++;
        }while(x<getLargeurGrilleJeux());
        x=0;    //reset du x au debut

        //apres la ligne on verifi si il faut faire une animation
        if(!onARienFaitDeLaFonction){   //si on a fait quelque chose on verifie si on fait une animation ou pas
            if(AvecTempsAnimation && y>-1){
                QObject::connect(timer,SIGNAL(timeout()),this,SLOT(remonterLesBoule()));
                emit demandeUpdateBoule();
                timer->start(tempsAnimation_Classique);
                y =getHauteurGrilleJeux(x); //on sort du for
            }
            else{   //on recommence
                y=CaseTerrain::numDernierLigne;   //on ajoute +1 a la fin
                onARienFaitDeLaFonction=true;
                onarienfait=true;
            }
        }
    }

    if(onARienFaitDeLaFonction){    //si on a trouver aucune boule a echanger on elimine les combo
        enleveComboDuTerrain(AvecTempsAnimation);
    }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SystemCandyCrush_V2::remonterLesBoule(){
    remonterLesBoule(true);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SystemCandyCrush_V2::remonterBouleVersCaseSup(int x, int y, int xSup){
    bool onarienfait=true;
    if(y-1>=CaseTerrain::numDernierLigne && xSup>=0 && xSup<getLargeurGrilleJeux()){

        if(getCaseJeux(xSup,y-1)->estOccuper()){

            getCaseJeux(x,y)->recupBouleCaseAuDessus(getCaseJeux(xSup,y-1));

            onarienfait=false;
        }
    }
    return onarienfait;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SystemCandyCrush_V2::deselectionAuto(CaseTerrain *CaseSelectionner){
    CaseSelectionner->deselection();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SystemCandyCrush_V2::empecheLeJoueurDeJouer(){
    for(int x=0 ; x<getLargeurGrilleJeux() ; x++){
        for(int y=CaseTerrain::numDernierLigne; y<getHauteurGrilleJeux(x) ; y++){
            QObject::disconnect(getCaseJeux(x,y),SIGNAL(cliked(CaseTerrain*)),this,SLOT(echangeBoule(CaseTerrain*)));
            QObject::connect(getCaseJeux(x,y),SIGNAL(cliked(CaseTerrain*)),this,SLOT(deselectionAuto(CaseTerrain*)));
        }
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SystemCandyCrush_V2::autoriseLeJoueurAJouer(){
    for(int x=0 ; x<getLargeurGrilleJeux() ; x++){
        for(int y=CaseTerrain::numDernierLigne; y<getHauteurGrilleJeux(x) ; y++){
            QObject::connect(getCaseJeux(x,y),SIGNAL(cliked(CaseTerrain*)),this,SLOT(echangeBoule(CaseTerrain*)));
            QObject::disconnect(getCaseJeux(x,y),SIGNAL(cliked(CaseTerrain*)),this,SLOT(deselectionAuto(CaseTerrain*)));
        }
    }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SystemCandyCrush_V2::enleveComboDuTerrain(bool AvecTempsAnimation){
    QList<CaseTerrain*> temp;
    QList <CaseTerrain*> lesCombo;
    QList<CaseTerrain*> meilleurs;
    bool onarienfait=true;
    for(int x=0 ; x<getLargeurGrilleJeux() ; x++){
        for(int y=0; y<getHauteurGrilleJeux(x) ; y++){
            if(getCaseJeux(x,y)->estJouable()){

                temp=getCombo(getCaseJeux(x,y));
                onarienfait= onarienfait && temp.size()==0;

                //on teste si l'une des boule peut donner un meilleur coup
                for(int i=0; i< temp.size() ; i++){
                   meilleurs=getCombo(temp[i]);
                   if(meilleurs.size()>temp.size()){
                       temp=meilleurs;
                       i=0;             //on retest les boule
                   }
                   meilleurs.clear();
                }

                transformationBouleCombo(temp); //on detecte les combo de plus de 3

                //on les cache des combo possible
                for(int i=0 ; i<temp.size() ; i++){
                    temp[i]->setARemonter();
                }
                if(temp.size()>0){
                    lesCombo+=temp; //on memorise les case pour les compter apres
                }


                temp.clear();
            }
        }
    }


    

    if(onarienfait){
        finDeTour();
    }
    else{
        nbComboEnCour++;        //on compte un combo en plus
        destructionCombo(lesCombo,AvecTempsAnimation);//on  compte tout les boule prise et active les effect de boule combo
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SystemCandyCrush_V2::ilResteDesCoup(){
    bool tupeuxjouer=false;
    for(int x=0 ; x<getLargeurGrilleJeux() ; x++){
        for(int y=0; y<getHauteurGrilleJeux(x) ; y++){

            if(getCaseJeux(x,y)->estOccuper()){
                tupeuxjouer=deplacementPosible(getCaseJeux(x,y),x-1,y) || tupeuxjouer; //cas deplacement vers la gauche
                tupeuxjouer=deplacementPosible(getCaseJeux(x,y),x+1,y) || tupeuxjouer; //cas deplacement vers la droite
                tupeuxjouer=deplacementPosible(getCaseJeux(x,y),x,y-1) || tupeuxjouer; //cas deplacement vers la haut
                tupeuxjouer=deplacementPosible(getCaseJeux(x,y),x,y+1) || tupeuxjouer; //cas deplacement vers la bas

                if(tupeuxjouer){                //arret si on trouve
                    x=getLargeurGrilleJeux()-1;
                    y=getHauteurGrilleJeux(x);
                }
            }
          }
    }
    return tupeuxjouer;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SystemCandyCrush_V2::deplacementPosible(CaseTerrain *CaseAtester, int xCible, int yCible){
    QList<CaseTerrain*> temp;
    bool tupeuxjouer=false;
    if(yCible>=0 && yCible<getHauteurGrilleJeux(xCible)){  //cas du x faut gerer autaumatiquement par le y <-1
        if(getCaseJeux(xCible,yCible)->estCapableDetreBougerParJoueur()){
            CaseAtester->echangeBoule(getCaseJeux(xCible,yCible));
            //on teste
                temp=getCombo(CaseAtester);
                tupeuxjouer=temp.size()>0;
            //on remets en place
            CaseAtester->echangeBoule(getCaseJeux(xCible,yCible));
        }
    }
    return tupeuxjouer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SystemCandyCrush_V2::melangeTerrain(){

     bool onarienfait;

    if(compteBouleTerrain()){
        for(int x=0 ; x<getLargeurGrilleJeux() ; x++){
            for(int y=0; y<getHauteurGrilleJeux(x) ; y++){
                if(getCaseJeux(x,y)->estCapableDetreBougerParJoueur()){
                    onarienfait=true;
                    int xRand,yRand;

                    while(onarienfait){//tant que l'on ne la pas echanger
                        xRand= rand()%getLargeurGrilleJeux();
                        yRand= rand()%getHauteurGrilleJeux(xRand);
                        if(getCaseJeux(xRand,yRand)->estCapableDetreBougerParJoueur() && getCaseJeux(xRand,yRand)!=getCaseJeux(x,y)){
                            getCaseJeux(x,y)->echangeBoule(getCaseJeux(xRand,yRand));
                            onarienfait=false;
                        }
                    }
                }
            }
        }
    }
    else{
        emit passerUnMessage("le jeu est bloquer!! ","Game Over");
    }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SystemCandyCrush_V2::compteBouleTerrain(){
    QList<int> compteur;
    for(int i=0; i < nbCouleurDeBoule ; i++){
        compteur << 0 ;
    }

    for(int x=0 ; x<getLargeurGrilleJeux() ; x++){
        for(int y=0; y<getHauteurGrilleJeux(x) ; y++){
            if(getCaseJeux(x,y)->estJouable()){
                compteur[Boule::getColorPredefini(getCaseJeux(x,y)->getBouleContenu()->getColor())]++;
            }
        }
    }

    bool possible=false;
    for(int i=0; i < nbCouleurDeBoule ; i++){
        possible= possible || compteur.size()>=3;
    }
    return possible;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SystemCandyCrush_V2::pasDeCombosurTerrain(){
    QList<CaseTerrain*> temp;
    bool pasdecombo=true;
    for(int x=0 ; x<getLargeurGrilleJeux() ; x++){
        for(int y=0; y<getHauteurGrilleJeux(x) ; y++){
            if(getCaseJeux(x,y)->estJouable()){

                temp=getCombo(getCaseJeux(x,y));
                pasdecombo= pasdecombo && temp.size()==0;

                temp.clear();
            }
        }
    }
    return pasdecombo;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

bool SystemCandyCrush_V2::existeGrille()const{
    return !grilleVide;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SystemCandyCrush_V2::transformationBouleCombo(QList<CaseTerrain *> &combo){
    if(combo.size()>3){//on regarde si c'est un combo
        int nbBouleHori=1;
        int nbBouleVerti=1;

        for(int i=0; i<combo.size()-1 ; i++){   //on ne compte pas la derniere boule qui est celle qui cree le combo
            if(combo[i]->getCooX() == combo[combo.size()-1]->getCooX()){//boule aligne sur x
                nbBouleVerti++; //meme colonne donc verticale
            }
            else{if(combo[i]->getCooY() == combo[combo.size()-1]->getCooY()){//boule aligne sur y
                    nbBouleHori++; //meme ligne donc horizontal
                }

            }
        }
        combo[combo.size()-1]->transformerBouleEnBouleCombo(Boule::definirTypeCombo(nbBouleHori,nbBouleVerti));  //on transform et protege
    }
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int SystemCandyCrush_V2::getNbCouleur()const{
    return nbCouleurDeBoule;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SystemCandyCrush_V2::demandeUpdate(){
    emit demandeUpdateBoule();
    emit demandeUpdateCoup(score->getNbCoup(),score->getLimiteCoup());
    emit demandeUpdateScore(score->getNbBouleDetruit(),score->getObjectifBoule());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void SystemCandyCrush_V2::finDeTour(){
    if(score->estVictoire()){
        emit passerUnMessage("Victoire","Bravo");
        emit onSignaleLaFinDePartie(true);
    }
    else{
        if(score->estDefaite()){
            emit passerUnMessage("Perdu","Défaite");
            emit onSignaleLaFinDePartie(false);
        }
        else{
        autoriseLeJoueurAJouer();
        if(!ilResteDesCoup()){
            emit passerUnMessage("Plus de coup","Info");
            while((!ilResteDesCoup()) || (!pasDeCombosurTerrain())){    //si pas de coup ou des combo sur le terrain
                melangeTerrain();
            }
            emit demandeUpdateBoule();
        }
     }
    }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SystemCandyCrush_V2::effectBouleCombo(CaseTerrain *caseBouleCombo){
    bool retour=false;
    if(caseBouleCombo->getBouleContenu()->estBouleCombo() ){ //si boule de combo
        if(caseBouleCombo->estARemonter() || caseBouleCombo->estExplosif()){    //si la boule remonte ou est explosif

            //cas d'une boule qui explose
            retour=true;
            emit demandeAnimationEffectBouleCombo(caseBouleCombo);

            switch (caseBouleCombo->getBouleContenu()->getTypeCombo()) {
            case Boule::TypeCombo_Horizontal:
                effectBouleCombo_Horizontal(caseBouleCombo);
                break;
            case Boule::TypeCombo_Vertical:
                effectBouleCombo_Vertical(caseBouleCombo);
                break;
            case Boule::TypeCombo_X:
                effectBouleCombo_Horizontal(caseBouleCombo);
                effectBouleCombo_Vertical(caseBouleCombo);
                break;
            default: emit OnATrouverUneErreur("Impossible de trouver le type du combo");
                break;
            }

        }

    }
    return retour;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SystemCandyCrush_V2::effectBouleCombo_Horizontal(CaseTerrain *caseBouleCombo){
    for(int i=0 ; i<getLargeurGrilleJeux() ; i++){  //on enleve la ligne (effect hori)

        if(getCaseJeux(i,caseBouleCombo->getCooY())->estJouable() && (!getCaseJeux(i,caseBouleCombo->getCooY())->estResistantRemonter())){//si c'est une case jouable et non resistant a la remonter

            if(getCaseJeux(i,caseBouleCombo->getCooY())->estFrozen()){
                getCaseJeux(i,caseBouleCombo->getCooY())->unfrozen();
            }
            else{
                score->compterBouleDetruit(getCaseJeux(i,caseBouleCombo->getCooY())->getBouleContenu()->getColor());    //on compte
                getCaseJeux(i,caseBouleCombo->getCooY())->setARemonter();                                               //on remonte
                emit demandeAnimationRecup(getCaseJeux(i,caseBouleCombo->getCooY()));                                   //animation de la boule qui va dans le collecteur

                if(getCaseJeux(i,caseBouleCombo->getCooY())->getBouleContenu()->estBouleCombo()){
                    effectBouleCombo(getCaseJeux(i,caseBouleCombo->getCooY()));                     //active les effect des boule combo
                }

                getCaseJeux(i,caseBouleCombo->getCooY())->randomBouleColor(nbCouleurDeBoule);                           //on recolorie
            }
        }
    }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SystemCandyCrush_V2::effectBouleCombo_Vertical(CaseTerrain *caseBouleCombo){
    for(int i=0 ; i<getHauteurGrilleJeux() ; i++){  //on enleve la ligne (effect hori)

        if(getCaseJeux(caseBouleCombo->getCooX(),i)->estJouable() && (!getCaseJeux(caseBouleCombo->getCooX(),i)->estResistantRemonter())){//si c'est une case jouable et non resistant a la remonter


            if(getCaseJeux(caseBouleCombo->getCooX(),i)->estFrozen()){
                getCaseJeux(caseBouleCombo->getCooX(),i)->unfrozen();
            }
            else{

                score->compterBouleDetruit(getCaseJeux(caseBouleCombo->getCooX(),i)->getBouleContenu()->getColor());    //on compte
                getCaseJeux(caseBouleCombo->getCooX(),i)->setARemonter();                                               //on remonte              
                emit demandeAnimationRecup(getCaseJeux(caseBouleCombo->getCooX(),i));                                   //animation de la boule qui va dans le collecteur

                if(getCaseJeux(caseBouleCombo->getCooX(),i)->getBouleContenu()->estBouleCombo()){
                    effectBouleCombo(getCaseJeux(caseBouleCombo->getCooX(),i));                     //active les effect des boule combo
                }

                getCaseJeux(caseBouleCombo->getCooX(),i)->randomBouleColor(nbCouleurDeBoule);                           //on recolorie
              }
        }
    }
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SystemCandyCrush_V2::destructionCombo(QList<CaseTerrain *> &combo, bool avecTempsAnimation){
    //on compte toute les boule du combo
    bool effectActiver=false;
    bool memoire=false;


    afficherTextCombo();

    for(int i=0 ; i<combo.size() ; i++){
        if(!combo[i]->estFrozen()){
            score->compterBouleDetruit(combo[i]->getBouleContenu()->getColor()); //on compte les boules
            combo[i]->setARemonter();            //on change pas les couleur pour garder en memoire les boule de combo
            emit demandeAnimationRecup(combo[i]);
        }
        else{
            combo[i]->unfrozen();
        }
    }

    //puis on active les effect de combo
    for(int i=0 ; i<combo.size() ; i++){
            memoire=effectBouleCombo(combo[i]);
            effectActiver=effectActiver || memoire;
            if(combo[i]->estARemonter()){    //si est a remonter on la change de couleur
                combo[i]->randomBouleColor(nbCouleurDeBoule);//on les change de couleur maintenat que l'eefect de boule combo est passer
            }
            memoire=false;
    }

    //on deprotege les nouvelle boules combo
    for(int i=0 ; i<combo.size() ; i++){
        if(combo[i]->estResistantRemonter() ){
            combo[i]->enleverResistanceRemonter();
        }

    }

    emit demandeUpdateScore(score->getNbBouleDetruit(),score->getObjectifBoule());  //refresh score

    if(avecTempsAnimation){
        QObject::connect(timer,SIGNAL(timeout()),this,SLOT(remonterLesBoule()));
        if(effectActiver){
            timer->start(tempsAnimation_Classique+tempsAnimation_BouleCombo);
        }
        else{
            timer->start(tempsAnimation_Classique);
        }
    }
    else{
        remonterLesBoule(avecTempsAnimation);
    }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void SystemCandyCrush_V2::changerCouleurBouleFrozen(){
    for(int x=0; x<getLargeurGrilleJeux();x++){
        for(int y=0; y<getHauteurGrilleJeux(x) ;y++){
            if(getCaseJeux(x,y)->estFrozen()){
                getCaseJeux(x,y)->randomBouleColor(nbCouleurDeBoule);
            }
        }
    }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SystemCandyCrush_V2::afficherTextCombo(){
    switch (nbComboEnCour) {
    case 0:break;   //pas de texte
    case 1:
        emit demandeAffichageTextCombo("  Cool !!");
        break;
    case 2:
        emit demandeAffichageTextCombo(" Super !!");
        break;
    case 3:
        emit demandeAffichageTextCombo("Amazing !!");
        break;
    default: emit demandeAffichageTextCombo("Supreme !!");
        break;
    }
}
