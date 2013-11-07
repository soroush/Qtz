#include "wizard-page-create-database-configure.h"
#include "ui_wizard-page-create-database-configure.h"

#include <QSet>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include <qtz/core/settings.h>
#include <qtz/data/database.h>
#include <qtz/data/data-provider-information.h>

#include <QDebug>

WizardPageCreateDatabaseConfigure::WizardPageCreateDatabaseConfigure(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::WizardPageCreateDatabaseConfigure)
{
    ui->setupUi(this);
    initializeDatabaseSystems();
    createConnections();
    updateDatabaseType(0);


    registerField("provider",ui->comboBoxDatabaseType);
    registerField("host",ui->lineEditHost);
    registerField("port",ui->spinBoxPort);
    registerField("name",ui->lineEditDatabase);
    registerField("removeExisting",ui->checkBoxRemoveExisting);
    registerField("username",ui->lineEditUser);
    registerField("password",ui->lineEditPassword);
    registerField("dbname-local",ui->lineEditDatabaseLocation);
    registerField("remember",ui->checkBoxRemember);
}

WizardPageCreateDatabaseConfigure::~WizardPageCreateDatabaseConfigure()
{
    delete ui;
}

void WizardPageCreateDatabaseConfigure::changeEvent(QEvent *e)
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

void WizardPageCreateDatabaseConfigure::initializeDatabaseSystems() {
    QVector<DataProvider> systems =
            DataProviderInformation::getInstance()->getSupportedProviders();
    foreach (DataProvider p, systems) {
        ui->comboBoxDatabaseType->addItem(p.providerName(),p.providerCode());
    }
}

void WizardPageCreateDatabaseConfigure::createConnections() {
    connect(ui->comboBoxDatabaseType, SIGNAL(currentIndexChanged(int)), this,
            SLOT(updateDatabaseType(int)));
    connect(ui->checkBoxLocal, SIGNAL(toggled(bool)), ui->lineEditHost,
            SLOT(setDisabled(bool)));
    connect(ui->checkBoxLocal, SIGNAL(toggled(bool)), this ,
            SLOT(updateLocalHostStatus(bool)));
    connect(ui->checkBoxDefaultPort, SIGNAL(toggled(bool)), ui->spinBoxPort,
            SLOT(setDisabled(bool)));
    connect(ui->checkBoxDefaultPort, SIGNAL(toggled(bool)), this ,
            SLOT(updateDefaultPortStatus(bool)));

    connect(ui->comboBoxDatabaseType, SIGNAL(currentIndexChanged(int)),this,SIGNAL(completeChanged()));
    connect(ui->lineEditHost, SIGNAL(textChanged(QString)),this,SIGNAL(completeChanged()));
    connect(ui->spinBoxPort, SIGNAL(valueChanged(int)),this,SIGNAL(completeChanged()));
    connect(ui->lineEditDatabase, SIGNAL(textChanged(QString)),this,SIGNAL(completeChanged()));
    connect(ui->lineEditUser, SIGNAL(textChanged(QString)),this,SIGNAL(completeChanged()));
    connect(ui->lineEditPassword, SIGNAL(textChanged(QString)),this,SIGNAL(completeChanged()));
    connect(ui->checkBoxRemoveExisting, SIGNAL(stateChanged(int)),this,SIGNAL(completeChanged()));
}

bool WizardPageCreateDatabaseConfigure::isComplete() const
{
    if(currentType == Database::Type::SQLite)
    {
        // TODO: Check existence and override
        return true;
    }
    else if(
               field("host").toString().isEmpty() ||
               field("name").toString().isEmpty() ||
               field("username").toString().isEmpty()
            )
    {
        return false;
    }
    return true;
}

bool WizardPageCreateDatabaseConfigure::validatePage()
{
    wizard()->setProperty("providerCode",ui->comboBoxDatabaseType->itemData(ui->comboBoxDatabaseType->currentIndex()).toUInt());
    return true;
}

void WizardPageCreateDatabaseConfigure::updateLocalHostStatus(bool checked) {
    if(checked) {
        this->lastCustomHost = ui->lineEditHost->text();
        ui->lineEditHost->setText(DataProviderInformation::getInstance()->getProviderInfo(
                                      currentType).defaultHost());
    }
    else {
        ui->lineEditHost->setText(lastCustomHost);
    }
}

void WizardPageCreateDatabaseConfigure::updateDefaultPortStatus(bool checked) {
    if(checked) {
        this->lastCustomPort = ui->spinBoxPort->value();
        if((quint8)currentType!=0){
            quint32 port = DataProviderInformation::getInstance()->getProviderInfo(
                        currentType).defaultPort();
            ui->spinBoxPort->setValue(port);
        }
    }
    else {
        ui->spinBoxPort->setValue(lastCustomPort);
    }
}

void WizardPageCreateDatabaseConfigure::updateDatabaseType(int i) {
    currentType = static_cast<Database::Type>(ui->comboBoxDatabaseType->itemData(i).toUInt());
    switch(currentType) {
    case Database::Type::SQLServer2005:
    case Database::Type::SQLServer2008:
    case Database::Type::SQLServer2010:
    case Database::Type::SQLServer2012:
    case Database::Type::SQLServer:
    case Database::Type::MySQL5:
        ui->stackedWidgetDatabases->setCurrentIndex(0);
        break;
    case Database::Type::SQLite:
        ui->stackedWidgetDatabases->setCurrentIndex(1);
        break;
    default:
        // TODO: Handle error
        break;
    }
    updateLocalHostStatus(ui->checkBoxLocal->isChecked());
    updateDefaultPortStatus(ui->checkBoxDefaultPort->isChecked());
}
