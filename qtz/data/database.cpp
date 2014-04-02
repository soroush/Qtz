#include "database.h"
#include <qtz/core/settings.h>
#include <qtz/core/auth-provider.h>
#include <qtz/core/qio.h>
#include <QCoreApplication>
#include <QStack>
#include <QFile>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlDriver>
#include <QDate>
#include <QDateTime>
#include <QTime>
#include <QDataStream>
#include <QRegExp>
#include <QStringList>
#include <string>
#include <iostream>

//#include "table-node.h"
#include "data-provider-information.h"

#include <QDebug>

using namespace std;

Database *Database::instance;

Database::Database(QObject *parent):
    QObject(parent),
    m_blockSize(100)
{
}

Database::Database(const Database &other):
    QObject(other.parent()),
    m_blockSize(100)
{
}

Database *Database::getInstance()
{
    if(instance != nullptr) {
        return instance;
    }
    else {
        instance = new Database(nullptr);
        return instance;
    }
}

QSqlDatabase *Database::database()
{
    return &(instance->m_database);
}

void Database::setType(const Type &newType)
{
    if(m_type != newType ) {
        m_type = newType;
        m_database = QSqlDatabase::addDatabase(
                         DataProviderInformation::getInstance()->getDriverName(m_type),
                         "mainDB");
    }
}

Database::Type Database::type()
{
    return m_type;
}

void Database::readConnectionInfo()
{
    Type driver = static_cast<Type>
                  (Settings::getInstance()->value("db:type").toUInt());
    QString driverName = DataProviderInformation::getInstance()->getDriverName(
                             driver);
    instance->m_database = QSqlDatabase::addDatabase(driverName,"mainDB");
    instance->m_type = driver;
    instance->m_database.setHostName(
        Settings::getInstance()->value("db:host").toString());
    instance->m_database.setPort(
        Settings::getInstance()->value("db:port").toInt());
    instance->m_database.setDatabaseName(
        Settings::getInstance()->value("db:database").toString());
    instance->m_database.setUserName(
        Settings::getInstance()->value("db:user").toString());
    instance->m_database.setPassword(
        AuthProvider::instance()->decryptPassword(
            Settings::getInstance()->value("db:password").toString()));
}

void Database::writeConnectionInfo()
{
    Settings::getInstance()->setValue("db:type",
                                      static_cast<quint8>(instance->m_type));
    Settings::getInstance()->setValue("db:host", instance->m_database.hostName());
    Settings::getInstance()->setValue("db:port", instance->m_database.port());
    Settings::getInstance()->setValue("db:database",
                                      instance->m_database.databaseName());
    Settings::getInstance()->setValue("db:user", instance->m_database.userName());
    Settings::getInstance()->setValue(
        "db:password", AuthProvider::instance()->encryptPassword(
            instance->m_database.password()));
}

void Database::setBlockSize(const unsigned int &size)
{
    this->m_blockSize = size;
}

unsigned int Database::blockSize() const
{
    return this->m_blockSize;
}

void Database::backup(const QString &filename,
                      const Database::BackupStrategy &strategy)
{
    switch(strategy) {
    case BackupStrategy::BinaryByVariant:
        backupByVariant(filename);
        break;
    case BackupStrategy::BinaryRuntimeCheck:
        backupByRuntimeCheck(filename);
        break;
    case BackupStrategy::BinaryCompileCheck:
        break;
    case BackupStrategy::TextBased:
        break;
    }
}

