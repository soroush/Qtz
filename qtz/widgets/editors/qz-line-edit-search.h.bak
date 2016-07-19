#ifndef QZLINEEDITSEARCH_H
#define QZLINEEDITSEARCH_H

#include <QLineEdit>
#include "../export.h"

class QEvent;

class QTZ_WIDGETS_SHARED_EXPORT QzLineEditSearch : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(QString searchText READ searchText WRITE setSearchText NOTIFY searchTextChanged USER true)
    Q_PROPERTY(QString searchStyleSheet READ searchStyleSheet WRITE setSearchStyleSheet USER true)
//    Q_ENUMS(State)

public:
    explicit QzLineEditSearch(QWidget *parent = 0);
    QString searchText();
    QString searchStyleSheet();

    enum class State {
        Search,
        Empty,
        Text
    };

protected:
    bool event(QEvent *e);
    QString m_searchText;
    State m_currentState;
    void setStatus(const State&);
    State status();

private:
    QString m_searchStyleSheet;
    QString m_textStyleSheet;
    bool m_internalChange;

public slots:
    void updateStatus(const QString &);
    void setSearchText(const QString& text);
    void setSearchStyleSheet(const QString& style);

signals:
    void searchTextChanged(QString);
};

#endif // QZLINEEDITSEARCH_H
