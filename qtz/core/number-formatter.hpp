#ifndef NUMBERFORMATTER_H
#define NUMBERFORMATTER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include "qtz-core.hpp"

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
    void setValue(quint64);
    void setValue(QString);

private:
    void convertInt2Numeral();
    QString convertHundreds2Numeral(unsigned long long);
    quint64 m_value;
    QString m_numeral;
};

#endif // NUMBERFORMATTER_H
