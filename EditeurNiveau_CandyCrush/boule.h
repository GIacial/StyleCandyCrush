#ifndef BOULE_H
#define BOULE_H

#include <QGraphicsWidget>
#include <QObject>
#include <QColor>
#include <QPixmap>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <math.h>
//enum test {teg,tui}; eample de type enum
class Boule: public QGraphicsWidget
{
    Q_OBJECT
public:
    Boule( int nbCouleur, QGraphicsItem *parent = NULL);
    Boule( QGraphicsItem *parent=NULL);
    QRectF boundingRect() const;//sert a determiner la taille de l'object
    QPainterPath shape() const;//determine la zone cliquable
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *);//style graphics de item
    void setBouleDeCombo();//transformer la boule en boule de combo
    QColor getColor()const;//renvoi la couleur de la boule
    void changeColor(int nbCouleur);//change la couleur de la boule
    void setColor(QColor couleur);//permet de mettre une couleur a la boule
    void bouleDeselectionner();//permet de deseclectionner la boule

    static QColor getColorPredefini(int numCouleur);//renvoi une couleur
    static int getColorPredefini(QColor couleur);//renvoi le int qui corespond a la couleur
    void setFrozen(bool gele);//permet de geler ou pas la boule


    //verification

    bool estBouleCombo()const;//permet de savoir si la boule est une boule de combo
    bool estMemeCouleur(Boule* autreBoule)const;//permet de savoir si ce sont les meme couleur
    bool estFrozen()const;


signals:
    void pressed();//emit quand on clique

public slots:


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *);//appeller quand on clique desus
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *);//appeler quand on relache le click
    void randomColor(int nbCouleur);//attribut une couleur aleatoire
    void creationDeLaSurbrianlleDeSelection(QPainter *painter, const QStyleOptionGraphicsItem *option);

    //dessin
    QPainterPath getFormeBoule(QRectF r, int origineX=0, int origineY=0)const;
    QPainterPath getForm_Fraise(QRectF r, int origineX=0, int origineY=0)const;
    QPainterPath getForm_Triangle(QRectF r, int origineX=0, int origineY=0)const;

private:
    QColor color;//couleurs de la boule
    bool bouleDeCombo;//permet de savoir si la boule est nee d'un combo
    bool bouleSelectionner;//permet de savoir si on a choisi la boule
    bool frozen ;//permet de savoir si elle est freze ou pas
};

#endif // BOULE_H
