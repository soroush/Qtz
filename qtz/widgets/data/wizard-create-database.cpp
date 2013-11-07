#include "wizard-create-database.h"
#include "wizard-page-create-database-intro.h"
#include "wizard-page-create-database-configure.h"
#include "wizard-page-create-database-confirm.h"
#include "wizard-page-create-database-operation.h"

WizardCreateDatabase::WizardCreateDatabase(QWidget *parent) :
    QWizard(parent)
{
    this->setWizardStyle(QWizard::ModernStyle);
    this->addPage(new WizardPageCreateDatabaseIntro(this));
    this->addPage(new WizardPageCreateDatabaseConfigure(this));
    this->addPage(new WizardPageCreateDatabaseConfirm(this));
    this->addPage(new WizardPageCreateDatabaseOperation(this));
}
