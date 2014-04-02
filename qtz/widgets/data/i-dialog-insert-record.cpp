#include "i-dialog-insert-record.h"
#include <QDialog>

IDialogInsertRecord::IDialogInsertRecord(QSqlTableModel *model, QObject *parent)
    :IInsertRecord(model,parent)
{
}

void IDialogInsertRecord::startCallback()
{
    qobject_cast<QDialog *>(this->parent())->setEnabled(false);
}

void IDialogInsertRecord::finishCallback()
{
    qobject_cast<QDialog *>(this->parent())->setEnabled(true);
}
