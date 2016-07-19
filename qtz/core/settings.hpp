#ifndef SETTING_H
#define SETTING_H

#include <QObject>
#include <QSettings>
#include "qtz-core.hpp"

class QTZ_SHARED_EXPORT Settings {
private:
    Settings();
    Settings(const Settings& other);
    static QSettings* instance;
    static bool set;
public:
    void release();
    static void initialize(const QString& organization, const QString& program);
    static QSettings* getInstance();
    bool isFirstRun();
    void setFirstRun(const bool& value);
};

#endif // SETTING_H
