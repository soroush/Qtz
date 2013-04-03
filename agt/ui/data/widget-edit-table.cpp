#include "widget-edit-table.h"
#include "ui_widget-edit-table.h"

WidgetEditTable::WidgetEditTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditTableWidget)
{
    ui->setupUi(this);
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

void WidgetEditTable::insert()
{
    insertDialog->exec();
}

void WidgetEditTable::remove()
{

}

void WidgetEditTable::edit()
{
}

void WidgetEditTable::refresh()
{
}

void WidgetEditTable::revert()
{
}

void WidgetEditTable::save()
{
}
