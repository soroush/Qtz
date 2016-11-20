#include "data-provider.hpp"

DataProvider::DataProvider() {
}

DataProvider::DataProvider(const QString& name,
                           Type type,
                           const QString& defaultHost,
                           quint32 defaultPort,
                           const QString& defaultDatabase,
                           const QString& defaultUsername):
    m_providerName(name),
    m_type(type),
    m_defaultHost(defaultHost),
    m_defaultPort(defaultPort),
    m_defaultUsername(defaultUsername),
    m_defaultDatabase(defaultDatabase) {
}

QString DataProvider::providerName() const {
    return this->m_providerName;
}

quint8 DataProvider::providerCode() const {
    return static_cast<quint8>(this->m_type);
}

QString DataProvider::defaultHost() const {
    return this->m_defaultHost;
}

quint32 DataProvider::defaultPort() const {
    return this->m_defaultPort;
}

QString DataProvider::defaultUsername() const {
    return this->m_defaultUsername;
}

QString DataProvider::defaultDatabase() const {
    return this->m_defaultDatabase;
}
