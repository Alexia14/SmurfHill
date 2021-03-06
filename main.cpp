#include "view.h"
#include "model.h"
#include "controller.h"
#include "autre.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QVBoxLayout>
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    View w;
    Model m;
    Controller *c = new Controller(&m, &w);

    QTranslator translator;
    translator.load("smurfHill_en_EN");
    a.installTranslator(&translator);

    w.show();

    return a.exec();
}
