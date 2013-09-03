#include "data/wizard-create-database.h"
#include "ui_wizard-create-database.h"

WizardCreateDatabase::WizardCreateDatabase(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::WizardCreateDatabase)
{
    ui->setupUi(this);
}

WizardCreateDatabase::~WizardCreateDatabase()
{
    delete ui;
}

void WizardCreateDatabase::changeEvent(QEvent *e)
{
    QWizard::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
