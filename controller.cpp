#include "view.h"
#include "controller.h"
#include "model.h"
#include "autre.h"
#include "personnages.h"
#include "ressources.h"
#include "decor.h"

#include <QMessageBox>
#include <QGraphicsScene>
#include <QPixmap>
#include <QTimer>
#include <QFileDialog>
#include <QGraphicsView>
#include <QDateTime>
#include <QCoreApplication>
#include <QPointF>
#include <QtDebug>
#include <math.h>
#include <time.h>
#include <QDateTime>

Controller::Controller(Model *model, View *view): QObject(0) {
    this->model = model;
    this->view = view;
    this->view->setControl(this);
    chrono = new QTimer;
    this->timer = new QTimer;
    this->timerWheat = new QTimer;
    this->timerSpawnMadPerso = new QTimer;
    //Q_ASSERT(timer == NULL);
}

void Controller::startNewGame() {
    //Suppression de la sauvegarde précédente
    QFile file("sauvegarde.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::about(view, "Erreur", "Impossible de sauvegarder");
    }
    else {
        QTextStream flux(&file);
        flux << "";
    }
    file.close();

    QString listSetting = "";
    listSetting += "F7 100 50 F1 750 150 F2 1400 100 F3 2000 100 F5 2600 50 "; //1ère ligne
    listSetting += "F6 350 550 F6 1950 400 F4 2600 600 "; //2ème ligne
    listSetting += "F7 100 1100 F4 1600 1300 F2 2100 950 F3 2500 1300 "; //3ème ligne
    listSetting += "F4 200 1600 F5 1000 1400 F6 2300 1600 "; //dernière ligne
    listSetting += "V 1000 800 ";

    QString persoG1 = "1725 1000 1 50 100 10 ", persoG2 = "1775 950 1 60 100 10 ", persoG3 = "1850 975 1 100 100 10 ",
            persoG4 = "1780 1050 1 88 100 10 ", persoG5 = "1845 1040 1 100 100 10 ";
    QString persoG6 = "1100 550 1 100 100 10 ", persoG7 = "1200 600 1 100 100 10 ", persoG8 = "1150 500 1 100 100 10 ",
            persoG9 = "1300 550 1 100 100 10 ", persoG10 = "1250 500 1 100 100 10 ";
    QString persoG11 = "800 1050 1 100 100 10 ", persoG12 = "875 1075 1 100 100 10 ", persoG13 = "750 1125 1 100 100 10 ",
            persoG14 = "825 1150 1 100 100 10 ", persoG15 = "925 1125 1 100 100 10 ";
    QString listNicePerso = persoG1 + persoG2 + persoG3 + persoG4 + persoG5 + persoG6 + persoG7 + persoG8 +
            persoG9 + persoG10;
    listNicePerso += persoG11 + persoG12 + persoG13 + persoG14 + persoG15;

    QString persoM1 = "300 50 10 150 10 ", persoM2 = "650 300 10 150 10 ", persoM3 = "950 250 10 150 10 ",
            persoM4 = "1450 250 10 150 10 ", persoM5 = "2150 25 10 150 10 ";
    QString persoM6 = "2140 250 10 150 10 ", persoM7 = "2500 280 10 150 10 ", persoM8 = "2850 300 10 150 10 ",
            persoM9 = "300 1100 10 150 10 ", persoM10 = "625 1300 10 150 10 ";
    QString persoM11 = "300 1600 10 150 10 ", persoM12 = "225 1900 10 150 10 ", persoM13 = "900 1625 10 150 10 ",
            persoM14 = "1500 1650 10 150 10 ", persoM15 = "1700 1300 10 150 10 ";
    QString persoM16 = "2700 625 10 150 10 ", persoM17 = "2650 875 10 150 10 ", persoM18 = "2175 1150 10 150 10 ",
            persoM19 = "2650 1200 10 150 10 ", persoM20 = "2650 1475 10 150 10 ";
    QString listMadPersoFixed = persoM1 + persoM2 + persoM3 + persoM4 + persoM5 + persoM6 + persoM7 + persoM8 + persoM9 + persoM10;
    listMadPersoFixed += persoM11 + persoM12 + persoM13 + persoM14 + persoM15 + persoM16 + persoM17 + persoM18 + persoM19 + persoM20;

    QString listMadPersoMobile;

    this->model->initFarmer();
    this->model->initBaker();
    this->model->initHefty();
    this->model->initScore(0);

    startGame(listSetting, listNicePerso, listMadPersoFixed, listMadPersoMobile, "");

}

