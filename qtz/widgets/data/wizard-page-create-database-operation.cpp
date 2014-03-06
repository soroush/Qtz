#include "wizard-page-create-database-operation.h"
#include "ui_wizard-page-create-database-operation.h"
#include <QtConcurrentRun>
#include <QDateTime>

#include <QDebug>

WizardPageCreateDatabaseOperation::WizardPageCreateDatabaseOperation(
    QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::WizardPageCreateDatabaseOperation),
    fw(new QFutureWatcher<QSqlQuery>())
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setResizeMode(0,
            QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setResizeMode(1,
            QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setResizeMode(2,
            QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setResizeMode(3,
            QHeaderView::ResizeToContents);
    connect(this->fw,SIGNAL(finished()),this,SLOT(taskFinished()));
    connect(ui->pushButtonPRS,SIGNAL(clicked()),this,SLOT(startTasks()));
}

WizardPageCreateDatabaseOperation::~WizardPageCreateDatabaseOperation()
{
    delete ui;
}

void WizardPageCreateDatabaseOperation::addSql(QString &query,
        const QString title, const quint8 &progress)
{
    query.replace("%database%",field("database").toString());
    this->m_tasks.append({progress,title,query});
}

void WizardPageCreateDatabaseOperation::startTasks()
{
    disconnect(ui->pushButtonPRS);
    db= QSqlDatabase::database("ConnectionTest");
    this->m_currentTask = this->m_tasks.begin();
    performNextTask();
}

void WizardPageCreateDatabaseOperation::pauseTasks()
{
}

void WizardPageCreateDatabaseOperation::resumeTasks()
{
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
    //qDebug() << "performNextTask()";
    if(this->m_currentTask != m_tasks.end()) {
        QSqlQuery query;
        query.prepare(m_currentTask->query);
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row,0,new QTableWidgetItem {QIcon{":/images/images/task-ongoing.png"},m_currentTask->title});
        ui->tableWidget->setItem(row,1,new QTableWidgetItem {QDateTime::currentDateTime().toString()});
        f = QtConcurrent::run(this, &WizardPageCreateDatabaseOperation::performTask,
                              query);
        fw->setFuture(f);
    }
}

QSqlQuery WizardPageCreateDatabaseOperation::performTask(QSqlQuery query)
{
    qDebug() << "performTask";
    sleep(2);

    return query;
}

void WizardPageCreateDatabaseOperation::taskFinished()
{
    qDebug() << "taskFinished";
    ui->progressBar->setValue(ui->progressBar->value()+m_currentTask->progress);
    m_currentTask++;
    taskCallback(f.result());
}

void WizardPageCreateDatabaseOperation::taskCallback(QSqlQuery query)
{
    qDebug() << "taskCallback";
    // perform checks, update GUI, show messages, do next task
    performNextTask();
}