void Database::backupByVariant(const QString &filename)
{
    QStringList tables = getTables();
    auto parents = getParents(tables);
    QStringList sortedList = sortTables(parents);
    // Starting task
    QFile backupFile(filename);
    if(backupFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        emit backupStageChanged(tr("Analyzing database information..."));
        uint totalRows = getNumberOfDBRows();
        uint writtenRows = 0;
        QDataStream out(&backupFile);
        emit backupStageChanged(tr("Writing database information..."));
        out << static_cast<quint8>(BackupStrategy::BinaryByVariant);
        out << Database::getInstance()->database()->databaseName(); // Stored as QString
        out << quint32(sortedList.size());
        out << quint32(totalRows);
        emit backupStageChanged(tr("Writing data..."));
        foreach(QString table, sortedList) {
            quint32 fieldCount = getNumberOfTableColumns(table);
            quint32 rowCount = getNumberOfTableRows(table);
            out << table;
            out << quint32(rowCount);
            uint blockCount = rowCount / blockSize();
            if(rowCount % blockSize() != 0) {
                ++blockCount;
            }
            QSqlQuery selectQuery;
            for (uint i = 0; i < blockCount; ++i) {
                selectQuery.prepare(QString("SELECT * FROM %1 LIMIT %2,%3")
                                    .arg(table)
                                    .arg(i * blockSize())
                                    .arg(blockSize()));
                if(selectQuery.exec()) {
                    while(selectQuery.next()) {
                        // First strategy:
                        // Store all data in QVariant format
                        for (uint f = 0; f < fieldCount; ++f) {
                            out << selectQuery.value(f);
                        }
                        ++writtenRows;
                    }
                    emit backupCompleted(100.0 * static_cast<double>(writtenRows) /
                                         static_cast<double>(totalRows));
                }
                else {
                    // TODO: through exception
                }
            }
        } // end of foreach (table)
        backupFile.close();
        emit backupStageChanged(tr("Done."));
        emit backupCompleted();
    }
    else {
        QIO::cerr << tr("Unable to open backup file for writing.") << endl;
        // TODO: Through exception
    }
}

void Database::backupByRuntimeCheck(const QString &filename)
{
    QStringList tables = getTables();
    auto parents = getParents(tables);
    QStringList sortedList = sortTables(parents);
    // Starting task
    QFile backupFile(filename);
    if(backupFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        emit backupStageChanged(tr("Analyzing database information..."));
        uint totalRows = getNumberOfDBRows();
        uint writtenRows = 0;
        QDataStream out(&backupFile);
        emit backupStageChanged(tr("Writing database information..."));
        out << static_cast<quint8>(BackupStrategy::BinaryByVariant);
        out << Database::getInstance()->database()->databaseName(); // Stored as QString
        out << quint32(sortedList.size());
        out << quint32(totalRows);
        emit backupStageChanged(tr("Writing data..."));
        foreach(QString table, sortedList) {
            quint32 fieldCount = getNumberOfTableColumns(table);
            quint32 rowCount = getNumberOfTableRows(table);
            QVector<FieldType> types;
            getTableFiledTypes(table, types);
            out << table;
            out << quint32(rowCount);
            uint blockCount = rowCount / blockSize();
            if(rowCount % blockSize() != 0) {
                ++blockCount;
            }
            QSqlQuery selectQuery;
            for (uint i = 0; i < blockCount; ++i) {
                selectQuery.prepare(QString("SELECT * FROM %1 LIMIT %2,%3")
                                    .arg(table)
                                    .arg(i * blockSize())
                                    .arg(blockSize()));
                if(selectQuery.exec()) {
                    while(selectQuery.next()) {
                        // First strategy:
                        // Store all data in QVariant format
                        for (uint f = 0; f < fieldCount; ++f) {
                            switch(types[f]) {
                            case FieldType::INT_8:
                                out << static_cast<qint8>(selectQuery.value(f).toInt());
                                break;
                            case FieldType::UINT_8:
                                out << static_cast<quint8>(selectQuery.value(f).toUInt());
                                break;
                            case FieldType::INT_16:
                                out << static_cast<qint16>(selectQuery.value(f).toInt());
                                break;
                            case FieldType::UINT_16:
                                out << static_cast<quint16>(selectQuery.value(f).toUInt());
                                break;
                            case FieldType::INT_32:
                                out << static_cast<qint32>(selectQuery.value(f).toInt());
                                break;
                            case FieldType::UINT_32:
                                out << static_cast<quint32>(selectQuery.value(f).toUInt());
                                break;
                            case FieldType::INT_64:
                                out << static_cast<qint64>(selectQuery.value(f).toInt());
                                break;
                            case FieldType::UINT_64:
                                out << static_cast<quint64>(selectQuery.value(f).toUInt());
                                break;
                            case FieldType::BOOL:
                                out << selectQuery.value(f).toBool();
                                break;
                            case FieldType::TEXT:
                                out << QString::fromUtf8(selectQuery.value(f).toByteArray());
                                break;
                            case FieldType::FLOAT:
                                out << selectQuery.value(f).toFloat();
                                break;
                            case FieldType::DOUBLE:
                                out << selectQuery.value(f).toDouble();
                                break;
                            case FieldType::DATE_TIME:
                                out << selectQuery.value(f).toDateTime();
                                break;
                            case FieldType::DATE:
                                out << selectQuery.value(f).toDate();
                                break;
                            case FieldType::TIME:
                                out << selectQuery.value(f).toTime();
                                break;
                            }
                        }
                        ++writtenRows;
                    }
                    emit backupCompleted(100.0 * static_cast<double>(writtenRows) /
                                         static_cast<double>(totalRows));
                }
                else {
                    // TODO: through exception
                }
            }
        } // end of foreach (table)
        backupFile.close();
        emit backupStageChanged(tr("Done."));
        emit backupCompleted();
    }
    else {
        QIO::cerr << tr("Unable to open backup file for writing.") << endl;
        // TODO: Through exception
    }
}

