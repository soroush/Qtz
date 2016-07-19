#ifndef DATAPROVIDER_H
#define DATAPROVIDER_H

#include <QString>
#include "data.h"

class QTZ_DATA_SHARED_EXPORT DataProvider {
    friend class DataProviderInformation;

public:
    enum class Type {
        MySQL5,
        SQLite,
        Oracle
    };
    DataProvider();
    DataProvider(const QString &, Type, const QString &, quint32,
                 const QString &, const QString &);
    QString providerName() const;
    quint8 providerCode() const;
    QString defaultHost() const;
    quint32 defaultPort() const;
    QString defaultUsername() const;
    QString defaultDatabase() const;
private:
    QString m_providerName;
    Type m_type;
    QString m_defaultHost;
    quint32 m_defaultPort;
    QString m_defaultUsername;
    QString m_defaultDatabase;
};

#endif // DATAPROVIDER_H
