#ifndef SETTING_H
#define SETTING_H

#include <QObject>
#include <QSettings>
#include <agt/global.h>

class AGTSHARED_EXPORT Settings : public QObject
{
    Q_OBJECT
private:
    Settings(QObject* parent=NULL):
        QObject(parent){}
    Settings(const Settings& other):
        QObject(other.parent()){}
    static QSettings *instance;
    static bool set;
public:
    static void initialize(const QString& organization, const QString& program);
    static QSettings* getInstance();
signals:
    
public slots:
    
};

#endif // SETTING_H
