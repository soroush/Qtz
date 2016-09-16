#ifndef QTZ_PASSWORD_HPP
#define QTZ_PASSWORD_HPP

#include "token.hpp"
#include <QString>

class Password : public Token {
public:
    Password();
    Password(const QString& password);
    void setPassword(const QString& password);
    QString getPassword() const;
private:
    QString m_password;
};

#endif // QTZ_PASSWORD_HPP