void Controller::startGame(QString setting, QString nicePerso, QString madPersoFixed, QString madPersoMobile, QString resourceItem) {

    view->installScene();

    srand(time(NULL));

    QStringList listSetting = setting.split(" ");
    for (int i=0; i<listSetting.size()-1; i+=3) {
        if (listSetting[i] == "V") view->addVillage(this->model->setVillage(listSetting[i+1].toInt(), listSetting[i+2].toInt(), view));
        else if(listSetting[i] == "F1") view->addForest1(this->model->setForest1(listSetting[i+1].toInt(), listSetting[i+2].toInt(), view));
        else if(listSetting[i] == "F2") view->addForest2(this->model->setForest2(listSetting[i+1].toInt(), listSetting[i+2].toInt(), view));
        else if(listSetting[i] == "F3") view->addForest3(this->model->setForest3(listSetting[i+1].toInt(), listSetting[i+2].toInt(), view));
        else if(listSetting[i] == "F4") view->addForest4(this->model->setForest4(listSetting[i+1].toInt(), listSetting[i+2].toInt(), view));
        else if(listSetting[i] == "F5") view->addForest5(this->model->setForest5(listSetting[i+1].toInt(), listSetting[i+2].toInt(), view));
        else if(listSetting[i] == "F6") view->addForest6(this->model->setForest6(listSetting[i+1].toInt(), listSetting[i+2].toInt(), view));
        else if(listSetting[i] == "F7") view->addForest7(this->model->setForest7(listSetting[i+1].toInt(), listSetting[i+2].toInt(), view));
    }

    if (nicePerso != "") {
        QStringList listNicePerso = nicePerso.split(" ");
        for (int i=0; i<listNicePerso.size()-1; i+=6) {
            NicePerso *S = new NicePerso(listNicePerso[i].toInt(), listNicePerso[i+1].toInt(), listNicePerso[i+2].toInt(),
                    listNicePerso[i+3].toInt(), listNicePerso[i+4].toInt(), listNicePerso[i+5].toInt());
            model->addNicePerso(S);
            view->addNormalPerso(S);
        }
    }

    if (madPersoFixed != "") {
        QStringList listMadPersoFixed = madPersoFixed.split(" ");
        for (int i=0; i<listMadPersoFixed.size()-1; i+=5) {
            MadPerso *S = new MadPerso(listMadPersoFixed[i].toInt(), listMadPersoFixed[i+1].toInt(),
                    listMadPersoFixed[i+2].toInt(), listMadPersoFixed[i+3].toInt(), listMadPersoFixed[i+4].toInt());
            model->addMadPersoFixed(S);
            view->addNormalPerso(S);
        }
    }

    if (madPersoMobile != "") {
        QStringList listMadPersoMobile = madPersoMobile.split(" ");
        for (int i=0; i<listMadPersoMobile.size()-1; i+=5) {
            MadPerso *S = new MadPerso(listMadPersoMobile[i].toInt(), listMadPersoMobile[i+1].toInt(),
                    listMadPersoMobile[i+2].toInt(), listMadPersoMobile[i+3].toInt(), listMadPersoMobile[i+4].toInt());
            model->addMadPersoFixed(S);
            view->addNormalPerso(S);
        }
    }

    this->displayResourceItem(resourceItem);

    /*this->model->addResourceItem(new ResourceItem("Wheat", 10, new Image(":/images/ble"),
                                                    0, 0));
    this->view->scene->addItem(this->model->getlistResourceItem()->last()->getImageItem());
    connect(this->model->getlistResourceItem()->first()->getImageItem(), SIGNAL(clicked()), this, SLOT(onResourceItemClicked()));*/

    /*QPointF *point = new QPointF(0,0);
    Image *imageTest = new Image(":/images/ble");
    imageTest->setPos(*point);
    this->view->scene->addItem(imageTest);
    */

    this->view->displayFrontMessage();

    this->model->setTimeStart(timeStart());
    connect(timer, SIGNAL(timeout()), this, SLOT(gameLoop()));
    this->timer->start(16);
    connect(timerWheat, SIGNAL(timeout()), this->model->getWheatField(), SLOT(ete()));
    this->timerWheat->start(1000);
    connect(timerSpawnMadPerso, SIGNAL(timeout()), this, SLOT(checkMadPerso()));
    this->timerSpawnMadPerso->start(10000);

    this->model->getBagSarsaparillas()->addResource(1000);

    MadPerso *perso = new MadPerso(0, 0, 1, 20, 1);
    this->model->addMadPersoMobile(perso);
    this->view->scene->addItem(perso->getImagePerso());
    perso->moveTo(perso->getPosX()+21, perso->getPosY()+21);
    qDebug() << "move to [" << QString::number(perso->getPosX()+1) << "," << QString::number(perso->getPosY()+1) << "]";
}

void Controller::save() {
    QFile file("save.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::about(view, tr("Erreur"), tr("Impossible de sauvegarder"));
    }
    else {
        QTextStream flux(&file);
        flux << tr("Date de la derniere sauvegarde : ") << QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss") << endl;

        for (Setting *D : *this->model->getlistSetting()) {
            flux << D->getType() << " " << D->getPosX() << " " << D->getPosY() << " ";
        }
        flux << endl;
        for (NicePerso *PG : *this->model->getlistNicePerso()) {
            flux << PG->getPosX() << " " << PG->getPosY() << " " << PG->getDamage() << " " << PG->getHp() << " " << PG->getHpMax() << " " << PG->getSpeed() << " ";
        }
        flux << endl;
        for (MadPerso *PMF : *this->model->getlistMadPersoFixed()) {
            flux << PMF->getPosX() << " " << PMF->getPosY() << " " << PMF->getDamage() << " " << PMF->getHp() << " " << PMF->getSpeed() << " ";
        }
        flux << endl;
        for (MadPerso *PMM : *this->model->getlistMadPersoMobile()) {
            qDebug() << "test ";
            flux << PMM->getPosX() << " " << PMM->getPosY() << " " << PMM->getDamage() << " " << PMM->getHp() << " " << PMM->getSpeed() << " ";
        }
        flux << endl;
        for (ResourceItem *R : *this->model->getlistResourceItem()) {
            flux << R->getName() << " " << R->getNumber() << " " << R->getPosX() << " " << R->getPosY() << " ";
        }

        flux << endl << gameDuration();
        flux << endl << getScore();
        flux << endl << getNumberSarsaparilla();
        flux << endl << getNumberAcorn();
        flux << endl << getNumberBerry();
        flux << endl << getNumberWheat();
        flux << endl << getNumberBread();

        flux << endl << getLevelFarmer();
        flux << endl << getLevelBaker();
        flux << endl << getLevelHefty();
    }
    file.close();
}

