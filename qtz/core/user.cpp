#include "user.hpp"

User::User()
{

}

User::User(const QString &)
{

}

void User::setUsername(const QString &username)
{
    this->m_username=username;
}

QString User::getUsername()
{
    return this->m_username;
}
