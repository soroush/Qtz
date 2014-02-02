#ifndef WIZARDPAGECREATEDATABASEOPERATION_H
#define WIZARDPAGECREATEDATABASEOPERATION_H

#include <QWizardPage>

QT_BEGIN_NAMESPACE
namespace Ui {
class WizardPageCreateDatabaseOperation;
}
QT_END_NAMESPACE

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
