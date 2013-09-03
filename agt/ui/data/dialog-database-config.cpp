#include "dialog-database-config.h"
#include "ui_dialog-database-config.h"

#include <QSet>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include <agt/core/settings.h>

DialogDatabaseConfig::DialogDatabaseConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDatabaseConfig),
    tested(false),connected(false)
{
    ui->setupUi(this);
    // Remove maximize and minimize buttons
    setWindowFlags( Qt::Dialog | Qt::WindowCloseButtonHint );
    // Initialize Database systems:
    QSet<Database::DatabaseType> supportedSystems = Database::getSupportedSystems();
    foreach(Database::DatabaseType type, supportedSystems)
    {
        switch(type)
        {
        case Database::SQLServer2005:
            ui->comboBoxDatabaseType->addItem(tr("SQL Server 2005"),
                                              QVariant(static_cast<quint8>(Database::SQLServer2005)));
            break;
        case Database::SQLServer2008:
            ui->comboBoxDatabaseType->addItem(tr("SQL Server 2008"),
                                              QVariant(static_cast<quint8>(Database::SQLServer2008)));
            break;
        case Database::SQLServer2010:
            ui->comboBoxDatabaseType->addItem(tr("SQL Server 2010"),
                                              QVariant(static_cast<quint8>(Database::SQLServer2010)));
            break;
        case Database::SQLServer2012:
            ui->comboBoxDatabaseType->addItem(tr("SQL Server 2012"),
                                              QVariant(static_cast<quint8>(Database::SQLServer2012)));
            break;
        case Database::MySQL5:
            ui->comboBoxDatabaseType->addItem(tr("Oracle MySQL 5"),
                                              QVariant(static_cast<quint8>(Database::MySQL5)));
            break;
        case Database::SQLite:
            ui->comboBoxDatabaseType->addItem(tr("SQLite"),
                                              QVariant(static_cast<quint8>(Database::SQLite)));
            break;
        case Database::SQLServer:
            ui->comboBoxDatabaseType->addItem(tr("SQL Server (generic)"),
                                              QVariant(static_cast<quint8>(Database::SQLServer)));
            break;
        }
    }
    createConnections();

    // TODO: Implement
//    int firstRun = Settings::getInstance()->value("first-run",-1).toInt();
//    if(firstRun == -1) {
//        //TODO: prompt for create database
//        QMessageBox::critical(this,
//                              tr("First Run!"),
//                              tr("This is first time you are running this application! "
//                                 "Would you like to create a database using wizard?"));
//    }
//    else {
//    }

    // TODO: Remove
    bool shouldRemember = Settings::getInstance()->value("ui:data:dbconfig:remember").toBool();
    if(shouldRemember)
        readConnectionInfo();
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

void DialogDatabaseConfig::createConnections()
{
    connect(ui->comboBoxDatabaseType, SIGNAL(currentIndexChanged(int)),this,SLOT(updateDatabaseType(int)));
    connect(ui->pushButtonTest, SIGNAL(clicked()),this,SLOT(test()));
    connect(ui->checkBoxLocal,SIGNAL(toggled(bool)), ui->lineEditHost, SLOT(setDisabled(bool)));
    connect(ui->checkBoxLocal,SIGNAL(clicked()), this , SLOT(updateLocalHostStatus()));
    connect(ui->checkBoxDefaultPort,SIGNAL(toggled(bool)), ui->spinBoxPort, SLOT(setDisabled(bool)));
    connect(ui->checkBoxDefaultPort,SIGNAL(clicked()), this , SLOT(updateDefaultPortStatus()));
}

bool DialogDatabaseConfig::testConnection()
{
    QSqlDatabase db;
    tested = true;
    switch(currentType)
    {
    case Database::SQLServer2005:
        break;
    case Database::SQLServer2008:
        break;
    case Database::SQLServer2010:
        break;
    case Database::SQLServer2012:
        break;
    case Database::MySQL5:
        db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName(ui->lineEditHost->text());
        db.setPort(ui->spinBoxPort->value());
        db.setDatabaseName(ui->lineEditDatabase->text());
        db.setUserName(ui->lineEditUser->text());
        db.setPassword(ui->lineEditPassword->text());
        if(db.open())
        {
            db.close();
            QMessageBox::information(this,
                                     tr("No problem occured."),
                                     tr("Successfuly connected to database."));
            return true;
        }
        else
        {
            QMessageBox::critical(this,
                                  tr("Unable connected to database."),
                                  tr("Could not establish connection with database management system provider.\n"
                                     "System has reported following error:\n%1")
                                  .arg(db.lastError().text())
                                  );
            return false;
        }
        break;
    case Database::SQLite:
        break;
    case Database::SQLServer:
        break;
    }
    return false;
}

void DialogDatabaseConfig::establishActualConnection()
{
    switch(currentType)
    {
    case Database::SQLServer2005:
        break;
    case Database::SQLServer2008:
        break;
    case Database::SQLServer2010:
        break;
    case Database::SQLServer2012:
        break;
    case Database::MySQL5:
        Database::setInstance(QSqlDatabase::addDatabase("QMYSQL"));
        Database::getInstance()->database()->setHostName(ui->lineEditHost->text());
        Database::getInstance()->database()->setPort(ui->spinBoxPort->value());
        Database::getInstance()->database()->setDatabaseName(ui->lineEditDatabase->text());
        Database::getInstance()->database()->setUserName(ui->lineEditUser->text());
        Database::getInstance()->database()->setPassword(ui->lineEditPassword->text());
        Database::getInstance()->database()->open();
        break;
    case Database::SQLite:
        break;
    case Database::SQLServer:
        break;
    }
}

void DialogDatabaseConfig::readConnectionInfo()
{
    ui->lineEditHost->setText(Settings::getInstance()->value("ui:data:dbconfig:host").toString());
    QString type = Settings::getInstance()->value("ui:data:dbconfig:type").toString();
    int index = ui->comboBoxDatabaseType->findText(type);
    ui->comboBoxDatabaseType->setCurrentIndex(index);
    ui->checkBoxLocal->setChecked(Settings::getInstance()->value("ui:data:dbconfig:local").toBool());
    ui->checkBoxDefaultPort->setChecked(Settings::getInstance()->value("ui:data:dbconfig:defaultPort").toBool());
    ui->spinBoxPort->setValue(Settings::getInstance()->value("ui:data:dbconfig:port").toInt());
    ui->lineEditDatabase->setText(Settings::getInstance()->value("ui:data:dbconfig:database").toString());
    ui->lineEditUser->setText(Settings::getInstance()->value("ui:data:dbconfig:user").toString());
}

void DialogDatabaseConfig::writeConnectionInfo()
{
    Settings::getInstance()->setValue("ui:data:dbconfig:host",ui->lineEditHost->text());
    Settings::getInstance()->setValue("ui:data:dbconfig:type",ui->comboBoxDatabaseType->currentText());
    Settings::getInstance()->setValue("ui:data:dbconfig:local",ui->checkBoxLocal->isChecked());
    Settings::getInstance()->setValue("ui:data:dbconfig:port",ui->spinBoxPort->value());
    Settings::getInstance()->setValue("ui:data:dbconfig:defaultPort",ui->checkBoxDefaultPort->isChecked());
    Settings::getInstance()->setValue("ui:data:dbconfig:database",ui->lineEditDatabase->text());
    Settings::getInstance()->setValue("ui:data:dbconfig:user",ui->lineEditUser->text());
    // Obviously! :
    Settings::getInstance()->setValue("ui:data:dbconfig:remember",ui->checkBoxRemember->isChecked());
    // TODO : Encrypt and save password in a binary file
}

void DialogDatabaseConfig::clearConnectionInfo()
{
    Settings::getInstance()->setValue("ui:data:dbconfig:remember",false);
}

void DialogDatabaseConfig::accept()
{
    if(tested)
    {
        if(connected)
        {
            establishActualConnection();
            QDialog::accept();
        }
        else
        {
            QMessageBox message;
            message.setIcon(QMessageBox::Critical);
            message.setText(tr("Unable to connect to database.\n"
                               "Do you want to continue anyway?"));
            message.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            int returnCode = message.exec();
            if(returnCode == QMessageBox::Yes)
            {
                establishActualConnection();
                QDialog::accept();
            }
        }
        if(ui->checkBoxRemember->isChecked())
            writeConnectionInfo();
        else
            clearConnectionInfo();

    }
    else
    {
        QMessageBox message;
        message.setIcon(QMessageBox::Critical);
        message.setText(tr("Your connection is not tested yet.\n"
                           "Please make sure that you can connect to a valid database."));
        message.setStandardButtons(QMessageBox::Ok);
        message.exec();
    }
}

void DialogDatabaseConfig::test()
{
    connected = testConnection();
}

void DialogDatabaseConfig::updateLocalHostStatus()
{
    static QString lastCustomHost;
    if(ui->checkBoxLocal->isChecked()){
        lastCustomHost = ui->lineEditHost->text();
        ui->lineEditHost->setText("127.0.0.1");
    }
    else{
        ui->lineEditHost->setText(lastCustomHost);
    }
}

void DialogDatabaseConfig::updateDefaultPortStatus()
{
    static int lastCustomPort;
    if(ui->checkBoxDefaultPort->isChecked()){
        lastCustomPort = ui->spinBoxPort->value();
        switch(this->currentType) {
        // TODO: complete list
        case Database::MySQL5:
            ui->spinBoxPort->setValue(3306);
            break;
        }
    }
    else {
        ui->spinBoxPort->setValue(lastCustomPort);
    }
}

void DialogDatabaseConfig::updateDatabaseType(int i)
{
    currentType = static_cast<Database::DatabaseType>(ui->comboBoxDatabaseType->itemData(i).toUInt());
    switch(currentType)
    {
    case Database::SQLServer2005:
        break;
    case Database::SQLServer2008:
        break;
    case Database::SQLServer2010:
        break;
    case Database::SQLServer2012:
        break;
    case Database::MySQL5:
        ui->stackedWidgetDatabases->setCurrentIndex(0);
        break;
    case Database::SQLite:
        ui->stackedWidgetDatabases->setCurrentIndex(1);
        break;
    case Database::SQLServer:
        break;
    }
}
