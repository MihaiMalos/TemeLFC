#include "AutomatonGUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AutomatonGUI w;
    w.show();
    return a.exec();
}
