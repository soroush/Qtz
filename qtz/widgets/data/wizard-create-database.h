#ifndef WIZARDCREATEDATABASE_H
#define WIZARDCREATEDATABASE_H

#include <QWizard>

class WizardPageCreateDatabaseOperation;

class WizardCreateDatabase : public QWizard {
    Q_OBJECT
public:
    explicit WizardCreateDatabase(QWidget *parent = 0);
public:
    void setSqlFile(const QString& filename);
    void setSqlContent(const QString& filename);
private:
    WizardPageCreateDatabaseOperation* m_operationPage;
};

#endif // WIZARDCREATEDATABASE_H
