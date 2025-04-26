#include "objectentry.h"

// std::vector<triangle> ObjectEntry::triangulate()
// {

// }

ObjectEntry::ObjectEntry() {
    setColor(QColor::fromRgbF(1, 0, 0, 1));
}

void ObjectEntry::setName(QString s){
    _name=s;
    emit nameChanged(s);
}

QString ObjectEntry::name(){
    return _name;
}

void ObjectEntry::setExpression(QString exp) {
    _expression = exp;
    emit expressionChanged(exp);
}

QString ObjectEntry::expression() {
    return _expression;
}

void ObjectEntry::setColor(QColor color_)
{
    _color = color_;
    emit colorChanged(_color);
}

QColor ObjectEntry::color()
{
    return _color;
}
