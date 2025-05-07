#ifndef OBJECTENTRY_H
#define OBJECTENTRY_H
#include "triangles.h"
#include <QObject>
#include <qcolor.h>
#include <qqmlintegration.h>
#include "strings_to_screen_version1.h"
#include <QJsonObject>
class ObjectEntry:public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(ObjectEntry)
public:
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString expression READ expression WRITE setExpression NOTIFY expressionChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)
    Q_PROPERTY(bool project READ project WRITE setProject NOTIFY projectChanged)
    Q_INVOKABLE QJsonObject toJson();
    Q_INVOKABLE void readJson(QJsonObject);
    // std::vector<triangle> triangulate();
    ObjectEntry();
    void setName(QString s);
    QString name();
    void setExpression(QString exp);
    QString expression();
    void setColor(QColor);
    QColor color();
    void setVisible(bool val);
    bool visible();
    void setProject(bool new_project);
    bool project();
    std::optional<Object> intreprete(Context&);
    std::optional<Figure> get_render_figure(Context& context);
signals:
    void nameChanged(QString new_name);
    void expressionChanged(QString new_exp);
    void colorChanged(QColor);
    void visibleChanged(bool new_visible);
    void projectChanged(bool new_project);
private:
    QString _name, _expression;
    QColor _color=QColor::fromRgbF(1, 0, 0);
    bool _visible=true;
    bool _project=false;
};

#endif // OBJECTENTRY_H
