#ifndef QTZ_DATAPROVIDER_HPP
#define QTZ_DATAPROVIDER_HPP

#include <QString>
#include "data.hpp"

class QTZ_DATA_SHARED_EXPORT DataProvider {
    friend class DataProviderInformation;

public:
    enum class Type {
        MySQL5 = 1,
        Oracle = 2,
        SQLite = 3,
    };
    DataProvider();
    DataProvider(const QString&, Type, const QString&, quint32,
                 const QString&, const QString&);
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

#endif // QTZ_DATAPROVIDER_HPP
