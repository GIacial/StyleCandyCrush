#ifndef BOULE_H
#define BOULE_H

#include <QGraphicsWidget>
#include <QObject>
#include <QColor>
#include <QPixmap>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <math.h>


//type

class Boule: public QGraphicsWidget
{

    Q_OBJECT
private:
    //type
    enum ResistanceALaRemonter{RESISTANT,EXPLOSIF,FAIBLE};

public:
    enum TypeCombo{TypeCombo_Aucun,TypeCombo_Horizontal,TypeCombo_Vertical,TypeCombo_X};

    //constructeur
    Boule( int nbCouleur, QGraphicsItem *parent = NULL);
    Boule( QGraphicsItem *parent=NULL);

    QRectF boundingRect() const;                                //sert a determiner la taille de l'object
    QPainterPath shape() const;                                 //determine la zone cliquable
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *);//style graphics de item
    void setBouleDeCombo(Boule::TypeCombo type);                       //transformer la boule en boule de combo
    QColor getColor()const;                                     //renvoi la couleur de la boule
    void changeColor(int nbCouleur);                            //change la couleur de la boule
    void setColor(QColor couleur);                              //permet de mettre une couleur a la boule
    void enleverResistanceRemonter();                           //enleve la resistance a la remonter
    Boule::TypeCombo getTypeCombo()const;                              //permet de savoir si le type de combo
    void setFrozen(bool geler);                                 //gele la boule

    //verification

    bool estBouleCombo()const;                  //permet de savoir si la boule est une boule de combo
    bool estBouleComboHorizontal()const;        //permet de savoir si son effect est horizontal
    bool estBouleComboVertical()const;          //effect horizontal
    bool estBouleComboX()const;                 //permet de savoir si elle est horizontal et verticale
    bool estMemeCouleur(Boule* autreBoule)const;//permet de savoir si ce sont les meme couleur
    bool estResitantRemonter()const;            //permet e savoir si la boule est resistant a la remonter
    bool estExplosif()const;                    //permet de savoir si elle resiste tout en explosant

    //les static

    static QColor getColorPredefini(int numCouleur);//renvoi une couleur
    static int getColorPredefini(QColor couleur);//renvoi le int qui corespond a la couleur
    static TypeCombo definirTypeCombo(int nbAlignHori,int nbAlignVerti);//donne le type combo correspondant au nb de boule aligne


signals:
    void pressed();//emit quand on clique

public slots:

    void bouleDeselectionner();//permet de deseclectionner la boule

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *);//appeller quand on clique desus
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *);//appeler quand on relache le click
    void randomColor(int nbCouleur);//attribut une couleur aleatoire
    void creationDeLaSurbrianlleDeSelection(QPainter *painter, const QStyleOptionGraphicsItem *option);

    //dessin
    QPainterPath getFormeBoule(QRectF r, int origineX=0, int origineY=0)const;
    QPainterPath getForm_Fraise(QRectF r, int origineX=0, int origineY=0)const;
    QPainterPath getForm_Triangle(QRectF r, int origineX=0, int origineY=0)const;
    QPainterPath getForm_HautCarotte(QRectF r, int origineX=0, int origineY=0)const;
    QPainterPath getForm_Carotte(QRectF r, int origineX=0, int origineY=0)const;
    QPainterPath getForm_Goutte(QRectF r, int origineX=0, int origineY=0)const;
    QPainterPath getForm_Coeur(QRectF r, int origineX=0, int origineY=0)const;
    QPainterPath getForm_Etoile(QRectF r, int origineX=0, int origineY=0)const;

    //coloriage
    void dessineFormColorier(QPainter* painter, QRectF r, const QStyleOptionGraphicsItem *option);
    void dessineFormCombo(QPainter* painter, QRectF r, const QStyleOptionGraphicsItem *option);
    void dessineEffectFrozen(QPainter *painter, QRectF r, const QStyleOptionGraphicsItem *option);
    QLinearGradient getColoriageBoule(QRectF r, const QStyleOptionGraphicsItem *option)const;
    QLinearGradient getColoriageBoule_Horizontal(QRectF r, const QStyleOptionGraphicsItem *option)const;
    QLinearGradient getColoriageBoule_Vertical(QRectF r, const QStyleOptionGraphicsItem *option)const;
    void dessine_Carotte(QRectF r,const QStyleOptionGraphicsItem *option,int xdecal,int ydecal,QPainter* painter)const;

private:

    QColor color;//couleurs de la boule
    TypeCombo bouleDeCombo;//permet de savoir si la boule est nee d'un combo
    TypeCombo bouledeComboSuiv;//permet de savoir le prochain effect
    bool bouleSelectionner;//permet de savoir si on a choisi la boule
    bool frozen;

    ResistanceALaRemonter resistanceRemonter;
};

#endif // BOULE_H
