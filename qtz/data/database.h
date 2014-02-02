#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QList>
#include <QQueue>
#include <QVector>
#include <QSqlDatabase>
#include <QMap>
#include <qtz/core/library.h>

QT_BEGIN_NAMESPACE
//class QMap<QString, QStringList>;
QT_END_NAMESPACE

class QTZ_SHARED_EXPORT Database : public QObject {
    Q_OBJECT
    friend class DatabaseTest;
private:
    Database(QObject *parent = nullptr);
    Database(const Database &other);

public:
    enum class BackupStrategy {
        TextBased           = 01,
        BinaryByVariant     = 02,
        BinaryRuntimeCheck  = 03,
        BinaryCompileCheck  = 04
    };

    enum class FieldType {
        BOOL,
        INT_8,
        INT_16,
        INT_32,
        INT_64,
        UINT_8,
        UINT_16,
        UINT_32,
        UINT_64,
        FLOAT,
        DOUBLE,
        TEXT,
        DATE_TIME,
        DATE,
        TIME
    };

    enum class RestoreExecuteMode {
        Normal,
        Batch
    };

    enum class Type {
        SQLServer2005 = 0x01,
        SQLServer2008 = 0x02,
        SQLServer2010 = 0x03,
        SQLServer2012 = 0x04,
        MySQL5       = 0x05,
        SQLite       = 0x06,
        SQLServer    = 0x07
    };

    //    static void setInstance(const QSqlDatabase &database, bool destroy = false);
    /**
     * @brief This is a static method to retreive the single instance of @ref Database class.
     * @return A poniter to Database instance->
     */
    static Database *getInstance();
    /**
     * @brief database
     * @return A pointer to underlying QSqlDatabase object.
     */
    QSqlDatabase *database();
    void setType(const Type &);
    Type type();
    void readConnectionInfo();
    void writeConnectionInfo();
    void setBlockSize(const unsigned int &size);
    unsigned int blockSize() const;

    void backup(const QString &filename,
                const BackupStrategy &strategy = BackupStrategy::BinaryByVariant);
    void backupByVariant(const QString &filename);
    void backupByRuntimeCheck(const QString &filename);
    void restore(const QString &filename);

    QList<Type> getSupportedSystems();

signals:
    void backupStageChanged(QString);
    void backupCompleted(double);
    void backupCompleted();
    void restoreCompleted(double);
    void restoreCompleted();

private:
    static Database *instance;

    QSqlDatabase m_database;
    Type m_type;
    unsigned int m_blockSize;
    RestoreExecuteMode executeMode;
    uint getNumberOfDBRows();
    uint getNumberOfTableRows(const QString &tableName);
    uint getNumberOfTableColumns(const QString &tableName);
    void getTableFiledTypes(const QString &tableName, QVector<FieldType> &types);
    QStringList getTables();
    QMap<QString, QStringList> getParents(const QStringList &tables);
    QStringList sortTables(QMap<QString, QStringList> &sortedMap);

    void restoreByVariant(QDataStream &in, const quint32 &totalRows,
                          quint32 &restoredRecords);
    void restoreVN(QDataStream &in, const quint32 &totalRows,
                   quint32 &restoredRecords);
    void restoreVB(QDataStream &in, const quint32 &totalRows,
                   quint32 &restoredRecords);
    void clearDatabase();
};

#endif // DATABASE_H
