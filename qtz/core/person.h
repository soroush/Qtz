#ifndef PERSON_H
#define PERSON_H

#include <QObject>
#include "library.h"

class QTZ_SHARED_EXPORT Person : public QObject {
    Q_OBJECT

public:
    explicit Person(QObject *parent = 0);

    QString name() const;
    QString last() const;

public slots:
    void name(const QString &);
    void last(const QString &);

private:
    QString name_;
    QString last_;
};

#endif // PERSON_H
