#ifndef USER_H
#define USER_H

#include "person.h"
#include "core.h"

class User : public Person {
    Q_OBJECT
public:
    explicit User(QObject *parent = 0);

    QString username();
    QString password();

signals:

public slots:
    void username(const QString &);
    void password(const QString &);

private:
    quint64 id_;
    QString username_;
    QString password_;
};

#endif // USER_H
