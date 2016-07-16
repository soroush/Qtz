#ifndef QTZ_USER_HPP
#define QTZ_USER_HPP

#include <QString>

class User
{
public:
    User();
    User(const QString&);
    void setUsername(const QString&username);
    QString getUsername();
private:
    QString m_username;
};

#endif // QTZ_USER_HPP
