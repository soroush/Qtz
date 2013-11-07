#ifndef WIZARDPAGECREATEDATABASEINTRO_H
#define WIZARDPAGECREATEDATABASEINTRO_H

#include <QWizardPage>

QT_BEGIN_NAMESPACE
namespace Ui {
class WizardPageCreateDatabaseIntro;
}
QT_END_NAMESPACE

class WizardPageCreateDatabaseIntro : public QWizardPage
{
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
