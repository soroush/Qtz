#ifndef WIZARDCREATEDATABASE_H
#define WIZARDCREATEDATABASE_H

#include <QWizard>

QT_BEGIN_NAMESPACE
namespace Ui {
class WizardCreateDatabase;
}
QT_END_NAMESPACE

class WizardCreateDatabase : public QWizard
{
    Q_OBJECT
    
public:
    enum class Page {intro, };
    explicit WizardCreateDatabase(QWidget *parent = 0);
    ~WizardCreateDatabase();
    bool SqlVisibility();

public slots:
    void setSqlVisibility(bool);
    
protected:
    void changeEvent(QEvent *e);
    
private:
    int nextId() const;
    Ui::WizardCreateDatabase *ui;
    bool m_sqlVisibility;
};

#endif // WIZARDCREATEDATABASE_H
