#ifndef QTZ_AUTHENTICATOR_H
#define QTZ_AUTHENTICATOR_H


#include "identity.hpp"
#include "token.hpp"
#include "authentication-source.hpp"

class Authenticator {
public:
    Authenticator();
    virtual ~Authenticator();
    virtual bool authenticate(const Identity* id, const Token* token)=0;
    virtual void installAuthenticationSource(const AuthenticationSource* source)=0;
};

#endif // QTZ_AUTHENTICATOR_H
