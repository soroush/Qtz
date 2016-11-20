#ifndef QTZ_DATAPROVIDERINFORMATION_HPP
#define QTZ_DATAPROVIDERINFORMATION_HPP

#include <QtGlobal>
#include <QString>
#include <QVector>
#include "data.hpp"
#include "data-provider.hpp"

class QTZ_DATA_SHARED_EXPORT DataProviderInformation {
    friend class QVector<DataProvider>;
private:
    DataProviderInformation();
    void initialize();
    QVector<DataProvider> supportedSystems;

public:
    const DataProvider getProviderInfo(DataProvider::Type&) const;
    static DataProviderInformation* getInstance();
    static DataProviderInformation* m_instance;
    QList<DataProvider::Type> availableSystems;
    QVector<DataProvider> getSupportedProviders();
    QList<DataProvider::Type> getAvailableSystems();
    void generateAvailableSystems();
    QString getDriverName(DataProvider::Type);
};

#endif // QTZ_DATAPROVIDERINFORMATION_HPP
