#ifndef DATAPROVIDERINFORMATION_H
#define DATAPROVIDERINFORMATION_H

#include <QtGlobal>
#include <QString>
#include <QVector>
#include "data.h"
#include "data-provider.h"

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

#endif // DATAPROVIDERINFORMATION_H