void Controller::loadSave() {

    QString setting, nicePerso, madPersoFixed, madPersoMobile, ressouceItem;
    QString time, score, sarsaparillas, berries, acorn, wheat, bread;
    QString farmer, baker, hefty;
    QFile file("save.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::about(view, tr("Erreur"), tr("Impossible de lire le fichier de sauvegarde"));
    }
    else {
        QTextStream flux(&file);
        QString date = flux.readLine();
        QMessageBox::information(view, tr("Date de sauvegarde"), date);

        setting = flux.readLine();
        nicePerso = flux.readLine();
        madPersoFixed = flux.readLine();
        madPersoMobile = flux.readLine();
        ressouceItem = flux.readLine();

        time = flux.readLine();
        score = flux.readLine();
        sarsaparillas = flux.readLine();
        acorn = flux.readLine();
        berries = flux.readLine();
        wheat = flux.readLine();
        bread = flux.readLine();

        farmer = flux.readLine();
        baker = flux.readLine();
        hefty = flux.readLine();

    }
    file.close();

    // Remplir les arguments du paysan avec des valeurs sauvegardées
    this->model->initFarmer();
    this->model->farmerUp(farmer.toInt());
    this->model->initBaker();
    this->model->bakerUp(baker.toInt());
    this->model->initHefty();
    this->model->heftyUp(hefty.toInt());
    this->model->initScore(score.toInt());

    this->model->setTimePreviouslyPlayed(time.toInt());
    this->model->getBagSarsaparillas()->addResource(sarsaparillas.toInt());
    this->model->getBagAcorns()->addResource(acorn.toInt());
    this->model->getBagBerries()->addResource(berries.toInt());
    this->model->getBagWheats()->addResource(wheat.toInt());
    this->model->getBagBreads()->addResource(bread.toInt());

    startGame(setting, nicePerso, madPersoFixed, madPersoMobile, ressouceItem);

    /*this->timer->start(16);
    this->timerWheat->start(1000);*/
}

void Controller::saveQuit() {
    save();
    QCoreApplication::quit();
}

int Controller::gameDuration() {

    QDateTime timeNow = QDateTime::currentDateTime();
    QDateTime timeStart = getTimeStart();
    qint64 millisecondsDiff = timeStart.msecsTo(timeNow);
    int chrono = millisecondsDiff/1000 + (this->model->getTimePreviouslyPlayed());

    return chrono;
}

QDateTime Controller::timeStart() {return QDateTime::currentDateTime();}

void Controller::displayResourceItem(QString resourceItem) {
    if (resourceItem == "") {
        for (Setting * d : *this->model->getlistSetting()) {
            Forest *forest = dynamic_cast<Forest *>(d);
            if (forest) {
                this->model->addResourceItem(new ResourceItem("Wheat", ((rand() % 6) + 1), new Image(":/images/ble"),
                                                                forest->getPosXWheat(), forest->getPosYWheat()));
                this->view->scene->addItem(this->model->getlistResourceItem()->last()->getImageItem());
                this->model->addResourceItem(new ResourceItem("Acorn", ((rand() % 5) + 1), new Image(":/images/noisette"),
                                                                forest->getPosXAcorn(), forest->getPosYAcorn()));
                this->view->scene->addItem(this->model->getlistResourceItem()->last()->getImageItem());
                this->model->addResourceItem(new ResourceItem("Baie", ((rand() % 5) + 1), new Image(":/images/baie"),
                                                                forest->getPosXBerries(), forest->getPosYBerries()));
                this->view->scene->addItem(this->model->getlistResourceItem()->last()->getImageItem());
             }
        }

        for (QPointF * point : *this->model->getlistSpotResourceItem()) {
            int i = ((rand() % 10) + 1);

            if (i > 9) {
                this->model->addResourceItem(new ResourceItem("Sarsaparilla", ((rand() % 3) + 1), new Image(":/images/salsepareille"),
                                                                point->x(), point->y()));
            }
            else if (i > 6) {
                this->model->addResourceItem(new ResourceItem("Wheat", ((rand() % 3) + 1), new Image(":/images/ble"),
                                                                point->x(), point->y()));
            }
            else if (i > 3) {
                this->model->addResourceItem(new ResourceItem("Acorn", ((rand() % 3) + 1), new Image(":/images/noisette"),
                                                                point->x(), point->y()));
            }
            else if (i > 0) {
                this->model->addResourceItem(new ResourceItem("Berry", ((rand() % 3) + 1), new Image(":/images/baie"),
                                                                point->x(), point->y()));
            }
            this->view->scene->addItem(this->model->getlistResourceItem()->last()->getImageItem());
        }
    }
    else {
        QStringList listRessouceItem = resourceItem.split(" ");
        for (int i=0; i<listRessouceItem.size()-1; i+=4) {
            if (listRessouceItem[i] == "Sarsaparilla") {
                this->model->addResourceItem(new ResourceItem(listRessouceItem[i], listRessouceItem[i+1].toInt(), new Image(":/images/salsepareille"),
                                                                listRessouceItem[i+2].toInt(), listRessouceItem[i+3].toInt()));
            }
            else if (listRessouceItem[i] == "Wheat") {
                this->model->addResourceItem(new ResourceItem(listRessouceItem[i], listRessouceItem[i+1].toInt(), new Image(":/images/ble"),
                                                                listRessouceItem[i+2].toInt(), listRessouceItem[i+3].toInt()));
            }
            else if (listRessouceItem[i] == "Acorn") {
                this->model->addResourceItem(new ResourceItem(listRessouceItem[i], listRessouceItem[i+1].toInt(), new Image(":/images/noisette"),
                                                                listRessouceItem[i+2].toInt(), listRessouceItem[i+3].toInt()));
            }
            else if (listRessouceItem[i] == "Berry") {
                this->model->addResourceItem(new ResourceItem(listRessouceItem[i], listRessouceItem[i+1].toInt(), new Image(":/images/baie"),
                                                                listRessouceItem[i+2].toInt(), listRessouceItem[i+3].toInt()));
            }
            this->view->scene->addItem(this->model->getlistResourceItem()->last()->getImageItem());
        }
    }
}

