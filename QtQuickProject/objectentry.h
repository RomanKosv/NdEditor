#ifndef OBJECTENTRY_H
#define OBJECTENTRY_H
#include <QObject>
#include <qqmlintegration.h>
class ObjectEntry:public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(ObjectEntry)
    QString _name, _expression;
public:
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString expression READ expression WRITE setExpression NOTIFY expressionChanged)
    ObjectEntry();
    void setName(QString s);
    QString name();
    void setExpression(QString exp);
    QString expression();
signals:
    void nameChanged(QString new_name);
    void expressionChanged(QString new_exp);
};

#endif // OBJECTENTRY_H
