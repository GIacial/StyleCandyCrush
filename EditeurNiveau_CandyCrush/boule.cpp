#include "boule.h"

Boule::Boule(int nbCouleur, QGraphicsItem *parent):  QGraphicsWidget(parent)
{
    setAcceptHoverEvents(true);//permet d'activer la detection du passage de la souris sur item
    setCacheMode(DeviceCoordinateCache);//def la resolution de l'item
    bouleDeCombo=false;
    bouleSelectionner=false;
       randomColor(nbCouleur);//attribut une couleur pour color

}

Boule::Boule( QGraphicsItem *parent):  QGraphicsWidget(parent){
    setAcceptHoverEvents(true);//permet d'activer la detection du passage de la souris sur item
    setCacheMode(DeviceCoordinateCache);//def la resolution de l'item
    bouleDeCombo=false;
    bouleSelectionner=false;
    frozen=false;
    setColor(getColorPredefini(0));
}

void Boule:: mousePressEvent(QGraphicsSceneMouseEvent *){
    bouleSelectionner=true;
    update();  
    emit pressed();
}

void Boule::mouseReleaseEvent(QGraphicsSceneMouseEvent *){
    update();
}

QRectF Boule::boundingRect() const{
       return QRectF(0,0,50,50);
}

QPainterPath Boule::shape() const{
        return this->getFormeBoule(this->boundingRect());
}

QPainterPath Boule::getFormeBoule(QRectF r,int origineX,int origineY)const{
    QPainterPath path;
    path.moveTo(origineX,origineY);

    switch (getColorPredefini(color) ) {
    case 0:  //rouge
            path = getForm_Fraise(r,origineX,origineY);
            break;
    case 1: //vert
            path.addRoundedRect(r,15,15);
        break;
    case 2: //bleu
            path=getForm_Triangle(r,origineX,origineY);
        break;
    case 4: //jaune
            path.setFillRule(Qt::WindingFill);
            //etoile
            path.moveTo((r.width()/2)+origineX, origineY);
            path.lineTo((r.width()/8)+origineX,r.height()+origineY);//bas droite
            path.lineTo(r.width()+origineX,(r.height()/3)+origineY);//droite
            path.lineTo(origineX,(r.height()/3)+origineY);//gauche
            path.lineTo((7*r.width()/8)+origineX,r.height()+origineY);//bas gauche
            path.lineTo((r.width()/2)+origineX, origineY);//retour a l'origine
        break;
    case 5://rose coeur
        path.setFillRule(Qt::WindingFill);
        path.moveTo((r.width()/3)+origineX,origineY);
        path.lineTo((r.width()/2)+origineX, (r.height()/10)+origineY);
        path.lineTo((2*r.width()/3)+origineX,origineY);
        //bas du coeur
        path.cubicTo((r.width()*1.25)+origineX,origineY,r.width()+origineX,(4*r.height()/5)+origineY,(r.width()/2)+origineX,r.height()+origineY);//partie droite
        path.moveTo((r.width()/3)+origineX,origineY);
        path.cubicTo((-r.width()*0.25)+origineX,origineY,origineX,(4*r.height()/5)+origineY,(r.width()/2)+origineX,r.height()+origineY);//partie gauche
        break;
    case 6://cyan
        //goutte

        //partie droite
        path.moveTo((r.width()/2)+origineX, origineY+r.height());
        path.cubicTo((r.width()*0.9)+origineX,(r.height()*0.9)+origineY,r.width()+origineX,(2*r.height()/3)+origineY,(r.width()/2)+origineX,origineY);
        //partie gauche
        path.moveTo((r.width()/2)+origineX,origineY+r.height());
        path.cubicTo((r.width()*0.1)+origineX,(r.height()*0.9)+origineY,origineX,(2*r.height()/3)+origineY,(r.width()/2)+origineX,origineY);
        break;
    case 7://orange
        //carotte
        path.moveTo((r.width()/2)+origineX, (r.height()/4)+origineY);
        path.arcTo((r.width()/4)+origineX,origineY,(r.width()/2),(r.height()/4),0,180);
        path.moveTo((r.width()/2)+origineX, (r.height()/4)+origineY);
        path.cubicTo(r.width()+origineX,(r.height()/2)+origineY,(r.width()/2)+origineX,(r.height()/2)+origineY,(r.width()/2)+origineX,origineY+r.height());
        path.moveTo((r.width()/2)+origineX, (r.height()/4)+origineY);
        path.cubicTo(origineX,(r.height()/2)+origineY,(r.width()/2)+origineX,(r.height()/2)+origineY,(r.width()/2)+origineX,origineY+r.height());
        break;
    default:
         path.addEllipse(r);//defini la zone cliquable
        break;
    }
    return path;
}