void Controller::gameLoop() {

    // NicePerso
    for (NicePerso *S : *this->model->getlistNicePerso()) {

        if (S->hasAction()) {
            ActionMove* nextActionMove = dynamic_cast<ActionMove*>(S->getFirstAction());
            ActionFight* nextActionFight = dynamic_cast<ActionFight*>(S->getFirstAction());

            if (nextActionMove) {
                // On regarde s'il avance il ne tape pas contre un decor
                if (this->view->scene->itemAt(QPointF(S->getPosX() + nextActionMove->getDepX(), S->getPosY() +
                                                      nextActionMove->getDepY()), QTransform()) == NULL)
                    S->setPos(S->getPosX() + nextActionMove->getDepX(), S->getPosY() + nextActionMove->getDepY());
                S->removeFirstAction();

                QList<QGraphicsItem *> listCollision = S->getImagePerso()->collidingItems();

                for (QGraphicsItem * i : listCollision) {

                    ImageSetting *itemSetting = dynamic_cast<ImageSetting *>(i);

                    if (itemSetting) {

                        QPointF *pointDestination = S->getDestination();
                        S->removeAllActions();

                        QPointF *pointSuivant = new QPointF(S->getPosX(), S->getPosY());

                        //qDebug() << "point SUivant 1 [" << pointSuivant->x() << ";" << pointSuivant->y() << "]";

                        //S->setPos(S->getPosX() - nextAction->getDepX(), S->getPosY() - nextAction->getDepY());

                        QPointF *pointOrigine = new QPointF(S->getPosX()-nextActionMove->getDepX(),
                                                            S->getPosY()-nextActionMove->getDepY());

                        //qDebug() << "point Origine [" << pointOrigine->x() << ";" << pointOrigine->y() << "]";

                        pointSuivant = this->getPointDecale(pointOrigine, pointSuivant);

                        //qDebug() << "point SUivant 2 [" << pointSuivant->x() << ";" << pointSuivant->y() << "]";

                        //delete nextAction;

                        S->setPos(pointSuivant->x(), pointSuivant->y());
                        S->moveTo(pointDestination->x(), pointDestination->y());

                    }

                }
            }
            else if (nextActionFight) {

                if (nextActionFight->getDefender()->getHurt(nextActionFight->getFighter()->getDamage())) {
                    //qDebug() << "Prend un coup " << QString::number(nextActionFight->getDefender()->getHp());

                    MadPerso* perso = dynamic_cast<MadPerso*>(nextActionFight->getDefender());
                    //this->view->scene->removeItem(nextActionFight->getDefender()->getImagePerso()->parentItem());
                    if (perso) {
                        this->model->getlistMadPersoFixed()->removeAll(perso);
                        /*QGraphicsItem *itemPerso = this->view->scene->itemAt(QPointF(perso->getPosX()+20, perso->getPosY()+20),
                                                                             QTransform());*/
                        this->view->scene->removeItem(this->view->scene->itemAt(QPointF(perso->getPosX()+20, perso->getPosY()+20),
                                                                                QTransform()));

                        this->model->addPoints(100);
                    }
                    //nextActionFight->getDefender()->deleteLater();
                }


                S->removeFirstAction();

            }

            // Regarder s'il n'y a pas collision

            bool premier = true;

            for (MadPerso *M : *this->model->getlistMadPersoFixed()) {

                if (distanceBetweenPoints(new QPointF(S->getPosX(), S->getPosY()), new QPointF(M->getPosX(), M->getPosY())) < 100) {

                    // Sauvegarder les actions si c'est un déplacement
                    //S->removeAllActions();

                    if (premier) {
                        ActionFight *action = new ActionFight(S, M);
                        S->addActionInFirst(action);

                        /*Image *imageBang = new Image(":/images/bang");
                        imageBang->setPos(S->getPosX(), S->getPosY());
                        this->view->scene->addItem(imageBang);
                        this->model->getlistBang()->append(new Image(":/images/bang"));
                        QTimer::singleShot(16, this, SLOT(deleteFirstBang()));*/

                        premier = false;
                    }

                }

            }

            for (ResourceItem *I : *this->model->getlistResourceItem()) {

                if (distanceBetweenPoints(new QPointF(S->getPosX(), S->getPosY()), new QPointF(I->getPosX(), I->getPosY())) < 100) {

                    this->view->scene->removeItem(this->view->scene->itemAt(QPointF(I->getPosX()+20, I->getPosY()+20),
                                                                            QTransform()));
                    if (I->getName() == "Wheat") {
                        this->model->getBagWheats()->addResource(I->getNumber());
                    }
                    else if (I->getName() == "Berry") {
                        this->model->getBagBerries()->addResource(I->getNumber());
                    }
                    else if (I->getName() == "Bread") {
                        this->model->getBagBreads()->addResource(I->getNumber());
                    }
                    else if (I->getName() == "Acorn") {
                        this->model->getBagAcorns()->addResource(I->getNumber());
                    }
                    else if (I->getName() == "Sarsaparilla") {
                        this->model->getBagSarsaparillas()->addResource(I->getNumber());
                    }

                    this->model->addPoints(2);
                    this->model->getlistResourceItem()->removeAll(I);
                }

            }

            premier = true;

            // Si oui retour en arrière et décallage
            // Puis on regarde les ennemis
        }
    }

    // MadPerso Immobile
    for (MadPerso *S : *this->model->getlistMadPersoFixed()) {

        if (S->hasAction()) {
            ActionFight* nextActionFight = dynamic_cast<ActionFight*>(S->getFirstAction());

            if (nextActionFight) {

                if (nextActionFight->getDefender()->getHurt(nextActionFight->getFighter()->getDamage())) {
                    //qDebug() << "Prend un coup " << QString::number(nextActionFight->getDefender()->getHp());

                    NicePerso* perso = dynamic_cast<NicePerso*>(nextActionFight->getDefender());
                    //this->view->scene->removeItem(nextActionFight->getDefender()->getImagePerso()->parentItem());
                    if (perso) {
                        this->model->getlistNicePerso()->removeAll(perso);
                        /*QGraphicsItem *itemPerso = this->view->scene->itemAt(QPointF(perso->getPosX()+20, perso->getPosY()+20),
                                                                             QTransform());*/
                        this->view->scene->removeItem(this->view->scene->itemAt(QPointF(perso->getPosX()+20, perso->getPosY()+20),
                                                                                QTransform()));
                    }
                    //nextActionFight->getDefender()->deleteLater();
                }


                S->removeFirstAction();

            }

            // Regarder s'il n'y a pas collision

            bool premier = true;

            for (NicePerso *M : *this->model->getlistNicePerso()) {

                if (distanceBetweenPoints(new QPointF(S->getPosX(), S->getPosY()), new QPointF(M->getPosX(), M->getPosY())) < 100) {

                    if (premier) {
                        ActionFight *action = new ActionFight(S, M);
                        S->addActionInFirst(action);

                        premier = false;
                    }
                }
            }

            premier = true;
        }
    }

    // MadPerso Mobile
    for (MadPerso *S : *this->model->getlistMadPersoMobile()) {

        if (S->hasAction()) {
            ActionMove* nextActionMove = dynamic_cast<ActionMove*>(S->getFirstAction());
            ActionFight* nextActionFight = dynamic_cast<ActionFight*>(S->getFirstAction());

            if (nextActionMove) {
                // On regarde s'il avance il ne tape pas contre un decor
                /*if (this->view->scene->itemAt(QPointF(S->getPosX() + nextActionMove->getDepX(), S->getPosY() +
                                                      nextActionMove->getDepY()), QTransform()) == NULL)
                    S->setPos(S->getPosX() + nextActionMove->getDepX(), S->getPosY() + nextActionMove->getDepY());*/
                //qDebug() << "set pos [" << QString::number(nextActionMove->getDepX()) << ";" << QString::number(nextActionMove->getDepY()) << "]";
                S->setPos(nextActionMove->getDepX() + S->getPosX(), nextActionMove->getDepY() + S->getPosY());

                qDebug() << "pos [" << QString::number(S->getPosX()) << ";" << QString::number(S->getPosY()) << "]";
                S->moveTo(S->getPosX() + nextActionMove->getDepX() + 20, S->getPosY() + nextActionMove->getDepY() + 20);

                qDebug() << "move to [" << QString::number(S->getPosX() + nextActionMove->getDepX()) << ";" << QString::number(S->getPosY() + nextActionMove->getDepY()) << "]";
                S->removeFirstAction();

                /*QList<QGraphicsItem *> listCollision = S->getImagePerso()->collidingItems();

                for (QGraphicsItem * i : listCollision) {

                    ImageSetting *itemSetting = dynamic_cast<ImageSetting *>(i);

                    if (itemSetting) {

                        //QPointF *pointDestination = S->getDestination();
                        //S->removeAllActions();

                        //QPointF *pointSuivant = new QPointF(S->getPosX(), S->getPosY());

                        //qDebug() << "point SUivant 1 [" << pointSuivant->x() << ";" << pointSuivant->y() << "]";

                        //S->setPos(S->getPosX() - nextAction->getDepX(), S->getPosY() - nextAction->getDepY());

                        //QPointF *pointOrigine = new QPointF(S->getPosX()-nextActionMove->getDepX(),S->getPosY()-nextActionMove->getDepY());

                        //qDebug() << "point Origine [" << pointOrigine->x() << ";" << pointOrigine->y() << "]";

                        //pointSuivant = this->getPointDecale(pointOrigine, pointSuivant);

                        //qDebug() << "point SUivant 2 [" << pointSuivant->x() << ";" << pointSuivant->y() << "]";

                        //delete nextAction;

                        S->setPos(nextActionMove->getDepX(), nextActionMove->getDepY());
                        S->moveTo(S->getPosX() + nextActionMove->getDepX(), S->getPosY() + nextActionMove->getDepY());


                    }

                }*/
            }
            else if (nextActionFight) {

                if (nextActionFight->getDefender()->getHurt(nextActionFight->getFighter()->getDamage())) {
                    //qDebug() << "Prend un coup " << QString::number(nextActionFight->getDefender()->getHp());

                    NicePerso* perso = dynamic_cast<NicePerso*>(nextActionFight->getDefender());
                    //this->view->scene->removeItem(nextActionFight->getDefender()->getImagePerso()->parentItem());
                    if (perso) {
                        this->model->getlistNicePerso()->removeAll(perso);
                        /*QGraphicsItem *itemPerso = this->view->scene->itemAt(QPointF(perso->getPosX()+20, perso->getPosY()+20),
                                                                             QTransform());*/
                        this->view->scene->removeItem(this->view->scene->itemAt(QPointF(perso->getPosX()+20, perso->getPosY()+20),
                                                                                QTransform()));
                    }
                    //nextActionFight->getDefender()->deleteLater();
                }


                S->removeFirstAction();

            }

            // Regarder s'il n'y a pas collision

            bool premier = true;

            for (MadPerso *M : *this->model->getlistMadPersoFixed()) {

                if (distanceBetweenPoints(new QPointF(S->getPosX(), S->getPosY()), new QPointF(M->getPosX(), M->getPosY())) < 100) {

                    // Sauvegarder les actions si c'est un déplacement
                    //S->removeAllActions();

                    if (premier) {
                        ActionFight *action = new ActionFight(S, M);
                        S->addActionInFirst(action);

                        /*Image *imageBang = new Image(":/images/bang");
                        imageBang->setPos(S->getPosX(), S->getPosY());
                        this->view->scene->addItem(imageBang);
                        this->model->getlistBang()->append(new Image(":/images/bang"));
                        QTimer::singleShot(16, this, SLOT(deleteFirstBang()));*/

                        premier = false;
                    }

                }

            }

            /*for (ResourceItem *I : *this->model->getlistResourceItem()) {

                if (distanceBetweenPoints(new QPointF(S->getPosX(), S->getPosY()), new QPointF(I->getPosX(), I->getPosY())) < 100) {

                    this->view->scene->removeItem(this->view->scene->itemAt(QPointF(I->getPosX()+20, I->getPosY()+20),
                                                                            QTransform()));
                    if (I->getName() == "Wheat") {
                        this->model->getBagWheats()->addResource(I->getNumber());
                    }
                    else if (I->getName() == "BErry") {
                        this->model->getBagBerries()->addResource(I->getNumber());
                    }
                    else if (I->getName() == "Bread") {
                        this->model->getBagBreads()->addResource(I->getNumber());
                    }
                    else if (I->getName() == "Acorn") {
                        this->model->getBagAcorns()->addResource(I->getNumber());
                    }
                    else if (I->getName() == "Sarsaparilla") {
                        this->model->getBagSarsaparillas()->addResource(I->getNumber());
                    }

                    this->model->getlistResourceItem()->removeAll(I);
                }

            }*/

            premier = true;

            // Si oui retour en arrière et décallage
            // Puis on regarde les ennemis
        }

    }

}

