#include "cryptominder.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Cryptominder w;
    w.show();
    return QApplication::exec();
}
