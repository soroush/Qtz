#include <QObject>
#include <cstdlib>
#include <iostream>
#include <QtGlobal>
#include "settings.hpp"
#include "qio.hpp"

QSettings* Settings::instance = Q_NULLPTR;
bool Settings::set = false;

void Settings::release() {
    if(instance != Q_NULLPTR) {
        delete instance;
        instance = Q_NULLPTR;
    }
}

void Settings::initialize(const QString& organization, const QString& program) {
    instance = new QSettings(organization, program);
    set = true;
}

QSettings* Settings::getInstance() {
    if(!set) {
        std::cerr << "Request for non-existing instance. Returning null."
                  << std::endl;
        return Q_NULLPTR;
    }
    return instance;
}

bool Settings::isFirstRun() {
    if(set) {
        bool result = instance->value("first-run", true).toBool();
        return result;
    }
    return true;
}

void Settings::setFirstRun(const bool& value) {
    if(set) {
        instance->setValue("first-run", QVariant(value));
    }
}
