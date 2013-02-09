#include <QSettings>
#include "settings.h"
#include "qio.h"

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
        QIO::qcerr << tr("Request for non-existing instance. Returning null.") << endl;
        return NULL;
    }
    return instance;
}
