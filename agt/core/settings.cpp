#include <QSettings>
#include "settings.h"
#include "qio.h"
#include <cstdlib>
#include <iostream>

Settings* Settings::instance;
bool Settings::set;

void Settings::initialize(const QString &organization, const QString &program)
{
    instance = (Settings*)(new QSettings(organization,program));
    set = true;
}

Settings *Settings::getInstance()
{
    if(!set)
    {
        std::wcerr << tr("Request for non-existing instance. Returning null.").toStdWString() << std::endl;
        return NULL;
    }
    return instance;
}

bool Settings::isFirstRun()
{
    if(!set)
        return true; //TODO: add exception here
    bool result = instance->value("first-run",true).toBool();
    return result;
}

void Settings::setFirstRun(const bool &value)
{
    if(!set)
    {
        // TODO: add exception
        return;
    }
    instance->setValue("first-run",QVariant(value));
}
