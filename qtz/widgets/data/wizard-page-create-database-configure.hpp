#ifndef WIZARDPAGECREATEDATABASECONFIGURE_H
#define WIZARDPAGECREATEDATABASECONFIGURE_H

#include <QWizardPage>
#include <qtz/data/data-provider.h>
#include "qtz-widgets.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class QTZ_WIDGETS_SHARED_EXPORT WizardPageCreateDatabaseConfigure;
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
    bool m_connected;

    void initializeDatabaseSystems();
    void createConnections();
    bool isComplete() const;
    bool validatePage();

    DataProvider::Type currentType;
    QString lastCustomHost;
    quint32 lastCustomPort;

private slots:
    void updateDatabaseType(int);
    void updateLocalHostStatus(bool);
    void updateDefaultPortStatus(bool);
};

#endif // WIZARDPAGECREATEDATABASECONFIGURE_H
