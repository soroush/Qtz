#include "data-provider-information.h"

QString DataProviderInformation::defaultHost()
{
    return this->m_defaultHost;
}

quint8 DataProviderInformation::defaultPort()
{
    return this->m_defaultPort;
}

QString DataProviderInformation::defaultUsername()
{
    return this->m_defaultUsername;
}

QString DataProviderInformation::defaultDatabase()
{
    return this->m_defaultDatabase;
}
