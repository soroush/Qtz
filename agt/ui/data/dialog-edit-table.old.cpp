#include "dialog-edit-table.h"
#include "ui_dialog-edit-table.h"

DialogEditTable::DialogEditTable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEditTable) {
    ui->setupUi(this);
}

DialogEditTable::~DialogEditTable()
{
    delete ui;
}

void DialogEditTable::setModel(QSqlRelationalTableModel * model)
{    
    ui->tableView->setModel(model);
}

void DialogEditTable::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void DialogEditTable::insert()
{
    int result = this->insertDialog->exec();
    // TODO: complete this code
    switch(result)
    {
    case QDialog::Accepted:
        break;
    case QDialog::Rejected:
        break;
    default:
        break;
    }
}

void DialogEditTable::remove()
{
    // TODO: Ask before remove
    QModelIndexList selection = ui->tableView->selectionModel()->selectedIndexes();
    while(! selection.isEmpty())
    {
        QModelIndex i = selection.last();
        this->ui->tableView->model()->removeRow(i.row(),i.parent());
        selection.removeLast();
    }
}
