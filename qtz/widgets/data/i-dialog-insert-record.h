#ifndef IDIALOGINSERTRECORD_H
#define IDIALOGINSERTRECORD_H

#include <QSqlTableModel>
#include <qtz/data/i-insert-record.h>
#include "qtz-widgets.h"

class QTZ_WIDGETS_SHARED_EXPORT IDialogInsertRecord : public IInsertRecord
{
    Q_OBJECT
public:
    explicit IDialogInsertRecord(QSqlTableModel* model, QObject *parent);
public slots:
    virtual void startCallback();
    virtual void finishCallback();

};

#endif // IDIALOGINSERTRECORD_H
