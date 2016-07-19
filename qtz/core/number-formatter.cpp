#include "number-formatter.h"
#include <QLocale>

QString NumberFormatter::persianAnd = QString::fromUtf8("\xd9\x88",2);
QString NumberFormatter::persianE9 = QString::fromUtf8("\xd9\x85\xdb\x8c\xd9\x84\xdb\x8c\xd8\xa7\xd8\xb1\xd8\xaf",14);
QString NumberFormatter::persianE6 = QString::fromUtf8("\xd9\x85\xdb\x8c\xd9\x84\xdb\x8c\xd9\x88\xd9\x86",12);
QString NumberFormatter::persianE3 = QString::fromUtf8("\xd9\x87\xd8\xb2\xd8\xa7\xd8\xb1",8);
QStringList NumberFormatter::persian100to900 = QStringList({
                QString::fromUtf8("\xdb\x8c\xda\xa9\xd8\xb5\xd8\xaf",8),
                QString::fromUtf8("\xd8\xaf\xd9\x88\xdb\x8c\xd8\xb3\xd8\xaa",10),
                QString::fromUtf8("\xd8\xb3\xdb\x8c\xd8\xb5\xd8\xaf",8),
                QString::fromUtf8("\xda\x86\xd9\x87\xd8\xa7\xd8\xb1\xd8\xb5\xd8\xaf",12),
                QString::fromUtf8("\xd9\xbe\xd8\xa7\xd9\x86\xd8\xb5\xd8\xaf",10),
                QString::fromUtf8("\xd8\xb4\xd8\xb4\xd8\xb5\xd8\xaf",8),
                QString::fromUtf8("\xd9\x87\xd9\x81\xd8\xaa\xd8\xb5\xd8\xaf",10),
                QString::fromUtf8("\xd9\x87\xd8\xb4\xd8\xaa\xd8\xb5\xd8\xaf",10),
                QString::fromUtf8("\xd9\x86\xd9\x87\xd8\xb5\xd8\xaf",8)
            });
QStringList NumberFormatter::persian10to19 = QStringList({
                QString::fromUtf8("\xd8\xaf\xd9\x87",4),
                QString::fromUtf8("\xdb\x8c\xd8\xa7\xd8\xb2\xd8\xaf\xd9\x87",10),
                QString::fromUtf8("\xd8\xaf\xd9\x88\xd8\xa7\xd8\xb2\xd8\xaf\xd9\x87",12),
                QString::fromUtf8("\xd8\xb3\xdb\x8c\xd8\xb2\xd8\xaf\xd9\x87",10),
                QString::fromUtf8("\xda\x86\xd9\x87\xd8\xa7\xd8\xb1\xd8\xaf\xd9\x87",12),
                QString::fromUtf8("\xd9\xbe\xd8\xa7\xd9\x86\xd8\xb2\xd8\xaf\xd9\x87",12),
                QString::fromUtf8("\xd8\xb4\xd8\xa7\xd9\x86\xd8\xb2\xd8\xaf\xd9\x87",12),
                QString::fromUtf8("\xd9\x87\xd9\x81\xd8\xaf\xd9\x87",8),
                QString::fromUtf8("\xd9\x87\xd8\xac\xd8\xaf\xd9\x87",8),
                QString::fromUtf8("\xd9\x86\xd9\x88\xd8\xb2\xd8\xaf\xd9\x87",10)});
QStringList NumberFormatter::persian20to90 = QStringList({
                QString::fromUtf8("\xd8\xa8\xdb\x8c\xd8\xb3\xd8\xaa",8),
                QString::fromUtf8("\xd8\xb3\xdb\x8c",4),
                QString::fromUtf8("\xda\x86\xd9\x87\xd9\x84",6),
                QString::fromUtf8("\xd9\xbe\xd9\x86\xd8\xac\xd8\xa7\xd9\x87",10),
                QString::fromUtf8("\xd8\xb4\xd8\xb5\xd8\xaa",6),
                QString::fromUtf8("\xd9\x87\xd9\x81\xd8\xaa\xd8\xa7\xd8\xaf",10),
                QString::fromUtf8("\xd9\x87\xd8\xb4\xd8\xaa\xd8\xa7\xd8\xaf",10),
                QString::fromUtf8("\xd9\x86\xd9\x88\xd8\xaf",6)});
