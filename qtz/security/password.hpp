#ifndef PASSWORD_HPP
#define PASSWORD_HPP

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

#endif // PASSWORD_HPP
