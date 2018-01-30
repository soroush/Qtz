#include "data-provider-information.hpp"
#include <QObject>
#include <QStringList>
#include <QFile>
#include <QXmlStreamReader>
#include <QSqlDatabase>
#include <iostream>
#include <stdexcept>

DataProviderInformation* DataProviderInformation::m_instance = nullptr;


DataProviderInformation::DataProviderInformation() {
    Q_INIT_RESOURCE(QtzData);
}

void DataProviderInformation::initialize() {
    supportedSystems.clear();
    QList<DataProvider::Type> availableSystems = getAvailableSystems();
    QFile providersFile(":/qtzdata/resources/database-providers.xml");
    if(! providersFile.open(QFile::ReadOnly)) {
        std::cerr << "Unable to open database providers information file.";
    }
    QXmlStreamReader xml(&providersFile);
    bool valid;
    while(!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if(xml.name() == "Provider") {
            valid = false;
            continue;
        } else if(xml.name() == "Code") {
            quint8 code = xml.readElementText().toUInt();
            if(code != 0) {
                DataProvider::Type type = static_cast<DataProvider::Type>(code);
                if(availableSystems.contains(type)) {
                    valid = true;
                    DataProvider provider;
                    provider.m_type = type;
                    this->supportedSystems << provider;
                }
            }
        } else if(xml.name() == "Name" && valid) {
            supportedSystems.last().m_providerName = xml.readElementText();
        } else if(xml.name() == "DefaultHost" && valid) {
            supportedSystems.last().m_defaultHost = xml.readElementText();
        } else if(xml.name() == "DefaultPort" && valid) {
            supportedSystems.last().m_defaultPort = xml.readElementText().toUInt();
        }
    }
}

const DataProvider DataProviderInformation::getProviderInfo(DataProvider::Type& type) const {
    foreach(DataProvider db, this->supportedSystems) {
        if(db.m_type == type) {
            return db;
        }
    }
    // TODO: Handle error
    return DataProvider();
}

QVector<DataProvider> DataProviderInformation::getSupportedProviders() {
    return this->supportedSystems;
}

DataProviderInformation* DataProviderInformation::getInstance() {
    if(m_instance == nullptr) {
        m_instance = new DataProviderInformation();
        m_instance->initialize();
    }
    return m_instance;
}

QList<DataProvider::Type> DataProviderInformation::getAvailableSystems() {
    generateAvailableSystems();
    return availableSystems;
}


void DataProviderInformation::generateAvailableSystems() {
    availableSystems.clear();
    QStringList drivers = QSqlDatabase::drivers();
    foreach(QString driver, drivers) {
        if(driver=="QMYSQL" || driver == "QMYSQL3") {
            m_instance->availableSystems.push_back(DataProvider::Type::MySQL5);
        } else if(driver=="QIBASE") {
            //TODO: implement
        } else if(driver=="QOCI") {
            //TODO: implement
        } else if(driver=="QODBC"|| driver == "QODBC3") {
            //TODO: implement
        } else if(driver=="QPSQL") {
            // TODO: implement
        } else if(driver=="QSQLITE2") {
            // TODO: implement
        } else if(driver=="QSQLITE") {
            this->availableSystems.append(DataProvider::Type::SQLite);
        }
    }
}

QString DataProviderInformation::getDriverName(DataProvider::Type type) {
    switch(type) {
        case DataProvider::Type::MySQL5:
            return QString("QMYSQL");
            break;
        case DataProvider::Type::SQLite:
            return QString("QSQLITE");
        default:
            return QString("");
    }
}
