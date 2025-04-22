#include "objectentry.h"

ObjectEntry::ObjectEntry() {}

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
