#include "jalali-date-edit.hpp"
#include <QLineEdit>
#include <QKeyEvent>
#include <QStringList>

#include <QDebug>

JalaliDateEdit::JalaliDateEdit(QWidget* parent):
    QAbstractSpinBox(parent),
    m_displayFormat("yyyy/MM/dd") {
    m_locale.setNumberOptions(QLocale::c().numberOptions());
    m_locale.setNumberOptions(QLocale::OmitGroupSeparator);
    connect(this->lineEdit(),SIGNAL(textEdited(QString)),this,SLOT(updateDate()));
    connect(this->lineEdit(),SIGNAL(textChanged(QString)),this,SLOT(checkAndEmmit()));
    m_jdate.setDate(1390,1,1);
    updateText();
}

void JalaliDateEdit::stepBy(int input) {
    int steps = input>0 ? 1 : -1;
    int position = lineEdit()->cursorPosition();
    if(position < 5) { // Changing year
        addYears(steps);
        updateText();
        lineEdit()->setSelection(0,4);
    } else if(position > 4 && position < 8) {  // Changing month
        addMonths(steps);
        updateText();
        lineEdit()->setSelection(5,2);
    } else if(position > 7 && position < 11) {  // Changing day
        addDays(steps);
        updateText();
        lineEdit()->setSelection(8,2);
    }
}

void JalaliDateEdit::setDate(const QDate& date) {
    if(m_jdate.toGregorian() != date) {
        m_jdate = JalaliDate::fromGregorian(date);
        //gregorianToJalali();
        updateText();
        emit dateChanged(m_jdate.toGregorian());
    }
}

void JalaliDateEdit::updateText() {
    lineEdit()->setText(m_jdate.toString(m_displayFormat));
    updateDate();
    emit dateChanged(m_jdate.toGregorian());
}

void JalaliDateEdit::keyPressEvent(QKeyEvent* event) {
    switch(event->key()) {
        case Qt::Key_Up:
            stepBy(1);
            event->ignore();
            break;
        case Qt::Key_Down:
            stepBy(-1);
            event->ignore();
            break;
        case Qt::Key_0:
        case Qt::Key_1:
        case Qt::Key_2:
        case Qt::Key_3:
        case Qt::Key_4:
        case Qt::Key_5:
        case Qt::Key_6:
        case Qt::Key_7:
        case Qt::Key_8:
        case Qt::Key_9:
            if(inputSanityCheck()) {
                QKeyEvent e2(event->type(),event->key(),event->modifiers(),
                             m_locale.toString(event->text().toInt())
                            );
                QAbstractSpinBox::keyPressEvent(&e2);
                processInput();
            } else {
                event->ignore();
            }
            break;
        case Qt::Key_Left:
        case Qt::Key_Right:
        case Qt::Key_Backslash:
            QAbstractSpinBox::keyPressEvent(event);
            break;
        default:
            event->ignore();
            break;
    }
}

void JalaliDateEdit::focusInEvent(QFocusEvent* event) {
    if(event->reason()==Qt::TabFocusReason) {
        lineEdit()->setSelection(0,4);
        setFocus();
        event->ignore();
    } else if(event->reason()==Qt::BacktabFocusReason) {
        lineEdit()->setSelection(8,2);
        setFocus();
        event->ignore();
    } else {
        QAbstractSpinBox::focusOutEvent(event);
    }
}

void JalaliDateEdit::focusOutEvent(QFocusEvent* event) {
    if(event->reason()==Qt::TabFocusReason) {
        int position = lineEdit()->cursorPosition();
        if(position < 5) { // On year
            lineEdit()->setSelection(5,2);
            setFocus();
            event->ignore();
        } else if(position > 4 && position < 8) {
            lineEdit()->setSelection(8,2);
            setFocus();
            event->ignore();
        } else if(position > 7) {
            QAbstractSpinBox::focusOutEvent(event);
        }
    } else if(event->reason()==Qt::BacktabFocusReason) {
        int position = lineEdit()->cursorPosition();
        if(position < 5) {
            QAbstractSpinBox::focusOutEvent(event);
        } else if(position > 4 && position < 8) {
            lineEdit()->setSelection(0,4);
            setFocus();
            event->ignore();
        } else if(position > 7) {
            lineEdit()->setSelection(5,2);
            setFocus();
            event->ignore();
        }
    } else {
        QAbstractSpinBox::focusOutEvent(event);
    }
}

void JalaliDateEdit::wheelEvent(QWheelEvent* event) {
    if(event->delta()>0) {
        stepBy(1);
    } else {
        stepBy(-1);
    }
    event->accept();
}

