#include "jalali-date.hpp"
#include <QObject>

int g_days_in_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
int j_days_in_month[12] = {31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 29};

JalaliDate::JalaliDate():
    m_jdate(0,0,0) ,
    m_isNull(true) {
    m_locale.setNumberOptions(QLocale::c().numberOptions());
    m_locale.setNumberOptions(QLocale::OmitGroupSeparator);
}

JalaliDate::JalaliDate(int y, int m, int d) {
    if(isValid(y,m,d)) {
        m_jdate.year =y;
        m_jdate.month =m;
        m_jdate.day =d;
        m_isNull = false;
        jalaliToGregorian();
    } else {
        m_isNull = true;
    }
    m_locale.setNumberOptions(QLocale::c().numberOptions());
    m_locale.setNumberOptions(QLocale::OmitGroupSeparator);
}

JalaliDate::JalaliDate(const QDate& other) {
    m_isNull = other.isNull();
    m_date = other;
    gregorianToJalali();
    m_locale.setNumberOptions(QLocale::c().numberOptions());
    m_locale.setNumberOptions(QLocale::OmitGroupSeparator);
}

JalaliDate::JalaliDate(const JalaliDate& other):
    m_date(other.m_date),
    m_jdate(other.m_jdate),
    m_isNull(other.m_isNull),
    m_locale(other.m_locale) {
}

QDate JalaliDate::toGregorian() const {
    if(isValid() && !isNull()) {
        return m_date;
    }
    return QDate();
}

JalaliDate JalaliDate::fromGregorian(const QDate& source) {
    if(source.isValid()) {
        JalaliDate d;
        d.m_date = source;
        d.gregorianToJalali();
        return d;
    }
    return JalaliDate();
}

JalaliDate JalaliDate::addDays(qint64 ndays) const {
    if(!m_isNull) {
        QDate newDate = m_date.addDays(ndays);
        return JalaliDate(newDate);
    } else {
        return JalaliDate();
    }
}

JalaliDate JalaliDate::addMonths(int nmonths) const {
    if(!m_isNull) {
        QDate newDate = m_date.addMonths(nmonths);
        return JalaliDate(newDate);
    } else {
        return JalaliDate();
    }
}

JalaliDate JalaliDate::addYears(int nyears) const {
    if(!m_isNull) {
        QDate newDate = m_date.addYears(nyears);
        return JalaliDate(newDate);
    } else {
        return JalaliDate();
    }
}

int JalaliDate::day() const {
    return m_jdate.day;
}

int JalaliDate::dayOfWeek() const {
    int d = (m_date.dayOfWeek()+2)%7;
    if(d==0) {
        d=7;
    }
    return d;
}

int JalaliDate::dayOfYear() const {
    if(m_isNull) {
        return 0;
    }
    int day = m_jdate.day;
    for(int month=m_jdate.month-1; month>0; month--) {
        if(month<7) {
            day+=31;
        } else {
            day+=30;
        }
    }
    return day;
}

int JalaliDate::daysInMonth() const {
    if(m_isNull) {
        return 0;
    }
    if(m_jdate.month<7) {
        return 31;
    } else if(m_jdate.month < 12) {
        return 30;
    } else if(m_jdate.month == 12) {
        if(isLeapYear(m_jdate.year)) {
            return 30;
        } else {
            return 29;
        }
    }
    return 0;
}

int JalaliDate::daysInYear() const {
    if(m_isNull) {
        return 0;
    }
    if(isLeapYear(m_jdate.year)) {
        return 366;
    } else {
        return 365;
    }
}

qint64 JalaliDate::daysTo(const JalaliDate& d) const {
    if(d.isNull() || isNull()) {
        return 0;
    } else {
        return m_date.daysTo(d.toGregorian());
    }
}

void JalaliDate::getDate(int* year, int* month, int* day) const {
    if(!m_isNull) {
        if(year!=nullptr) {
            *year = m_jdate.year;
        }
        if(month!=nullptr) {
            *month = m_jdate.month;
        }
        if(day!=nullptr) {
            *day = m_jdate.day;
        }
    }
}

