#ifndef WIZARDBACKUPDATABASE_H
#define WIZARDBACKUPDATABASE_H

#include <QWizard>
#include <qtz/core/library.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class WizardBackupDatabase;
}
QT_END_NAMESPACE

class QTZ_SHARED_EXPORT WizardBackupDatabase : public QWizard {
    Q_OBJECT

public:
    explicit WizardBackupDatabase(QWidget *parent = 0);
    ~WizardBackupDatabase();
    bool validateCurrentPage();

protected:
    void changeEvent(QEvent *e);

private:
    enum class Page{
        Intro = 0,
        DataSource = 1,
        Settings = 2,
        Verify = 3,
        Operation = 4
    };

    Ui::WizardBackupDatabase *ui;

    bool tested;
    bool connected;

    void initializeDatabaseSystems();
    bool checkSource() const;
    bool checkSettings() const;

private slots:
    void updateDatasource(int);
    void testConnection();
    void handleTestConnection();
};

#endif // WIZARDBACKUPDATABASE_H
