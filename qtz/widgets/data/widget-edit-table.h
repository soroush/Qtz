#ifndef EDITTABLEWIDGET_H
#define EDITTABLEWIDGET_H

#include <QWidget>
#include <QSqlRelationalTableModel>
#include "dialog-insert-item.h"
#include "dialog-edit-item.h"
#include "qtz-widgets.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class EditTableWidget;
}
QT_END_NAMESPACE

class WidgetEditTable : public QWidget {
    Q_OBJECT

public:
    explicit WidgetEditTable(QWidget *parent = 0);
    ~WidgetEditTable();

    void setModel(QSqlRelationalTableModel *model);

signals:
    void add();
    void remove(QModelIndexList);
    void edit(QModelIndex);
    void refresh();
    void revert();
    void save();

protected:
    void changeEvent(QEvent *e);

protected slots:
    void emitRemove();
    void emitEdit();

private:
    Ui::EditTableWidget *ui;

};

#endif // EDITTABLEWIDGET_H
