#include "username.hpp"

Username::Username(const QString& username):
    m_username(username) {
}

void Username::setUsername(const QString& username) {
    this->m_username = username;
}

QString Username::getUsername() const {
    return this->m_username;
}