void Database::restore(const QString &filename)
{
    QFile backupFile(filename);
    if(backupFile.open(QIODevice::ReadOnly)) {
        QDataStream in(&backupFile);
        QString schemaName;
        quint32 tableCount, totalRows, restoredRecords = 0;
        quint8 rawBackupType;
        in >> rawBackupType;
        BackupStrategy backupType = static_cast<BackupStrategy>(rawBackupType);
        in >> schemaName;
        in >> tableCount >> totalRows;
        if(schemaName != database()->databaseName()) {
            wcerr << tr("Database names mismatch").toStdWString() << endl;
            return;
        }
        for(quint32 t = 0; t < tableCount; ++t) {
            switch(backupType) {
            case BackupStrategy::BinaryByVariant:
                restoreByVariant(in, totalRows, restoredRecords);
                break;
            case BackupStrategy::BinaryRuntimeCheck:
                break;
            case BackupStrategy::BinaryCompileCheck:
                break;
            case BackupStrategy::TextBased:
                break;
            }
        }
    }
    else {
        QIO::cerr << tr("Unable to open backup file for reading.") << endl;
        // TODO: through exception
    }
}

uint Database::getNumberOfDBRows()
{
    QSqlQuery prepareData;
    if(!prepareData.exec("CALL COUNT_ALL_RECORDS_BY_TABLE")) {
        wcerr << tr("Unable to call stored procedure `CALL COUNT_ALL_RECORDS_BY_TABLE' "
                    "in order to get count of total records of database:").toStdWString()
              << endl;
        wcerr << prepareData.lastError().text().toStdWString() << endl;
        return 0;
    }
    QSqlQuery getData;
    if( getData.exec("SELECT SUM(RECORD_COUNT) AS TOTAL_DATABASE_RECORD_CT FROM TCOUNTS")) {
        getData.next();
        uint result = getData.value(0).toUInt();
        return result;
    }
    return 0;
}

uint Database::getNumberOfTableRows(const QString &tableName)
{
    QString countQueryText = QString("SELECT COUNT(*) from %1").arg(tableName);
    QSqlQuery countQuery;
    countQuery.prepare(countQueryText);
    if(countQuery.exec()) {
        countQuery.next();
        uint result = countQuery.value(0).toUInt();
        return result;
    }
    else {
        wcerr << tr("Unable to fetch number of records in table `%1'").arg(
                  tableName).toStdWString() << endl;
    }
    return 0;
}