void Controller::checkMadPerso() {

    for (QPointF *point : *this->model->getlistSpotMadPersoFixed()) {
        MadPerso *perso = new MadPerso(point->x(), point->y());
        if (!this->containsMadPersoFixed(perso)) {
            this->model->addMadPersoFixed(perso);
            this->view->scene->addItem(perso->getImagePerso());
        }
    }

}

int Controller::distanceBetweenPoints(QPointF *p1, QPointF *p2) {
    return sqrt(pow((p1->x() - p2->x()), 2) + pow((p1->y() - p2->y()), 2));
}

QPointF *Controller::getPointDecale(QPointF *beginningPoint, QPointF *nextPoint) {

    QPointF *newPoint = new QPointF;
    double rayon = sqrt(pow((beginningPoint->x() - nextPoint->x()), 2) + pow((beginningPoint->y() - nextPoint->y()), 2));

    newPoint->setX(beginningPoint->x() + (rayon * sin(M_PI + (M_PI / 3))));
    newPoint->setY(beginningPoint->y() + (rayon * cos(M_PI + (M_PI / 3))));

    return newPoint;

}


void Controller::actionPerso(int x, int y, int nbS) {
    this->model->actionPerso(x, y, nbS);
}

int Controller::getNumberFreePerso() {
    return this->model->getNumberFreePerso();
}

