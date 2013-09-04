#ifndef DATAPROVIDERINFORMATION_H
#define DATAPROVIDERINFORMATION_H

#include <QtGlobal>

QT_BEGIN_NAMESPACE
class QString;
QT_END_NAMESPACE

class DataProviderInformation
{
private:
    DataProviderInformation();
    QString m_defaultHost;
    quint8 m_defaultPort;
    QString m_defaultUsername;
    QString m_defaultDatabase;
    static DataProviderInformation* m_mysql;
public:
    static DataProviderInformation* getProviderInfo();
    QString defaultHost();
    quint8 defaultPort();
    QString defaultUsername();
    QString defaultDatabase();
};

#endif // DATAPROVIDERINFORMATION_H
