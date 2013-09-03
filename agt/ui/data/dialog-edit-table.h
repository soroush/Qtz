#ifndef DIALOGEDITTABLE_H
#define DIALOGEDITTABLE_H

#include <QDialog>
#include <QModelIndex>
#include <QModelIndexList>

class QVBoxLayout;
class WidgetEditTable;
class QSqlRelationalTableModel;
class DialogInsertItem;

class DialogEditTable : public QDialog
{
    Q_OBJECT
public:
    explicit DialogEditTable(QWidget *parent = 0);
    virtual ~DialogEditTable();
    virtual void setModel(QSqlRelationalTableModel* model_);
    virtual void setAdderDialog(DialogInsertItem* dialog_);
    DialogInsertItem* adderDialog();
protected slots:
    virtual void add();
    virtual void remove(QModelIndexList);
    virtual void edit(QModelIndex);
    virtual void refresh();
    virtual void revert();
    virtual void save();
private:
    // UI
    QVBoxLayout* verticalLayout;
    WidgetEditTable* editTable;
    // Dialogs
    DialogInsertItem* insertDialog;
    // Data
    QSqlRelationalTableModel* model;
};

#endif // DIALOGEDITTABLE_H
