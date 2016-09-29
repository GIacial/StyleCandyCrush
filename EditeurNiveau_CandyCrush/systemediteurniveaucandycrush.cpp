#include "systemediteurniveaucandycrush.h"

systemediteurniveauCandyCrush::systemediteurniveauCandyCrush(QObject *parent) : QObject(parent)
{
    grilleVide = true;
    nbCouleurDeBoule=4;
    score=NULL;
    action=BLOC;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void systemediteurniveauCandyCrush::creeGrilleRectangulaire(int largeur, int hauteur){
  if(grilleVide){
    for(int x=0 ; x<largeur ; x++){
         QList<CaseTerrain*> temp;
        for(int y =0 ; y<hauteur+5 ; y++){
            temp << new CaseTerrain(x,y-5);
            QObject::connect(temp[y],SIGNAL(OnATrouverUnProbleme(QString)),this,SLOT(OnSignaleUneErreur(QString)));
            QObject::connect(temp[y],SIGNAL(cliked(CaseTerrain*)),this,SLOT(bloquerLaCase (CaseTerrain*)));
            temp[y]->randomBouleColor(nbCouleurDeBoule);
            if(y<5){
                temp[y]->setCacher();
            }
        }
        grilleJeux << temp;
    }
    grilleVide=false;
    score= new TableaudesScores(nbCouleurDeBoule,this);
  }
  else{
      OnSignaleUneErreur("Une grille existe déjà");
  }
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void systemediteurniveauCandyCrush::chargerUneGrille(QString chemin){
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
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void systemediteurniveauCandyCrush::sauvegarderUneGrille(QString chemin){
    QFile file(chemin);//lien vers le fichier
    if(!grilleVide){
        if (!file.open(QIODevice::ReadWrite | QIODevice::Text)){//vérifie si le fichier s'ouvre readWrite pour ecrire et lire et "ACTION"Only sert a une fonction

          emit OnATrouverUneErreur("Impossible de sauvegarder cette grille");//informe si ne s'ouvre pas

        }
        else{

            QTextStream save(&file);//lie un text au fichier
            save << "v0.00" << "\n";
            save << nbCouleurDeBoule << "\n";
            save << getLargeurGrilleJeux() << "\n";
            save << getHauteurGrilleJeux(0) << "\n";

            for(int x=0; x<getLargeurGrilleJeux();x++){
                for(int y=0; y<getHauteurGrilleJeux(x);y++){
                      save << !getCaseJeux(x,y)->estBloquer() << "\n";
                      save << !getCaseJeux(x,y)->estFrozen() << "\n";

                }
            }

            //on cree les score
            for(int i=0; i<nbCouleurDeBoule ; i++){
                save << score->getObjectifBoule(i)<< "\n";
            }

            //on donne un nb de coup
            save << score->getLimiteCoup()<< "\n";

           file.close();//ferme le fichier
        }
   }
    else{
        emit OnATrouverUneErreur("Il n'y a pas de grille a sauvegarder");
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

CaseTerrain* systemediteurniveauCandyCrush::getCaseJeux(int x, int y){
    CaseTerrain* retour = NULL;
    y=y+5;//decalage des 5 cacher
  if(!grilleVide){
    if(x <grilleJeux.size()){
        if(y <grilleJeux[x].size()){
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

void systemediteurniveauCandyCrush::OnSignaleUneErreur(QString texte){
    emit OnATrouverUneErreur(texte);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int systemediteurniveauCandyCrush::getHauteurGrilleJeux( int colonne) const{
    int retour =-1;
    if(colonne<grilleJeux.size() && colonne>=0 ){
        retour=grilleJeux[colonne].size()-5;
    }
    return retour;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int systemediteurniveauCandyCrush::getLargeurGrilleJeux()const{
    return grilleJeux.size();
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void systemediteurniveauCandyCrush::bloquerLaCase(CaseTerrain *caseS){
    switch (action) {
    case BLOC:
        caseS->setCaseBloquer();
        break;
    case FROZ:
        if(caseS->estFrozen()){
            caseS->unfrozen();
        }
        else{
            caseS->setFrozen();
        }
        break;
    default:
        break;
    }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool systemediteurniveauCandyCrush::grilleExiste()const{
    return !grilleVide;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void systemediteurniveauCandyCrush::setNbCouleur(int nbCouleur){
    nbCouleurDeBoule=nbCouleur;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void systemediteurniveauCandyCrush::setObjectif(int objectif, int couleur){
    score->setObjectifBoule(objectif,couleur);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void systemediteurniveauCandyCrush::setLimiteCoup(int limite){
    score->setLimiteCoup(limite);
}

int systemediteurniveauCandyCrush::getNbCouleur()const{
    return nbCouleurDeBoule;
}

void systemediteurniveauCandyCrush::setAction(ACTION act){
    action=act;
}
