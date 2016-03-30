#include "wizard-page-create-database-operation.h"
#include "ui_wizard-page-create-database-operation.h"
#if QT_VERSION >= 0x050000
#include <QtConcurrent/QtConcurrentRun>
#else
#include <QtConcurrentRun>
#endif
#include <QDateTime>
#include <QMessageBox>
#include <QRegExp>
#include <qtz/data/database.h>

#include <QDebug>
#include <QSqlError>

WizardPageCreateDatabaseOperation::WizardPageCreateDatabaseOperation(
    QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::WizardPageCreateDatabaseOperation),
    m_futureWatcher(new QFutureWatcher<bool>()),
    m_finished(false)
{
    ui->setupUi(this);
#if QT_VERSION >= 0x050000
//    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0,
//            QHeaderView::Stretch);
//    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1,
//            QHeaderView::ResizeToContents);
//    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2,
//            QHeaderView::ResizeToContents);
//    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3,
//            QHeaderView::ResizeToContents);
#else
    ui->tableWidget->horizontalHeader()->setResizeMode(0,
            QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setResizeMode(1,
            QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setResizeMode(2,
            QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setResizeMode(3,
            QHeaderView::ResizeToContents);
#endif
    connect(this->m_futureWatcher,SIGNAL(finished()),this,SLOT(taskFinished()));
}

WizardPageCreateDatabaseOperation::~WizardPageCreateDatabaseOperation()
{
    delete ui;
}

void WizardPageCreateDatabaseOperation::addSql(QString &query,
        const QString title, const quint8 &progress)
{
    this->m_tasks.append({progress,title,query});
}

void WizardPageCreateDatabaseOperation::initializePage()
{
    // Replace placeholders:
    QRegExp reg {"__database__"};
    for(auto& task : this->m_tasks) {
        task.query.replace(reg,field("database").toString());
    }
    Database::Type providerType = static_cast<Database::Type>
                                  (wizard()->property("providerCode").toUInt());
    switch (providerType) {
    case Database::Type::SQLite:
        break;
    case Database::Type::MySQL5:
        m_db = QSqlDatabase::addDatabase("QMYSQL","createDatabase");
        break;
        // TODO: Support these databases:
    case Database::Type::SQLServer:
    case Database::Type::SQLServer2005:
    case Database::Type::SQLServer2008:
    case Database::Type::SQLServer2010:
    case Database::Type::SQLServer2012:
        // TODO: Support these databases
        break;
    default:
        break;
    }
    m_db.setDatabaseName("");
    m_db.setHostName(field("host").toString());
    m_db.setPassword(field("password").toString());
    m_db.setPort(field("port").toUInt());
    m_db.setUserName(field("username").toString());
    // TODO: Assert:
    m_db.open();
    // Set start time:
    this->m_start.start();
    ui->labelShowStartTime->setText(m_start.toString("hh:mm:ss.zzz"));
    // Start operations
    this->m_currentTask = this->m_tasks.begin();
    performNextTask();
}

bool WizardPageCreateDatabaseOperation::isComplete() const
{
    return this->m_finished;
}

void WizardPageCreateDatabaseOperation::estimateTime()
{
    int delta = this->m_start.elapsed();
    int ert = delta*100/ui->progressBar->value();
}

void WizardPageCreateDatabaseOperation::changeEvent(QEvent *e)
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

void WizardPageCreateDatabaseOperation::performNextTask()
{
    if(this->m_currentTask != m_tasks.end()) {
        QSqlQuery query {m_db};
        query.prepare(m_currentTask->query);
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row,0,new QTableWidgetItem {QIcon{":/images/images/task-ongoing.png"},m_currentTask->title});
        ui->tableWidget->setItem(row,1,new QTableWidgetItem {QDateTime::currentDateTime().toString("hh:mm:ss.zzz")});
        ui->tableWidget->scrollToBottom();
        m_future = QtConcurrent::run(this,
                                     &WizardPageCreateDatabaseOperation::performTask,
                                     query);
        m_futureWatcher->setFuture(m_future);
    }
    else {
        this->m_finished = true;
        emit completeChanged();
    }
}

bool WizardPageCreateDatabaseOperation::performTask(QSqlQuery query)
{
    return query.exec();
}

void WizardPageCreateDatabaseOperation::taskFinished()
{
    ui->progressBar->setValue(ui->progressBar->value()+m_currentTask->progress);
    m_currentTask++;
    taskCallback(m_future.result());
}

void WizardPageCreateDatabaseOperation::taskCallback(bool result)
{
    if(result) {
        int row = ui->tableWidget->rowCount()-1;
        ui->tableWidget->item(row,0)->setIcon(QIcon {":/images/images/task-complete.png"});
        ui->tableWidget->setItem(row,2,new QTableWidgetItem {QDateTime::currentDateTime().toString("hh:mm:ss.zzz")});
        performNextTask();
    }
    else {
        QMessageBox::critical(this,
                              tr("Database Error"),
                              tr("Unable to execute provided query.\n"
                                 "Database Management System has provided following error(s):\n%1")
                              .arg(m_db.lastError().text()));
        m_finished = false;
    }
}