bool JalaliDate::isNull() const {
    return m_isNull;
}

bool JalaliDate::isValid() const {
    if(m_isNull) {
        return false;
    } else {
        return JalaliDate::isValid(m_jdate.year,m_jdate.month,m_jdate.day);
    }
}

int JalaliDate::month() const {
    return m_jdate.month;
}

bool JalaliDate::setDate(int year, int month, int day) {
    m_jdate.year=year;
    m_jdate.month=month;
    m_jdate.day=day;
    if(isValid(year, month, day)) {
        m_isNull=false;
        jalaliToGregorian();
        return true;
    } else {
        return false;
    }
}

QString qt_readEscapedFormatString(const QString& format, int* idx) {
    int& i = *idx;
    Q_ASSERT(format.at(i) == QLatin1Char('\''));
    ++i;
    if (i == format.size()) {
        return QString();
    }
    if (format.at(i).unicode() == '\'') { // "''" outside of a quoted stirng
        ++i;
        return QLatin1String("'");
    }
    QString result;
    while (i < format.size()) {
        if (format.at(i).unicode() == '\'') {
            if (i + 1 < format.size() && format.at(i + 1).unicode() == '\'') {
                // "''" inside of a quoted string
                result.append(QLatin1Char('\''));
                i += 2;
            } else {
                break;
            }
        } else {
            result.append(format.at(i++));
        }
    }
    if (i < format.size()) {
        ++i;
    }
    return result;
}

int qt_repeatCount(const QString& s, int i) {
    QChar c = s.at(i);
    int j = i + 1;
    while (j < s.size() && s.at(j) == c) {
        ++j;
    }
    return j - i;
}

QString JalaliDate::toString(const QString& format) const {
    QString result;
    int i = 0;
    while (i < format.size()) {
        if (format.at(i).unicode() == '\'') {
            result.append(qt_readEscapedFormatString(format, &i));
            continue;
        }
        const QChar c = format.at(i);
        int repeat = qt_repeatCount(format, i);
        bool used = false;
        switch (c.unicode()) {
            case 'y':
                used = true;
                if (repeat >= 4) {
                    repeat = 4;
                } else if (repeat >= 2) {
                    repeat = 2;
                }
                switch (repeat) {
                    case 4: {
                            const int yr = this->year();
                            const int len = (yr < 0) ? 5 : 4;
                            result.append(QString("%1").arg(m_locale.toString(this->year()),4,m_locale.toString(0).at(0)));
                            break;
                        }
                    case 2:
                        result.append(QString("%1").arg(m_locale.toString(this->year() % 100),2,m_locale.toString(0).at(0)));
                        break;
                    default:
                        repeat = 1;
                        result.append(c);
                        break;
                }
                break;
            case 'M':
                used = true;
                repeat = qMin(repeat, 4);
                switch (repeat) {
                    case 1:
                        result.append(m_locale.toString(this->month()));
                        break;
                    case 2:
                        result.append(QString("%1").arg(m_locale.toString(this->month()),2,m_locale.toString(0).at(0)));
                        break;
                    case 3:
                        result.append(JalaliDate::shortMonthName(this->month()));
                        break;
                    case 4:
                        result.append(JalaliDate::longMonthName(this->month()));
                        break;
                }
                break;
            case 'd':
                used = true;
                repeat = qMin(repeat, 4);
                switch (repeat) {
                    case 1:
                        result.append(m_locale.toString(this->day()));
                        break;
                    case 2:
                        result.append(QString("%1").arg(m_locale.toString(this->day()),2,m_locale.toString(0).at(0)));
                        break;
                    case 3:
                        result.append(JalaliDate::shortDayName(this->dayOfWeek()));
                        break;
                    case 4:
                        result.append(JalaliDate::longDayName(this->dayOfWeek()));
                        break;
                }
                break;
            default:
                break;
        }
        if (!used) {
            result.append(QString(repeat, c));
        }
        i += repeat;
    }
    return result;
}

