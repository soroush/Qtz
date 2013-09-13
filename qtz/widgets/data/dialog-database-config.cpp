#include "dialog-database-config.h"
#include "ui_dialog-database-config.h"

#include <QSet>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include <qtz/core/settings.h>
#include <qtz/data/data-provider-information.h>

#include <QDebug>

DialogDatabaseConfig::DialogDatabaseConfig(QWidget *parent) :
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
        Settings::getInstance()->value("ui:data:dbconfig:remember").toBool();
    if(shouldRemember) {
        readConnectionInfo();
    }
}

DialogDatabaseConfig::~DialogDatabaseConfig() {
    delete ui;
}

void DialogDatabaseConfig::changeEvent(QEvent *e) {
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void DialogDatabaseConfig::initializeDatabaseSystems() {
    QVector<DataProviderInformation> systems =
        DataProviderInformation::getInstance()->getAvailableDatabases();
    foreach (DataProviderInformation pi, systems) {
        ui->comboBoxDatabaseType->addItem(pi.providerName(),pi.providerCode());
    }
}

void DialogDatabaseConfig::createConnections() {
    connect(ui->comboBoxDatabaseType, SIGNAL(currentIndexChanged(int)), this,
            SLOT(updateDatabaseType(int)));
    connect(ui->pushButtonTest, SIGNAL(clicked()), this, SLOT(test()));
    connect(ui->checkBoxLocal, SIGNAL(toggled(bool)), ui->lineEditHost,
            SLOT(setDisabled(bool)));
    connect(ui->checkBoxLocal, SIGNAL(clicked()), this ,
            SLOT(updateLocalHostStatus()));
    connect(ui->checkBoxDefaultPort, SIGNAL(toggled(bool)), ui->spinBoxPort,
            SLOT(setDisabled(bool)));
    connect(ui->checkBoxDefaultPort, SIGNAL(clicked()), this ,
            SLOT(updateDefaultPortStatus()));
}

bool DialogDatabaseConfig::testConnection() {
    QSqlDatabase db;
    tested = true;
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
        db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName(ui->lineEditHost->text());
        db.setPort(ui->spinBoxPort->value());
        db.setDatabaseName(ui->lineEditDatabase->text());
        db.setUserName(ui->lineEditUser->text());
        db.setPassword(ui->lineEditPassword->text());
        if(db.open()) {
            db.close();
            QMessageBox information(QMessageBox::Information,
                                    tr("No problem occured."),
                                    tr("Successfuly connected to database."),
                                    QMessageBox::Ok,
                                    this);
            information.exec();
            return true;
        }
        else {
            QMessageBox::critical(this,
                                  tr("Unable connected to database."),
                                  tr("Could not establish connection with database management system provider.\n"
                                     "System has reported following error:\n%1")
                                  .arg(db.lastError().text())
                                 );
            return false;
        }
        break;
    case Database::Type::SQLite:
        break;
    case Database::Type::SQLServer:
        break;
    }
    return false;
}

void DialogDatabaseConfig::establishActualConnection() {
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
        Database::setInstance(QSqlDatabase::addDatabase("QMYSQL"));
        Database::getInstance()->database()->setHostName(ui->lineEditHost->text());
        Database::getInstance()->database()->setPort(ui->spinBoxPort->value());
        Database::getInstance()->database()->setDatabaseName(
            ui->lineEditDatabase->text());
        Database::getInstance()->database()->setUserName(ui->lineEditUser->text());
        Database::getInstance()->database()->setPassword(ui->lineEditPassword->text());
        Database::getInstance()->database()->open();
        break;
    case Database::Type::SQLite:
        break;
    case Database::Type::SQLServer:
        break;
    }
}

void DialogDatabaseConfig::readConnectionInfo() {
    if(Database::getInstance() != nullptr) {
        Database::getInstance()->readConnectionInfo();
        ui->lineEditHost->setText(Database::getInstance()->database()->hostName());
        quint32 type = Settings::getInstance()->value("db:type").toUInt();
        int index = ui->comboBoxDatabaseType->findData(type);
        ui->comboBoxDatabaseType->setCurrentIndex(index);
        ui->checkBoxLocal->setChecked(
            Settings::getInstance()->value("ui:data:dbconfig:local").toBool());
        ui->checkBoxDefaultPort->setChecked(
            Settings::getInstance()->value("ui:data:dbconfig:defaultPort").toBool());
        ui->spinBoxPort->setValue(Database::getInstance()->database()->port());
        ui->lineEditDatabase->setText(
            Database::getInstance()->database()->databaseName());
        ui->lineEditUser->setText(Database::getInstance()->database()->userName());
        ui->lineEditPassword->setText(Database::getInstance()->database()->password());
    }
}

void DialogDatabaseConfig::writeConnectionInfo() {
    Database::getInstance()->writeConnectionInfo();
//    Settings::getInstance()->setValue("ui:data:dbconfig:host",
//                                      ui->lineEditHost->text());
//    Settings::getInstance()->setValue("ui:data:dbconfig:type",
//                                      ui->comboBoxDatabaseType->currentText());
    Settings::getInstance()->setValue("ui:data:dbconfig:local",
                                      ui->checkBoxLocal->isChecked());
//    Settings::getInstance()->setValue("ui:data:dbconfig:port",
//                                      ui->spinBoxPort->value());
    Settings::getInstance()->setValue("ui:data:dbconfig:defaultPort",
                                      ui->checkBoxDefaultPort->isChecked());
//    Settings::getInstance()->setValue("ui:data:dbconfig:database",
//                                      ui->lineEditDatabase->text());
//    Settings::getInstance()->setValue("ui:data:dbconfig:user",
//                                      ui->lineEditUser->text());
    // Obviously! :
    Settings::getInstance()->setValue("ui:data:dbconfig:remember",
                                      ui->checkBoxRemember->isChecked());
}

void DialogDatabaseConfig::clearConnectionInfo() {
    Settings::getInstance()->setValue("ui:data:dbconfig:remember", false);
}

void DialogDatabaseConfig::accept() {
    if(tested) {
        if(connected) {
            establishActualConnection();
            QDialog::accept();
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
                QDialog::accept();
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

void DialogDatabaseConfig::test() {
    connected = testConnection();
}

void DialogDatabaseConfig::updateLocalHostStatus() {
    static QString lastCustomHost;
    if(ui->checkBoxLocal->isChecked()) {
        lastCustomHost = ui->lineEditHost->text();
        QString host = DataProviderInformation::getInstance()->getProviderInfo(
                           currentType).defaultHost();
        ui->lineEditHost->setText(host);
    }
    else {
        ui->lineEditHost->setText(lastCustomHost);
    }
}

void DialogDatabaseConfig::updateDefaultPortStatus() {
    static int lastCustomPort;
    if(ui->checkBoxDefaultPort->isChecked()) {
        lastCustomPort = ui->spinBoxPort->value();
        quint32 port = DataProviderInformation::getInstance()->getProviderInfo(
                           currentType).defaultPort();
        ui->spinBoxPort->setValue(port);
    }
    else {
        ui->spinBoxPort->setValue(lastCustomPort);
    }
}

void DialogDatabaseConfig::updateDatabaseType(int i) {
    currentType = static_cast<Database::Type>(ui->comboBoxDatabaseType->itemData(
                      i).toUInt());
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
        ui->stackedWidgetDatabases->setCurrentIndex(0);
        break;
    case Database::Type::SQLite:
        ui->stackedWidgetDatabases->setCurrentIndex(1);
        break;
    case Database::Type::SQLServer:
        break;
    }
    updateLocalHostStatus();
    updateDefaultPortStatus();
}
