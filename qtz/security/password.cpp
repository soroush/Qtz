#include "password.hpp"

Password::Password() {
}

Password::Password(const QString& password):
    m_password(password) {
}

void Password::setPassword(const QString& password) {
    this->m_password = password;
}

QString Password::getPassword() const {
    return this->m_password;
}
