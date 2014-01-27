#include "user.h"

User::User(QObject *parent) :
    Person(parent)
{
}

QString User::username()
{
    return this->username_;
}

QString User::password()
{
    return this->password_;
}

void User::username(const QString &newUsername)
{
    this->username_ = newUsername;
}

void User::password(const QString &newPassword)
{
    this->password_ = newPassword;
}
