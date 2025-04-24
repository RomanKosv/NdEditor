#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include "objectentry.h"

class Model:public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(NdModel)
    QVector<ObjectEntry*> objects;
public:
    Model();
    Q_INVOKABLE void add_element(ObjectEntry*);
    Q_INVOKABLE void clear();
    QVector<ObjectEntry*> get_objects();
};

#endif // MODEL_H
