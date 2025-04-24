#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "parser/outparse.hh"

int main(int argc, char *argv[])
{
    common_parsing::OUTPARSE_LOGGER.flag=false;
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("QtQuickProject", "Main");

    return app.exec();
}
