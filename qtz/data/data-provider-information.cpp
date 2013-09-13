#include "data-provider-information.h"
#include <QObject>
#include <QFile>
#include <QXmlStreamReader>
#include <iostream>
#include <stdexcept>

#include <QDebug>

using namespace std;

DataProviderInformation *DataProviderInformation::m_instance = nullptr;

DataProviderInformation::DataProviderInformation():
    initialized(false) {
}

void DataProviderInformation::initialize() {
//    databases.clear();
    QList<Database::Type> supportedTypes =
        Database::getInstance()->getSupportedSystems();
    QFile providersFile("://resources/database-providers.xml");
    if(! providersFile.open(QFile::ReadOnly | QFile::Text)) {
        //qDebug() << "Error Opening file: " << providersFile.errorString();
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
                if(supportedTypes.contains(type)) {
                    valid = true;
                    DataProviderInformation info;
                    info.m_type = type;
                    this->databases << info;
                }
            }
        }
        else if(xml.name() == "Name" && valid) {
            databases.last().m_providerName = xml.readElementText();
        }
        else if(xml.name() == "DefaultHost" && valid) {
            databases.last().m_defaultHost = xml.readElementText();
        }
        else if(xml.name() == "DefaultPort" && valid) {
            databases.last().m_defaultPort = xml.readElementText().toUInt();
        }
    }
    initialized = true;
}

const DataProviderInformation &DataProviderInformation::getProviderInfo(
    const Database::Type &type) const {
    foreach (DataProviderInformation db, this->databases) {
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
DataProviderInformation::getAvailableDatabases() {
    return this->databases;
}

DataProviderInformation *DataProviderInformation::getInstance() {
    if(m_instance == nullptr) {
        m_instance = new DataProviderInformation();
        m_instance->initialize();
    }
    return m_instance;
}
