#ifndef WIZARDPAGECREATEDATABASEOPERATION_H
#define WIZARDPAGECREATEDATABASEOPERATION_H

#include <QWizardPage>
#include <QList>
#include <QPair>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFutureWatcher>
#include <QFuture>

QT_BEGIN_NAMESPACE
namespace Ui {
class WizardPageCreateDatabaseOperation;
}
QT_END_NAMESPACE

class WizardPageCreateDatabaseOperation : public QWizardPage {
    Q_OBJECT

public:
    explicit WizardPageCreateDatabaseOperation(QWidget *parent = 0);
    ~WizardPageCreateDatabaseOperation();
    void addSql(QString &query, const QString title=QString {}, const quint8& progress=0);

public slots:
    void startTasks();
    void pauseTasks();
    void resumeTasks();

signals:
    void taskCompleted(QSqlQuery query);

protected:
    void changeEvent(QEvent *e);

private:
    struct Operation{
        quint8 progress;
        QString title;
        QString query;
    };

    Ui::WizardPageCreateDatabaseOperation *ui;
    //QList<QPair<QString,QString>> m_tasks;
    //QList<QPair<QString,QString>>::iterator m_currentTask;
    QList<Operation> m_tasks;
    QList<Operation>::iterator m_currentTask;
    QFutureWatcher<QSqlQuery> *fw;
    QFuture<QSqlQuery> f;

    void performNextTask();
    QSqlQuery performTask(QSqlQuery query);
    QSqlDatabase db;

private slots:
    void taskFinished();
    void taskCallback(QSqlQuery query);
};

#endif // WIZARDPAGECREATEDATABASEOPERATION_H
