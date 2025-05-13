#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "parser/function_parse_instructions.hh"
#include "parser/operator_parse_instructions.hh"
#include "parser/outparse.hh"

int main(int argc, char *argv[])
{
    common_parsing::OUTPARSE_LOGGER.flag=false;
    common_parsing::PARSE_FUN_LOGGER.flag=false;
    common_parsing::OPERATOR_PARSE_LOGGER.flag=false;
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.addImportPath("qml");
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("QtQuickProject", "Main");

    return app.exec();
}
