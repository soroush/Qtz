#include "sexagesimal-spinbox.hpp"
#include <QLineEdit>
#include <cmath>

#include <QDebug>

SexagesimalSpinBox::SexagesimalSpinBox(QWidget* parent) :
    QAbstractSpinBox(parent),
    m_value(0) {
    connect(this->lineEdit(),SIGNAL(textEdited(QString)),this,
            SLOT(setText(QString)));
    d2s();
    updateText();
}

void SexagesimalSpinBox::setValue(const double& newValue) {
    if(m_value != newValue) {
        this->m_value = newValue;
        emit valueChanged(m_value);
        d2s();
        updateText();
    }
}

void SexagesimalSpinBox::setValue(const int& d, const int& m, const double& s) {
    if(!(d==m_degree && m==m_minute && s==m_second)) {
        if(d>=0 && d<60 && m>=0 && m<60 && s>=0.0 && s<60.0) {
            this->m_degree = d;
            this->m_minute= m;
            this->m_second= s;
            s2d();
            emit valueChanged(m_value);
            updateText();
        }
    }
}

void SexagesimalSpinBox::keyPressEvent(QKeyEvent* event) {
    switch(event->key()) {
        case Qt::Key_Up:
            stepUp();
            break;
        case Qt::Key_Down:
            stepDown();
            break;
        default:
            break;
    }
    QAbstractSpinBox::keyPressEvent(event);
}

void SexagesimalSpinBox::focusInEvent(QFocusEvent* event) {
    if(event->reason()==Qt::TabFocusReason) {
        lineEdit()->setSelection(0,4);
        setFocus();
        event->ignore();
    } else if(event->reason()==Qt::BacktabFocusReason) {
        lineEdit()->setSelection(10,2);
        setFocus();
        event->ignore();
    } else {
        QAbstractSpinBox::focusOutEvent(event);
    }
}

void SexagesimalSpinBox::focusOutEvent(QFocusEvent* event) {
    if(event->reason()==Qt::TabFocusReason) {
        int position = lineEdit()->cursorPosition();
        if(position < 5) {
            lineEdit()->setSelection(6,2);
            setFocus();
            event->ignore();
        } else if(position >= 5 && position <= 8) {
            lineEdit()->setSelection(10,2);
            setFocus();
            event->ignore();
        } else if(position >= 8) {
            QAbstractSpinBox::focusOutEvent(event);
        }
    } else if(event->reason()==Qt::BacktabFocusReason) {
        int position = lineEdit()->cursorPosition();
        if(position < 5) {
            QAbstractSpinBox::focusOutEvent(event);
        } else if(position >= 5 && position <= 8) {
            lineEdit()->setSelection(0,4);
            setFocus();
            event->ignore();
        } else if(position >= 8) {
            lineEdit()->setSelection(6,2);
            setFocus();
            event->ignore();
        }
    } else {
        QAbstractSpinBox::focusOutEvent(event);
    }
}

void SexagesimalSpinBox::wheelEvent(QWheelEvent* event) {
    //+000° 00′ 00″
    int proposedChange = (event->delta()/120.0);
    this->stepBy(proposedChange);
    event->accept();
}

QAbstractSpinBox::StepEnabled SexagesimalSpinBox::stepEnabled() const {
    StepEnabled flags;
    int position = this->lineEdit()->cursorPosition();
    int number = 30;
    if(position >= 5 && position <= 8) {
        number = this->m_minute;
    } else if(position >= 10) {
        number = this->m_second;
    }
    switch(number) {
        case 59:
            flags = StepDownEnabled;
            break;
        case 0:
            flags = StepUpEnabled;
            break;
        default:
            flags = StepDownEnabled | StepUpEnabled;
            break;
    }
    return flags;
}

void SexagesimalSpinBox::addDegree(const int& d) {
    if((this->m_degree + d)<180 && (this->m_degree + d)>-180) {
        this->m_degree += d;
        s2d();
    }
}

void SexagesimalSpinBox::addMinute(const int& m) {
    if((this->m_minute + m)<60 && (this->m_minute + m)>=0) {
        this->m_minute += m;
        s2d();
    }
}

void SexagesimalSpinBox::addSecond(const int& s) {
    if((this->m_second + s)<60 && (this->m_second + s)>=0) {
        this->m_second += s;
        s2d();
    }
}

void SexagesimalSpinBox::s2d() {
    this->m_value =
        this->m_degree +
        static_cast<double>(this->m_minute) / 60.0 +
        static_cast<double>(this->m_second) / 3600.0;
}

void SexagesimalSpinBox::d2s() {
    this->m_degree = static_cast<int>(floor(this->m_value));
    double d_minute = (this->m_value-floor(this->m_value))*60.0;
    qDebug() << d_minute;
    this->m_minute = static_cast<int>(floor(d_minute));
    qDebug() << m_minute;
    this->m_second = static_cast<int>((d_minute-floor(d_minute))*60.0);
    qDebug() << m_second;
}

void SexagesimalSpinBox::stepBy(int steps) {
    int position = this->lineEdit()->cursorPosition();
    if(position < 5) {
        this->addDegree(steps);
        updateText();
        lineEdit()->setSelection(0,4);
    } else if(position >= 5 && position <= 8) {
        this->addMinute(steps);
        updateText();
        lineEdit()->setSelection(6,2);
    } else if(position >= 10) {
        this->addSecond(steps);
        updateText();
        lineEdit()->setSelection(10,2);
    }
}

void SexagesimalSpinBox::setText(const QString& input) {
    //+000° 00′ 00″
    // TODO: Use a regular expression to parse all valid inputs
    int d,m,s;
    bool ok = false;
    d = input.mid(0,4).toInt(&ok);
    if(ok) {
        m = input.mid(6,2).toInt(&ok);
    }
    if(ok) {
        s = input.mid(10,2).toDouble(&ok);
    }
    if(ok) {
        this->setValue(d,m,s);
    }
}

void SexagesimalSpinBox::updateText() {
    this->lineEdit()->setText(QString::fromUtf8(u8"%4%1\u00b0 %2\u2032 %3\u2033")
                              .arg(m_degree,4,10,QLatin1Char('0'))
                              .arg(m_minute,2,10,QLatin1Char('0'))
                              .arg(m_second,2,'g',2,QLatin1Char('0'))
                              .arg(m_value >= 0 ? "+" : ""));
}
