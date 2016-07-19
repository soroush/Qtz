<<<<<<< HEAD
#include "wizard-page-create-database-configure.h"
#include "ui_wizard-page-create-database-configure.h"

#include <QSet>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <qtz/data/data-provider-information.h>

#include <QDebug>

WizardPageCreateDatabaseConfigure::WizardPageCreateDatabaseConfigure(
    QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::WizardPageCreateDatabaseConfigure),
    m_connected(false)
{
    ui->setupUi(this);
    initializeDatabaseSystems();
    createConnections();
    updateDatabaseType(0);
    registerField("provider",ui->comboBoxDatabaseType);
    registerField("host",ui->lineEditHost);
    registerField("port",ui->spinBoxPort);
    registerField("database",ui->lineEditDatabase);
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

void WizardPageCreateDatabaseConfigure::initializeDatabaseSystems()
{
    QVector<DataProvider> systems =
        DataProviderInformation::getInstance()->getSupportedProviders();
    foreach (DataProvider p, systems) {
        ui->comboBoxDatabaseType->addItem(p.providerName(),p.providerCode());
    }
    // Establish first valid state:
    quint8 type = ui->comboBoxDatabaseType->itemData(
                      ui->comboBoxDatabaseType->currentIndex()
                ).toUInt();
    currentType = static_cast<Database::Type>(type);
    quint32 port = DataProviderInformation::getInstance()->getProviderInfo(
                       currentType).defaultPort();
    this->lastCustomPort = port;
    ui->spinBoxPort->setValue(port);
}

void WizardPageCreateDatabaseConfigure::createConnections()
{
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
    connect(ui->comboBoxDatabaseType, SIGNAL(currentIndexChanged(int)),this,
            SIGNAL(completeChanged()));
    connect(ui->lineEditHost, SIGNAL(textChanged(QString)),this,
            SIGNAL(completeChanged()));
    connect(ui->spinBoxPort, SIGNAL(valueChanged(int)),this,
            SIGNAL(completeChanged()));
    connect(ui->lineEditDatabase, SIGNAL(textChanged(QString)),this,
            SIGNAL(completeChanged()));
    connect(ui->lineEditUser, SIGNAL(textChanged(QString)),this,
            SIGNAL(completeChanged()));
    connect(ui->lineEditPassword, SIGNAL(textChanged(QString)),this,
            SIGNAL(completeChanged()));
    connect(ui->checkBoxRemoveExisting, SIGNAL(stateChanged(int)),this,
            SIGNAL(completeChanged()));
}

bool WizardPageCreateDatabaseConfigure::isComplete() const
{
    if(currentType == Database::Type::SQLite) {
        // TODO: Check existence and override
        return true;
    }
    else if(
        field("host").toString().isEmpty() ||
        field("database").toString().isEmpty() ||
        field("username").toString().isEmpty()
    ) {
        return false;
    }
    return true;
}

bool WizardPageCreateDatabaseConfigure::validatePage()
{
    // FIXME: Move all staff into a new thread to prevent GUI locks
    // Last minute property sets:
    quint8 dbTypeCode =
        ui->comboBoxDatabaseType->itemData(
            ui->comboBoxDatabaseType->currentIndex()).toUInt();
    wizard()->setProperty("providerCode",
                          dbTypeCode);
    // check database connection:
    QSqlDatabase db;
    Database::Type dbType = static_cast<Database::Type>(dbTypeCode);
    switch (dbType) {
    case Database::Type::MySQL5:
        db= QSqlDatabase::addDatabase("QMYSQL","testConnection");
        break;
    case Database::Type::SQLite:
        break;
    case Database::Type::SQLServer:
    case Database::Type::SQLServer2005:
    case Database::Type::SQLServer2008:
    case Database::Type::SQLServer2010:
    case Database::Type::SQLServer2012:
        // TODO: Implement Microsoft SQL Server Backend
        break;
    default:
        break;
    }
    // TODO: Implement SSL backend
    db.setHostName(field("host").toString());
    db.setDatabaseName(""); // Connect to provider, not a database
    db.setUserName(field("username").toString());
    db.setPassword(field("password").toString());
    db.setPort(field("password").toUInt());
    bool isOk = db.open();
    isOk = isOk & db.isValid();
    if(isOk) {
        // FIXME: This only works for MySQL
        // Check for database existence
        db.close();
        db.setDatabaseName("information_schema");
        qDebug() << "dbopen" << db.open();
        qDebug() << db.isOpen();
        //FIXME: QSqlQuery does not work here. Prevent SQL injections:
        QSqlQuery query{db};
        query.prepare("SELECT SCHEMA_NAME FROM INFORMATION_SCHEMA.SCHEMATA WHERE SCHEMA_NAME = :database");
        query.bindValue(":database",field("database").toString());
        if(query.exec()) {
            qDebug() << query.executedQuery();
            bool databaseExists = query.next();
            qDebug() << "databaseExists: " << databaseExists;
            if(databaseExists && !ui->checkBoxRemoveExisting->isChecked()) {
                QMessageBox::critical(this,tr("Database Exists")
                                      ,tr("Database `%1' already exists and you did't choose to "
                                          "remove old database").arg(field("database").toString())
                                     );
                isOk = false;
                qDebug() << "We have next";
            }
        }
        else { // Query does not execute successfully
            QMessageBox::critical(this,tr("Database Error")
                                  ,tr("Unable to execute provided query. Database Management System reported "
                                      "following errors:\n%1").arg(db.lastError().text())
                                 );
            isOk=false;
        }
    }
    else {
        QMessageBox::critical(this,tr("Database Error")
                              ,tr("Unabel to connect to database. Database Management System has reported following error(s):")
                              +
                              db.lastError().text()
                             );
    }
    return isOk;
}

void WizardPageCreateDatabaseConfigure::updateLocalHostStatus(bool checked)
{
    if(checked) {
        this->lastCustomHost = ui->lineEditHost->text();
        ui->lineEditHost->setText(
            DataProviderInformation::getInstance()->getProviderInfo(
                currentType).defaultHost());
    }
    else {
        ui->lineEditHost->setText(lastCustomHost);
    }
}

void WizardPageCreateDatabaseConfigure::updateDefaultPortStatus(bool checked)
{
    if(checked) {
        this->lastCustomPort = ui->spinBoxPort->value();
        if((quint8)currentType!=0) {
            quint32 port = DataProviderInformation::getInstance()->getProviderInfo(
                               currentType).defaultPort();
            ui->spinBoxPort->setValue(port);
        }
    }
    else {
        ui->spinBoxPort->setValue(lastCustomPort);
    }
}

void WizardPageCreateDatabaseConfigure::updateDatabaseType(int i)
{
    currentType = static_cast<Database::Type>(ui->comboBoxDatabaseType->itemData(
                      i).toUInt());
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
=======
#include "wizard-page-create-database-configure.h"
#include "ui_wizard-page-create-database-configure.h"

#include <QSet>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <qtz/core/settings.h>
#include <qtz/data/database.h>
#include <qtz/data/data-provider-information.h>

#include <QDebug>

WizardPageCreateDatabaseConfigure::WizardPageCreateDatabaseConfigure(
    QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::WizardPageCreateDatabaseConfigure),
    m_connected(false)
{
    ui->setupUi(this);
    initializeDatabaseSystems();
    createConnections();
    updateDatabaseType(0);
    registerField("provider",ui->comboBoxDatabaseType);
    registerField("host",ui->lineEditHost);
    registerField("port",ui->spinBoxPort);
    registerField("database",ui->lineEditDatabase);
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

void WizardPageCreateDatabaseConfigure::initializeDatabaseSystems()
{
    QVector<DataProvider> systems =
        DataProviderInformation::getInstance()->getSupportedProviders();
    foreach (DataProvider p, systems) {
        ui->comboBoxDatabaseType->addItem(p.providerName(),p.providerCode());
    }
    // Establish first valid state:
    quint8 type = ui->comboBoxDatabaseType->itemData(
                      ui->comboBoxDatabaseType->currentIndex()
                ).toUInt();
    currentType = static_cast<Database::Type>(type);
    quint32 port = DataProviderInformation::getInstance()->getProviderInfo(
                       currentType).defaultPort();
    this->lastCustomPort = port;
    ui->spinBoxPort->setValue(port);
}

void WizardPageCreateDatabaseConfigure::createConnections()
{
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
    connect(ui->comboBoxDatabaseType, SIGNAL(currentIndexChanged(int)),this,
            SIGNAL(completeChanged()));
    connect(ui->lineEditHost, SIGNAL(textChanged(QString)),this,
            SIGNAL(completeChanged()));
    connect(ui->spinBoxPort, SIGNAL(valueChanged(int)),this,
            SIGNAL(completeChanged()));
    connect(ui->lineEditDatabase, SIGNAL(textChanged(QString)),this,
            SIGNAL(completeChanged()));
    connect(ui->lineEditUser, SIGNAL(textChanged(QString)),this,
            SIGNAL(completeChanged()));
    connect(ui->lineEditPassword, SIGNAL(textChanged(QString)),this,
            SIGNAL(completeChanged()));
    connect(ui->checkBoxRemoveExisting, SIGNAL(stateChanged(int)),this,
            SIGNAL(completeChanged()));
}

bool WizardPageCreateDatabaseConfigure::isComplete() const
{
    if(currentType == Database::Type::SQLite) {
        // TODO: Check existence and override
        return true;
    }
    else if(
        field("host").toString().isEmpty() ||
        field("database").toString().isEmpty() ||
        field("username").toString().isEmpty()
    ) {
        return false;
    }
    return true;
}

bool WizardPageCreateDatabaseConfigure::validatePage()
{
    // FIXME: Move all staff into a new thread to prevent GUI locks
    // Last minute property sets:
    quint8 dbTypeCode =
        ui->comboBoxDatabaseType->itemData(
            ui->comboBoxDatabaseType->currentIndex()).toUInt();
    wizard()->setProperty("providerCode",
                          dbTypeCode);
    // check database connection:
    QSqlDatabase db;
    Database::Type dbType = static_cast<Database::Type>(dbTypeCode);
    switch (dbType) {
    case Database::Type::MySQL5:
        db= QSqlDatabase::addDatabase("QMYSQL","testConnection");
        break;
    case Database::Type::SQLite:
        break;
    case Database::Type::SQLServer:
    case Database::Type::SQLServer2005:
    case Database::Type::SQLServer2008:
    case Database::Type::SQLServer2010:
    case Database::Type::SQLServer2012:
        // TODO: Implement Microsoft SQL Server Backend
        break;
    default:
        break;
    }
    // TODO: Implement SSL backend
    db.setHostName(field("host").toString());
    db.setDatabaseName(""); // Connect to provider, not a database
    db.setUserName(field("username").toString());
    db.setPassword(field("password").toString());
    db.setPort(field("password").toUInt());
    bool isOk = db.open();
    isOk = isOk & db.isValid();
    if(isOk) {
        // FIXME: This only works for MySQL
        // Check for database existence
        db.close();
        db.setDatabaseName("information_schema");
        qDebug() << "dbopen" << db.open();
        qDebug() << db.isOpen();
        //FIXME: QSqlQuery does not work here. Prevent SQL injections:
        QSqlQuery query{db};
        query.prepare("SELECT SCHEMA_NAME FROM INFORMATION_SCHEMA.SCHEMATA WHERE SCHEMA_NAME = :database");
        query.bindValue(":database",field("database").toString());
        if(query.exec()) {
            qDebug() << query.executedQuery();
            bool databaseExists = query.next();
            qDebug() << "databaseExists: " << databaseExists;
            if(databaseExists && !ui->checkBoxRemoveExisting->isChecked()) {
                QMessageBox::critical(this,tr("Database Exists")
                                      ,tr("Database `%1' already exists and you did't choose to "
                                          "remove old database").arg(field("database").toString())
                                     );
                isOk = false;
                qDebug() << "We have next";
            }
        }
        else { // Query does not execute successfully
            QMessageBox::critical(this,tr("Database Error")
                                  ,tr("Unable to execute provided query. Database Management System reported "
                                      "following errors:\n%1").arg(db.lastError().text())
                                 );
            isOk=false;
        }
    }
    else {
        QMessageBox::critical(this,tr("Database Error")
                              ,tr("Unabel to connect to database. Database Management System has reported following error(s):")
                              +
                              db.lastError().text()
                             );
    }
    return isOk;
}

void WizardPageCreateDatabaseConfigure::updateLocalHostStatus(bool checked)
{
    if(checked) {
        this->lastCustomHost = ui->lineEditHost->text();
        ui->lineEditHost->setText(
            DataProviderInformation::getInstance()->getProviderInfo(
                currentType).defaultHost());
    }
    else {
        ui->lineEditHost->setText(lastCustomHost);
    }
}

void WizardPageCreateDatabaseConfigure::updateDefaultPortStatus(bool checked)
{
    if(checked) {
        this->lastCustomPort = ui->spinBoxPort->value();
        if((quint8)currentType!=0) {
            quint32 port = DataProviderInformation::getInstance()->getProviderInfo(
                               currentType).defaultPort();
            ui->spinBoxPort->setValue(port);
        }
    }
    else {
        ui->spinBoxPort->setValue(lastCustomPort);
    }
}

void WizardPageCreateDatabaseConfigure::updateDatabaseType(int i)
{
    currentType = static_cast<Database::Type>(ui->comboBoxDatabaseType->itemData(
                      i).toUInt());
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
>>>>>>> 52b95dabc0b7249c705c50e06fcb21a168b6bbff
