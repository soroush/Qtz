#include "wizard-backup-database.h"
#include "ui_wizard-backup-database.h"
#include <qtz/data/database.h>
#include <qtz/data/data-provider.h>
#include <qtz/data/data-provider-information.h>
#include <QMessageBox>

#include <QDebug>

WizardBackupDatabase::WizardBackupDatabase(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::WizardBackupDatabase)
{
    ui->setupUi(this);
    QPixmap logo = QPixmap(":/images/database-backup.png");
    this->setPixmap(LogoPixmap, logo);
    // create connections:
    connect(ui->comboBoxDataSource, SIGNAL(currentIndexChanged(int)),this,
            SLOT(updateDatasource(int)));
    // initialize data systems
    initializeDatabaseSystems();
    // initialize ui by fake signal emmitions
    updateDatasource(ui->comboBoxDataSource->currentIndex());
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

void WizardBackupDatabase::initializeDatabaseSystems()
{
    QVector<DataProvider> systems =
        DataProviderInformation::getInstance()->getSupportedProviders();
    foreach (DataProvider p, systems) {
        ui->comboBoxDatabaseType->addItem(p.providerName(),p.providerCode());
    }
}

bool WizardBackupDatabase::validateCurrentPage()
{
    Page currentPageType = static_cast<Page>(currentId());
    switch (currentPageType) {
    case Page::Intro:
        return true;
    case Page::DataSource:
        return checkSource();
    case Page::Settings:
        return true;
    case Page::Verify:
        return true;
    case Page::Operation:
        return true;
    default:
        return true;
    }
}

bool WizardBackupDatabase::checkSource() const
{
    switch(ui->comboBoxDataSource->currentIndex()) {
    case 0: // Current Database
        if(Database::getInstance()->database()->isOpen()) {
            return true;
        }
        else {
            QMessageBox::critical(nullptr,
                                  tr("Database Error"),
                                  tr("Could not connect to database."),
                                  QMessageBox::Ok);
            return false;
        }
    case 1: {
            //            QSqlDatabase::database("testDB").close();
            //            QSqlDatabase db = QSqlDatabase::addDatabase("testDB");
            //            // ...
            //            bool isOk = db.open();
            //            isOk = isOk && db.isValid();
            //            return isOk;
        }
        ;
    default:
        return false;
    }
}

bool WizardBackupDatabase::checkSettings() const
{
    return true;
}

void WizardBackupDatabase::updateDatasource(int dataSource)
{
    switch(dataSource) {
    case 0: // Current database
        ui->comboBoxDatabaseType->hide();
        ui->labelDatabaseType->hide();
        ui->stackedWidgetDatabases->hide();
        break;
    case 1: // Current database
        ui->comboBoxDatabaseType->show();
        ui->labelDatabaseType->show();
        ui->stackedWidgetDatabases->show();
        break;
    default:
        break;
    }
}

void WizardBackupDatabase::testConnection()
{
}

void WizardBackupDatabase::handleTestConnection()
{
}
