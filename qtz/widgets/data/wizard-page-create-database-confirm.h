#ifndef WIZARDPAGECREATEDATABASECONFIRM_H
#define WIZARDPAGECREATEDATABASECONFIRM_H

#include <QWizardPage>
#include "qtz-widgets.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class WizardPageCreateDatabaseConfirm;
}
QT_END_NAMESPACE

class QTZ_WIDGETS_SHARED_EXPORT WizardPageCreateDatabaseConfirm : public QWizardPage {
    Q_OBJECT

public:
    explicit WizardPageCreateDatabaseConfirm(QWidget *parent = 0);
    ~WizardPageCreateDatabaseConfirm();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::WizardPageCreateDatabaseConfirm *ui;
    void initializePage();

private slots:
    void showHidePasswrod(bool);
};

#endif // WIZARDPAGECREATEDATABASECONFIRM_H
