#include "dialog-database-config.hpp"
#include "ui_dialog-database-config.h"

#include <QSet>
#include <QSqlError>
#include <QMessageBox>
#include <QCursor>
#include <qtz/core/settings.hpp>
#include <qtz/security/crypto.hpp>
#if QT_VERSION >= 0x050000
    #include <QtConcurrent/QtConcurrentRun>
#else
    #include <QtConcurrentRun>
#endif

#include <qtz/data/data-provider-information.hpp>

#include <QDebug>

DialogDatabaseConfig::DialogDatabaseConfig(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::DialogDatabaseConfig),
    tested(false), connected(false) {
    ui->setupUi(this);
    // Remove maximize and minimize buttons
    this->setWindowFlags(Qt::Dialog | Qt::WindowContextHelpButtonHint |
                         Qt::WindowCloseButtonHint);
    initializeDatabaseSystems();
    createConnections();
    bool shouldRemember =
        Settings::getInstance()->value("ui/data/dbconfig/remember").toBool();
    if(shouldRemember) {
        readConnectionInfo();
    }
    updateSecurityOption(ui->comboBoxConnectionSecurity->currentIndex());
    updateDatabaseType(ui->comboBoxDatabaseType->currentIndex());
}

DialogDatabaseConfig::~DialogDatabaseConfig() {
    delete ui;
}

