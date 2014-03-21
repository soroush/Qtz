#include "dialog-database-config.h"
#include "ui_dialog-database-config.h"

#include <QSet>
#include <QSqlError>
#include <QMessageBox>
#include <QCursor>
#if QT_VERSION >= 0x050000
#include <QtConcurrent/QtConcurrentRun>
#else
#include <QtConcurrentRun>
#endif

#include <qtz/core/settings.h>
#include <qtz/data/data-provider-information.h>

#include <QDebug>

DialogDatabaseConfig::DialogDatabaseConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDatabaseConfig),
    tested(false), connected(false)
{
    ui->setupUi(this);
    // Remove maximize and minimize buttons
    this->setWindowFlags(Qt::Dialog | Qt::WindowContextHelpButtonHint |
                         Qt::WindowCloseButtonHint);
    initializeDatabaseSystems();
    createConnections();
    bool shouldRemember =
        Settings::getInstance()->value("ui:data:dbconfig:remember").toBool();
    if(shouldRemember) {
        readConnectionInfo();
    }
    updateSecurityOption(ui->comboBoxConnectionSecurity->currentIndex());
    updateDatabaseType(ui->comboBoxDatabaseType->currentIndex());
    setWindowModified(false);
}

DialogDatabaseConfig::~DialogDatabaseConfig()
{
    delete ui;
}

void DialogDatabaseConfig::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void DialogDatabaseConfig::initializeDatabaseSystems()
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

void DialogDatabaseConfig::createConnections()
{
    connect(ui->comboBoxDatabaseType, SIGNAL(currentIndexChanged(int)), this,
            SLOT(updateDatabaseType(int)));
    connect(ui->pushButtonTest, SIGNAL(clicked()), this, SLOT(test()));
    connect(ui->checkBoxLocal, SIGNAL(toggled(bool)), ui->lineEditHost,
            SLOT(setDisabled(bool)));
    connect(ui->checkBoxLocal, SIGNAL(toggled(bool)), this ,
            SLOT(updateLocalHostStatus(bool)));
    connect(ui->checkBoxDefaultPort, SIGNAL(toggled(bool)), ui->spinBoxPort,
            SLOT(setDisabled(bool)));
    connect(ui->checkBoxDefaultPort, SIGNAL(toggled(bool)), this ,
            SLOT(updateDefaultPortStatus(bool)));
    connect(ui->comboBoxConnectionSecurity,SIGNAL(currentIndexChanged(int)),this,
            SLOT(updateSecurityOption(int)));
    // Window Modified flags
    connect(ui->comboBoxDatabaseType, SIGNAL(currentIndexChanged(int)), this,
            SLOT(modifyWindow()));
    connect(ui->checkBoxLocal, SIGNAL(toggled(bool)), this,
            SLOT(modifyWindow()));
    connect(ui->checkBoxDefaultPort, SIGNAL(toggled(bool)), this ,
            SLOT(modifyWindow()));
    connect(ui->spinBoxPort,SIGNAL(valueChanged(int)),this,
            SLOT(modifyWindow()));
    connect(ui->lineEditHost,SIGNAL(textChanged(QString)),this,
            SLOT(modifyWindow()));
    connect(ui->spinBoxPort,SIGNAL(valueChanged(int)),this,
            SLOT(modifyWindow()));
    connect(ui->lineEditDatabase,SIGNAL(textChanged(QString)),this,
            SLOT(modifyWindow()));
    connect(ui->lineEditUser,SIGNAL(textChanged(QString)),this,
            SLOT(modifyWindow()));
    connect(ui->lineEditPassword,SIGNAL(textChanged(QString)),this,
            SLOT(modifyWindow()));
    connect(ui->comboBoxConnectionSecurity, SIGNAL(currentIndexChanged(int)), this,
            SLOT(modifyWindow()));
    // TODO: add signals for choose file
    // Concurrency Connections:
    connect(&FW_testDBOpen,SIGNAL(finished()),this,SLOT(handleTestResult()));
    connect(&FW_testDBOpen,SIGNAL(finished()),this,SLOT(releaseGUI()));
    connect(&FW_mainDBOpen,SIGNAL(finished()),this,SLOT(handleActualConnection()));
    connect(&FW_mainDBOpen,SIGNAL(finished()),this,SLOT(releaseGUI()));
}

