#ifndef WIZARDPAGECREATEDATABASEOPERATION_H
#define WIZARDPAGECREATEDATABASEOPERATION_H

#include <QWizardPage>

namespace Ui {
class WizardPageCreateDatabaseOperation;
}

class WizardPageCreateDatabaseOperation : public QWizardPage {
    Q_OBJECT

public:
    explicit WizardPageCreateDatabaseOperation(QWidget *parent = 0);
    ~WizardPageCreateDatabaseOperation();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::WizardPageCreateDatabaseOperation *ui;
};

#endif // WIZARDPAGECREATEDATABASEOPERATION_H
