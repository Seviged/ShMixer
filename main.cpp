#include "shmixer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ShMixer w;
    w.show();
    return a.exec();
}
