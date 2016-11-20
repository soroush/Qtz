#ifndef QTZ_JALALIDATE_HPP
#define QTZ_JALALIDATE_HPP

#include <QDate>
#include <QLocale>
#include "qtz-core.hpp"

class QTZ_SHARED_EXPORT JalaliDate {
public:
    enum InvalidReason {
        BigDay,
        SmallDay,
        BigMonth,
        SmallMonth,
    };

    explicit JalaliDate();
    JalaliDate(int y, int m, int d);
    JalaliDate(const QDate&);
    JalaliDate(const JalaliDate&);
    QDate toGregorian() const;
    JalaliDate addDays(qint64 ndays) const;
    JalaliDate addMonths(int nmonths) const;
    JalaliDate addYears(int nyears) const;
    int day() const;
    int dayOfWeek() const;
    int dayOfYear() const;
    int daysInMonth() const;
    int daysInYear() const;
    qint64 daysTo(const JalaliDate& d) const;
    void getDate(int* year, int* month, int* day) const;
    bool isNull() const;
    bool isValid() const;
    int month() const;
    bool setDate(int year, int month, int day);
    // qint64 toJulianDay() const; // Deleted

    QString toString(const QString& format) const;
    QString toString(Qt::DateFormat format = Qt::TextDate) const;
    int weekNumber(int* yearNumber = Q_NULLPTR) const;
    int year() const;
    bool operator!=(const JalaliDate& d) const;
    bool operator==(const JalaliDate& d) const;
    bool operator<(const JalaliDate& d) const;
    bool operator<=(const JalaliDate& d) const;
    bool operator>(const JalaliDate& d) const;
    bool operator>=(const JalaliDate& d) const;

    static JalaliDate currentDate();
    // static JalaliDate fromJulianDay(qint64 jd); // DELETED
    //static JalaliDate fromString(const QString& string, Qt::DateFormat format = Qt::TextDate);
    //static JalaliDate fromString(const QString& string, const QString& format);
    static bool isLeapYear(int year);
    static bool isValid(int year, int month, int day, InvalidReason* reason=Q_NULLPTR);
    static QString longDayName(int weekday, QDate::MonthNameType type = QDate::DateFormat);
    static QString longMonthName(int month, QDate::MonthNameType type = QDate::DateFormat);
    static QString shortDayName(int weekday, QDate::MonthNameType type = QDate::DateFormat);
    static QString shortMonthName(int month,  QDate::MonthNameType type = QDate::DateFormat);
    static JalaliDate fromGregorian(const QDate&);
private:
    void gregorianToJalali();
    static JalaliDate gregorianToJalali(int year, int month, int day);
    void jalaliToGregorian();
    struct JalaliDatePrivate {
        JalaliDatePrivate()=default;
        JalaliDatePrivate(quint16 year, quint8 month, quint8 day);
        quint16 year;
        quint8 month;
        quint8 day;
    };

    QDate m_date;
    JalaliDatePrivate m_jdate;
    // Control nullity
    bool m_isNull;
    // Locale
    QLocale m_locale;
};

#endif // QTZ_JALALIDATE_HPP
