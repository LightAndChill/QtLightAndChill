#include "lightandchill.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LightAndChill w;
    w.show();

    return a.exec();
}