uint Database::getNumberOfTableColumns(const QString &tableName)
{
    QString selectFieldCountText = QString(
                                       "SELECT COUNT(*) FROM information_schema.`COLUMNS`"
                                       "WHERE table_name = '%1'"
                                       "AND TABLE_SCHEMA = DATABASE()").arg(tableName);
    QSqlQuery selectFieldCount;
    selectFieldCount.prepare(selectFieldCountText);
    selectFieldCount.exec();
    selectFieldCount.next();
    uint fieldCount = selectFieldCount.value(0).toUInt();
    return fieldCount;
}

void Database::getTableFiledTypes(const QString &tableName,
                                  QVector<FieldType> &types)
{
    QSqlQuery selectFieldType;
    selectFieldType.prepare(QString("DESCRIBE %1").arg(tableName));
    if(!selectFieldType.exec()) {
        wcerr << tr("Unable to get types of fields for table `%1'").arg(
                  tableName).toStdWString() << endl;
        wcerr << selectFieldType.lastError().text().toStdWString() << endl;
        return;
    }
    types.clear();
    int index = selectFieldType.record().indexOf("Type");
#ifdef DEBUG
    qDebug() << "Types of table: " << tableName;
#endif
    while(selectFieldType.next()) {
        QString typeDescription = selectFieldType.value(index).toString();
#ifdef DEBUG
        qDebug() << typeDescription;
#endif
        QRegExp typeExpression;
        typeExpression.setCaseSensitivity(Qt::CaseInsensitive);
        typeExpression.setPattern("BIT(\\(1\\))?|TINYINT\\(1\\)\\s*(unsigned)?|BOOL|BOOLEAN");
        if(typeExpression.exactMatch(typeDescription)) {
            types << FieldType::BOOL;
#ifdef DEBUG
            qDebug() << "Type_BOOL";
#endif
            continue;
        }
        typeExpression.setPattern("TINYINT(\\(\\d\\))?");
        if(typeExpression.exactMatch(typeDescription)) {
            types << FieldType::INT_8;
#ifdef DEBUG
            qDebug() << "Type_I8";
#endif
            continue;
        }
        typeExpression.setPattern("TINYINT(\\(\\d\\))?\\s*(unsigned)");
        if(typeExpression.exactMatch(typeDescription)) {
            types << FieldType::UINT_8;
#ifdef DEBUG
            qDebug() << "Type_UI8";
#endif
            continue;
        }
        typeExpression.setPattern("SMALLINT(\\(\\d\\))?");
        if(typeExpression.exactMatch(typeDescription)) {
            types << FieldType::INT_16;
#ifdef DEBUG
            qDebug() << "Type_I16";
#endif
            continue;
        }
        typeExpression.setPattern("SMALLINT(\\(\\d\\))?\\s*(unsigned)|YEAR\\(2|4\\)");
        if(typeExpression.exactMatch(typeDescription)) {
            types << FieldType::UINT_16;
#ifdef DEBUG
            qDebug() << "Type_UI16";
#endif
            continue;
        }
        typeExpression.setPattern("(MEDIUMINT|INTEGER|INT)(\\(\\d\\))?");
        if(typeExpression.exactMatch(typeDescription)) {
            types << FieldType::INT_32;
#ifdef DEBUG
            qDebug() << "Type_I32";
#endif
            continue;
        }
        typeExpression.setPattern("(MEDIUMINT|INTEGER|INT)(\\(\\d\\))?\\s*(unsigned)");
        if(typeExpression.exactMatch(typeDescription)) {
            types << FieldType::UINT_32;
#ifdef DEBUG
            qDebug() << "Type_UI32";
#endif
            continue;
        }
        typeExpression.setPattern("BIGINT(\\(\\d\\))?");
        if(typeExpression.exactMatch(typeDescription)) {
            types << FieldType::INT_64;
#ifdef DEBUG
            qDebug() << "Type_I64";
#endif
            continue;
        }
        typeExpression.setPattern("BIGINT(\\(\\d\\))?\\s*(unsigned)");
        if(typeExpression.exactMatch(typeDescription)) {
            types << FieldType::UINT_64;
#ifdef DEBUG
            qDebug() << "Type_UI64";
#endif
            continue;
        }
        // Text data
        typeExpression.setPattern("(CHAR|VARCHAR|BINARY|VARBINARY|BLOB|TEXT|TINYTEXT|MEDIUMTEXT|LONGTEXT|ENUM|SET).*");
        if(typeExpression.exactMatch(typeDescription)) {
            types << FieldType::TEXT;
#ifdef DEBUG
            qDebug() << "Type_TEXT";
#endif
            continue;
        }
        // Temporal data
        typeExpression.setPattern("DATE");
        if(typeExpression.exactMatch(typeDescription)) {
            types << FieldType::DATE;
#ifdef DEBUG
            qDebug() << "Type_DATE";
#endif
            continue;
        }
        typeExpression.setPattern("DATETIME|TIMESTAMP");
        if(typeExpression.exactMatch(typeDescription)) {
            types << FieldType::DATE_TIME;
#ifdef DEBUG
            qDebug() << "Type_DATE_TIME";
#endif
            continue;
        }
        typeExpression.setPattern("TIME");
        if(typeExpression.exactMatch(typeDescription)) {
            types << FieldType::TIME;
#ifdef DEBUG
            qDebug() << "Type_TIME";
#endif
            continue;
        }
    }
}

