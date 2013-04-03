#ifndef EDITTABLEWIDGET_H
#define EDITTABLEWIDGET_H

#include <QWidget>
#include <QSqlTableModel>
#include "dialog-insert-item.h"
#include "dialog-edit-item.h"

namespace Ui {
class EditTableWidget;
}

class WidgetEditTable : public QWidget
{
    Q_OBJECT
    
public:
    explicit WidgetEditTable(QWidget *parent = 0);
    ~WidgetEditTable();

protected:
    void changeEvent(QEvent *e);
    
private:
    Ui::EditTableWidget *ui;
    DialogInsertItem* insertDialog;
    DialogEditItem* editDialog;

    QSqlTableModel* model;

private slots:
    void insert();
    void remove();
    void edit();
    void refresh();
    void revert();
    void save();
};

#endif // EDITTABLEWIDGET_H
