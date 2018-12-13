#ifndef QTZ_AUTHENTICATOR_HPP
#define QTZ_AUTHENTICATOR_HPP

#include "identity.hpp"
#include "token.hpp"
#include "authentication-source.hpp"

/*
 * Authentication is the act of confirming the truth of an attribute of a
 * single piece of data claimed true by an entity. In contrast with
 * identification, which refers to the act of stating or otherwise indicating a
 * claim purportedly attesting to a person or thing's identity, authentication
 * is the process of actually confirming that identity.
*/

class Authenticator {
public:
    Authenticator();
    virtual ~Authenticator();
    virtual bool authenticate(const Identity* id, const Token* token) = 0;
    virtual void installAuthenticationSource(const AuthenticationSource* source) = 0;
};

#endif // QTZ_AUTHENTICATOR_HPP
