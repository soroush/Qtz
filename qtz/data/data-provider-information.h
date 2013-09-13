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
    QVector<DataProviderInformation> supportedSystems;
    bool initialized;

public:
    const DataProviderInformation &getProviderInfo(const Database::Type &) const;
    QString defaultHost() const;
    quint32 defaultPort() const;
    QString defaultUsername() const;
    QString defaultDatabase() const;
    QString providerName() const;
    quint8 providerCode();
    static DataProviderInformation *getInstance();
    static DataProviderInformation *m_instance;
    QList<Database::Type> availableSystems;
    QVector<DataProviderInformation> getSupportedProviders();
    QList<Database::Type> getAvailableSystems();
    void generateAvailableSystems();
    QString getDriverName(const Database::Type &);
};

#endif // DATAPROVIDERINFORMATION_H
