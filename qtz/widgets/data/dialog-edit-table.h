#ifndef DIALOGEDITTABLE_H
#define DIALOGEDITTABLE_H

#include <QDialog>
#include <QModelIndex>
#include <QModelIndexList>
#include <qtz/core/library.h>

QT_BEGIN_NAMESPACE
class QVBoxLayout;
class QSqlRelationalTableModel;
QT_END_NAMESPACE
class WidgetEditTable;
class DialogInsertItem;

class QTZ_SHARED_EXPORT DialogEditTable : public QDialog {
    Q_OBJECT
public:
    explicit DialogEditTable(QWidget *parent = 0);
    virtual ~DialogEditTable();
    virtual void setModel(QSqlRelationalTableModel *model_);
    virtual void setAdderDialog(DialogInsertItem *dialog_);
    DialogInsertItem *adderDialog();
protected slots:
    virtual void add();
    virtual void remove(QModelIndexList);
    virtual void edit(QModelIndex);
    virtual void refresh();
    virtual void revert();
    virtual void save();
private:
    // UI
    QVBoxLayout *verticalLayout;
    WidgetEditTable *editTable;
    // Dialogs
    DialogInsertItem *insertDialog;
    // Data
    QSqlRelationalTableModel *model;
};

#endif // DIALOGEDITTABLE_H
