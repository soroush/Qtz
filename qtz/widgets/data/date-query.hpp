#ifndef QTZ_DATEQUERY_HPP
#define QTZ_DATEQUERY_HPP

#include <QWidget>
#include "../qtz-widgets.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class DateQuery;
}
QT_END_NAMESPACE

class QTZ_WIDGETS_SHARED_EXPORT DateQuery : public QWidget {
    Q_OBJECT

public:
    explicit DateQuery(QWidget* parent = 0);
    ~DateQuery();
    QString statement() const;
public slots:
    void makeStatement();
    void setFieldName(QString);
signals:
    void fieldNameChanged(QString);
    void statementChanged(QString);

protected:
    void changeEvent(QEvent* e);

private:
    Ui::DateQuery* ui;
    QString fieldName;
    QString m_statement;
    const QString standardFormat;
};

#endif // QTZ_DATEQUERY_HPP
