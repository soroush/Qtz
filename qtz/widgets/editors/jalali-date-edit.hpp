#ifndef QTZ_JALALIDATEEDIT_HPP
#define QTZ_JALALIDATEEDIT_HPP

#include <QObject>
#include <QWidget>
#include <QDate>
#include <QAbstractSpinBox>
#include <qtz/core/jalali-date.hpp>
#include "../qtz-widgets.hpp"

class QTZ_WIDGETS_SHARED_EXPORT JalaliDateEdit : public QAbstractSpinBox {
    Q_OBJECT
    Q_PROPERTY(QDate date READ date WRITE setDate USER true)
    Q_PROPERTY(QString displayFormat READ displayFormat WRITE setDisplayFormat USER true)
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

#endif // QTZ_JALALIDATEEDIT_HPP
