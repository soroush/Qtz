#include "widget-edit-table.h"
#include "ui_widget-edit-table.h"

WidgetEditTable::WidgetEditTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditTableWidget)
{
    ui->setupUi(this);
    connect(ui->toolButtonAdd, SIGNAL(clicked()), this, SIGNAL(add()));
    connect(ui->toolButtonRemove, SIGNAL(clicked()), this, SLOT(emitRemove()));
    connect(ui->toolButtonEdit, SIGNAL(clicked()), this, SLOT(emitEdit()));
    connect(ui->toolButtonRefresh, SIGNAL(clicked()), this, SIGNAL(refresh()));
    connect(ui->toolButtonRevert, SIGNAL(clicked()), this, SIGNAL(revert()));
    connect(ui->toolButtonSave, SIGNAL(clicked()), this, SIGNAL(save()));
}

WidgetEditTable::~WidgetEditTable()
{
    delete ui;
}

void WidgetEditTable::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void WidgetEditTable::setModel(QSqlRelationalTableModel *model)
{
    ui->tableView->setModel(model);
}

void WidgetEditTable::emitRemove()
{
    if(ui->tableView->currentIndex().isValid()) {
        QModelIndexList list = ui->tableView->selectionModel()->selectedIndexes();
        if(! list.empty()) {
            emit remove(list);
        }
    }
}

void WidgetEditTable::emitEdit()
{
    if(ui->tableView->currentIndex().isValid()) {
        QModelIndex i = ui->tableView->selectionModel()->selectedIndexes().at(0);
        if(i.isValid()) {
            emit edit(i);
        }
    }
}
