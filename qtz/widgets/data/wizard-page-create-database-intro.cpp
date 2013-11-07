#include "wizard-page-create-database-intro.h"
#include "ui_wizard-page-create-database-intro.h"

WizardPageCreateDatabaseIntro::WizardPageCreateDatabaseIntro(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::WizardPageCreateDatabaseIntro)
{
    ui->setupUi(this);
}

WizardPageCreateDatabaseIntro::~WizardPageCreateDatabaseIntro()
{
    delete ui;
}

void WizardPageCreateDatabaseIntro::changeEvent(QEvent *e)
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
