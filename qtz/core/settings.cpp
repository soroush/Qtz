#include <QObject>
#include <cstdlib>
#include <iostream>
#include "settings.h"
#include "qio.h"

QSettings *Settings::instance = NULL;
bool Settings::set = false;

Settings::Settings()
{
    // Foo ctor
}

void Settings::release()
{
    if(instance != NULL) {
        delete instance;
    }
}

void Settings::initialize(const QString &organization, const QString &program)
{
    instance = new QSettings(organization, program);
    set = true;
}

QSettings *Settings::getInstance()
{
    if(!set) {
        std::wcerr <<
                   QObject::tr("Request for non-existing instance. Returning null.").toStdWString()
                   << std::endl;
        return NULL;
    }
    return instance;
}

bool Settings::isFirstRun()
{
    if(!set) {
        return true;    //TODO: add exception here
    }
    bool result = instance->value("first-run", true).toBool();
    return result;
}

void Settings::setFirstRun(const bool &value)
{
    if(!set) {
        // TODO: add exception
        return;
    }
    instance->setValue("first-run", QVariant(value));
}

