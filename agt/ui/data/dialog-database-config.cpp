#include "dialog-database-config.h"
#include "ui_dialog-database-config.h"

#include <QSet>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>

DialogDatabaseConfig::DialogDatabaseConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDatabaseConfig),
    tested(false),connected(false)
{
    ui->setupUi(this);
    QSet <Database::DatabaseType> supportedSystems = Database::getSupportedSystems();
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
            ui->plainTextEditResult->setPlainText(tr("Successfuly connected to database.\n"));
            return true;
        }
        else
        {
            ui->plainTextEditResult->setPlainText(tr("ERROR: Unable connected to database."));
            ui->plainTextEditResult->appendPlainText(QSqlDatabase::database().lastError().text());
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

bool DialogDatabaseConfig::establishActualConnection()
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
