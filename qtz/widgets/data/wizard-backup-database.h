#ifndef WIZARDBACKUPDATABASE_H
#define WIZARDBACKUPDATABASE_H

#include <QWizard>

QT_BEGIN_NAMESPACE
namespace Ui {
class WizardBackupDatabase;
}
QT_END_NAMESPACE

class WizardBackupDatabase : public QWizard {
    Q_OBJECT

public:
    explicit WizardBackupDatabase(QWidget *parent = 0);
    ~WizardBackupDatabase();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::WizardBackupDatabase *ui;
};

#endif // WIZARDBACKUPDATABASE_H
