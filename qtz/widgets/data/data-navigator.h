#ifndef DATANAVIGATOR_H
#define DATANAVIGATOR_H

#include <QWidget>
#include <QSqlTableModel>

QT_BEGIN_NAMESPACE
namespace Ui {
class DataNavigator;
}
QT_END_NAMESPACE

class DataNavigator : public QWidget {
    Q_OBJECT

public:
    explicit DataNavigator(QWidget *parent = 0);
    ~DataNavigator();

    QSqlTableModel *model() const;
    size_t rowCount() const;

public slots:
    void setModel(QSqlTableModel *model);
    void setRowCount(const size_t &count);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DataNavigator *ui;
    QSqlTableModel *m_model;
    size_t m_rowCount;
};

#endif // DATANAVIGATOR_H
