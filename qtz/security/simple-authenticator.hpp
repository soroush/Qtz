#ifndef QTZ_SIMPLEAUTHENTICATOR_HPP
#define QTZ_SIMPLEAUTHENTICATOR_HPP

#include "authenticator.hpp"
#include <QtSql/QSqlDatabase>

class SimpleAuthenticator : public Authenticator {
public:
    SimpleAuthenticator();
    void installAuthenticationSource();
    bool authenticate(const Identity* id, const Token* token);
};

#endif // QTZ_SIMPLEAUTHENTICATOR_HPP