QString JalaliDate::toString(Qt::DateFormat format) const {
    switch (format) {
        case Qt::TextDate:
            return toString("ddd MMM d yyyy");
        case Qt::ISODate:
            return toString("YYYY-MM-DD");
        case Qt::SystemLocaleShortDate:
            return toString("YYYY-MM-DD");
        case Qt::SystemLocaleLongDate:
            return toString("dddd, MMMM dd, yyyy");
        case Qt::DefaultLocaleShortDate:
            return toString("M/d/yyyy");
        case Qt::DefaultLocaleLongDate:
            return toString("dddd, MMMM dd, yyyy");
        case Qt::RFC2822Date:
            return toString("ddd MMM dd yyyy");
        default:
            return toString("ddd MMM d yyyy");
    }
}

int JalaliDate::weekNumber(int* yearNumber) const {
    JalaliDate firstDayOfYear = JalaliDate(m_jdate.year,1,1);
    int y_1_1 = firstDayOfYear.dayOfWeek();
    int firstShanbe = (7-y_1_1+2);
    JalaliDate firstShanbeOfYear = JalaliDate(m_jdate.year,1,firstShanbe);
    int diff = firstShanbeOfYear.m_date.daysTo(m_date);
    if(yearNumber!=nullptr) {
        *yearNumber=m_jdate.year;
    }
    return (diff/7)+1;
}

int JalaliDate::year() const {
    return m_jdate.year;
}

bool JalaliDate::operator!=(const JalaliDate& d) const {
    return !(*this==d);
}

bool JalaliDate::operator==(const JalaliDate& d) const {
    return m_jdate.day == d.m_jdate.day &&
           m_jdate.month == d.m_jdate.month &&
           m_jdate.year == d.m_jdate.year;
}

