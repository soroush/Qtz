#ifndef DIALOGDATABASECONFIG_H
#define DIALOGDATABASECONFIG_H

#include <QDialog>
#include <QFutureWatcher>
#include <QFuture>
#include <QSqlDatabase>
#include <qtz/data/database.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class DialogDatabaseConfig;
}
QT_END_NAMESPACE

class DialogDatabaseConfig : public QDialog {
    Q_OBJECT
    friend class GuiTest;
public:
    explicit DialogDatabaseConfig(QWidget *parent = 0);
    ~DialogDatabaseConfig();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogDatabaseConfig *ui;
    void initializeDatabaseSystems();
    void createConnections();
    bool testConnection();
    void establishActualConnection();
    void readConnectionInfo();
    void writeConnectionInfo();
    void clearConnectionInfo();

    Database::Type currentType;
    bool tested;
    bool connected;
    QString lastCustomHost;
    quint32 lastCustomPort;
    QString lastSSLCA;
    QString lastSSLCert;
    QString lastSSLKey;
    // TODO: Move data operations into another thread
    QFutureWatcher<bool> FW_testDBOpen;
    QFutureWatcher<bool> FW_mainDBOpen;
    QFuture<bool> F_testDBOpen;
    QFuture<bool> F_mainDBOpen;

    QSqlDatabase testDB;

public slots:
    void accept();

private slots:
    void updateDatabaseType(int);
    void test();
    void handleTestResult();
    void handleActualConnection();
    void updateLocalHostStatus(bool);
    void updateDefaultPortStatus(bool);
    void updateSecurityOption(int);
    void modifyWindow();
    void lockGUI();
    void releaseGUI();

private:
    bool testDBOpen();
};

#endif // DIALOGDATABASECONFIG_H
