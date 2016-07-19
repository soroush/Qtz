#ifndef TEXTQUERY_H
#define TEXTQUERY_H

#include <QWidget>
#include "../qtz-widgets.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class TextQuery;
}
QT_END_NAMESPACE

class QTZ_WIDGETS_SHARED_EXPORT TextQuery : public QWidget {
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
