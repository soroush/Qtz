#include "i-dialog-insert-record.hpp"
#include <QDialog>

//IDialogInsertRecord::IDialogInsertRecord(QWidget *parent, QSqlTableModel *model):
//    QDialog(parent), m_inserter(this,model){
//    connect(&this->m_inserter,SIGNAL(started()),this,SLOT(start()));
//    connect(&this->m_inserter,SIGNAL(finished()),this,SLOT(finish()));
//}

//IDialogInsertRecord::~IDialogInsertRecord(){
//}

//void IDialogInsertRecord::start()
//{
//    m_lastCursor = this->cursor();
//    QCursor waitingCursor{Qt::WaitCursor};
//    this->setCursor(waitingCursor);
//    this->setDisabled(true);
//}

//void IDialogInsertRecord::finish()
//{
//    this->setCursor(m_lastCursor);
//    this->setEnabled(true);
//}

//IInsertRecord& IDialogInsertRecord::getInserter()
//{
//    return this->m_inserter;
//}