QList<NicePerso *> *Controller::getlistNicePerso() {
    QList<NicePerso *> *listSorted = this->model->getlistNicePerso();

    for (int i=(listSorted->size()-1); i>0; i--) {
        for (int j=0; j<(i); j++) {
            if ( (listSorted->at(j+1)->getHpMax() - listSorted->at(j+1)->getHp()) > (listSorted->at(j)->getHpMax() - listSorted->at(j)->getHp()) ) {
                listSorted->swap(j+1,j);
            }
        }
    }

    /*for (int i=0; i<listSorted->size(); i++) {
        qDebug() << " i = " << i << "getHpMax : " << listSorted->at(i)->getHpMax() << " PV : " << listSorted->at(i)->getHp();
    }*/

    return listSorted;
}

void Controller::harvestWheat() {
    this->model->getWheatField()->harvest(this->model->getBagWheats());
}

void Controller::sellWheat(int nbWheat) {
    this->model->useWheat(nbWheat);
    this->model->addSarsaparilla(nbWheat * this->model->getCostWheat());
}

void Controller::farmerUp(int cout) {
    if (!this->model->getBagSarsaparillas()->removeResource(cout)) {
        this->view->displayMistakeMoney();
    }
    else {
        this->model->farmerUp(1);
        this->model->addPoints(50);
    }
}

