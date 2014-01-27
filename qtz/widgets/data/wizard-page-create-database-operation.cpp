#include "wizard-page-create-database-operation.h"
#include "ui_wizard-page-create-database-operation.h"

WizardPageCreateDatabaseOperation::WizardPageCreateDatabaseOperation(
    QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::WizardPageCreateDatabaseOperation)
{
    ui->setupUi(this);
}

WizardPageCreateDatabaseOperation::~WizardPageCreateDatabaseOperation()
{
    delete ui;
}

void WizardPageCreateDatabaseOperation::changeEvent(QEvent *e)
{
    QWizardPage::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
