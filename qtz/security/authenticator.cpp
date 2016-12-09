#include "authenticator.hpp"


Authenticator::Authenticator() {
}

Authenticator::~Authenticator() {
}

void Authenticator::installAuthenticationSource(const AuthenticationSource*) {
}

bool Authenticator::authenticate(const Identity*, const Token*) {
    return false;
}
