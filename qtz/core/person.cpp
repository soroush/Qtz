#include "person.h"

Person::Person(QObject *parent) :
    QObject(parent) {
}

void Person::name(const QString &name) {
    this->name_ = name;
}

void Person::last(const QString &last) {
    this->last_ = last;
}

QString Person::name() const {
    return this->name_;
}

QString Person::last() const {
    return this->last_;
}
