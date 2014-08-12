#ifndef IDIALOGINSERTRECORD_H
#define IDIALOGINSERTRECORD_H

#include <QDialog>
#include <QSqlTableModel>
#include <qtz/data/i-insert-record.h>
#include <qtz/core/library.h>

class QTZ_SHARED_EXPORT IDialogInsertRecord : public QDialog
{
    Q_OBJECT
public:
    explicit IDialogInsertRecord(QWidget *parent=nullptr, QSqlTableModel* model=nullptr);
    virtual ~IDialogInsertRecord();
    IInsertRecord &getInserter();
private slots:
    void start();
    void finish();
private:
    IInsertRecord m_inserter;
    QCursor m_lastCursor;
};

#endif // IDIALOGINSERTRECORD_H
