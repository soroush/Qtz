#include "dialog-edit-table.h"
#include "ui_dialog-edit-table.h"

DialogEditTable::DialogEditTable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEditTable)
{
    ui->setupUi(this);
}

DialogEditTable::~DialogEditTable()
{
    delete ui;
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
