#include "authentication-token.hpp"

AuthenticationToken::AuthenticationToken(Type t):
    m_type(t) {

}

AuthenticationToken::Type AuthenticationToken::type() const
{
    return this->m_type;
}

PasswordToken::PasswordToken() :
    AuthenticationToken(Type::PasswordToken) {
}

void PasswordToken::setPassword(const QString &password) {
    this->m_password = password;
}

QString PasswordToken::getPassword() const {
    return this->m_password;
}
