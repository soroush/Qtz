#ifndef WIZARDPAGECREATEDATABASECONFIGURE_H
#define WIZARDPAGECREATEDATABASECONFIGURE_H

#include <QWizardPage>
#include <qtz/data/database.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class WizardPageCreateDatabaseConfigure;
}
QT_END_NAMESPACE

class WizardPageCreateDatabaseConfigure : public QWizardPage {
    Q_OBJECT

public:
    explicit WizardPageCreateDatabaseConfigure(QWidget *parent = 0);
    ~WizardPageCreateDatabaseConfigure();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::WizardPageCreateDatabaseConfigure *ui;

    void initializeDatabaseSystems();
    void createConnections();
    bool isComplete() const;
    bool validatePage();

    Database::Type currentType;
    QString lastCustomHost;
    quint32 lastCustomPort;

private slots:
    void updateDatabaseType(int);
    void updateLocalHostStatus(bool);
    void updateDefaultPortStatus(bool);
};

#endif // WIZARDPAGECREATEDATABASECONFIGURE_H