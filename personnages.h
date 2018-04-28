#ifndef PERSONNAGES_H
#define PERSONNAGES_H

#include <QObject>
#include <QList>
#include <QPointF>

#include "autre.h"
#include "action.h"

class Perso : public QObject
{

    Q_OBJECT

    public:

        Perso(QString nom, int x, int y);
        ~Perso();

        QString getNom() {return this->nom;}
        int getPosX() {return this->posX;}
        int getPosY() {return this->posY;}
        void setPos(int x, int y) {this->posX = x; this->posY = y;}

    private:

    protected:
        QString nom;
        int posX;
        int posY;


};

class PersoNormaux : public Perso
{

    Q_OBJECT

    public:

        PersoNormaux(QString nom, int x, int y, int pv=0, int vitesse=0);
        ~PersoNormaux();

        int getPv() {return this->pv;}
        int getVitesse() {return this->vitesse;}
        void setImagePerso(Image *image) {this->imagePerso = image;}
        Image *getImagePerso() {return this->imagePerso;}
        void setPos(int x, int y);
        void setPosX(int x) {this->posX = x;}
        void setPosY(int y) {this->posY = y;}
        void moveTo(int x, int y);
        bool hasAction() {return !listActions->empty();}
        Action *getFirstAction() {return listActions->first();}
        void removeFirstAction() {this->listActions->removeFirst();}
        void removeAllActions();
        QPointF *getDestination();

    public slots:
        void checkAction();
        //void moveTo(int x, int y);

    protected:

        int pv;
        int vitesse;
        Image *imagePerso;
        int deplacementX;
        int deplacementY;
        int distancePositionClicked;
        QList<Action*> *listActions = new QList<Action*>;

};

class PersoGentil : public PersoNormaux
{

    Q_OBJECT

    public:

        PersoGentil(QString nom, int x, int y, int pv=0, int vitesse=0);
        ~PersoGentil();
        //void moveTo(int x, int y);
        bool isFree() {return this->listActions->empty();}
        void setDistancePositionClicked(int a) {this->distancePositionClicked = a;}
        int getDistancePositionClicked() {return this->distancePositionClicked;}

    private:

        int deplacementX;
        int deplacementY;
        int distancePositionClicked;

};

class PersoMechant : public PersoNormaux
{

    Q_OBJECT

    public:

        PersoMechant(QString nom, int x, int y, int pv=0, int vitesse=0);
        ~PersoMechant();
        //void moveTo(int x, int y);
        //bool isFree() {return this->listActions->empty();}
        //void setDistancePositionClicked(int a) {this->distancePositionClicked = a;}
        //int getDistancePositionClicked() {return this->distancePositionClicked;}

    private:

        int deplacementX;
        int deplacementY;
        //int distancePositionClicked;

};


#endif // PERSONNAGES_H
