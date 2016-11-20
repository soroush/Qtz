#ifndef WIZARDPAGECREATEDATABASEINTRO_H
#define WIZARDPAGECREATEDATABASEINTRO_H

#include <QWizardPage>
#include "qtz-widgets.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class WizardPageCreateDatabaseIntro;
}
QT_END_NAMESPACE

class QTZ_WIDGETS_SHARED_EXPORT WizardPageCreateDatabaseIntro : public QWizardPage {
    Q_OBJECT

public:
    explicit WizardPageCreateDatabaseIntro(QWidget *parent = 0);
    ~WizardPageCreateDatabaseIntro();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::WizardPageCreateDatabaseIntro *ui;
};

#endif // WIZARDPAGECREATEDATABASEINTRO_H