QAbstractSpinBox::StepEnabled JalaliDateEdit::stepEnabled() const {
    StepEnabled flags;
    int position = lineEdit()->cursorPosition();
    if(position < 5) { // Changing year
        if(m_jdate.year() <= 1) {
            flags = StepUpEnabled;
        } else {
            flags = StepUpEnabled | StepDownEnabled;
        }
    } else if(position > 4 && position < 8) {  // Changing month
        if(m_jdate.month() == 1) {
            flags = StepUpEnabled;
        } else if(m_jdate.month() == 12) {
            flags = StepDownEnabled;
        } else {
            flags = StepDownEnabled | StepUpEnabled;
        }
    } else if(position > 7 && position < 11) {  // Changing day
        int upper;
        if(m_jdate.month()<=6) {
            upper = 31;
        } else {
            upper = 30;
        }
        // TODO: Handle leap years too
        if(m_jdate.day()==upper) {
            flags = StepDownEnabled;
        } else if(m_jdate.day()==1) {
            flags = StepDownEnabled;
        } else {
            flags = StepDownEnabled | StepUpEnabled;
        }
    }
    return flags;
}

void JalaliDateEdit::updateDate() {
    QString input = lineEdit()->text();
    int y,m,d;
    bool ok = false;
    y = m_locale.toInt(input.mid(0,4),&ok);
    if(ok) {
        m = m_locale.toInt(input.mid(5,2),&ok);
    }
    if(ok) {
        d = m_locale.toInt(input.mid(8,2),&ok);
    }
    if(ok) {
        m_jdate.setDate(y,m,d);
        emit dateChanged(m_jdate.toGregorian());
    }
}

void JalaliDateEdit::checkAndEmmit() {
    emit dateChanged(m_jdate.toGregorian());
}

QDate JalaliDateEdit::date() const {
    return this->m_jdate.toGregorian();
}

QSize JalaliDateEdit::sizeHint() const {
    QSize orig = QAbstractSpinBox::sizeHint();
    const QFontMetrics fm(fontMetrics());
    int h = orig.height();
    int w = 0;
    QString s = lineEdit()->text() + QLatin1Char(' ');
    w = fm.width(s) + 30; // cursor blinking space.
    // TODO: calculate width of indicators instead
    return QSize(w, h);
}

QString JalaliDateEdit::displayFormat() const {
    return m_displayFormat;
}

void JalaliDateEdit::setDisplayFormat(const QString& format) {
    m_displayFormat = format;
    this->updateText();
}

void JalaliDateEdit::addYears(int years) {
    m_jdate = m_jdate.addYears(years);
}

void JalaliDateEdit::addMonths(int months) {
    int new_month = m_jdate.month() + months;
    JalaliDate::InvalidReason reason;
    if(JalaliDate::isValid(m_jdate.year(),new_month,m_jdate.day(),&reason)) {
        m_jdate.setDate(m_jdate.year(),new_month,m_jdate.day());
    } else {
        if(reason==JalaliDate::BigDay) {
            if(JalaliDate::isValid(m_jdate.year(),new_month,m_jdate.day()-1)) {
                m_jdate.setDate(m_jdate.year(),new_month,m_jdate.day()-1);
            }
        }
    }
}

void JalaliDateEdit::addDays(int days) {
    int new_day = m_jdate.day() + days;
    if(JalaliDate::isValid(m_jdate.year(),m_jdate.month(),new_day)) {
        m_jdate.setDate(m_jdate.year(),m_jdate.month(),new_day);
    }
}

void JalaliDateEdit::processInput() {
    int position = lineEdit()->cursorPosition();
    switch(position) {
        case 4:
            lineEdit()->setSelection(5,2);
            break;
        case 7:
            lineEdit()->setSelection(8,2);
            break;
        default:
            break;
    }
}

bool JalaliDateEdit::inputSanityCheck() {
    int position = lineEdit()->cursorPosition();
    QStringList parts = lineEdit()->text().split('/');
    // Determine section
    if(!lineEdit()->selectedText().isEmpty()) {
        return true;
    }
    if(parts.size()!=3) {
        // Handle error
    }
    if(position<5) {
        if(parts.at(0).length() >= 4) {
            lineEdit()->setSelection(5,2);
            return false;
        } else {
            return true;
        }
    } else if(position > 4 && position < 8) {
        if(parts.at(1).length() >= 2) {
            lineEdit()->setSelection(8,2);
            return false;
        } else {
            return true;
        }
    } else if(position >8 && position < 11) {
        if(parts.at(2).length() >= 2) {
            return false;
        } else {
            return true;
        }
    } else {
        return false;
    }
}
