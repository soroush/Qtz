#include <QSettings>
#include "settings.h"

QSettings* Settings::instance;
bool Settings::set;

void Settings::initialize(const QString &organization, const QString &program)
{
    instance = new QSettings(organization,program);
}

QSettings *Settings::getInstance()
{
    if(!set)
    {
        return NULL;
    }
    return instance;
}
