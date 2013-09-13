#ifndef DATAPROVIDERINFORMATION_H
#define DATAPROVIDERINFORMATION_H

#include <QtGlobal>
#include <QString>
#include <QVector>
#include "database.h"

class DataProviderInformation {
    friend class QVector<DataProviderInformation>;
private:
    DataProviderInformation();
    QString m_defaultHost;
    quint32 m_defaultPort;
    QString m_defaultUsername;
    QString m_defaultDatabase;
    QString m_providerName;
    Database::Type m_type;
    void initialize();
    QVector<DataProviderInformation> databases;
    bool initialized;

public:
    const DataProviderInformation &getProviderInfo(const Database::Type &) const;
    QString defaultHost() const;
    quint32 defaultPort() const;
    QString defaultUsername() const;
    QString defaultDatabase() const;
    QString providerName() const;
    quint8 providerCode();
    QVector<DataProviderInformation> getAvailableDatabases();
    static DataProviderInformation *getInstance();
    static DataProviderInformation *m_instance;
};

#endif // DATAPROVIDERINFORMATION_H
