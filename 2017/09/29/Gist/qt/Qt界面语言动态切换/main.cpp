
#include "lang.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Lang w;
    w.show();

    return a.exec();
}
