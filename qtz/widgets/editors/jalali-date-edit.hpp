#ifndef JALALIDATEEDIT_HPP
#define JALALIDATEEDIT_HPP

#include <QObject>
#include <QWidget>
#include <QDate>
#include <QAbstractSpinBox>
#include <qtz/core/jalali-date.hpp>
#include "../qtz-widgets.h"

class QTZ_WIDGETS_SHARED_EXPORT JalaliDateEdit : public QAbstractSpinBox {
    Q_OBJECT
public:
    explicit JalaliDateEdit(QWidget* parent = 0);
    virtual void stepBy(int steps);
    QDate date() const;
    QSize sizeHint() const;
    QString displayFormat() const;
    void setDisplayFormat(const QString& format);
signals:
    void dateChanged(QDate);
public slots:
    void setDate(const QDate& date);
    void updateText();
protected:
    void keyPressEvent(QKeyEvent* event);
    void focusInEvent(QFocusEvent* event);
    void focusOutEvent(QFocusEvent* event);
    void wheelEvent(QWheelEvent* event);
    StepEnabled stepEnabled() const;
private slots:
    void updateDate();
    void checkAndEmmit();
private:
    void addYears(int years);
    void addMonths(int months);
    void addDays(int days);
    void processInput();
    bool inputSanityCheck();
    JalaliDate m_jdate;
    QLocale m_locale;
    QString m_displayFormat;

};

#endif // JALALIDATEEDIT_HPP