void DialogDatabaseConfig::changeEvent(QEvent* e) {
    QDialog::changeEvent(e);
    switch(e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

void DialogDatabaseConfig::initializeDatabaseSystems() {
    QVector<DataProvider> systems =
        DataProviderInformation::getInstance()->getSupportedProviders();
    foreach(DataProvider p, systems) {
        ui->comboBoxDatabaseType->addItem(p.providerName(),p.providerCode());
    }
    // Establish first valid state:
    quint8 type = ui->comboBoxDatabaseType->itemData(
                      ui->comboBoxDatabaseType->currentIndex()
                  ).toUInt();
    currentType = static_cast<DataProvider::Type>(type);
    quint32 port = DataProviderInformation::getInstance()->getProviderInfo(
                       currentType).defaultPort();
    this->lastCustomPort = port;
    ui->spinBoxPort->setValue(port);
}

void DialogDatabaseConfig::createConnections() {
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
    // TODO: add signals for choose file
    // Concurrency Connections:
    connect(&FW_testDBOpen,SIGNAL(finished()),this,SLOT(handleTestResult()));
    connect(&FW_testDBOpen,SIGNAL(finished()),this,SLOT(releaseGUI()));
    connect(&FW_mainDBOpen,SIGNAL(finished()),this,SLOT(handleActualConnection()));
    connect(&FW_mainDBOpen,SIGNAL(finished()),this,SLOT(releaseGUI()));
}

void DialogDatabaseConfig::readConnectionInfo() {
    QSettings* instance = Settings::getInstance();
    currentType = static_cast<DataProvider::Type>(instance->value("database/type",1).toInt());
    ui->lineEditHost->setText(instance->value("database/host").toString());
    int index = ui->comboBoxDatabaseType->findData(static_cast<int>(currentType));
    ui->comboBoxDatabaseType->setCurrentIndex(index);
    updateDatabaseType(index);
    ui->checkBoxLocal->setChecked(
        Settings::getInstance()->value("ui/data/dbconfig/local").toBool());
    ui->checkBoxDefaultPort->setChecked(
        Settings::getInstance()->value("ui/data/dbconfig/defaultPort").toBool());
    ui->checkBoxRemember->setChecked(
        Settings::getInstance()->value("ui/data/dbconfig/remember").toBool());
    ui->spinBoxPort->setValue(instance->value("database/port").toInt());
    ui->lineEditSchema->setText(instance->value("database/schema").toString());
    QString userNameC = instance->value("database/username").toString();
    QString usernameP;
    usernameP = Crypto::decrypt(userNameC);
    ui->lineEditUser->setText(usernameP);
    QString passwordC = instance->value("database/password").toString();
    QString passwordP;
    passwordP = Crypto::decrypt(passwordC);
    ui->lineEditPassword->setText(passwordP);
}

void DialogDatabaseConfig::writeConnectionInfo() {
    QSettings* s = Settings::getInstance();
    s->setValue("database/type",
                ui->comboBoxDatabaseType->currentData(Qt::UserRole).toInt());
    s->setValue("database/host",
                ui->lineEditHost->text());
    s->setValue("database/port",
                ui->spinBoxPort->value());
    s->setValue("database/schema",
                ui->lineEditSchema->text());
    QString usernameC = Crypto::encrypt(ui->lineEditUser->text());
    s->setValue("database/username",
                usernameC);
    QString passwordC = Crypto::encrypt(ui->lineEditPassword->text());
    s->setValue("database/password",
                passwordC);
    s->setValue("ui/data/dbconfig/local",
                ui->checkBoxLocal->isChecked());
    s->setValue("ui/data/dbconfig/defaultPort",
                ui->checkBoxDefaultPort->isChecked());
    s->setValue("ui/data/dbconfig/remember",
                ui->checkBoxRemember->isChecked());
}

void DialogDatabaseConfig::clearConnectionInfo() {
    Settings::getInstance()->setValue("ui/data/dbconfig/remember", false);
}

void DialogDatabaseConfig::accept() {
    if(tested) {
        if(!connected) {
            QMessageBox message;
            message.setIcon(QMessageBox::Critical);
            message.setText(tr("Unable to connect to database.\n"
                               "Do you want to continue anyway?"));
            message.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            int returnCode = message.exec();
            if(returnCode == QMessageBox::Yes) {
                QDialog::accept();
            }
        }
        if(ui->checkBoxRemember->isChecked()) {
            writeConnectionInfo();
        } else {
            clearConnectionInfo();
        }
        QDialog::accept();
    } else {
        int result = QMessageBox::question(this,tr("Connection Not Tested!"),
                                           tr("Your connection is not tested yet.\n"
                                              "Do you want to continue anyway?"));
        switch(result) {
            case QMessageBox::Yes:
                QDialog::accept();
                break;
            default:
                break;
        }
    }
}

void DialogDatabaseConfig::test() {
    tested = true;
    currentType = static_cast<DataProvider::Type>(ui->comboBoxDatabaseType->itemData(
                      ui->comboBoxDatabaseType->currentIndex()).toUInt());
    switch(currentType) {
        case DataProvider::Type::Oracle:
            break;
        case DataProvider::Type::MySQL5:
            QSqlDatabase::database("testConnection").close();
            QSqlDatabase::removeDatabase("testConnection");
            testDB = QSqlDatabase::addDatabase("QMYSQL","testConnection");
            testDB.setHostName(ui->lineEditHost->text());
            testDB.setPort(ui->spinBoxPort->value());
            testDB.setDatabaseName(ui->lineEditSchema->text());
            testDB.setUserName(ui->lineEditUser->text());
            testDB.setPassword(ui->lineEditPassword->text());
            // TODO: Connect with SSL
            lockGUI();
            F_testDBOpen = QtConcurrent::run(this, &DialogDatabaseConfig::testDBOpen);
            FW_testDBOpen.setFuture(F_testDBOpen);
            break;
        case DataProvider::Type::SQLite:
            break;
    }
}

void DialogDatabaseConfig::handleTestResult() {
    if(F_testDBOpen.result() && testDB.isOpen() && testDB.isValid()) {
        testDB.close();
        QMessageBox information(QMessageBox::Information,
                                tr("No problem occured."),
                                tr("Successfuly connected to database."),
                                QMessageBox::Ok,
                                this);
        information.exec();
        connected= true;
    } else {
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

void DialogDatabaseConfig::handleActualConnection() {
    if(F_mainDBOpen.result()) {
        QDialog::accept();
    }
}

void DialogDatabaseConfig::updateLocalHostStatus(bool checked) {
    if(checked) {
        this->lastCustomHost = ui->lineEditHost->text();
        if((quint8)currentType!=0) {
            ui->lineEditHost->setText(
                DataProviderInformation::getInstance()->getProviderInfo(
                    currentType).defaultHost());
        }
    } else {
        ui->lineEditHost->setText(lastCustomHost);
    }
}

void DialogDatabaseConfig::updateDefaultPortStatus(bool checked) {
    if(checked) {
        this->lastCustomPort = ui->spinBoxPort->value();
        if((quint8)currentType!=0) {
            quint32 port = DataProviderInformation::getInstance()->getProviderInfo(
                               currentType).defaultPort();
            ui->spinBoxPort->setValue(port);
        }
    } else {
        ui->spinBoxPort->setValue(lastCustomPort);
    }
}

void DialogDatabaseConfig::updateSecurityOption(int index) {
    switch(index) {
        case 0: // No encryption
            this->ui->labelSSLCA->hide();
            this->ui->labelSSLCert->hide();
            this->ui->labelSSLKey->hide();
            //            this->ui->chooseFileSSLCA->hide();
            //            this->ui->chooseFileSSLCert->hide();
            //            this->ui->chooseFileSSLKey->hide();
            break;
        case 1: // Encrypt over SSL
            this->ui->labelSSLCA->show();
            this->ui->labelSSLCert->show();
            this->ui->labelSSLKey->show();
        //            this->ui->chooseFileSSLCA->show();
        //            this->ui->chooseFileSSLCert->show();
        //            this->ui->chooseFileSSLKey->show();
        default:
            break;
    }
    adjustSize();
}

void DialogDatabaseConfig::lockGUI() {
    QCursor wait {Qt::WaitCursor};
    this->setCursor(wait);
    this->setEnabled(false);
}

void DialogDatabaseConfig::releaseGUI() {
    QCursor wait {Qt::ArrowCursor};
    this->setCursor(wait);
    this->setEnabled(true);
}

bool DialogDatabaseConfig::testDBOpen() {
    bool validity = testDB.open();
    validity = validity && testDB.isValid() && testDB.isOpen();
    return validity;
}

void DialogDatabaseConfig::updateDatabaseType(int i) {
    currentType = static_cast<DataProvider::Type>(ui->comboBoxDatabaseType->itemData(i).toUInt());
    switch(currentType) {
        case DataProvider::Type::Oracle:
        case DataProvider::Type::MySQL5:
            ui->stackedWidgetDatabases->setCurrentIndex(0);
            break;
        case DataProvider::Type::SQLite:
            ui->stackedWidgetDatabases->setCurrentIndex(1);
            break;
        default:
            break;
    }
    updateLocalHostStatus(ui->checkBoxLocal->isChecked());
    updateDefaultPortStatus(ui->checkBoxDefaultPort->isChecked());
}
