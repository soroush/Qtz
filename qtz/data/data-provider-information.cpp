#include "data-provider-information.h"
#include <QObject>
#include <QStringList>
#include <QFile>
#include <QXmlStreamReader>
#include <iostream>
#include <stdexcept>

#include <QDebug>

using namespace std;

DataProviderInformation *DataProviderInformation::m_instance = nullptr;

DataProviderInformation::DataProviderInformation():
    initialized(false) {
    Q_INIT_RESOURCE(resources);
}

void DataProviderInformation::initialize() {
    supportedSystems.clear();
    QList<Database::Type> availableSystems =getAvailableSystems();
    qDebug() << "size of availableSystems: " << availableSystems.size();
    QFile providersFile(":/en/resources/database-providers.xml");
    if(! providersFile.open(QFile::ReadOnly)) {
        qDebug() << "Error Opening file: " << providersFile.errorString() << providersFile.fileName();
        return;
    }
    QXmlStreamReader xml(&providersFile);
    while (!xml.atEnd() && !xml.hasError()) {
        bool valid;
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
                    DataProviderInformation info;
                    info.m_type = type;
                    this->supportedSystems << info;
                    qDebug() << "added";
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
    qDebug() << "size of supportedSystems: " << supportedSystems.size();

    initialized = true;
}

const DataProviderInformation &DataProviderInformation::getProviderInfo(
    const Database::Type &type) const {
    foreach (DataProviderInformation db, this->supportedSystems) {
        if(db.m_type == type) {
            return db;
        }
    }
    return *this;
}

QString DataProviderInformation::defaultHost() const {
    return this->m_defaultHost;
}

quint32 DataProviderInformation::defaultPort() const {
    return this->m_defaultPort;
}

QString DataProviderInformation::defaultUsername() const {
    return this->m_defaultUsername;
}

QString DataProviderInformation::defaultDatabase() const {
    return this->m_defaultDatabase;
}

QString DataProviderInformation::providerName() const {
    return this->m_providerName;
}

quint8 DataProviderInformation::providerCode() {
    return static_cast<quint8>(this->m_type);
}

QVector<DataProviderInformation>
DataProviderInformation::getSupportedProviders() {
    return this->supportedSystems;
}

DataProviderInformation *DataProviderInformation::getInstance() {
    if(m_instance == nullptr) {
        m_instance = new DataProviderInformation();
        m_instance->initialize();
    }
    return m_instance;
}

QList<Database::Type> DataProviderInformation::getAvailableSystems() {
    generateAvailableSystems();
    return availableSystems;
}


void DataProviderInformation::generateAvailableSystems() {
    availableSystems.clear();
    QStringList drivers = QSqlDatabase::drivers();
    qDebug() << drivers;
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

QString DataProviderInformation::getDriverName(const Database::Type &type) {
    switch (type) {
    case Database::Type::MySQL5:
        return "QMYSQL";
        break;
    case Database::Type::SQLServer:
    case Database::Type::SQLServer2005:
    case Database::Type::SQLServer2008:
    case Database::Type::SQLServer2010:
    case Database::Type::SQLServer2012:
        return "QODBC";
        break;
    case Database::Type::SQLite:
        return "QSQLITE";
        break;
    default:
        break;
    }
}
