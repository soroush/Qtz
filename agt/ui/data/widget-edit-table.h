#ifndef EDITTABLEWIDGET_H
#define EDITTABLEWIDGET_H

#include <QWidget>
#include "dialog-insert-item.h"

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
    QDialog* editDialog;

private slots:
    void genericInsert();
    void genericRemove();
    void genericEdit();
    void genericRefresh();
    void genericRevert();
    void genericSave();
};

#endif // EDITTABLEWIDGET_H
