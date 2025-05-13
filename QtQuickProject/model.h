#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QJsonArray>
#include <QUrl>
#include <QFile>
#include <QIODevice>
#include <QByteArray>
#include <QJsonDocument>
#include "objectentry.h"
#include "log.h"

class Model:public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(NdModel)
    QVector<ObjectEntry*> objects;
public:
    Model();
    Q_INVOKABLE void add_element(ObjectEntry*);
    Q_INVOKABLE bool removeAt(qsizetype index);
    Q_INVOKABLE void clear();
    Q_INVOKABLE QVector<ObjectEntry*> get_objects();
    Q_INVOKABLE QJsonArray toJson();
    Q_INVOKABLE void readJson(QJsonArray array);
    Q_INVOKABLE bool readJsonFile(QUrl url);
    Q_INVOKABLE bool writeJsonFile(QUrl url);
    vector<triangle> get_render(strings_to_screen_version1 pipeline, Context &context);
};

#endif // MODEL_H