void DialogDatabaseConfig::establishActualConnection()
{
    Database::getInstance()->setType(currentType);
    switch(currentType) {
    case Database::Type::SQLServer2005:
        break;
    case Database::Type::SQLServer2008:
        break;
    case Database::Type::SQLServer2010:
        break;
    case Database::Type::SQLServer2012:
        break;
    case Database::Type::MySQL5:
        Database::getInstance()->database()->setHostName(ui->lineEditHost->text());
        Database::getInstance()->database()->setPort(ui->spinBoxPort->value());
        Database::getInstance()->database()->setDatabaseName(
            ui->lineEditDatabase->text());
        Database::getInstance()->database()->setUserName(ui->lineEditUser->text());
        Database::getInstance()->database()->setPassword(ui->lineEditPassword->text());
        lockGUI();
        F_mainDBOpen = QtConcurrent::run(Database::getInstance()->database(),
                                         &QSqlDatabase::open);
        FW_mainDBOpen.setFuture(F_mainDBOpen);
        //Database::getInstance()->database()->open();
        break;
    case Database::Type::SQLite:
        break;
    case Database::Type::SQLServer:
        break;
    }
}

void DialogDatabaseConfig::readConnectionInfo()
{
    if(Database::getInstance() != nullptr) {
        Database::getInstance()->readConnectionInfo();
        currentType = Database::getInstance()->type();
        ui->lineEditHost->setText(Database::getInstance()->database()->hostName());
        int index = ui->comboBoxDatabaseType->findData(static_cast<int>(currentType));
        ui->comboBoxDatabaseType->setCurrentIndex(index);
        updateDatabaseType(index);
        ui->checkBoxLocal->setChecked(
            Settings::getInstance()->value("ui:data:dbconfig:local").toBool());
        ui->checkBoxDefaultPort->setChecked(
            Settings::getInstance()->value("ui:data:dbconfig:defaultPort").toBool());
        ui->checkBoxRemember->setChecked(
            Settings::getInstance()->value("ui:data:dbconfig:remember").toBool());
        ui->spinBoxPort->setValue(Database::getInstance()->database()->port());
        ui->lineEditDatabase->setText(
            Database::getInstance()->database()->databaseName());
        ui->lineEditUser->setText(Database::getInstance()->database()->userName());
        ui->lineEditPassword->setText(Database::getInstance()->database()->password());
    }
}

void DialogDatabaseConfig::writeConnectionInfo()
{
    Database::getInstance()->writeConnectionInfo();
    Settings::getInstance()->setValue("ui:data:dbconfig:local",
                                      ui->checkBoxLocal->isChecked());
    Settings::getInstance()->setValue("ui:data:dbconfig:defaultPort",
                                      ui->checkBoxDefaultPort->isChecked());
    Settings::getInstance()->setValue("ui:data:dbconfig:remember",
                                      ui->checkBoxRemember->isChecked());
}

void DialogDatabaseConfig::clearConnectionInfo()
{
    Settings::getInstance()->setValue("ui:data:dbconfig:remember", false);
}

void DialogDatabaseConfig::accept()
{
    if(isWindowModified()) {
        if(tested) {
            if(connected) {
                establishActualConnection();
            }
            else {
                QMessageBox message;
                message.setIcon(QMessageBox::Critical);
                message.setText(tr("Unable to connect to database.\n"
                                   "Do you want to continue anyway?"));
                message.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                int returnCode = message.exec();
                if(returnCode == QMessageBox::Yes) {
                    establishActualConnection();
                }
            }
            if(ui->checkBoxRemember->isChecked()) {
                writeConnectionInfo();
            }
            else {
                clearConnectionInfo();
            }
        }
        else {
            QMessageBox message;
            message.setIcon(QMessageBox::Critical);
            message.setText(tr("Your connection is not tested yet.\n"
                               "Please make sure that you can connect to a valid database."));
            message.setStandardButtons(QMessageBox::Ok);
            message.exec();
        }
    }
    else {
        establishActualConnection();
    }
}