QPainterPath Boule::getForm_Fraise(QRectF r, int origineX, int origineY)const{
    QPainterPath path;
    //fraise
    path.moveTo((r.width()/2)+origineX, origineY);
    path.cubicTo((r.width()*1.25)+origineX,origineY,origineX+r.width(),(4*r.height()/5)+origineY,(r.width()/2)+origineX,origineY+r.height());//partie droite
    path.moveTo((r.width()/2)+origineX,origineY);
    path.cubicTo((-r.width()*0.25)+origineX,origineY,origineX,(4*r.height()/5)+origineY,(r.width()/2)+origineX,origineY+r.height());//partie gauche
    return path;
}

QPainterPath Boule::getForm_Triangle(QRectF r, int origineX, int origineY) const{
    QPainterPath path;
    //triangle
    path.moveTo((r.width()/2)+origineX, origineY);
    path.lineTo(origineX,origineY+r.height());
    path.lineTo(r.width()+origineX,origineY+r.height());
    path.lineTo((r.width()/2)+origineX, origineY);
    return path;
}

void Boule::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *){
    QRectF r = boundingRect();
    QLinearGradient grad(r.topLeft(), r.bottomRight());
     bool down = option->state & QStyle::State_Sunken;

     creationDeLaSurbrianlleDeSelection(painter,option);//met en valeur si selectionner

     //on cree l'object
     if(frozen){

         grad.setColorAt(down ? 1 : 0, option->state & QStyle::State_MouseOver ? Qt::white : Qt::blue);
         grad.setColorAt(down ? 0 : 1, QColor(100,100,255));
     }
     else{
         grad.setColorAt(down ? 1 : 0, option->state & QStyle::State_MouseOver ? Qt::white : color.lighter());
         grad.setColorAt(down ? 0 : 1, color);
     }

     painter->setPen(color);
     painter->setBrush(grad);
     painter->drawPath(shape());//on fait la forme predefini

     //init des variable du swich

     QPainterPath boutDeCarotte;

     switch (getColorPredefini(color) ) {
     case 7:  //orange carotte
            painter->setPen(Qt::darkGreen);
            grad.setColorAt(down ? 1 : 0, option->state & QStyle::State_MouseOver ? Qt::white : Qt::green);
            grad.setColorAt(down ? 0 : 1, Qt::green);
            painter->setBrush(grad);
            boutDeCarotte.moveTo((r.width()/2), (r.height()/4));
            boutDeCarotte.arcTo((r.width()/4),0,(r.width()/2),(r.height()/4),0,180);
            painter->drawPath(boutDeCarotte);//on fait la forme predefini

         break;
     default:break;
     }

     //logo du centre quand combo
     if(estBouleCombo()){
         grad.setColorAt(down ? 1 : 0, option->state & QStyle::State_MouseOver ? Qt::white : color.lighter());
         grad.setColorAt(down ? 0 : 1, color);

         painter->setPen(color.darker());
         painter->setBrush(grad);
         painter->drawPath(getFormeBoule(QRectF(r.width()/4,r.height()/4,r.width()/2,r.height()/2),r.width()/4,r.height()/4));
     }
}

void Boule::randomColor(int nbCouleur){
    bouleDeCombo=false;
    color=getColorPredefini(rand()%nbCouleur);
    update();
}

QColor Boule::getColor()const{
    return color;
}

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

int Boule::getColorPredefini(QColor couleur){
    for(int i=0 ; i<=7 ;i++){
        if(couleur == getColorPredefini(i)){
            return i;
        }
    }
    return -1;
}

void Boule::changeColor(int nbCouleur){
    randomColor(nbCouleur);
    update();
}

void Boule::setColor(QColor couleur){
    bouleDeCombo=false;
    color=couleur;
    update();
}

void Boule::setBouleDeCombo(){
    bouleDeCombo=true;
    this->update();
}

void Boule::setFrozen(bool gele){
    frozen=gele;
}

bool Boule::estFrozen()const{
    return frozen;
}

void Boule::bouleDeselectionner(){
    bouleSelectionner=false;
    this->update();
}

void Boule::creationDeLaSurbrianlleDeSelection(QPainter *painter, const QStyleOptionGraphicsItem *option){
    QRectF r = boundingRect();
    QLinearGradient grad(r.topLeft(), r.bottomRight());
    bool down = option->state & QStyle::State_Sunken;

    if(bouleSelectionner){
        grad.setColorAt(down ? 0 : 1, Qt::white);
        painter->setPen(Qt::black);
        painter->setBrush(grad);

        painter->drawRoundedRect(r,2,2);
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------VERIFICATION-------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

bool Boule::estBouleCombo()const{
    return bouleDeCombo;
}

bool Boule::estMemeCouleur(Boule* autreBoule)const{
    return this->getColor() == autreBoule->getColor();
}
