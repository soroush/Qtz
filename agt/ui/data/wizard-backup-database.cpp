#include "wizard-backup-database.h"
#include "ui_wizard-backup-database.h"

WizardBackupDatabase::WizardBackupDatabase(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::WizardBackupDatabase)
{
    ui->setupUi(this);
    QPixmap logo = QPixmap(":/images/database-backup.png");
    this->setPixmap(LogoPixmap,logo);
}

WizardBackupDatabase::~WizardBackupDatabase()
{
    delete ui;
}

void WizardBackupDatabase::changeEvent(QEvent *e)
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
