//#include "mainwindow.h"
#include "chatgpt_interface.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ModelEditor window;
    window.show();

    return app.exec();
}
