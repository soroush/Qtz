#include "wizard-page-create-database-confirm.h"
#include "ui_wizard-page-create-database-confirm.h"
#include <qtz/data/data-provider-information.h>

WizardPageCreateDatabaseConfirm::WizardPageCreateDatabaseConfirm(
    QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::WizardPageCreateDatabaseConfirm)
{
    ui->setupUi(this);
    connect(ui->toolButtonShowHide,SIGNAL(toggled(bool)),this,
            SLOT(showHidePasswrod(bool)));
    setCommitPage(true);
    setButtonText(QWizard::CommitButton,tr("Create"));
}

WizardPageCreateDatabaseConfirm::~WizardPageCreateDatabaseConfirm()
{
    delete ui;
}

void WizardPageCreateDatabaseConfirm::changeEvent(QEvent *e)
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

void WizardPageCreateDatabaseConfirm::initializePage()
{
    DataProvider::Type providerType = static_cast<DataProvider::Type>
                                  (wizard()->property("providerCode").toUInt());
    QString providerText = DataProviderInformation::getInstance()->getProviderInfo(
                               providerType).providerName();
    if(providerType != DataProvider::Type::SQLite) {
        ui->stackedWidget->setCurrentIndex(0);
        ui->lineEditProvider->setText(providerText);
        ui->lineEditHost->setText(field("host").toString());
        ui->lineEditPort->setText(field("port").toString());
        ui->lineEditDatabase->setText(field("database").toString());
        ui->lineEditUsername->setText(field("username").toString());
        ui->lineEditPassword->setText(field("password").toString());
    }
    else {
        ui->stackedWidget->setCurrentIndex(1);
    }
}

void WizardPageCreateDatabaseConfirm::showHidePasswrod(bool showPassword)
{
    if(showPassword) {
        ui->lineEditPassword->setEchoMode(QLineEdit::Normal);
    }
    else {
        ui->lineEditPassword->setEchoMode(QLineEdit::Password);
    }
}