void Controller::makeBread(int nbBread) {
    if (!this->model->getBagSarsaparillas()->removeResource(nbBread * this->getCostProductionBread())) {
        this->view->displayMistakeMoney();
    }
    else {
        this->model->useWheat(nbBread * 2);
        this->model->makeBread(nbBread);
        this->model->addPoints(1);
    }
}

void Controller::bakerUp(int cout) {
    if (!this->model->getBagSarsaparillas()->removeResource(cout)) {
        this->view->displayMistakeMoney();
    }
    else {
        this->model->bakerUp(1);
        this->model->addPoints(50);
    }
}

void Controller::creationNewSmurf(int cout) {

    if (!this->model->getBagSarsaparillas()->removeResource(cout)) {
        this->view->displayMistakeMoney();
    }
    else {
        QPointF *point = this->getFreeSpawnNicePerso();
        NicePerso *perso = new NicePerso(point->x(), point->y(), this->model->getHpHefty(), this->model->getHpHefty(),
                                         this->model->getDamageHefty(), this->model->getSpeedHefty());
        this->view->addNormalPerso(perso);
        //connect(perso->getImagePerso(), SIGNAL(clicked()), perso, SLOT(actionS()));
        this->model->addNewSmurf(perso);
    }
}

void Controller::heftyUp(int cout) {
    if (!this->model->getBagSarsaparillas()->removeResource(cout)) {
        this->view->displayMistakeMoney();
    }
    else {
        this->model->heftyUp(1);
        this->model->addPoints(50);
    }
}

QPointF *Controller::getFreeSpawnNicePerso() {

    int *tabAlea = new int[this->model->getlistSpawnNicePerso()->size()];

    for (int i=0; i<this->model->getlistSpawnNicePerso()->size(); i++) {
        tabAlea[i]=i;
    }

    for (int i=0; i<this->model->getlistSpawnNicePerso()->size(); i++) {
        int nb = (rand() % (this->model->getlistSpawnNicePerso()->size()-1)) + 1;
        int tmp = tabAlea[i];
        tabAlea[i] = tabAlea[nb];
        tabAlea[nb] = tmp;
    }

    for (int y=0; y<this->model->getlistSpawnNicePerso()->size(); y++) {

        QPointF *point = this->model->getlistSpawnNicePerso()->at(tabAlea[y]);

        QList<NicePerso *> *listPerso = this->model->getlistNicePerso();
        int i=0;

        while ((i < listPerso->size())
               && ((listPerso->at(i)->getPosX() > (point->x() + 5) || listPerso->at(i)->getPosX() < (point->x() - 5))
               || (listPerso->at(i)->getPosY() > (point->y() + 5) || listPerso->at(i)->getPosY() < (point->y() - 5)))) {
            i++;
        }

        if (i==listPerso->size()) {
            i = 0;
            QList<MadPerso *> *listPerso2 = this->model->getlistMadPersoMobile();
            while ((i < listPerso2->size())
                   && ((listPerso2->at(i)->getPosX() > (point->x() + 5) || listPerso2->at(i)->getPosX() < (point->x() - 5))
                   || (listPerso2->at(i)->getPosY() > (point->y() + 5) || listPerso2->at(i)->getPosY() < (point->y() - 5)))) {
                i++;
            }

            if (i==listPerso2->size()) {
                return point;
            }
        }
    }
    return this->model->getlistSpawnNicePerso()->at((rand() % (this->model->getlistSpawnNicePerso()->size()-1)) + 1);
}

