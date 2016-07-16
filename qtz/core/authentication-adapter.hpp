#ifndef QTZ_AUTHENTICATION_ADAPTER_H
#define QTZ_AUTHENTICATION_ADAPTER_H

#include "user.hpp"
#include "authentication-token.hpp"

class AuthenticationAdapter
{
public:
    AuthenticationAdapter();
    virtual bool check(User*, AuthenticationToken*)const =0;
};

class DatabaseAuthenticationAdapter : public AuthenticationAdapter
{
public:
    DatabaseAuthenticationAdapter();
    bool check(User*, AuthenticationToken*)const;
};

#endif // QTZ_AUTHENTICATION_ADAPTER_H
