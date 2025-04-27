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
    Q_INVOKABLE QVector<ObjectEntry*> get_objects();
    vector<triangle> get_render(Context& context);
};

#endif // MODEL_H
