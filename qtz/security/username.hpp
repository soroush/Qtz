#ifndef QTZ_USERNAME_HPP
#define QTZ_USERNAME_HPP

#include "identity.hpp"
#include <QString>

class Username : public Identity {
public:
    Username(const QString& username = "");
    ~Username() = default;
    void setUsername(const QString& username);
    QString getUsername() const;
private:
    QString m_username;
};

#endif // QTZ_USERNAME_HPP
