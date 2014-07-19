#ifndef NUMBERFORMATTER_H
#define NUMBERFORMATTER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include "library.h"

class QTZ_SHARED_EXPORT NumberFormatter : public QObject
{
    Q_OBJECT
public:
    explicit NumberFormatter(QObject *parent = nullptr);
    unsigned long long value() const;
    QString numeral() const;

signals:
    void valueChanged(unsigned long long);
    void numeralChanged(QString);

public slots:
    void setValue(int);
    void setValue(unsigned long long);
    void setValue(QString);

private:
    void convertInt2Numeral();
    QString convertHundreds2Numeral(unsigned long long);
    unsigned long long m_value;
    QString m_numeral;
    // Statics
    static QString persianAnd;
    static QString persianE9;
    static QString persianE6;
    static QString persianE3;
    static QStringList persian100to900;
    static QStringList persian10to19;
    static QStringList persian20to90;
    static QStringList persian1to9;
};

#endif // NUMBERFORMATTER_H
