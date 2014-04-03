#ifndef DATAPROVIDERINFORMATION_H
#define DATAPROVIDERINFORMATION_H

#include <QtGlobal>
#include <QString>
#include <QVector>
#include <qtz/core/library.h>
#include "data-provider.h"

class QTZ_SHARED_EXPORT DataProviderInformation {
    friend class QVector<DataProvider>;
private:
    DataProviderInformation();
    void initialize();
    QVector<DataProvider> supportedSystems;

public:
    const DataProvider getProviderInfo(const Database::Type &) const;
    static DataProviderInformation *getInstance();
    static DataProviderInformation *m_instance;
    QList<Database::Type> availableSystems;
    QVector<DataProvider> getSupportedProviders();
    QList<Database::Type> getAvailableSystems();
    void generateAvailableSystems();
    QString getDriverName(const Database::Type &);
};

#endif // DATAPROVIDERINFORMATION_H
