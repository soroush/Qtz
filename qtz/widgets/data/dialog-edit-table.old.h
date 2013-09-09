#ifndef DIALOGEDITTABLE_H
#define DIALOGEDITTABLE_H

#include <QDialog>
#include <QSqlRelationalTableModel>
#include "dialog-insert-item.h"

namespace Ui {
class DialogEditTable;
}

class DialogEditTable : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogEditTable(QWidget *parent = 0);
    ~DialogEditTable();

    void setModel(QSqlRelationalTableModel*);
    
protected:
    void changeEvent(QEvent *e);

protected slots:
    virtual void insert();
    virtual void remove();
    virtual void edit(){};
    virtual void refresh(){};
    virtual void revert(){};
    virtual void save(){};
    
private:
    Ui::DialogEditTable *ui;
    DialogInsertItem* insertDialog;

};

#endif // DIALOGEDITTABLE_H