QStringList Database::getTables()
{
    // TODO: Write a caching mechanism
    QStringList tables;
    QSqlQuery fetchTables;
    fetchTables.prepare("SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES where TABLE_SCHEMA=DATABASE()");
    if(fetchTables.exec()) {
        while(fetchTables.next()) {
            tables << fetchTables.value(0).toString();
        }
    }
    qDebug() << tables;
    return tables;
}

QMap<QString, QStringList> Database::getParents(const QStringList &tables)
{
    QMap<QString, QStringList> results;
    QStringList emptyList;
    QSqlQuery getParentsQuery;
    QFile queryFile(":/en/resources/mysql_fk_fetch.sql");
    if(!queryFile.open(QFile::ReadOnly | QFile::Text)) {
        QIO::cerr << tr("Unable to open SQL query file") << endl;
        // TODO: Through exception
    }
    QString genericQueryText = QString::fromUtf8(queryFile.readAll());
    foreach(QString table, tables) {
        results.insert(table,emptyList);
        QString queryText = genericQueryText.arg(table);
        getParentsQuery.prepare(queryText);
        if(getParentsQuery.exec()) {
            while(getParentsQuery.next()) {
                QString parentTableName = getParentsQuery.value(0).toString();
                results[table].append(parentTableName);
            }
        }
        else {
            QIO::cerr << tr("Unable to execute statement") << endl;
            QIO::cerr << getParentsQuery.lastError().text() << endl;
            // TODO: throw new exception
        }
    }
    return results;
}


QStringList Database::sortTables(QMap<QString, QStringList> &inputMap)
{
    QStringList sortedTables;
    QStack<QString> traceStack;
    while (!inputMap.empty()) {
        if(traceStack.empty()) {
            traceStack.push(inputMap.begin().key());
        }
        // Trace to find null
        while (!traceStack.empty()) {
            QString topElement = traceStack.top();
            if(inputMap[topElement].empty()) {
                traceStack.pop();
                sortedTables << topElement;
                inputMap.remove(topElement);
                for(auto i = inputMap.begin(); i!=inputMap.end(); ++i) {
                    i.value().removeAll(topElement);
                }
            }
            else {
                traceStack << inputMap[topElement].first();
            }
        }
    }
    return sortedTables;
}