bool JalaliDate::operator<(const JalaliDate& d) const {
    if(m_jdate.year < d.m_jdate.year) {
        return true;
    } else if(m_jdate.year == d.m_jdate.year) {
        if(m_jdate.month < d.m_jdate.month) {
            return true;
        } else if(m_jdate.month == d.m_jdate.month) {
            if(m_jdate.day < d.m_jdate.day) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool JalaliDate::operator<=(const JalaliDate& d) const {
    if(m_jdate.year < d.m_jdate.year) {
        return true;
    } else if(m_jdate.year == d.m_jdate.year) {
        if(m_jdate.month < d.m_jdate.month) {
            return true;
        } else if(m_jdate.month == d.m_jdate.month) {
            if(m_jdate.day <= d.m_jdate.day) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool JalaliDate::operator>(const JalaliDate& d) const {
    return !(*this<=d);
}

bool JalaliDate::operator>=(const JalaliDate& d) const {
    return !(*this<d);
}

JalaliDate JalaliDate::currentDate() {
    return JalaliDate(QDate::currentDate());
}

bool JalaliDate::isLeapYear(int year) {
    double a = 0.025;
    double b = 266;
    int leapDays0;
    int leapDays1;
    if (year > 0) {
        leapDays0 = ((year + 38) % 2820)*0.24219 + a;  // 0.24219 ~ extra days of one year
        leapDays1 = ((year + 39) % 2820)*0.24219 + a;  // 38 days is the difference of epoch to 2820-year cycle
    } else if(year < 0) {
        leapDays0 = ((year + 39) % 2820)*0.24219 + a;
        leapDays1 = ((year + 40) % 2820)*0.24219 + a;
    } else {
        return false;
    }
    int frac0 = (leapDays0 - static_cast<int>(leapDays0))*1000;
    int frac1 = (leapDays1 - static_cast<int>(leapDays1))*1000;
    if (frac0 <= b && frac1 > b) {
        return true;
    } else {
        return false;
    }
}

bool JalaliDate::isValid(int year, int month, int day, InvalidReason* reason) {
    if(month>0) {
        int mlimit;
        if(month<7) { // Months are 31 days
            mlimit=32;
        } else if (month > 6  && month <12) { // Months are 30 days
            mlimit=31;
        } else if(month==12) {
            if(isLeapYear(year)) {
                mlimit=31;
            } else {
                mlimit=30;
            }
        } else {
            if(reason) {
                *reason = JalaliDate::BigMonth;
            }
            return false;
        }
        // Check day
        if(day>0 && day <mlimit) {
            return true;
        } else {
            if(reason) {
                if(day>=mlimit) {
                    *reason = JalaliDate::BigDay;
                } else if(day<0) {
                    *reason = JalaliDate::SmallDay;
                }
            }
            return false;
        }
    } else {
        if(reason) {
            *reason = JalaliDate::SmallMonth;
        }
        return false;
    }
}

QString JalaliDate::longDayName(int weekday, QDate::MonthNameType type) {
    Q_UNUSED(type);
    switch (weekday) {
        case 1:
            return QObject::tr("Shanbe");
        case 2:
            return QObject::tr("Yekshanbe");
        case 3:
            return QObject::tr("Doshanbe");
        case 4:
            return QObject::tr("Seshanbe");
        case 5:
            return QObject::tr("Chaharshanbe");
        case 6:
            return QObject::tr("Panjshanbe");
        case 7:
            return QObject::tr("Jomee");
        default:
            return "";
    }
}

QString JalaliDate::longMonthName(int month, QDate::MonthNameType type) {
    Q_UNUSED(type);
    switch (month) {
        case 1:
            return QObject::tr("Farvardin");
        case 2:
            return QObject::tr("Ordibehesht");
        case 3:
            return QObject::tr("Khordad");
        case 4:
            return QObject::tr("Tir");
        case 5:
            return QObject::tr("Mordad");
        case 6:
            return QObject::tr("Shahrivar");
        case 7:
            return QObject::tr("Mehr");
        case 8:
            return QObject::tr("Aban");
        case 9:
            return QObject::tr("Azar");
        case 10:
            return QObject::tr("Dey");
        case 11:
            return QObject::tr("Bahman");
        case 12:
            return QObject::tr("Esfand");
        default:
            return "";
    }
}

QString JalaliDate::shortMonthName(int month, QDate::MonthNameType type) {
    Q_UNUSED(type);
    switch (month) {
        case 1:
            return QObject::tr("Frv");
        case 2:
            return QObject::tr("Ord");
        case 3:
            return QObject::tr("Khr");
        case 4:
            return QObject::tr("Tir");
        case 5:
            return QObject::tr("Mor");
        case 6:
            return QObject::tr("Shr");
        case 7:
            return QObject::tr("Meh");
        case 8:
            return QObject::tr("Abn");
        case 9:
            return QObject::tr("Azr");
        case 10:
            return QObject::tr("Dey");
        case 11:
            return QObject::tr("Bah");
        case 12:
            return QObject::tr("Esf");
        default:
            return "";
    }
}

QString JalaliDate::shortDayName(int weekday, QDate::MonthNameType type) {
    Q_UNUSED(type);
    switch (weekday) {
        case 1:
            return QObject::tr("Sha");
        case 2:
            return QObject::tr("1sh");
        case 3:
            return QObject::tr("2sh");
        case 4:
            return QObject::tr("3sh");
        case 5:
            return QObject::tr("4sh");
        case 6:
            return QObject::tr("5sh");
        case 7:
            return QObject::tr("Jom");
        default:
            return "";
    }
}

void JalaliDate::gregorianToJalali() {
    if(m_date.isNull()) {
        return;
    }
    int gy = m_date.year()-1600;
    int gm = m_date.month()-1;
    int gd = m_date.day()-1;
    long g_day_no, j_day_no;
    int j_np;
    int i;
    g_day_no = 365*gy+(gy+3)/4-(gy+99)/100+(gy+399)/400;
    for (i=0; i<gm; ++i) {
        g_day_no += g_days_in_month[i];
    }
    if (gm>1 && ((gy%4==0 && gy%100!=0) || (gy%400==0)))
        /* leap and after Feb */
    {
        ++g_day_no;
    }
    g_day_no += gd;
    j_day_no = g_day_no-79;
    j_np = j_day_no / 12053;
    j_day_no %= 12053;
    m_jdate.year = 979+33*j_np+4*(j_day_no/1461);
    j_day_no %= 1461;
    if (j_day_no >= 366) {
        m_jdate.year += (j_day_no-1)/365;
        j_day_no = (j_day_no-1)%365;
    }
    for (i = 0; i < 11 && j_day_no >= j_days_in_month[i]; ++i) {
        j_day_no -= j_days_in_month[i];
    }
    m_jdate.month = i+1;
    m_jdate.day = j_day_no+1;
}

JalaliDate JalaliDate::gregorianToJalali(int gy, int gm, int gd) {
    long g_day_no, j_day_no;
    int j_np;
    int i;
    int jyear;
    g_day_no = 365*gy+(gy+3)/4-(gy+99)/100+(gy+399)/400;
    for (i=0; i<gm; ++i) {
        g_day_no += g_days_in_month[i];
    }
    if (gm>1 && ((gy%4==0 && gy%100!=0) || (gy%400==0)))
        /* leap and after Feb */
    {
        ++g_day_no;
    }
    g_day_no += gd;
    j_day_no = g_day_no-79;
    j_np = j_day_no / 12053;
    j_day_no %= 12053;
    jyear = 979+33*j_np+4*(j_day_no/1461);
    j_day_no %= 1461;
    if (j_day_no >= 366) {
        jyear += (j_day_no-1)/365;
        j_day_no = (j_day_no-1)%365;
    }
    for (i = 0; i < 11 && j_day_no >= j_days_in_month[i]; ++i) {
        j_day_no -= j_days_in_month[i];
    }
    return JalaliDate(jyear,i+1,j_day_no+1);
}

void JalaliDate::jalaliToGregorian() {
    int gy, gm, gd;
    long g_day_no, j_day_no;
    int leap;
    int i;
    int j_y = m_jdate.year-979;
    int j_m = m_jdate.month-1;
    int j_d = m_jdate.day-1;
    j_day_no = 365*j_y + (j_y/33)*8 + (j_y%33+3)/4;
    for (i=0; i < j_m; ++i) {
        j_day_no += j_days_in_month[i];
    }
    j_day_no += j_d;
    g_day_no = j_day_no+79;
    gy = 1600 + 400*(g_day_no/146097); /* 146097 = 365*400 + 400/4 - 400/100 + 400/400 */
    g_day_no = g_day_no % 146097;
    leap = 1;
    if (g_day_no >= 36525) { /* 36525 = 365*100 + 100/4 */
        g_day_no--;
        gy += 100*(g_day_no/36524); /* 36524 = 365*100 + 100/4 - 100/100 */
        g_day_no = g_day_no % 36524;
        if (g_day_no >= 365) {
            g_day_no++;
        } else {
            leap = 0;
        }
    }
    gy += 4*(g_day_no/1461); /* 1461 = 365*4 + 4/4 */
    g_day_no %= 1461;
    if (g_day_no >= 366) {
        leap = 0;
        g_day_no--;
        gy += g_day_no/365;
        g_day_no = g_day_no % 365;
    }
    for (i = 0; g_day_no >= g_days_in_month[i] + (i == 1 && leap); i++) {
        g_day_no -= g_days_in_month[i] + (i == 1 && leap);
    }
    gm = i+1;
    gd = g_day_no+1;
    m_date = QDate(gy,gm,gd);
}

JalaliDate::JalaliDatePrivate::JalaliDatePrivate(quint16 y, quint8 m, quint8 d):
    year(y),month(m),day(d) {
}
