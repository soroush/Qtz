#include "dialog-edit-table.h"
#include "widget-edit-table.h"

#include <QVBoxLayout>
#include <QSqlRelationalTableModel>
#include <QEvent>

DialogEditTable::DialogEditTable(QWidget *parent) :
    QDialog(parent), insertDialog(NULL)
{
    verticalLayout = new QVBoxLayout(this);
    editTable = new WidgetEditTable(this);
    verticalLayout->addWidget(this->editTable);
    connect(editTable, SIGNAL(add()), this, SLOT(add()));
    connect(editTable, SIGNAL(edit(QModelIndex)), this, SLOT(edit(QModelIndex)));
    connect(editTable, SIGNAL(refresh()), this, SLOT(refresh()));
    connect(editTable, SIGNAL(remove(QModelIndexList)), this,
            SLOT(remove(QModelIndexList)));
    connect(editTable, SIGNAL(revert()), this, SLOT(revert()));
    connect(editTable, SIGNAL(save()), this, SLOT(save()));
}

DialogEditTable::~DialogEditTable()
{
    delete verticalLayout;
    delete editTable;
}

void DialogEditTable::setModel(QSqlRelationalTableModel *model_)
{
    this->editTable->setModel(model_);
    this->model = model_;
}

void DialogEditTable::setAdderDialog(DialogInsertItem *dialog_)
{
    this->insertDialog = dialog_;
}

DialogInsertItem *DialogEditTable::adderDialog()
{
    return this->insertDialog;
}

void DialogEditTable::add()
{
    if( insertDialog != NULL ) {
        int result = this->insertDialog->exec();
        // TODO: complete this code
        switch(result) {
        case QDialog::Accepted:
            break;
        case QDialog::Rejected:
            break;
        default:
            break;
        }
    }
}

void DialogEditTable::remove(QModelIndexList indexes)
{
    if(indexes.isEmpty()) {
        return;
    }
    qSort(indexes.begin(), indexes.end(), qGreater<QModelIndex>());
    for(QModelIndexList::const_iterator iter = indexes.constBegin();
            iter != indexes.constEnd(); ++iter) {
        model->removeRow((*iter).row(), (*iter).parent());
    }
}

void DialogEditTable::edit(QModelIndex)
{
}

void DialogEditTable::refresh()
{
}

void DialogEditTable::revert()
{
}

void DialogEditTable::save()
{
}