void Database::restoreByVariant(QDataStream &in, const quint32 &totalRows,
                                quint32 &restoredRecords)
{
    switch(executeMode) {
    case Database::RestoreExecuteMode::Normal:
        restoreVN(in, totalRows, restoredRecords);
        break;
    case Database::RestoreExecuteMode::Batch:
        restoreVB(in, totalRows, restoredRecords);
        break;
    }
}
void Database::restoreVN(QDataStream &in, const quint32 &totalRows,
                         quint32 &restoredRecords)
{
    QString tableName;
    int recordCount;
    in >> tableName;
    in >> recordCount;
    quint32 columnsCount = getNumberOfTableColumns(tableName);
    QSqlQuery insert;
    QString insertRecord = QString("INSERT INTO %1 VALUES (%2)").arg(tableName,
                           "%1");
    for(quint32 r = 0; r < recordCount; ++r) {
        QStringList valueList;
        valueList.clear();
        for(uint c = 0; c < columnsCount; ++c) {
            QVariant value;
            in >> value;
            switch(value.type()) {
            case QVariant::Date:
                valueList << QString("\"%1\"").arg(value.toDate().toString(Qt::ISODate));
                break;
            case QVariant::DateTime:
                valueList << QString("\"%1\"").arg(value.toDateTime().toString(Qt::ISODate));
                break;
            default:
                valueList << QString("\"%1\"").arg(value.toString());
                break;
            }
        }
        if(!insert.prepare(insertRecord.arg(valueList.join(",")))) {
            wcerr << tr("Unable to prepare SQL statement for data restore.").toStdWString()
                  << endl;
            wcerr << insert.lastError().text().toStdWString() << endl;
            return;
        }
        if(!insert.exec()) {
            wcerr << tr("Unable to execure SQL statement to restore data.").toStdWString()
                  << endl;
            wcerr << insert.lastError().text().toStdWString() << endl;
            return;
        }
        ++restoredRecords;
        emit restoreCompleted(static_cast<double>(restoredRecords) /
                              static_cast<double>(totalRows));
    }
}
void Database::restoreVB(QDataStream &in, const quint32 &totalRows,
                         quint32 &restoredRecords)
{
    QString tableName;
    int rowCount;
    in >> tableName;
    in >> rowCount;
    quint32 columnsCount = getNumberOfTableColumns(tableName);
    quint32 blockCount = rowCount / m_blockSize;
    quint32 remaining = rowCount % m_blockSize;
    QVector<QVariantList> data;
    data.resize(columnsCount);
    for(quint32 b = 0; b < blockCount; ++b) {
        foreach(QVariantList list, data)
        list.clear();
        QString insertRecord = QString("INSERT INTO %1 VALUES (%2)").arg(tableName,
                               "%1");
        for(quint32 i = 0; i < columnsCount - 1; ++i) {
            insertRecord = insertRecord.arg("?,%1");
        }
        insertRecord = insertRecord.arg("?");
        QSqlQuery insert;
        insert.prepare(insertRecord);
        for(quint32 r = 0; r < rowCount; ++r) {
            for(uint c = 0; c < columnsCount; ++c) {
                in >> data[c];
            }
        }
        insert.execBatch();
    }
    //    Remaining part of data:
    QSqlQuery insert;
    QString insertRecord = QString("INSERT INTO %1 VALUES (%2);\n").arg(tableName,
                           "%1");
    for(quint32 r = 0; r < remaining; ++r) {
        QStringList valueList;
        valueList.clear();
        for(uint c = 0; c < columnsCount; ++c) {
            QVariant value;
            in >> value;
        }
    }
}

void Database::clearDatabase()
{
    auto tables = getTables();
    foreach (auto table, tables) {
        m_database.exec(QString("DELETE FROM %1").arg(table));
    }
}
