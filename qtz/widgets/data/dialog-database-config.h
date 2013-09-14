#ifndef DIALOGDATABASECONFIG_H
#define DIALOGDATABASECONFIG_H

#include <QDialog>
#include <qtz/data/database.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class DialogDatabaseConfig;
}
QT_END_NAMESPACE

class DialogDatabaseConfig : public QDialog {
    Q_OBJECT

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

public slots:
    void accept();

private slots:
    void updateDatabaseType(int);
    void test();
    void updateLocalHostStatus(bool);
    void updateDefaultPortStatus(bool);
};

#endif // DIALOGDATABASECONFIG_H
