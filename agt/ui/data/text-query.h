#ifndef TEXTQUERY_H
#define TEXTQUERY_H

#include <QWidget>
#include <agt/global.h>


namespace Ui {
class TextQuery;
}

class TextQuery : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString inputMask READ inputMask WRITE setInputMask USER true)

public:
    explicit TextQuery(QWidget *parent = 0);
    ~TextQuery();

    QString statement();
    QString inputMask();

public slots:
     void setFieldName(QString);
     void setInputMask(QString);
     void makeStatement();
     void pressReturn();

signals:
    void fieldNameChanged(QString);
    void statementChanged(QString);
    void inputMaskChanged(QString);
    void returnPressed();

protected:
    void changeEvent(QEvent *e);
    
private:
    Ui::TextQuery *ui;
    QString fieldName;
    QString m_statement;
    QString m_inputMask;
};

#endif // TEXTQUERY_H
