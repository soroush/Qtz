#ifndef PERSON_H
#define PERSON_H

#include <QObject>
#include <agt/global.h>

class AGTSHARED_EXPORT Person : public QObject
{
    Q_OBJECT
public:
    explicit Person(QObject *parent = 0);

    void name(const QString&);
    void last(const QString&);

    QString name() const;
    QString last() const;

private:
    QString name_;
    QString last_;
};

#endif // PERSON_H
