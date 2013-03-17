#ifndef SETTING_H
#define SETTING_H

#include <QObject>
#include <QSettings>
#include <agt/global.h>

class AGTSHARED_EXPORT Settings : public QSettings
{
    Q_OBJECT
private:
    Settings(QObject* parent=NULL):
        QSettings(parent){}
    Settings(const Settings& other):
        QSettings(other.parent()){}
    static Settings *instance;
    static bool set;
public:
    static void initialize(const QString& organization, const QString& program);
    static Settings* getInstance();
    bool isFirstRun();
    void setFirstRun(const bool& value);
signals:
    
public slots:
    
};

#endif // SETTING_H
