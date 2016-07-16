#include "data-provider-information.h"
#include <QObject>
#include <QStringList>
#include <QFile>
#include <QXmlStreamReader>
#include <iostream>
#include <stdexcept>

using namespace std;

DataProviderInformation *DataProviderInformation::m_instance = nullptr;

DataProviderInformation::DataProviderInformation()
{
}

void DataProviderInformation::initialize()
{
    supportedSystems.clear();
    QList<Database::Type> availableSystems = getAvailableSystems();
    QFile providersFile(":/en/resources/database-providers.xml");
    if(! providersFile.open(QFile::ReadOnly)) {
        return;
    }
    QXmlStreamReader xml(&providersFile);
    bool valid;
    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if(xml.name() == "Provider") {
            valid = false;
            continue;
        }
        else if(xml.name() == "Code") {
            quint8 code = xml.readElementText().toUInt();
            if(code != 0) {
                Database::Type type = static_cast<Database::Type>(code);
                if(availableSystems.contains(type)) {
                    valid = true;
                    DataProvider provider;
                    provider.m_type = type;
                    this->supportedSystems << provider;
                }
            }
        }
        else if(xml.name() == "Name" && valid) {
            supportedSystems.last().m_providerName = xml.readElementText();
        }
        else if(xml.name() == "DefaultHost" && valid) {
            supportedSystems.last().m_defaultHost = xml.readElementText();
        }
        else if(xml.name() == "DefaultPort" && valid) {
            supportedSystems.last().m_defaultPort = xml.readElementText().toUInt();
        }
    }
}

const DataProvider DataProviderInformation::getProviderInfo(
    const Database::Type &type) const
{
    foreach (DataProvider db, this->supportedSystems) {
        if(db.m_type == type) {
            return db;
        }
    }
    // TODO: Handle error
    return DataProvider();
}

QVector<DataProvider> DataProviderInformation::getSupportedProviders()
{
    return this->supportedSystems;
}

DataProviderInformation *DataProviderInformation::getInstance()
{
    if(m_instance == nullptr) {
        m_instance = new DataProviderInformation();
        m_instance->initialize();
    }
    return m_instance;
}

QList<Database::Type> DataProviderInformation::getAvailableSystems()
{
    generateAvailableSystems();
    return availableSystems;
}


void DataProviderInformation::generateAvailableSystems()
{
    availableSystems.clear();
    QStringList drivers = QSqlDatabase::drivers();
    foreach (QString driver, drivers) {
        if(driver=="QMYSQL" || driver == "QMYSQL3") {
            m_instance->availableSystems.push_back(Database::Type::MySQL5);
        }
        else if(driver=="QIBASE") {
            //TODO: implement
        }
        else if(driver=="QOCI") {
            //TODO: implement
        }
        else if(driver=="QODBC"|| driver == "QODBC3") {
            this->availableSystems.push_back(Database::Type::SQLServer);
            this->availableSystems << Database::Type::SQLServer2005;
            this->availableSystems << Database::Type::SQLServer2008;
            this->availableSystems << Database::Type::SQLServer2010;
            this->availableSystems << Database::Type::SQLServer2012;
        }
        else if(driver=="QPSQL") {
            // TODO: implement
        }
        else if(driver=="QSQLITE2") {
            // TODO: implement
        }
        else if(driver=="QSQLITE") {
            this->availableSystems.append(Database::Type::SQLite);
        }
#if QT_VERSION <= 0x040700
        else if(driver=="QTDS") {
            // TODO: implement
        }
#endif
    }
}

QString DataProviderInformation::getDriverName(const Database::Type &type)
{
    switch (type) {
    case Database::Type::MySQL5:
        return QString("QMYSQL");
    case Database::Type::SQLServer:
    case Database::Type::SQLServer2005:
    case Database::Type::SQLServer2008:
    case Database::Type::SQLServer2010:
    case Database::Type::SQLServer2012:
        return QString("QODBC");
        break;
    case Database::Type::SQLite:
        return QString("QSQLITE");
    default:
        return QString("");
    }
}
