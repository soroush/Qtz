#ifndef WIZARDPAGECREATEDATABASEOPERATION_H
#define WIZARDPAGECREATEDATABASEOPERATION_H

#include <QWizardPage>
#include <QList>
#include <QPair>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFutureWatcher>
#include <QFuture>
#include <QTime>
#include "qtz-widgets.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class WizardPageCreateDatabaseOperation;
}
QT_END_NAMESPACE

class QTZ_WIDGETS_SHARED_EXPORT WizardPageCreateDatabaseOperation : public QWizardPage {
    Q_OBJECT

public:
    explicit WizardPageCreateDatabaseOperation(QWidget *parent = 0);
    ~WizardPageCreateDatabaseOperation();
    void addSql(QString &query, const QString title=QString {}, const quint8
                &progress=0);
    void initializePage();
    bool isComplete() const;

public slots:
    void estimateTime();

signals:
    void taskCompleted(QSqlQuery query);

protected:
    void changeEvent(QEvent *e);

private:
    void performNextTask();
    bool performTask(QSqlQuery query);

    struct Operation {
        quint8 progress;
        QString title;
        QString query;
    };

    Ui::WizardPageCreateDatabaseOperation *ui;
    QList<Operation> m_tasks;
    QList<Operation>::iterator m_currentTask;
    QFutureWatcher<bool> *m_futureWatcher;
    QFuture<bool> m_future;
    QSqlDatabase m_db;
    bool m_finished;
    QTime m_start;
    QTime m_end;

private slots:
    void taskFinished();
    void taskCallback(bool result);
};

#endif // WIZARDPAGECREATEDATABASEOPERATION_H
