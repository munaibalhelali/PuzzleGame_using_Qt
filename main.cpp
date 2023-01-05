#include "puzzlegame.h"
#include "welcomedialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WelcomeDialog w;
    w.show();
    return a.exec();
}
