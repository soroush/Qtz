#ifndef NUMERICQUERY_H
#define NUMERICQUERY_H

#include <QWidget>
#include <agt/global.h>

namespace Ui {
class NumericQuery;
}

class AGTSHARED_EXPORT NumericQuery : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(double minimum READ min WRITE setMin USER true)
    Q_PROPERTY(double maximum READ max WRITE setMax USER true)
    Q_PROPERTY(int decimals READ decimals WRITE setDecimals USER true)
public:
    explicit NumericQuery(QWidget *parent = 0);
    ~NumericQuery();

    QString statement();
    double min();
    double max();
    int decimals();

public slots:
    void makeStatement();
    void setFieldName(QString);
    void setMin(double);
    void setMax(double);
    void setDecimals(int);

signals:
    void fieldNameChanged(QString);
    void statementChanged(QString);

protected:
    void changeEvent(QEvent *e);
    
private:
    Ui::NumericQuery *ui;
    QString fieldName;
    QString m_statement;
    double m_min;
    double m_max;
    int m_decimals;
};

#endif // NUMERICQUERY_H