bool Controller::containsMadPersoFixed(MadPerso *perso) {

    for (MadPerso *p : *this->model->getlistMadPersoFixed()) {
        if (perso->getPosX() == p->getPosX() && perso->getPosY() == p->getPosY()) {
            return true;
        }
    }

    return false;
}

QGraphicsTextItem *Controller::getMessagePapaSmurf() {
    int MIN = 0;
    int MAX = this->model->getlistAdvicePapaSmurf()->size()-1;
    int random = (rand() % (MAX - MIN + 1)) + MIN;
    QGraphicsTextItem *texteItem = new QGraphicsTextItem(this->model->getMessagePapaSmurf(random));
    texteItem->setPos(this->getVillagePosX() + 155, this->getVillagePosY() - 80);

    return texteItem;
}

QGraphicsTextItem *Controller::getMessageSmurfette() {
    int MIN = 0;
    int MAX = this->model->getlistAdviceSmurfette()->size()-1;
    int random = (rand() % (MAX - MIN + 1)) + MIN;
    QGraphicsTextItem *texteItem = new QGraphicsTextItem(this->model->getMessageSmurfette(random));
    texteItem->setPos(this->getVillagePosX() + 200, this->getVillagePosY() - 110);

    return texteItem;
}
QGraphicsTextItem *Controller::getMessageBrainy() {
    QString texte = "";
    texte += tr("    Schtroumpf Paysan: niveau ") + QString::number(this->getLevelFarmer());
    texte += tr("\nPrix de revente du blé : ") + QString::number(this->getCostWheat());
    texte += tr("\nTaille d'un champs : ") + QString::number(this->getSizeFields());
    texte += tr("\nCapacité d'un champs : ") + QString::number(this->getCapacityFields());
    texte += tr("\n\n    Schtroumpf Boulanger : niveau ") + QString::number(this->getLevelBaker());
    texte += tr("\nPour production de pain : ") + QString::number(this->getResourceProductionBaker());
    texte += tr("\nCout de production du pain : ") + QString::number(this->getCostProductionBread());
    texte += tr("\n\n    Schtroumpf Costaud : niveau ") + QString::number(this->getLevelHefty());
    texte += tr("\nVitesse d'un nouveau Schtroumpf : ") + QString::number(this->getSpeedS());
    texte += tr("\nDegats d'un nouveau Schtroumpf : ") + QString::number(this->getDamageS());
    texte += tr("\nPV d'un nouveau Schtroumpf : ") + QString::number(this->getHpS());

    QGraphicsTextItem *texteItem = new QGraphicsTextItem(texte);
    texteItem->setPos(this->getVillagePosX() - 40, this->getVillagePosY() - 205);

    return texteItem;
}

Image *Controller::getImageBubblePapaSmurf() {
    Image *imageBubble = this->model->getImageBubblePapaSmurf();
    imageBubble->setPos(this->getVillagePosX() + 80, this->getVillagePosY() - 190);
    return imageBubble;
}
Image *Controller::getImageBubbleSmurfette() {
    Image *imageBubble = this->model->getImageBubbleSmurfette();
    imageBubble->setPos(this->getVillagePosX() + 125, this->getVillagePosY() - 210);
    return imageBubble;
}
Image *Controller::getImageBubbleBrainy() {
    Image *imageBubble = this->model->getImageBubbleBrainy();
    imageBubble->setPos(this->getVillagePosX() - 75, this->getVillagePosY() - 245);
    return imageBubble;
}

Image *Controller::getImagePapaSmurf() {
     Image *imagePapaSmurf = this->model->getImagePapaSmurf();
     imagePapaSmurf->setPos(this->getVillagePosX() + 370, this->getVillagePosY() - 180);
     return imagePapaSmurf;
}
Image *Controller::getImageSmurfette() {
    Image *imageSmurfette = this->model->getImageSmurfette();
    imageSmurfette->setPos(this->getVillagePosX() + 370, this->getVillagePosY() - 180);
    return imageSmurfette;
}
Image *Controller::getImageBrainy() {
    Image *imageBrainy = this->model->getImageBrainy();
    imageBrainy->setPos(this->getVillagePosX() + 200, this->getVillagePosY() - 175);
    return imageBrainy;
}

int Controller::setTreatment(int i) {
    int treatment = this->getlistNicePerso()->at(i)->getHpMax() - this->getlistNicePerso()->at(i)->getHp();
    return this->treatment = treatment;
}
int Controller::getMaxBreadForTreatment() {
    return getTreatment()/getHealthBread();
}
int Controller::getMaxBerriesForTreatment() {
    return getTreatment()/getHealthBerries();
}
int Controller::getMaxAcornForTreatment() {
    return getTreatment()/getHealthAcorn();
}

void Controller::treat(int i, int nbBreads, int nbBerries, int nbAcorns) {
    int hpGettingBack = (getHealthBread() * nbBreads) + (getHealthBerries() * nbBerries) + (getHealthAcorn() * nbAcorns);
    if (hpGettingBack <= getTreatment()) {
        this->getlistNicePerso()->at(i)->heal(hpGettingBack);
    }
    else {
        this->getlistNicePerso()->at(i)->heal(getTreatment());
    }
    this->model->getBagBreads()->removeResource(nbBreads);
    this->model->getBagBerries()->removeResource(nbBerries);
    this->model->getBagAcorns()->removeResource(nbAcorns);
}
