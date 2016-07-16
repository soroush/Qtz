#ifndef QTZ_AUTHENTICATION_TOKEN_HPP
#define QTZ_AUTHENTICATION_TOKEN_HPP

#include <QString>

class AuthenticationToken
{
public:
    enum class Type {
        PasswordToken = 0x01,
    };
protected:
    AuthenticationToken(Type t);
public:
    Type type() const;
private:
    Type m_type;

};

class PasswordToken : public AuthenticationToken
{
public:
    PasswordToken();
    void setPassword(const QString& password);
    QString getPassword() const;
private:
    QString m_password;
};

#endif // QTZ_AUTHENTICATION_TOKEN_HPP
