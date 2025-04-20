//#include "mainwindow.h"
#include "chatgpt_interface.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    WelcomeWindow window;
    window.show();

    return app.exec();
}