void DialogDatabaseConfig::test()
{
    tested = true;
    currentType = static_cast<Database::Type>(ui->comboBoxDatabaseType->itemData(
                      ui->comboBoxDatabaseType->currentIndex()).toUInt());
    switch(currentType) {
    case Database::Type::SQLServer2005:
        break;
    case Database::Type::SQLServer2008:
        break;
    case Database::Type::SQLServer2010:
        break;
    case Database::Type::SQLServer2012:
        break;
    case Database::Type::MySQL5:
        QSqlDatabase::database("testConnection").close();
        testDB = QSqlDatabase::addDatabase("QMYSQL","testConnection");
        testDB.setHostName(ui->lineEditHost->text());
        testDB.setPort(ui->spinBoxPort->value());
        testDB.setDatabaseName(ui->lineEditDatabase->text());
        testDB.setUserName(ui->lineEditUser->text());
        testDB.setPassword(ui->lineEditPassword->text());
        // TODO: Connect with SSL
        lockGUI();
        F_testDBOpen = QtConcurrent::run(testDB, &QSqlDatabase::open);
        FW_testDBOpen.setFuture(F_testDBOpen);
        break;
    case Database::Type::SQLite:
        break;
    case Database::Type::SQLServer:
        break;
    }
}

void DialogDatabaseConfig::handleTestResult()
{
    if(F_testDBOpen.result() && testDB.isOpen() && testDB.isValid()) {
        testDB.close();
        QMessageBox information(QMessageBox::Information,
                                tr("No problem occured."),
                                tr("Successfuly connected to database."),
                                QMessageBox::Ok,
                                this);
        information.exec();
        connected= true;
    }
    else {
        QMessageBox::critical(this,
                              tr("Unable to connect to database."),
                              tr("Could not establish connection within given information.\n"
                                 "The Database Management System Provider "
                                 "has reported following error(s):\n%1")
                              .arg(testDB.lastError().text())
                             );
        connected=false;
    }
}

void DialogDatabaseConfig::handleActualConnection()
{
    if(F_mainDBOpen.result()) {
        QDialog::accept();
    }
}

void DialogDatabaseConfig::updateLocalHostStatus(bool checked)
{
    if(checked) {
        this->lastCustomHost = ui->lineEditHost->text();
        if((quint8)currentType!=0) {
            ui->lineEditHost->setText(
                DataProviderInformation::getInstance()->getProviderInfo(
                    currentType).defaultHost());
        }
    }
    else {
        ui->lineEditHost->setText(lastCustomHost);
    }
}

void DialogDatabaseConfig::updateDefaultPortStatus(bool checked)
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

void DialogDatabaseConfig::updateSecurityOption(int index)
{
    switch (index) {
    case 0: // No encryption
        this->ui->labelSSLCA->hide();
        this->ui->labelSSLCert->hide();
        this->ui->labelSSLKey->hide();
        this->ui->chooseFileSSLCA->hide();
        this->ui->chooseFileSSLCert->hide();
        this->ui->chooseFileSSLKey->hide();
        break;
    case 1: // Encrypt over SSL
        this->ui->labelSSLCA->show();
        this->ui->labelSSLCert->show();
        this->ui->labelSSLKey->show();
        this->ui->chooseFileSSLCA->show();
        this->ui->chooseFileSSLCert->show();
        this->ui->chooseFileSSLKey->show();
    default:
        break;
    }
    adjustSize();
}

void DialogDatabaseConfig::modifyWindow()
{
    setWindowModified(true);
}

void DialogDatabaseConfig::lockGUI()
{
    QCursor wait {Qt::WaitCursor};
    this->setCursor(wait);
    this->setEnabled(false);
}

void DialogDatabaseConfig::releaseGUI()
{
    QCursor wait {Qt::ArrowCursor};
    this->setCursor(wait);
    this->setEnabled(true);
}

void DialogDatabaseConfig::updateDatabaseType(int i)
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