QStringList NumberFormatter::persian1to9 = QStringList({
                QString::fromUtf8("\xdb\x8c\xda\xa9",4),
                QString::fromUtf8("\xd8\xaf\xd9\x88",4),
                QString::fromUtf8("\xd8\xb3\xd9\x87",4),
                QString::fromUtf8("\xda\x86\xd9\x87\xd8\xa7\xd8\xb1",8),
                QString::fromUtf8("\xd9\xbe\xd9\x86\xd8\xac",6),
                QString::fromUtf8("\xd8\xb4\xd8\xb4",4),
                QString::fromUtf8("\xd9\x87\xd9\x81\xd8\xaa",6),
                QString::fromUtf8("\xd9\x87\xd8\xb4\xd8\xaa",6),
                QString::fromUtf8("\xd9\x86\xd9\x87",4)});

NumberFormatter::NumberFormatter(QObject *parent) :
    QObject(parent)
{
}

unsigned long long NumberFormatter::value() const
{
    return this->m_value;
}

QString NumberFormatter::numeral() const
{
    return this->m_numeral;
}

void NumberFormatter::setValue(int value)
{
    this->m_numeral.clear();
    if(this->m_value != value){
        this->m_value=value;
        emit valueChanged(this->m_value);
        convertInt2Numeral();
        emit numeralChanged(this->m_numeral);
    }
}

void NumberFormatter::setValue(unsigned long long value)
{
    this->m_numeral.clear();
    if(this->m_value != value){
        this->m_value=value;
        emit valueChanged(this->m_value);
        convertInt2Numeral();
        emit numeralChanged(this->m_numeral);
    }
}

void NumberFormatter::setValue(QString value)
{
    bool ok = false;
    unsigned long long converted = QLocale::system().toUInt(value,&ok);
    if(ok){
        setValue(converted);
    }
}

void NumberFormatter::convertInt2Numeral()
{
    int p1 = this->m_value%1000;
    int p2 = (this->m_value/1000)%1000; // hezar
    int p3 = (this->m_value/1000000)%1000; // milion
    int p4 = this->m_value/1000000000; //miliard

    if(p4 != 0)
        m_numeral += QString("%1 %2")
                .arg(convertHundreds2Numeral(p4))
                .arg(persianE9);

    if(p3 != 0)
    {
       if(p4 != 0){
           m_numeral += QString(" %1 %2 %3")
                   .arg(persianAnd)
                   .arg(convertHundreds2Numeral(p3))
                   .arg(persianE6);
       }
       else{
           m_numeral += QString("%1 %2")
                   .arg(convertHundreds2Numeral(p3))
                   .arg(persianE6);
       }
    }

    if(p2 != 0)
    {
       if(p4 != 0 || p3 != 0)
           m_numeral += QString(" %1 %2 %3")
                   .arg(persianAnd)
                   .arg(convertHundreds2Numeral(p2))
                   .arg(persianE3);
       else
           m_numeral += QString("%1 %2").arg(convertHundreds2Numeral(p2))
                   .arg(persianE3);
    }

    if(p1 != 0)
    {
       if(p4 != 0 || p3 != 0 || p2 != 0)
           m_numeral += QString(" %1 %2")
                   .arg(persianAnd)
                   .arg(convertHundreds2Numeral(p1));
       else
           m_numeral += convertHundreds2Numeral(p1);
    }
}

QString NumberFormatter::convertHundreds2Numeral(unsigned long long num)
{
    int p1 = num%10;
    int p2 = (num/10)%10;
    int p3 = num/100;

    QString result;

    if(p3 != 0)
        result += persian100to900.at(p3-1);

    if(p2 != 0)
    {
        if(p2 == 1)
        {
            if(p3 != 0)
                result += QString(" %1 %2").arg(persianAnd).arg(persian10to19.at(p1));
            else
                result += persian10to19.at(p1);
        }
        else
        {
            if(p3 != 0)
                result += QString(" %1 %2").arg(persianAnd).arg(persian20to90.at(p2-2));
            else
                result += persian20to90.at(p2-2);
        }
    }

    if(p1 != 0 && p2 != 1)
    {
        if(p3 != 0 || p2 != 0)
            result += QString(" %1 %2").arg(persianAnd).arg(persian1to9.at(p1-1));
        else
            result += persian1to9.at(p1-1);
    }

    return result;
}
