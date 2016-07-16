#include "authentication-adapter.hpp"

AuthenticationAdapter::AuthenticationAdapter()
{

}

bool DatabaseAuthenticationAdapter::check(User *, AuthenticationToken *) const
{
    // TODO: Implement
    return false;
}
