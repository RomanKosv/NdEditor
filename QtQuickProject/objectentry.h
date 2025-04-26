#ifndef OBJECTENTRY_H
#define OBJECTENTRY_H
#include "triangles.h"
#include <QObject>
#include <qcolor.h>
#include <qqmlintegration.h>
class ObjectEntry:public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(ObjectEntry)
public:
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString expression READ expression WRITE setExpression NOTIFY expressionChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged FINAL)
    // std::vector<triangle> triangulate();
    ObjectEntry();
    void setName(QString s);
    QString name();
    void setExpression(QString exp);
    QString expression();
    void setColor(QColor);
    QColor color();
signals:
    void nameChanged(QString new_name);
    void expressionChanged(QString new_exp);
    void colorChanged(QColor);
private:
    QString _name, _expression;
    QColor _color=QColor::fromRgbF(1, 0, 0);
};

#endif // OBJECTENTRY_H
