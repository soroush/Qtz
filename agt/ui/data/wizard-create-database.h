#ifndef WIZARDCREATEDATABASE_H
#define WIZARDCREATEDATABASE_H

#include <QWizard>

namespace Ui {
class WizardCreateDatabase;
}

class WizardCreateDatabase : public QWizard
{
    Q_OBJECT
    
public:
    explicit WizardCreateDatabase(QWidget *parent = 0);
    ~WizardCreateDatabase();
    
protected:
    void changeEvent(QEvent *e);
    
private:
    Ui::WizardCreateDatabase *ui;
};

#endif // WIZARDCREATEDATABASE_H
