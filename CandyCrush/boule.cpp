#include "boule.h"

Boule::Boule(int nbCouleur, QGraphicsItem *parent):  Boule(parent)
{

       randomColor(nbCouleur);//attribut une couleur pour color

}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Boule::Boule( QGraphicsItem *parent):  QGraphicsWidget(parent){
    setAcceptHoverEvents(true);//permet d'activer la detection du passage de la souris sur item
    setCacheMode(DeviceCoordinateCache);//def la resolution de l'item
    bouleDeCombo=TypeCombo_Aucun;
    bouledeComboSuiv=TypeCombo_Horizontal;
    bouleSelectionner=false;
    resistanceRemonter=FAIBLE;
    frozen=false;
    setColor(getColorPredefini(0));
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void Boule:: mousePressEvent(QGraphicsSceneMouseEvent *){
    if(!frozen){
        bouleSelectionner=true;
        update();
        emit pressed();
    }

}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void Boule::mouseReleaseEvent(QGraphicsSceneMouseEvent *){
    update();
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

QRectF Boule::boundingRect() const{
       return QRectF(0,0,50,50);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

QPainterPath Boule::shape() const{
        return this->getFormeBoule(this->boundingRect());
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

QPainterPath Boule::getFormeBoule(QRectF r,int origineX,int origineY)const{
    QPainterPath path;

    switch (getColorPredefini(color) ) {
    case 0:  //rouge
            path = getForm_Fraise(r,origineX,origineY);
            break;
    case 1: //vert
            path.addRoundedRect(origineX,origineY,r.width()-origineX/3,r.height()-origineY/3,15,15);
        break;
    case 2: //bleu
            path=getForm_Triangle(r,origineX,origineY);
        break;
    case 4: //jaune
        path=getForm_Etoile(r,origineX,origineY);
        break;
    case 5://rose coeur
            path=getForm_Coeur(r,origineX,origineY);
        break;
    case 6://cyan
        //goutte
        path=getForm_Goutte(r,origineX,origineY);
        break;
    case 7://orange
        //carotte
        path=getForm_Carotte(r,origineX,origineY);
        break;
    default://une boule classique
         path.addEllipse(QPointF((r.width()/2)+origineX,(r.height()/2)+origineY),r.width()/2-(origineX/2),r.height()/2-(origineY/2));//defini la zone cliquable
        break;
    }
    return path;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

QPainterPath Boule::getForm_Fraise(QRectF r, int origineX, int origineY)const{
    QPainterPath path;
    //fraise
    path.moveTo((r.width()/2)+origineX, origineY);
    path.cubicTo((r.width()*1.25)+origineX,origineY,origineX+r.width(),(4*r.height()/5)+origineY,(r.width()/2)+origineX,origineY+r.height());//partie droite
    path.moveTo((r.width()/2)+origineX,origineY);
    path.cubicTo((-r.width()*0.25)+origineX,origineY,origineX,(4*r.height()/5)+origineY,(r.width()/2)+origineX,origineY+r.height());//partie gauche
    return path;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

QPainterPath Boule::getForm_Triangle(QRectF r, int origineX, int origineY) const{
    QPainterPath path;
    //triangle
    path.moveTo((r.width()/2)+origineX, origineY);
    path.lineTo(origineX,origineY+r.height());
    path.lineTo(r.width()+origineX,origineY+r.height());
    path.lineTo((r.width()/2)+origineX, origineY);
    return path;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

QPainterPath Boule::getForm_HautCarotte(QRectF r, int origineX, int origineY) const{
    QPainterPath path;
    //haut de carotte
    path.moveTo((r.width()/2)+origineX, (r.height()/4)+origineY);
    path.arcTo((r.width()/4)+origineX,origineY,(r.width()/2),(r.height()/4),0,180);
    return path;

}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

QPainterPath Boule::getForm_Carotte(QRectF r, int origineX, int origineY) const{
    QPainterPath path;

    path=getForm_HautCarotte(r,origineX,origineY);
    path.moveTo((r.width()/2)+origineX, (r.height()/4)+origineY);
    path.cubicTo(r.width()+origineX,(r.height()/2)+origineY,(r.width()/2)+origineX,(r.height()/2)+origineY,(r.width()/2)+origineX,origineY+r.height());
    path.moveTo((r.width()/2)+origineX, (r.height()/4)+origineY);
    path.cubicTo(origineX,(r.height()/2)+origineY,(r.width()/2)+origineX,(r.height()/2)+origineY,(r.width()/2)+origineX,origineY+r.height());

    return path;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

QPainterPath Boule::getForm_Goutte(QRectF r, int origineX, int origineY) const{
    QPainterPath path;

    //partie droite
    path.moveTo((r.width()/2)+origineX, origineY+r.height());
    path.cubicTo((r.width()*0.9)+origineX,(r.height()*0.9)+origineY,r.width()+origineX,(2*r.height()/3)+origineY,(r.width()/2)+origineX,origineY);
    //partie gauche
    path.moveTo((r.width()/2)+origineX,origineY+r.height());
    path.cubicTo((r.width()*0.1)+origineX,(r.height()*0.9)+origineY,origineX,(2*r.height()/3)+origineY,(r.width()/2)+origineX,origineY);

    return path;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

QPainterPath Boule::getForm_Coeur(QRectF r, int origineX, int origineY) const{
    QPainterPath path;

    path.setFillRule(Qt::WindingFill);
    path.moveTo((r.width()/3)+origineX,origineY);
    path.lineTo((r.width()/2)+origineX, (r.height()/10)+origineY);
    path.lineTo((2*r.width()/3)+origineX,origineY);
    //bas du coeur
    path.cubicTo((r.width()*1.25)+origineX,origineY,r.width()+origineX,(4*r.height()/5)+origineY,(r.width()/2)+origineX,r.height()+origineY);//partie droite
    path.moveTo((r.width()/3)+origineX,origineY);
    path.cubicTo((-r.width()*0.25)+origineX,origineY,origineX,(4*r.height()/5)+origineY,(r.width()/2)+origineX,r.height()+origineY);//partie gauche

    return path;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
QPainterPath Boule::getForm_Etoile(QRectF r, int origineX, int origineY) const{
    QPainterPath path;

    path.setFillRule(Qt::WindingFill);
    //etoile
    path.moveTo((r.width()/2)+origineX, origineY);
    path.lineTo((r.width()/8)+origineX,r.height()+origineY);//bas droite
    path.lineTo(r.width()+origineX,(r.height()/3)+origineY);//droite
    path.lineTo(origineX,(r.height()/3)+origineY);//gauche
    path.lineTo((7*r.width()/8)+origineX,r.height()+origineY);//bas gauche
    path.lineTo((r.width()/2)+origineX, origineY);//retour a l'origine

    return path;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void Boule::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *){
    QRectF r = boundingRect();
    r.setWidth(r.width()-4);
    r.setHeight(r.height()-4);

    // bool down = option->state & QStyle::State_Sunken;

     creationDeLaSurbrianlleDeSelection(painter,option);//met en valeur si selectionner

     //preparation des couleur

    dessineFormColorier(painter,r,option);

    //dessin du combo

    dessineFormCombo(painter,r,option);

    //dessin des effect
    dessineEffectFrozen(painter,r,option);


}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Boule::dessineEffectFrozen(QPainter* painter, QRectF r, const QStyleOptionGraphicsItem *option){
    if(frozen){
        QLinearGradient glace=QLinearGradient(r.topLeft(),r.bottomRight());
        glace.setColorAt(0,QColor(100,120,255,100));
        glace.setColorAt(1,QColor(200,220,255,100));
        painter->setPen(Qt::NoPen);
        painter->setBrush(glace);
        painter->drawPath(getFormeBoule(r,2,2));
    }
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void Boule::dessineFormColorier(QPainter *painter, QRectF r, const QStyleOptionGraphicsItem *option){

       switch (getColorPredefini(color) ) {
         case 7:  //orange carotte
                dessine_Carotte(r,option,2,2,painter);

             break;
         default://on cree le coloriage par defautl

             painter->setBrush(getColoriageBoule(r,option));
             painter->setPen(color);

             //on mets les couleur et dessine
                  painter->drawPath(getFormeBoule(r,2,2));//on fait la forme predefini
             break;
       }

}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Boule::dessineFormCombo(QPainter *painter, QRectF r, const QStyleOptionGraphicsItem *option){
    switch (bouleDeCombo) {
    case TypeCombo_Horizontal:
            painter->setPen(Qt::NoPen);
            painter->setBrush(getColoriageBoule_Horizontal(r,option));
            painter->drawPath(getFormeBoule(r,2,2));
        break;
    case TypeCombo_Vertical:
            painter->setPen(Qt::NoPen);
            painter->setBrush(getColoriageBoule_Vertical(r,option));
            painter->drawPath(getFormeBoule(r,2,2));
        break;
    case TypeCombo_X:
            painter->setPen(Qt::NoPen);
            painter->setBrush(getColoriageBoule_Horizontal(r,option));
            painter->drawPath(getFormeBoule(r,2,2));
            painter->setBrush(getColoriageBoule_Vertical(r,option));
            painter->drawPath(getFormeBoule(r,2,2));
        break;
    default:
        break;
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
QLinearGradient Boule::getColoriageBoule(QRectF r, const QStyleOptionGraphicsItem *option)const{

    QLinearGradient grad(r.topLeft(), r.bottomRight()); //style diagonale
    bool down = option->state & QStyle::State_Sunken;

    grad.setColorAt(down ? 1 : 0, option->state & QStyle::State_MouseOver ? Qt::white : color.lighter());
    grad.setColorAt(down ? 0 : 1, color);

    return grad;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
QLinearGradient Boule::getColoriageBoule_Horizontal(QRectF r, const QStyleOptionGraphicsItem *option)const{

    QLinearGradient grad(r.topLeft(), r.bottomLeft()); //style verticale
    QColor transparent(0,0,0,0);

    for(double i=0.2; i<=1 ; i+=0.2){
         grad.setColorAt( i-0.2, Qt::white);
         grad.setColorAt( i-0.1, transparent);
         grad.setColorAt( i, Qt::white);
     }

    return grad;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
QLinearGradient Boule::getColoriageBoule_Vertical(QRectF r, const QStyleOptionGraphicsItem *option)const{

    QLinearGradient grad(r.topLeft(), r.topRight()); //style verticale
    QColor transparent(0,0,0,0);

    for(double i=0.2; i<=1 ; i+=0.2){
         grad.setColorAt( i-0.2, Qt::white);
         grad.setColorAt( i-0.1, transparent);
         grad.setColorAt( i, Qt::white);
     }

    return grad;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Boule::dessine_Carotte(QRectF r, const QStyleOptionGraphicsItem *option, int xdecal , int ydecal, QPainter *painter)const{

    QLinearGradient grad(r.topLeft(), r.bottomRight()); //style diagonale
    bool down = option->state & QStyle::State_Sunken;
    grad.setColorAt(down ? 1 : 0, option->state & QStyle::State_MouseOver ? Qt::white : color.lighter());
    grad.setColorAt(down ? 0 : 1, color);

    painter->setPen(color);
    painter->setBrush(grad);
    painter->drawPath(getForm_Carotte(r,xdecal,ydecal));//on fait la forme predefini

    painter->setPen(Qt::darkGreen);
    grad.setColorAt(down ? 1 : 0, option->state & QStyle::State_MouseOver ? Qt::white : Qt::green);
    grad.setColorAt(down ? 0 : 1, Qt::green);
    painter->setBrush(grad);

    painter->drawPath(getForm_HautCarotte(r,xdecal,ydecal));//on fait la forme predefini



}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void Boule::randomColor(int nbCouleur){
    bouleDeCombo=TypeCombo_Aucun;
    resistanceRemonter=FAIBLE;
    color=getColorPredefini(rand()%nbCouleur);
    update();
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

QColor Boule::getColor()const{
    return color;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

QColor Boule::getColorPredefini(int numCouleur){
    QColor couleur;
    switch (numCouleur) {
    case 0:couleur.setRgb(255,0,0);
        break;
    case 1:couleur.setRgb(0,255,0);
        break;
    case 2:couleur.setRgb(0,0,255);
        break;
    case 3: couleur.setRgb(105,105,105);
        break;
    case 4: couleur.setRgb(255,255,0);
        break;
    case 5: couleur.setRgb(255,0,255);
        break;
    case 6:couleur.setRgb(0,255,255);
        break;
    case 7:couleur.setRgb(255,125,0);
        break;
    default:couleur.setRgb(0,0,0);
        break;
    }
    return couleur;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int Boule::getColorPredefini(QColor couleur){
    for(int i=0 ; i<=7 ;i++){
        if(couleur == getColorPredefini(i)){
            return i;
        }
    }
    return -1;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void Boule::changeColor(int nbCouleur){
    randomColor(nbCouleur);
    update();
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void Boule::setColor(QColor couleur){
    bouleDeCombo=TypeCombo_Aucun;
    color=couleur;
    update();
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void Boule::setBouleDeCombo(TypeCombo type){
    if(estBouleCombo()){
        resistanceRemonter=EXPLOSIF;
        bouledeComboSuiv=type;
    }
    else{
        resistanceRemonter=RESISTANT;
        bouleDeCombo=type;
    }
    this->update();
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Boule::setFrozen(bool geler){
    frozen=geler;
    this->update();
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void Boule::bouleDeselectionner(){
    bouleSelectionner=false;
    this->update();
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void Boule::creationDeLaSurbrianlleDeSelection(QPainter *painter, const QStyleOptionGraphicsItem *option){
    QRectF r = boundingRect();
    QLinearGradient grad(r.topLeft(), r.bottomRight());
    bool down = option->state & QStyle::State_Sunken;

    if(bouleSelectionner){
        grad.setColorAt(down ? 0 : 1, Qt::black);
        painter->setPen(Qt::black);
        painter->setBrush(grad);

        painter->drawPath(getFormeBoule(boundingRect()));
    }
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Boule::enleverResistanceRemonter(){
    if(estExplosif()){
        bouleDeCombo=bouledeComboSuiv;
        update();
    }
    resistanceRemonter=FAIBLE;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Boule::TypeCombo Boule::definirTypeCombo(int nbAlignHori, int nbAlignVerti){
    TypeCombo type=TypeCombo_Aucun;
    if(nbAlignHori>=3 && nbAlignVerti>=3){
        type=TypeCombo_X;
    }
    else{if(nbAlignHori>3){
            type=TypeCombo_Horizontal;
        }
        else{
            if(nbAlignVerti>3){
                type=TypeCombo_Vertical;
            }
        }

    }
    return type;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Boule::TypeCombo Boule::getTypeCombo()const{
    return bouleDeCombo;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------VERIFICATION-------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

bool Boule::estBouleCombo()const{
    return bouleDeCombo!=TypeCombo_Aucun;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Boule::estBouleComboHorizontal()const{
    return bouleDeCombo==TypeCombo_Horizontal;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Boule::estBouleComboVertical()const{
    return bouleDeCombo==TypeCombo_Vertical;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Boule::estBouleComboX()const{
    return bouleDeCombo==TypeCombo_X;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

bool Boule::estMemeCouleur(Boule* autreBoule)const{
    return this->getColor() == autreBoule->getColor();
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Boule::estExplosif()const{
    return resistanceRemonter==EXPLOSIF;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Boule::estResitantRemonter()const{
    return resistanceRemonter!=FAIBLE;
}
